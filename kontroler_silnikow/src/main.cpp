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

#define BUSYPIN A0

#define TEST

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

static constexpr uint8_t FB = 0x0A;

void setBusy(bool busy)
{
	digitalWrite(BUSYPIN, busy ? LOW : HIGH);
	if (!busy) {
		smsg.sendPos = 0;
		smsg.recvPos = 0;
	}
}

volatile bool createStopResponse;
void setCreateStopMessageFun()
{
	createStopResponse = true;
}

void setStopSoft()
{
	SDN("SI");
	//mot->setStop(false);
	mot.setStop(false);
}

void setStopHard()
{
	SDN("HI");
	//mot->setStop(true);
	mot.setStop(true);
}

void motorImpulse()
{
	//mot->impulse();
	mot.impulse();
}

volatile bool received = false;

static void receiveEvent(int how);
static void requestEvent();
void setup()
{

  	Wire.begin(0x31);                // join i2c bus with address #8
  	Wire.onReceive(receiveEvent); // register event
	Wire.onRequest(requestEvent); 
	pinMode(BUSYPIN, OUTPUT);
	setBusy(false);

	mode.init();
	//mot = Motor();
	//mot->init();
	mot.init();
	digitalWrite(Motor::ENPIN, (mode.isDebugMode() && mode.getMode() == WorkMode::CHECKKRANC) ? HIGH : LOW);
	if (mode.isDebugMode()) {
		Serial.println("DEBUG MODE");
		return;
	}

	smsg.init(mode.getMode(), &mot);
	 

	Timer1.attachInterrupt(motorImpulse);
	Timer1.initialize(125000);
	Timer1.stop();

	attachInterrupt(digitalPinToInterrupt(Motor::STOPPIN), setStopSoft, FALLING);
	attachInterrupt(digitalPinToInterrupt(Motor::KRANCPIN), setStopHard, FALLING);

	setBusy(false);
	Serial.println("\nSTART");
}


unsigned long prev = 0;
unsigned long act = 0;
void loop()
{
	if (mode.isDebugMode())
	{
		//debugModeFun(const WorkMode & mode, uint8_t KRANCPIN, uint8_t DIRPIN, uint8_t PULSEPIN);
		debugModeFun(mode, Motor::KRANCPIN, Motor::DIRPIN, Motor::PULSEPIN);
		return;
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
		SDP(" ", smsg.recvBuff[i]);
	}
	SDN("]");
}

static void requestEvent()
{
	SDP("sendPos=", smsg.sendPos);SDP(" sizeSendMsg=", smsg.sizeSendMsg);
	SD("Sending : [");
	for (int i=0; i<smsg.sizeSendMsg; ++i) {
		SDP(" ", smsg.sendBuff[smsg.sendPos++]);
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



