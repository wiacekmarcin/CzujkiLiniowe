#include <Arduino.h>
#include "pins_arduino.h"

// SPI SLAVE (ARDUINO)
#include <SPI.h>
#include <TimerOne.h>

#include "crc8.hpp"
#include "proto.hpp"
#include "silnik.hpp"

#define BUSYPIN A0
#define STOPPIN 3
#define KRANCPIN 2

// #define DEBUG_PINDEBUG

#define ENPIN 7
#define DIRPIN 8
#define PULSEPIN 9
#define MOVEPIN 5

#define DEBUGPIN 4

#define DBG1 A1
#define DBG2 A2
#define DBG3 A3

typedef enum _debugWorkMode
{
	PIONOWA = 0,
	KATOWA = 1,
	KOLOWA = 2,
	POZIOMA = 4,
	CHECKKRANC = 7,
	UNKNOWN = 8
} DebugWorkMode;

DebugWorkMode dbgWorkMode = UNKNOWN;

#define DEBUG

Motor mot;
Message msg;
volatile bool received;
constexpr uint8_t maxBuff = 32;
volatile uint8_t recvBuff[2*maxBuff];
volatile uint8_t sendBuff[maxBuff];
volatile uint8_t recvPos;
volatile uint8_t sendPos;
volatile bool sendStopPos = false;

const uint8_t FB = 0x0A;

static void debugModeFun();
static void echoRequestFun(uint8_t addr);
static void progressRequestFun(uint8_t addr);
static void configurationRequest(uint8_t addr, Result status);
static void moveRequest(uint8_t addr, bool isHome, uint32_t steps);
static DebugWorkMode conv2DebugWorkMode(uint8_t d3, uint8_t d2, uint8_t d1);

#ifdef DEBUG
static void phex2(uint8_t b);
static void printSendBuf();
#endif

#ifdef DEBUG_PINDEBUG
static void debugModeFun2();
#endif

/**
 * M1 DIR=1 w prawo do nadajnika kroki 782(dla dir=0) / 778 (dla dir=1)
 * M2 DIR=1 do góry kroki 650
 * M3 DIR=1 przeciwnie do wskazowek zegara kroki 29825
 * M4 DIR=1 przeciwnie do wskazowek zegara kroki 29835
 * M5 DIR=1 ze wskazowkami zegara kroki 29750
 * M6 DIR=1 w prawo do najnika kroki 19930
 * M7 DIR=1 do gory 150000 krokow malo
 * M8 DIR=1 w prawo od odbiornika 1307 / 973 / 996
 * M9 DIR=1 dp dolu Kroki 826(dla dir=0) / 797(dla dir=1) 2 wdrugim przypadku 802 do 831
 */

uint32_t getDelayImp(DebugWorkMode d)
{
	switch (d)
	{
	case PIONOWA:
		return 50;
	case KATOWA:
		return 1000;
	case KOLOWA:
		return 60;
	case POZIOMA:
		return 250;
	case 3:
		return 200;
	default:
		return 1000000;
	}
}

uint32_t getMaxSteps(DebugWorkMode d)
{
	switch (d)
	{
	case PIONOWA:
		return 250000;
	case KATOWA:
		return 1500;
	case KOLOWA:
		return 32000;
	case POZIOMA:
		return 25000;
	case 3:
		return 3200;
	default:
		return 1000000;
	}
}

inline void setBusy(bool busy)
{
	digitalWrite(BUSYPIN, busy ? LOW : HIGH);
	if (!busy)
		SPDR = FB;
}

void setStopSoft()
{
	sendStopPos = true;
	mot.setStop(false);
	Timer1.stop();
}

void setStopHard()
{
	sendStopPos = true;
	mot.setStop(true);
	Timer1.stop();
}

void motorImpulse()
{
	// sendStopPos = mot.impulse();
	mot.impulse();
}

