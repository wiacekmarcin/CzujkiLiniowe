#include <Arduino.h>
#include "pins_arduino.h"

//SPI SLAVE (ARDUINO)
#include<SPI.h>

#include "crc8.hpp"
#include "proto.hpp"
#include "silnik.hpp"

#define BUSYPIN A0


Motor mot;
Message msg;

volatile bool received;
volatile uint8_t recvBuff[32];
volatile uint8_t sendBuff[32];
volatile uint8_t recvPos;


inline void setBusy(bool busy)
{
	digitalWrite(BUSYPIN, busy);;
}

void setStop()
{
	mot.setStop();
}

#define LEDPIN 9

//#define DEBUG_START
void setup()
{
#ifdef DEBUG_START
	pinMode(A0, OUTPUT);
	pinMode(A1, OUTPUT);
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);
	pinMode(MOSI, OUTPUT);
	pinMode(MISO, OUTPUT);
	pinMode(SCK, OUTPUT);
	pinMode(SS, OUTPUT);
	delay(100);
	digitalWrite(A0, HIGH);
	digitalWrite(A1, HIGH);
	digitalWrite(A2, HIGH);
	digitalWrite(A3, HIGH);
	digitalWrite(MOSI, HIGH);
	digitalWrite(MISO, HIGH);
	digitalWrite(SCK, HIGH);
	digitalWrite(SS, HIGH);
	delay(100);
	digitalWrite(A0, LOW);
	digitalWrite(A1, HIGH);
	digitalWrite(A2, HIGH);
	digitalWrite(A3, HIGH);
	digitalWrite(MOSI, HIGH);
	digitalWrite(MISO, HIGH);
	digitalWrite(SCK, HIGH);
	digitalWrite(SS, HIGH);
	delay(100);
	digitalWrite(A0, LOW);
	digitalWrite(A1, LOW);
	digitalWrite(A2, HIGH);
	digitalWrite(A3, HIGH);
	digitalWrite(MOSI, HIGH);
	digitalWrite(MISO, HIGH);
	digitalWrite(SCK, HIGH);
	digitalWrite(SS, HIGH);
	delay(100);
	digitalWrite(A0, LOW);
	digitalWrite(A1, LOW);
	digitalWrite(A2, LOW);
	digitalWrite(A3, HIGH);
	digitalWrite(MOSI, HIGH);
	digitalWrite(MISO, HIGH);
	digitalWrite(SCK, HIGH);
	digitalWrite(SS, HIGH);
	delay(100);
	digitalWrite(A0, LOW);
	digitalWrite(A1, LOW);
	digitalWrite(A2, LOW);
	digitalWrite(A3, LOW);
	digitalWrite(MOSI, HIGH);
	digitalWrite(MISO, HIGH);
	digitalWrite(SCK, HIGH);
	digitalWrite(SS, HIGH);
	delay(100);
	digitalWrite(A0, LOW);
	digitalWrite(A1, LOW);
	digitalWrite(A2, LOW);
	digitalWrite(A3, LOW);
	digitalWrite(MOSI, LOW);
	digitalWrite(MISO, HIGH);
	digitalWrite(SCK, HIGH);
	digitalWrite(SS, HIGH);
	delay(100);
	digitalWrite(A0, LOW);
	digitalWrite(A1, LOW);
	digitalWrite(A2, LOW);
	digitalWrite(A3, LOW);
	digitalWrite(MOSI, LOW);
	digitalWrite(MISO, LOW);
	digitalWrite(SCK, HIGH);
	digitalWrite(SS, HIGH);
	delay(100);
	digitalWrite(A0, LOW);
	digitalWrite(A1, LOW);
	digitalWrite(A2, LOW);
	digitalWrite(A3, LOW);
	digitalWrite(MOSI, LOW);
	digitalWrite(MISO, LOW);
	digitalWrite(SCK, LOW);
	digitalWrite(SS, HIGH);
	delay(100);
	digitalWrite(A0, LOW);
	digitalWrite(A1, LOW);
	digitalWrite(A2, LOW);
	digitalWrite(A3, LOW);
	digitalWrite(MOSI, LOW);
	digitalWrite(MISO, LOW);
	digitalWrite(SCK, LOW);
	digitalWrite(SS, HIGH);
	delay(100);
	digitalWrite(A0, LOW);
	digitalWrite(A1, LOW);
	digitalWrite(A2, LOW);
	digitalWrite(A3, LOW);
	digitalWrite(MOSI, LOW);
	digitalWrite(MISO, LOW);
	digitalWrite(SCK, LOW);
	digitalWrite(SS, LOW);
	delay(100);
