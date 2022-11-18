#include <Arduino.h>
#include "pins_arduino.h"

//SPI SLAVE (ARDUINO)
#include<SPI.h>
#include <TimerOne.h>


#include "crc8.hpp"
#include "proto.hpp"
#include "silnik.hpp"

#define BUSYPIN A0
#define STOPPIN 2
#define KRANCPIN 3
#define DEBUG

Motor mot;
Message  msg;
volatile bool received;
constexpr uint8_t maxBuff = 32;
volatile uint8_t recvBuff[maxBuff];
volatile uint8_t sendBuff[maxBuff];
volatile uint8_t recvPos;
volatile uint8_t sendPos;
volatile bool sendStopPos = false;

const uint8_t FB = 0x0A;

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
	for(uint8_t i=0; i<maxBuff;++i)
		phex2(sendBuff[i]);
	Serial.println("");
}
inline void setBusy(bool busy)
{
	digitalWrite(BUSYPIN, busy ? HIGH : LOW);
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
    //sendStopPos = mot.impulse();
	mot.impulse();
}


void setup()
{

	pinMode(BUSYPIN, OUTPUT);
	setBusy(false);

	pinMode(SS, INPUT);
	pinMode(SCK, INPUT);
	pinMode(MOSI, INPUT);
	pinMode(MISO, OUTPUT);

	

	Timer1.attachInterrupt(motorImpulse);
	Timer1.initialize(200);
    
	mot.init();
	msg.init();

  	pinMode(MISO,OUTPUT);                   //Sets MISO as OUTPUT (Have to Send data to Master IN 

	for (uint8_t i = 0; i < maxBuff; ++i) {
		recvBuff[i] = i;
		sendBuff[i] = 16+i;
	}
	
	sendBuff[0] = (ECHO_REP << 4) & 0xf0;
	sendBuff[1] = 10;

#ifdef DEBUG
	Serial.begin(115200);
#endif // DEBUG


	//attachInterrupt(digitalPinToInterrupt(STOPPIN), setStopSoft, FALLING);
	//attachInterrupt(digitalPinToInterrupt(KRANCPIN), setStopHard, FALLING);

	
	setBusy(false);

}




ISR (SPI_STC_vect)                        //Inerrrput routine function 
{
  	recvBuff[recvPos] = SPDR;             // Value received from master if store in variable slavereceived
  	SPDR = sendBuff[sendPos];
	recvPos = (recvPos+1) & 0x1f;
	sendPos = (sendPos+1) & 0x1f;
  	received = true;
}


bool cmd_in_progress = false;
uint8_t lenCmd = 0;
uint8_t actProcess = 0;
uint32_t counter = 6500000;