uint32_t impDelay = 12500;
bool isDebugMode = false;
uint32_t maxSteps = 0;
void setup()
{

	pinMode(BUSYPIN, OUTPUT);
	//setBusy(false);

	pinMode(DEBUGPIN, INPUT);
	pinMode(DBG3, INPUT);
	pinMode(DBG2, INPUT);
	pinMode(DBG1, INPUT);

	pinMode(SS, INPUT_PULLUP);
	pinMode(SCK, INPUT);
	pinMode(MOSI, INPUT);
	pinMode(MISO, OUTPUT);

	pinMode(ENPIN, OUTPUT);
	pinMode(DIRPIN, OUTPUT);
	pinMode(PULSEPIN, OUTPUT);
	pinMode(KRANCPIN, INPUT);
	pinMode(STOPPIN, INPUT_PULLUP);
	pinMode(MOVEPIN, OUTPUT);

	digitalWrite(ENPIN, LOW);
	digitalWrite(MOVEPIN, HIGH);
	Serial.begin(115200);

	isDebugMode = digitalRead(DEBUGPIN) == LOW;
	dbgWorkMode = conv2DebugWorkMode(digitalRead(DBG3), digitalRead(DBG2), digitalRead(DBG1));

	if (isDebugMode)
	{
		// Serial.begin(115200);

		digitalWrite(DIRPIN, LOW);
		impDelay = getDelayImp(dbgWorkMode);
		maxSteps = getMaxSteps(dbgWorkMode);
		if (dbgWorkMode == CHECKKRANC)
			digitalWrite(ENPIN, HIGH);
		else
			digitalWrite(ENPIN, LOW);
		return;
	}
#ifdef DEBUG_PINDEBUG
	return;
#endif
	return ;

	//Timer1.attachInterrupt(motorImpulse);
	//Timer1.initialize(125000);

	//mot.init();
	//msg.init();

	pinMode(MISO, OUTPUT); // Sets MISO as OUTPUT (Have to Send data to Master IN

	for (uint8_t i = 0; i < maxBuff; ++i)
	{
		recvBuff[2*i] = 2*i;
		recvBuff[2*i+1] = 2*i+1;
		sendBuff[i] = 16 + i;
	}

	sendBuff[0] = (ECHO_REP << 4) & 0xf0;
	sendBuff[1] = 10;

	//attachInterrupt(digitalPinToInterrupt(STOPPIN), setStopSoft, FALLING);
	//attachInterrupt(digitalPinToInterrupt(KRANCPIN), setStopHard, FALLING);

	//setBusy(false);

	mot.setDir(false);
}

ISR(SPI_STC_vect) // Inerrrput routine function
{
	recvBuff[recvPos] = SPDR; // Value received from master if store in variable slavereceived
	SPDR = sendBuff[sendPos];
	recvPos = (recvPos + 1) & 0x1f;
	sendPos = (sendPos + 1) & 0x1f;
	received = true;
}

bool cmd_in_progress = false;
uint8_t lenCmd = 0;
uint8_t actProcess = 0;
uint32_t counter = 6500000;

int8_t skipCharCnt = -1;

bool highlevel = false;

uint32_t loops = 0;
int delImp = 20;

bool debugDir = false;
unsigned long actTime = 0, prevTime = 0;
uint32_t steps = 0xffffffff;
uint32_t middleSteps = 0;
uint32_t timeDelay = 0;


bool czujkaInit = false;