#else

	pinMode(BUSYPIN, OUTPUT);
	pinMode(SS, INPUT);
	pinMode(SCK, INPUT);
	pinMode(MOSI, INPUT);
	pinMode(MISO, OUTPUT);
	pinMode(A0, OUTPUT);
	pinMode(A1, OUTPUT);
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);
	//setBusy(true);
	
	mot.init();
	msg.init();

  	pinMode(LEDPIN,OUTPUT);                 
  	pinMode(MISO,OUTPUT);                   //Sets MISO as OUTPUT (Have to Send data to Master IN 

             //Interuupt ON is set for SPI commnucation



	mot.setStop();
	
	for (uint8_t i = 0; i < 32; ++i) {
		recvBuff[i] = 0;
		sendBuff[i] = 0xCC;
	}

	Serial.begin(115200);

	//attachInterrupt(digitalPinToInterrupt(10), setStop);
	attachInterrupt(digitalPinToInterrupt(mot.KRANCPIN), setStop, FALLING);
	digitalWrite(A0, LOW);
	digitalWrite(A1, LOW);
	digitalWrite(A2, LOW);
	digitalWrite(A3, LOW);
	//setBusy(false);
#endif	
}



ISR (SPI_STC_vect)                        //Inerrrput routine function 
{
	digitalWrite(A1, HIGH);
	uint8_t pos = (recvPos++) & 0x1f;
  	recvBuff[pos] = SPDR;             // Value received from master if store in variable slavereceived
  	SPDR = sendBuff[pos];
  	received = true;
	digitalWrite(A1, LOW);                        //Sets received as True 
}


bool cmd_in_progress = false;
uint8_t lenCmd = 0;
uint8_t actProcess = 0;
void loop()
{ 
  	if(received)                            //Logic to SET LED ON OR OFF depending upon the value recerived from master
  	{
		received = false;
		digitalWrite(A3, HIGH);
		while (actProcess < recvPos) {
			digitalWrite(LEDPIN, HIGH);
			digitalWrite(A2, HIGH);
			//Serial.println(recvBuff[recvPos-1], HEX);
			if (msg.add(recvBuff[actProcess++])) {
				setBusy(true);
				recvPos = 0;
				
				Result status = msg.parse();
				if (status.ok) {
					switch(msg.getMsgCmd()) {
					case LAST_REP:
						for (uint8_t i = 0; i < 32; ++i)
							sendBuff[i] = 0;
						break;
					case CONF_REQ:
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
						msg.copy(sendBuff);	
						break;
					case MOVE_REQ:
						Serial.println("Ruch");
						if (status.data.move.isHome) 
							mot.moveHome();
						else 
							mot.movePosition(status.data.move.steps);
						break;
						msg.copy(sendBuff);	
					default:
						Serial.println("Uknonwn");
						break;
					}
				} else {
					Serial.println("Nie poprawna wiadomosc");
				}
				msg.clear();
				recvPos = 0;
				setBusy(false);
			}
			digitalWrite(A2, LOW);
			digitalWrite(LEDPIN, LOW);
		}
		digitalWrite(A3, LOW);
	}
}

/*
    
    received = false;
    Serial.print("Receive");
    Serial.println(Slavereceived);
    if (Slavereceived=='1') 
    {
      digitalWrite(LEDPIN,HIGH);         //Sets pin 7 as HIGH LED ON
      Serial.println("Slave LED ON");
    } else {
      digitalWrite(LEDPIN,LOW);          //Sets pin 7 as LOW LED OFF
      Serial.println("Slave LED OFF");
    }
    Slavesend=(0xff ^ Slavereceived);                             
    SPDR = Slavesend;                           //Sends the x value to master via SPDR 
    Serial.print("Send");
    Serial.println(Slavesend, HEX);
  }
}
*/
