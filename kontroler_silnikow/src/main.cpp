#include <Arduino.h>
#include "pins_arduino.h"

// SPI SLAVE (ARDUINO)
#include <SPI.h>
#include <TimerOne.h>

#include "crc8.hpp"
#include "proto.hpp"
#include "silnik.hpp"
#include "filtr.hpp"
#include "workmode.hpp"
#include "spi_msg.hpp"
#include "main.h"

#define BUSYPIN A0

//#define MAINDEBUG

Motor *mot; 
WorkMode mode;
SPIMessage smsg;

static constexpr uint8_t FB = 0x0A;

void setBusy(bool busy)
{
	digitalWrite(BUSYPIN, busy ? LOW : HIGH);
	if (!busy) {
		SPDR = FB;
		smsg.sendPos = 0;

	}
}

volatile bool createStopResponse;
void setCreateStopMessageFun()
{
	createStopResponse = true;
}

void setStopSoft()
{
	mot->setStop(false);
}

void setStopHard()
{
	mot->setStop(true);
}

void motorImpulse()
{
	mot->impulse();
}


void setup()
{

	pinMode(BUSYPIN, OUTPUT);
	setBusy(false);

	mode.init();
	mot = new Filtr();
	mot->init();
	digitalWrite(Motor::ENPIN, (mode.isDebugMode() && mode.getMode() == WorkMode::CHECKKRANC) ? HIGH : LOW);
	if (mode.isDebugMode()) {
		Serial.println("DEBUG MODE");
		return;
	}

	smsg.init(mode.getMode(), mot);
	SPI.attachInterrupt(); 
	SPCR |= _BV(SPE);
	pinMode(MISO, OUTPUT); //                       //Turn on SPI in Slave Mode
  	 

	Timer1.attachInterrupt(motorImpulse);
	Timer1.initialize(125000);

	attachInterrupt(digitalPinToInterrupt(Motor::STOPPIN), setStopSoft, FALLING);
	attachInterrupt(digitalPinToInterrupt(Motor::KRANCPIN), setStopHard, FALLING);

	setBusy(false);
	Serial.println("\nSTART");
}

volatile bool received = false;
ISR(SPI_STC_vect) // Inerrrput routine function
{
	smsg.recvBuff[smsg.recvPos++] = SPDR; // Value received from master if store in variable slavereceived
	SPDR = smsg.sendBuff[smsg.sendPos++];
	//smsg.recvPos = (smsg.recvPos + 1) & 0x3f;
	//smsg.sendPos = (smsg.sendPos + 1) & 0x1f;
	received = true;
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

void phex(uint8_t b)
{
	Serial.print(" ");
	if (b < 16)
		Serial.print("0");
	Serial.print(b, HEX);
}



