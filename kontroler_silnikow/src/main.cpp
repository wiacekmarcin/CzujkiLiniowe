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
#define DEBUG
Motor mot;
Message msg;

volatile bool received;
constexpr uint8_t maxBuff = 32;
volatile uint8_t recvBuff[maxBuff];
volatile uint8_t sendBuff[maxBuff];
volatile uint8_t recvPos;
volatile uint8_t sendPos;
uint8_t addr = 0;

void phex2(uint8_t b)
{
    Serial.print(" ");
    if (b < 10)
        Serial.print("0");
    Serial.print(b, HEX);
}

inline void setBusy(bool busy)
{
	digitalWrite(BUSYPIN, busy ? HIGH : LOW);;
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
	setBusy(false);
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
	
	for (uint8_t i = 0; i < maxBuff; ++i) {
		recvBuff[i] = 0;
		sendBuff[i] = 0;
	}
#ifdef DEBUG
	Serial.begin(115200);
#endif // DEBUG


	attachInterrupt(digitalPinToInterrupt(STOPPIN), setStop, FALLING);
	attachInterrupt(digitalPinToInterrupt(mot.KRANCPIN), setStop, FALLING);


	Timer1.initialize(100000);
    Timer1.attachInterrupt(readMeasurement);
	Timer1.stop();
	setBusy(false);
}




ISR (SPI_STC_vect)                        //Inerrrput routine function 
{
  	recvBuff[recvPos] = SPDR;             // Value received from master if store in variable slavereceived
  	SPDR = sendBuff[sendPos];
	recvPos = ++recvPos & 0x1f;
	sendPos = ++sendPos & 0x1f;
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
		setBusy(true);
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
					{
						for (uint8_t i = 0; i < maxBuff; ++i)
							sendBuff[i] = 0;
						setBusy(false);
						break;
					}

					case ECHO_REQ:
					{
						CRC8 crc;
						crc.restart();
						sendBuff[0] = 3;
						sendBuff[1] = (ECHO_REP << 4) & 0xf0;
						sendBuff[2] = ((addr << 4) & 0xf0) | 0x08;
						crc.add(sendBuff[1]);
						crc.add(sendBuff[2]);
						sendBuff[3] = crc.getCRC();
						sendBuff[4] = 0;
						sendBuff[5] = 0xff;
						sendPos = 0;
						break;
					}

					case PROGRESS_REQ:
					{
						CRC8 crc;
						crc.restart();
						sendBuff[0] = 0x07;
						sendBuff[1] = (( PROGRESS_REP << 4) & 0xf0) | 0x04;
						sendBuff[2] = ((addr << 4) & 0xf0) | 0x08;
						if (mot.getIsMoveHome())
							sendBuff[2] |= 0x04;
						crc.add(sendBuff[1]);
						int pos = mot.getGlobalPos();
						sendBuff[2] = (uint8_t)(pos >> 24) & 0xff;
						sendBuff[3] = (uint8_t)(pos >> 16) & 0xff;
						sendBuff[4] = (uint8_t)(pos >> 8) & 0xff;
						sendBuff[5] = (uint8_t)(pos) & 0xff;
						crc.add(sendBuff[2]);
						int32_t pos = mot.getGlobalPos();
						Serial.print("POS=");
						Serial.println(pos, DEC);
						sendBuff[3] = (uint8_t)((pos >> 24) & 0xff);
						sendBuff[4] = (uint8_t)((pos >> 16) & 0xff);
						sendBuff[5] = (uint8_t)((pos >> 8) & 0xff);
						sendBuff[6] = (uint8_t)((pos) & 0xff);
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
						break;
					}

					case CONF_REQ:
					{
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
						break;
					}

					case MOVE_REQ:
					{
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
						break;
					}
						
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
		setBusy(false);
		recvPos = 0;
		sendPos = 0;
		actProcess = 0;
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
