#include <Arduino.h>
#include "pins_arduino.h"

#include <TimerOne.h>
#include <Wire.h>

#include "crc8.hpp"
#include "proto.hpp"
#include "silnik.hpp"

#include "filtr.hpp"
#include "workmode.hpp"
#include "spi_msg.hpp"
#include "main.h"



//#define TEST

//#define MAINDEBUG
#define DEBUG
#ifdef DEBUG
	#define SD(T) Serial.print(T)
	#define SDN(T) Serial.println(T)
	#define SD2(T,P) Serial.print(T,P)
	#define SDN2(T,P) Serial.println(T,P)

	#define SDP(T, V) SD(T); SD(V)
	#define SDPN(T, V) SD(T); SDN(V)
    #define SPHEX(X) phex(X)
    #define SPRINT(N)	SD("Sending (");SD(__FILE__);SD(":");SD(__LINE___); SD(") [");for (int i=0;i<N;++i){ SPHEX(smsg.sendBuff[i]); }SDN("]");
#else
	#define SD(T) 
	#define SDN(T) 
	#define SD2(T,P) 
	#define SDN2(T,P) 

	#define SDP(T, V) 
	#define SDPN(T, V)
    #define SPHEX(X)
    #define SPRINT(N)
#endif
//Motor *mot; 
Motor mot; 
WorkMode mode;
SPIMessage smsg;

static constexpr uint8_t baseAddr = 0x30;
uint8_t address = baseAddr;

void setBusy(bool busy)
{
	digitalWrite(BUSYPIN, busy ? LOW : HIGH);
	if (!busy) {
		smsg.sendPos = 0;
		smsg.recvPos = 0;
	}
}

uint8_t getAddress()
{
	uint8_t add = baseAddr;

	add += (digitalRead(AD3) == HIGH) ? 8 : 0;
	add += (digitalRead(AD2) == HIGH) ? 4 : 0;
	add += (digitalRead(AD1) == HIGH) ? 2 : 0;	
	add += (digitalRead(AD0) == HIGH) ? 1 : 0;
	return add;
}

volatile bool stopMessagePrepare = false;
void setStopSoft()
{
	SDN("SI");
	//stopMessagePrepare = true;
	mot.setStop(false);
}

void setStopHard()
{
	SDN("HI");
	mot.setStop(true);
}

void motorImpulse()
{
	mot.impulse();
}

volatile bool received = false;

static void receiveEvent(int how);
static void requestEvent();
void setup()
{
	pinMode(MOVEPIN, OUTPUT); digitalWrite(MOVEPIN, HIGH);
	pinMode(BUSYPIN, OUTPUT); setBusy(true);
	pinMode(10, INPUT);
	pinMode(A3, INPUT);
	pinMode(A2, INPUT);
	pinMode(A1, INPUT);
	pinMode(DEBUGPIN, INPUT);

	pinMode(ENPIN, OUTPUT); digitalWrite(ENPIN, LOW);
	pinMode(DIRPIN, OUTPUT); digitalWrite(DIRPIN, LOW);
	pinMode(PULSEPIN, OUTPUT); 
	pinMode(KRANCPIN, INPUT);
	pinMode(STOPPIN, INPUT);
	
	address = getAddress();

	Serial.begin(115200);
	Serial.println(__DATE__);
	Serial.println(__TIME__);
  	Serial.print(digitalRead(10) == 0);
	Serial.print(digitalRead(A3) == 0);
	Serial.print(digitalRead(A2) == 0);
	Serial.println(digitalRead(A1) == 0);

	Serial.print("Addr = ");
	Serial.println(address, HEX);
	
	
	mode.init(address - baseAddr);
	mot.init();
	//digitalWrite(ENPIN, (mode.isDebugMode() && mode.getMode() == WorkMode::CHECKKRANC) ? HIGH : LOW);
	if (mode.isDebugMode()) {
		Serial.println("DEBUG MODE");
		return;
	}
	Wire.begin(address);           // join i2c bus with address #8
  	Wire.onReceive(receiveEvent); // register event
	Wire.onRequest(requestEvent); 

	smsg.init(mode.getMode(), &mot);
	 
	Timer1.attachInterrupt(motorImpulse);
	Timer1.initialize(125000);
	Timer1.stop();

	attachInterrupt(digitalPinToInterrupt(KRANCPIN), setStopHard, FALLING);

	setBusy(false);
	Serial.println("START");
}


unsigned long prev = 0;
unsigned long act = 0;
volatile bool message_in_progress = false;
void loop()
{
	if (mode.isDebugMode())
	{
		debugModeFun(mode);
		return;
	}

	if (!message_in_progress && stopMessagePrepare) {
		stopMessagePrepare = false;
		
	}

	while (true) {
		act = millis();
		if (act-prev > 5000) { Serial.println("Czekam..."); prev = act; }
		
		if (received) {
			smsg.proceed();
			received = false;
		}
		
	}
}

void receiveEvent(int how) {
	received = true;
	int s = 0;
	while (0 < Wire.available()) 
  	{ 
    	smsg.recvBuff[smsg.recvPos++] = Wire.read(); // receive byte as a character
		++s;
	}
	SDP("recvPos=", smsg.recvPos);SDP("s=",s);SDPN(" how=", how);SD("Receive : [");
	for (int i=0; i<s; ++i) {
		SD(" ");SD2(smsg.recvBuff[i], HEX);
	}
	SDN("]");
}

static void requestEvent()
{
	SDP("sendPos=", smsg.sendPos);SDP(" sizeSendMsg=", smsg.sizeSendMsg);
	SD("Sending : [");
	for (int i=0; i<smsg.sizeSendMsg; ++i) {
		SD(" ");SD2(smsg.sendBuff[i], HEX);
	}
	SDN("]");
	for (int t = 0; t < smsg.sizeSendMsg; t++)
		Wire.write(smsg.sendBuff[smsg.sendPos++]);
}

void phex(uint8_t b)
{
	Serial.print(" ");
	if (b < 16)
		Serial.print("0");
	Serial.print(b, HEX);
}