int8_t skipCharCnt = -1;
void loop()
{ 
	--counter ;
	if (counter < 2) {
		counter = 6500000;
		mot.print();
		delay(1);
	}

  	if(received)                            //Logic to SET LED ON OR OFF depending upon the value recerived from master
  	{
		
		Serial.print("REC.BEG actProcess=");
		Serial.print(actProcess,DEC);
		Serial.print(" recvPos=");
		Serial.println(recvPos, DEC);
		
		while (recvPos > 0 && actProcess < recvPos)
		{
			Serial.print(actProcess+1, DEC);
			Serial.print("/");
			Serial.print(recvPos, DEC);
			Serial.print(" CHAR=");
			phex2(recvBuff[actProcess]);
			if (skipCharCnt >= 0) {
				Serial.print(" Skip");
				Serial.print(" (");
				Serial.print(skipCharCnt, DEC);
				Serial.println(")");
				if (skipCharCnt == 0) {
					sendPos = 0;
					msg.clear();
					setBusy(false);
					Serial.println("BUSY OFF");
				}
				
				actProcess++;
				--skipCharCnt;
				
        		continue;
			}
			
			Serial.println(" Proc");

			if (!msg.add(recvBuff[actProcess++]))
				continue;
    
			
			Serial.println("BUSY ON");
			setBusy(true);
			
			Result status = msg.parse();

			if (!status.ok) {
				Serial.println("CMD:Nie poprawna wiadomosc");
				msg.clear();
				setBusy(false);
				Serial.println("BUSY OFF");
				continue;
			}


			uint8_t addr = msg.getAddr();
			switch(msg.getMsgCmd()) {
				case LAST_REQ:
				{
					Serial.println("CMD:LAST");
					for (uint8_t i = 0; i < maxBuff; ++i)
						sendBuff[i] = i;
					skipCharCnt = 16; 
					break;
				}

				case ECHO_REQ:
				{
					Serial.println("CMD:ECHO");
					printSendBuf();
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
					Serial.println("BUSY OFF");
					break;
				}

				case PROGRESS_REQ:
				{
					Serial.println("CMD:PROGRESS");
					printSendBuf();
					CRC8 crc;
					crc.restart();
					sendBuff[0] = 0x07;
					sendBuff[1] = (( PROGRESS_REP << 4) & 0xf0) | 0x04;
					sendBuff[2] = ((addr << 4) & 0xf0) | 0x08;
					if (mot.getIsMoveHome())
						sendBuff[2] |= 0x04;
					crc.add(sendBuff[1]);
					int32_t pos = mot.getGlobalPos();
					Serial.print("POS=");
					Serial.println(pos, DEC);
					sendBuff[3] = (pos >> 24) & 0xff;
					sendBuff[4] = (pos >> 16) & 0xff;
					sendBuff[5] = (pos >> 8) & 0xff;
					sendBuff[6] = (pos) & 0xff;
					crc.add(sendBuff[3]);
					crc.add(sendBuff[4]);
					crc.add(sendBuff[5]);
					crc.add(sendBuff[6]);
					sendBuff[7] = crc.getCRC();
					sendBuff[8] = 0;
					sendBuff[9] = 0xff;
					sendPos = 0;
					Serial.print("Send ");
					for(int i=0;i < 8; ++i)
						phex2(sendBuff[i]);
					Serial.println("");	
					msg.clear();
					setBusy(false);
					Serial.println("BUSY OFF");
					break;
				}

				case CONF_REQ:
				{
					Serial.println("CMD:CONF");
					printSendBuf();
					Serial.println("Konfiguracja:");
					Serial.print("reverse=");
					Serial.println(status.data.conf.reverse, DEC);
					Serial.print("en=");
					Serial.println(status.data.conf.enableAlways, DEC);
					Serial.print("max=");
					Serial.println(status.data.conf.maxStep, DEC);
					Serial.print("base=");
					Serial.println(status.data.conf.baseSteps, DEC);
					Serial.print("delay=");
					Serial.println(status.data.conf.delayImp, DEC);
					mot.setReverseMotor(status.data.conf.reverse);
					mot.setMaxSteps(status.data.conf.maxStep);
					mot.setEnabledAlways(status.data.conf.enableAlways);
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
					Serial.print("Send ");
					for(int i=0;i<4; ++i)
						phex2(sendBuff[i]);
					Serial.println("");	
					msg.clear();
					setBusy(false);
					Serial.println("BUSY OFF");
					break;
				}

				case MOVE_REQ:
				{
					Serial.println("CMD:MOVE");
					printSendBuf();
					Serial.print("Ruch. Home = ");
					Serial.print(status.data.move.isHome ? "Tak" : "Nie");
					Serial.print(" Kroki = ");
					Serial.println(status.data.move.steps, DEC);
					if (status.data.move.isHome) {
						mot.moveHome();
					} else {
						mot.movePosition(status.data.move.steps);
					}
					CRC8 crc;
					crc.restart();
					sendBuff[0] = 3;
					sendBuff[1] = (MOVE_REP << 4) & 0xf0;
					sendBuff[2] = ((addr << 4) & 0xf0) | 0x08;
					if (status.data.move.isHome)
						sendBuff[2] |= 0x01;
					crc.add(sendBuff[1]);
					crc.add(sendBuff[2]);
					sendBuff[3] = crc.getCRC();
					sendBuff[4] = 0;
					sendBuff[5] = 0xff;
					sendPos = 0;
					for(int i=0;i<4; ++i)
						phex2(sendBuff[i]);
					Serial.println("");	
					msg.clear();
					setBusy(false);
					Serial.println("BUSY OFF");
					break;
				}
					
				default:
				{
					Serial.println("CMD:Uknonwn");
					msg.clear();
					setBusy(false);
					Serial.println("BUSY OFF");
					break;
				}
					
			}//switch
		} //while
	
		actProcess = 0;
		recvPos = 0;
		received = false;
		Serial.println("REC.END actProcess=0 recvPos=0");
	}

	if (sendStopPos) {
		Serial.println("PIN STOP");
		setBusy(true);	
		sendStopPos = false;
		setBusy(false);	
	}
}