void loop()
{

	

	if (isDebugMode)
	{
		debugModeFun();
		return;
	}
#ifdef DEBUG_PINDEBUG
	else
	{
		debugModeFun2();
		return;
	}
#endif


	if (received) 
	{
#ifdef DEBUG		
		Serial.print("REC.BEG actProcess=");
		Serial.print(actProcess, DEC);
		Serial.print(" recvPos=");
		Serial.println(recvPos, DEC);
#endif
		while (recvPos > 0 && actProcess < recvPos)
		{
#ifdef DEBUG			
			Serial.print(actProcess + 1, DEC);
			Serial.print("/");
			Serial.print(recvPos, DEC);
			Serial.print(" CHAR=");
			phex2(recvBuff[actProcess]);
#endif			
			if (skipCharCnt >= 0)
			{
#ifdef DEBUG				
				Serial.print(" Skip");
				Serial.print(" (");
				Serial.print(skipCharCnt, DEC);
				Serial.println(")");
#endif				
				if (skipCharCnt == 0)
				{
#ifdef DEBUG					
					Serial.println("BUSY OFF");
#endif	
					sendPos = 0;
					msg.clear();
					setBusy(false);
				
				} 
				actProcess++;
				--skipCharCnt;
				continue;
			}
#ifdef DEBUG
			Serial.println(" Proc");
#endif			

			if (!msg.add(recvBuff[actProcess++]))
				continue;
#ifdef DEBUG
			Serial.println("BUSY ON");
#endif			
			setBusy(true);

			Result status = msg.parse();

			if (!status.ok)
			{
#ifdef DEBUG								
				Serial.println("CMD:Nie poprawna wiadomosc");
				Serial.println("BUSY OFF");
#endif				
				msg.clear();
				setBusy(false);
				continue;
			}

			uint8_t addr = msg.getAddr();
			switch (msg.getMsgCmd())
			{
			case LAST_REQ:
			{
#ifdef DEBUG				
				Serial.println("CMD:LAST");
#endif				
				for (uint8_t i = maxBuff; i > 0; --i)
					sendBuff[i] = i;
				skipCharCnt = 16;
				break;
			}

			case ECHO_REQ:
			{
				echoRequestFun(addr);
				break;
			}

			case PROGRESS_REQ:
			{
				progressRequestFun(addr);
				break;
			}

			case CONF_REQ:
			{
				configurationRequest(addr, status);
				break;
			}

			case MOVE_REQ:
			{
				moveRequest(addr, status.data.move.isHome, status.data.move.steps);
				break;
			}

			default:
			{
#ifdef DEBUG				
				Serial.println("CMD:Uknonwn");
				Serial.println("BUSY OFF");
#endif				
				msg.clear();
				setBusy(false);
				break;
			}

			} // switch
		}	  // while
#ifdef DEBUG
		Serial.print("AFTER WHILE actProcess=");
		Serial.print(actProcess, DEC);
		Serial.print(" recvPos=");
		Serial.println(recvPos, DEC);
#endif		
		actProcess = 0;
		recvPos = 0;
		sendPos = 0;
		received = false;
		Serial.println("REC.END");
	}

	if (sendStopPos)
	{
		Serial.println("PIN STOP");
		setBusy(true);
		sendStopPos = false;
		setBusy(false);
	}

	if (loops <= 10)
	{
		loops = 50000000;
		Serial.println("Czekam");
	}
	--loops;
}

DebugWorkMode conv2DebugWorkMode(uint8_t d3, uint8_t d2, uint8_t d1)
{
	uint8_t b = 0;
	b += (d3 == LOW) ? 0x01 : 0x00;
	b += (d2 == LOW) ? 0x02 : 0x00;
	b += (d1 == LOW) ? 0x04 : 0x00;
#ifdef DEBUG
	Serial.print("d3=");
	Serial.print(d3);
	Serial.print(" d2=");
	Serial.print(d2);
	Serial.print(" d1=");
	Serial.print(d1);
	Serial.print(" b=");
	Serial.print(b);
#endif
	switch (b)
	{
	case PIONOWA:
		return PIONOWA;
	case KATOWA:
		return KATOWA;
	case KOLOWA:
		return KOLOWA;
	case POZIOMA:
		return POZIOMA;
	case CHECKKRANC:
		return CHECKKRANC;
	default:
		return UNKNOWN;
	}
}

