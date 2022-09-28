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

void setup()
{
	pinMode(BUSYPIN, OUTPUT);
	setBusy(true);
	
	mot.init();
	msg.init();

  	pinMode(LEDPIN,OUTPUT);                 
  	pinMode(MISO,OUTPUT);                   //Sets MISO as OUTPUT (Have to Send data to Master IN 

             //Interuupt ON is set for SPI commnucation

  	digitalWrite(LEDPIN, HIGH);
  	delay(2000);
  	digitalWrite(LEDPIN, LOW);

	mot.setStop();
	
	for (uint8_t i = 0; i < 32; ++i) {
		recvBuff[i] = 0;
		sendBuff[i] = 0;
	}

	Serial.begin(115200);

	//attachInterrupt(digitalPinToInterrupt(10), setStop);
	attachInterrupt(digitalPinToInterrupt(mot.KRANCPIN), setStop, FALLING);

	setBusy(false);
}



ISR (SPI_STC_vect)                        //Inerrrput routine function 
{
	uint8_t pos = (recvPos++) & 0x1f;
  	recvBuff[pos] = SPDR;             // Value received from master if store in variable slavereceived
  	SPDR = sendBuff[pos];
  	received = true;                        //Sets received as True 
}


bool cmd_in_progress = false;
uint8_t lenCmd = 0;
void loop()
{ 
  	if(received)                            //Logic to SET LED ON OR OFF depending upon the value recerived from master
  	{
		if (msg.add(recvBuff[recvPos-1])) {
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
					mot.setReverseMotor(status.data.conf.reverse);
					mot.setMaxSteps(status.data.conf.maxStep);
					mot.setEnabledAlways(status.data.conf.enableAlways);
					mot.setBaseSteps(status.data.conf.baseSteps);
					mot.setDelayImp(status.data.conf.delayImp);
					msg.copy(sendBuff);	
					break;
				case MOVE_REQ:
					if (status.data.move.isHome) 
						mot.moveHome();
					else 
						mot.movePosition(status.data.move.steps);
					break;
					msg.copy(sendBuff);	
				default:
					break;
				}
			}
			msg.clear();
			recvPos = 0;
			setBusy(false);
		}
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
