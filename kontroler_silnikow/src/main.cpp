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

Motor mot;
Message msg;

volatile bool received;
volatile uint8_t recvBuff[32];
volatile uint8_t sendBuff[32];
volatile uint8_t recvPos;
uint8_t addr = 0;


inline void setBusy(bool busy)
{
	digitalWrite(BUSYPIN, busy);;
}

void setStop()
{
	mot.setStop();
	Timer1.stop();
}

void readMeasurement()
{
    mot.impulse();
}


void setup()
{
	pinMode(BUSYPIN, OUTPUT);
	setBusy(true);
	pinMode(SS, INPUT);
	pinMode(SCK, INPUT);
	pinMode(MOSI, INPUT);
	pinMode(MISO, OUTPUT);
	pinMode(A0, OUTPUT);


	
	mot.init();
	msg.init();

  	pinMode(MISO,OUTPUT);                   //Sets MISO as OUTPUT (Have to Send data to Master IN 

    //Interuupt ON is set for SPI commnucation



	mot.setStop();
	
	for (uint8_t i = 0; i < 32; ++i) {
		recvBuff[i] = 0;
		sendBuff[i] = 0xCC;
	}

	Serial.begin(115200);

	attachInterrupt(digitalPinToInterrupt(STOPPIN), setStop, FALLING);
	attachInterrupt(digitalPinToInterrupt(mot.KRANCPIN), setStop, FALLING);


	Timer1.initialize(100000);
    Timer1.attachInterrupt(readMeasurement);
	Timer1.stop();
	setBusy(false);
}




ISR (SPI_STC_vect)                        //Inerrrput routine function 
{
	uint8_t pos = (recvPos++) & 0x1f;
  	recvBuff[pos] = SPDR;             // Value received from master if store in variable slavereceived
  	SPDR = sendBuff[pos];
  	received = true;
}


bool cmd_in_progress = false;
uint8_t lenCmd = 0;
uint8_t actProcess = 0;
void loop()
{ 
  	if(received)                            //Logic to SET LED ON OR OFF depending upon the value recerived from master
  	{
		received = false;
		while (actProcess < recvPos) {
			if (msg.add(recvBuff[actProcess++])) {
				setBusy(true);
				recvPos = 0;
				actProcess = 0;
				
				Result status = msg.parse();
				if (status.ok) {
					uint8_t addr = msg.getAddr();
					switch(msg.getMsgCmd()) {
					case LAST_REQ:
						for (uint8_t i = 0; i < 32; ++i)
							sendBuff[i] = 0;
						setBusy(false);
						break;
					
					case ECHO_REQ:
						CRC8 crc;
						crc.restart();
						sendBuff[0] = (ECHO_REP << 4) & 0xf0;
						sendBuff[1] = ((addr << 4) & 0xf0) | 0x08;
						crc.add(sendBuff[0]);
						crc.add(sendBuff[1]);
						sendBuff[2] = crc.getCRC();
						setBusy(false);
						break;

					case PROGRESS_REQ:
						CRC8 crc;
						crc.restart();
						sendBuff[0] = (( PROGRESS_REP << 4) & 0xf0) | 0x04;
						sendBuff[1] = ((addr << 4) & 0xf0) | 0x08;
						if (mot.getIsMoveHome())
							sendBuff[1] |= 0x04;
						crc.add(sendBuff[0]);
						crc.add(sendBuff[1]);
						int pos = mot.getGlobalPos();
						sendBuff[2] = (uint8_t)(pos >> 24) & 0xff;
						sendBuff[3] = (uint8_t)(pos >> 16) & 0xff;
						sendBuff[4] = (uint8_t)(pos >> 8) & 0xff;
						sendBuff[5] = (uint8_t)(pos) & 0xff;
						crc.add(sendBuff[2]);
						crc.add(sendBuff[3]);
						crc.add(sendBuff[4]);
						crc.add(sendBuff[5]);
						sendBuff[6] = crc.getCRC();
						setBusy(false);
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
						Timer1.setPeriod(status.data.conf.delayImp);
						msg.copy(sendBuff);	
						break;

					case MOVE_REQ:
						Serial.print("Ruch. Home = ");
						Serial.print(status.data.move.isHome ? "Tak" : "Nie");
						Serial.print(" Kroki = ");
						Serial.println(status.data.move.steps, DEC);
						if (status.data.move.isHome) {
							mot.moveHome();
						} else {
							mot.movePosition(status.data.move.steps);
						}
						break;
						
					default:
						Serial.println("Uknonwn");
						break;
					}
				} else {
					Serial.println("Nie poprawna wiadomosc");
				}
				msg.clear();
				recvPos = 0;
				
			}
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