void debugModeFun()
{
	if (dbgWorkMode == CHECKKRANC)
	{
		Serial.print("Czujnik krancowki = ");
		Serial.println(digitalRead(KRANCPIN) == LOW ? "Tak" : "Nie");
		delay(500);
		prevTime = millis();
		return;
	}

	if (steps == 0xffffffff)
	{
		Serial.print("Rozpoczynam prace. Kierunek dir=");
		Serial.println(debugDir ? HIGH : LOW);
		Serial.print("maxSteps=");
		Serial.print(maxSteps);
		Serial.print(" impDelay=");
		Serial.print(impDelay);
		steps = 0;
		return;
	}

	if (steps > maxSteps)
	{
		debugDir = !debugDir;
		digitalWrite(DIRPIN, debugDir ? HIGH : LOW);
		Serial.print("Nie wykryto krancowki. Steps > ");
		Serial.println(maxSteps);
		Serial.print("Zmieniam kierunek na dir=");
		Serial.println(debugDir ? HIGH : LOW);
		steps = 0;
		if (dbgWorkMode == PIONOWA)
		{
			delay(5000);
		}
		return;
	}

	if (digitalRead(KRANCPIN) == LOW)
	{
		actTime = millis();
		Serial.print("\nWykryto krancowke po ");
		Serial.print(steps);
		Serial.print(" krokach. Czas [ms]=");
		Serial.println(actTime - prevTime);
		middleSteps = steps / 2;
		steps = 0;
		debugDir = !debugDir;
		digitalWrite(DIRPIN, debugDir ? HIGH : LOW);
		delay(3000);

		prevTime = millis();

		while (digitalRead(KRANCPIN) == LOW)
		{
			digitalWrite(PULSEPIN, HIGH);
			delayMicroseconds(impDelay);
			digitalWrite(PULSEPIN, LOW);
			delayMicroseconds(impDelay);
		}
		Serial.print("Zmiana kierunku dir=");
		Serial.println(debugDir);
	}

	if (false && dbgWorkMode == KATOWA && steps == middleSteps && steps > 0)
	{
		Serial.print("Srodek (kroki= ");
		Serial.print(steps);
		Serial.println(")");
		timeDelay += 10000;
		delay(10000);
	}
	digitalWrite(PULSEPIN, HIGH);
	delayMicroseconds(impDelay);
	digitalWrite(PULSEPIN, LOW);
	delayMicroseconds(impDelay);
	++steps;
	if (dbgWorkMode == KATOWA && steps % 50 == 0)
	{
		Serial.print(".");
	}

	if (dbgWorkMode == PIONOWA && steps % 10000 == 0)
	{
		Serial.print(".");
	}
}

#ifdef DEBUG_PINDEBUG
void debugModeFun2()
{
	Serial.print("DBG=");
	Serial.print(digitalRead(DEBUGPIN));
	Serial.print(" D3=");
	Serial.print(digitalRead(DBG3));
	Serial.print(" D2=");
	Serial.print(digitalRead(DBG2));
	Serial.print(" D1=");
	Serial.print(digitalRead(DBG1));
	Serial.print(" dbgWorkMode=");
	Serial.print(dbgWorkMode);
	Serial.print(" dbgWorkMode2=");
	Serial.print(conv2DebugWorkMode(digitalRead(DBG3), digitalRead(DBG2), digitalRead(DBG1)));
	Serial.print(" KRANC=");
	Serial.print(digitalRead(KRANCPIN));
	Serial.print(" STOP=");
	Serial.println(digitalRead(STOPPIN));
	delay(1000);
}
#endif

void echoRequestFun(uint8_t addr)
{
#ifdef DEBUG
	Serial.println("CMD:ECHO");
	printSendBuf();
#endif // DEBUG
	
	CRC8 crc;
	crc.restart();
	sendBuff[0] = 3;
	sendBuff[1] = (ECHO_REP << 4) & 0xf0;
	sendBuff[2] = ((addr << 4) & 0xf0) | 0x08;
	crc.add(sendBuff[1]);
	crc.add(sendBuff[2]);
	sendBuff[3] = crc.getCRC();
	sendBuff[4] = 5;
	sendBuff[5] = 0xff;
	sendPos = 0;
	msg.clear();
	setBusy(false);
#ifdef DEBUG	
	Serial.println("BUSY OFF");
#endif	
}

void progressRequestFun(uint8_t addr)
{
#ifdef DEBUG
	Serial.println("CMD:PROGRESS");
	printSendBuf();
#endif	
	CRC8 crc;
	crc.restart();
	sendBuff[0] = 0x07;
	sendBuff[1] = ((PROGRESS_REP << 4) & 0xf0) | 0x04;
	sendBuff[2] = ((addr << 4) & 0xf0) | 0x08;
	if (mot.getIsMoveHome())
		sendBuff[2] |= 0x04;
	crc.add(sendBuff[1]);
	int32_t pos = mot.getGlobalPos();
#ifdef DEBUG
	Serial.print("POS=");
	Serial.println(pos, DEC);
#endif	
	sendBuff[3] = (pos >> 24) & 0xff;
	sendBuff[4] = (pos >> 16) & 0xff;
	sendBuff[5] = (pos >> 8) & 0xff;
	sendBuff[6] = (pos)&0xff;
	crc.add(sendBuff[3]);
	crc.add(sendBuff[4]);
	crc.add(sendBuff[5]);
	crc.add(sendBuff[6]);
	sendBuff[7] = crc.getCRC();
	sendBuff[8] = 0;
	sendBuff[9] = 0xff;
	sendPos = 0;
#ifdef DEBUG	
	Serial.print("Send ");
	for (int i = 0; i < 8; ++i)
		phex2(sendBuff[i]);
	Serial.println("");
#endif	
	msg.clear();
	setBusy(false);
#ifdef DEBUG		
	Serial.println("BUSY OFF");
#endif	
}

void configurationRequest(uint8_t addr, Result status)
{
#ifdef DEBUG	
	Serial.println("CMD:CONF");
	printSendBuf();
	Serial.println("Konfiguracja:");
	Serial.print("reverse=");
	Serial.println(status.data.conf.reverse, DEC);
	Serial.print("max=");
	Serial.println(status.data.conf.maxStep, DEC);
	Serial.print("base=");
	Serial.println(status.data.conf.baseSteps, DEC);
	Serial.print("delay=");
	Serial.println(status.data.conf.delayImp, DEC);
#endif	
	mot.setReverseMotor(status.data.conf.reverse);
	mot.setMaxSteps(status.data.conf.maxStep);
	mot.setBaseSteps(status.data.conf.baseSteps);
	mot.setDelayImp(status.data.conf.delayImp);
	Timer1.setPeriod(status.data.conf.delayImp);
	CRC8 crc;
	crc.restart();
	sendBuff[0] = 3;
	sendBuff[1] = (CONF_REP << 4) & 0xf0;
	sendBuff[2] = ((addr << 4) & 0xf0) | 0x08;
	crc.add(sendBuff[1]);
	crc.add(sendBuff[2]);
	sendBuff[3] = crc.getCRC();
	sendBuff[4] = 0;
	sendBuff[5] = 0xff;
	sendPos = 0;
#ifdef DEBUG
	Serial.print("Send ");
	for (int i = 0; i < 4 ; ++i)
		phex2(sendBuff[i]);
	Serial.println("");
#endif // DEBUG	
	msg.clear();
	setBusy(false);
#ifdef DEBUG
	Serial.println("BUSY OFF");
#endif // DEBUG	 
	
}

void moveRequest(uint8_t addr, bool isHome, uint32_t steps)
{
#ifdef DEBUG
	Serial.println("CMD:MOVE");
	printSendBuf();
	Serial.print("Ruch. Home = ");
	Serial.print(isHome ? "Tak" : "Nie");
	Serial.print(" Kroki = ");
	Serial.println(steps, DEC);
#endif	
	if (isHome)
	{
		mot.moveHome();
	}
	else
	{
		mot.movePosition(steps);
	}
	CRC8 crc;
	crc.restart();
	sendBuff[0] = 3;
	sendBuff[1] = (MOVE_REP << 4) & 0xf0;
	sendBuff[2] = ((addr << 4) & 0xf0) | 0x08;
	if (isHome)
		sendBuff[2] |= 0x01;
	crc.add(sendBuff[1]);
	crc.add(sendBuff[2]);
	sendBuff[3] = crc.getCRC();
	sendBuff[4] = 0;
	sendBuff[5] = 0xff;
	sendPos = 0;
#ifdef DEBUG
	Serial.print("Send ");
	for (int i = 0; i < 6; ++i)
		phex2(sendBuff[i]);
	Serial.println("");
#endif // DEBUG	

	msg.clear();
	setBusy(false);
#ifdef DEBUG
	Serial.println("BUSY OFF");
#endif // DEBUG	
	
}

#ifdef DEBUG
void phex2(uint8_t b)
{
	Serial.print(" ");
	if (b < 10)
		Serial.print("0");
	Serial.print(b, HEX);
}

void printSendBuf()
{
	Serial.print("Send ");
	for (uint8_t i = 0; i < maxBuff; ++i)
		phex2(sendBuff[i]);
	Serial.println("");
}

#endif
