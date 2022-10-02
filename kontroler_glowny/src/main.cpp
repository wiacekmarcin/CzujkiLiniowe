#include <Arduino.h>
#include <SPI.h>
#include "pins_arduino.h"

#include "crc8.hpp"

#include <PinChangeInterrupt.h>
#include <TimerOne.h>

#define PIN_S1 46
#define PIN_S2 44
#define PIN_S3 42
#define PIN_RES 40

void finishJob()
{
  Serial.println("F");
}

void readMeasurement()
{
  Serial.print(".");
}

void setup (void)
{
  pinMode(SCK, OUTPUT);
  digitalWrite(SCK, LOW);
  pinMode(MISO, INPUT);
  pinMode(MOSI, OUTPUT);
  digitalWrite(MOSI, LOW);
  pinMode(PIN_S1, OUTPUT);
  digitalWrite(PIN_S1, HIGH);
  pinMode(PIN_S2, OUTPUT);
  digitalWrite(PIN_S2, HIGH);
  pinMode(PIN_S3, OUTPUT);
  digitalWrite(PIN_S3, HIGH);
  pinMode(PIN_RES, OUTPUT);
  digitalWrite(PIN_RES, HIGH);
  Serial.begin(115200);
  pinMode(A9, INPUT);

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();
  SPI.setClockDivider(SPI_CLOCK_DIV128);

  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(A9), finishJob, FALLING);

  Timer1.initialize(100000);
  Timer1.attachInterrupt(readMeasurement);
  
}  // end of setup

volatile char buf[5];
volatile byte pos = 0;
volatile uint32_t counterMsg = 0;





void sendConf(uint8_t addr, bool en, bool reverse, uint32_t maxStep, uint16_t baseStep, uint16_t delayImp)
{
	++counterMsg;
  uint8_t sendBuf[] = { 	
    uint8_t(0x20), 
    uint8_t(addr*0x10), 
    uint8_t((counterMsg >> 24) & 0xff), 
    uint8_t((counterMsg >> 16) & 0xff), 
    uint8_t((counterMsg >> 8) & 0xff), 
    uint8_t(counterMsg & 0xff), 
    uint8_t((0x02 * en) + (0x01 * reverse)), 
    uint8_t((maxStep >> 24) & 0xff), 
    uint8_t((maxStep >> 16) & 0xff), 
    uint8_t((maxStep >> 8) & 0xff),
    uint8_t(maxStep & 0xff), 
    uint8_t((baseStep >> 8) & 0xff), 
    uint8_t(baseStep & 0xff), 
    uint8_t((delayImp >> 8) & 0xff), 
    uint8_t(delayImp & 0xff),
    uint8_t(0xff)
  };
	uint8_t lenMsg = sizeof(sendBuf)/sizeof(uint8_t);
	sendBuf[0] = sendBuf[0] | ((lenMsg - 1 - 1 - 4 - 1) & 0x0f);
	CRC8 c;
    c.reset();
    for (int i=0; i< lenMsg-1; ++i ) {
      c.add(*(sendBuf+i));
		Serial.print(sendBuf[i], HEX);
		Serial.print(" ");
	}

  uint8_t pinMask[] = { 0x00, PIN_S1, PIN_S2, PIN_S3 };
	sendBuf[lenMsg-1] = c.getCRC();
	Serial.print("crc=");
	Serial.print(sendBuf[lenMsg-1], HEX);
	digitalWrite(pinMask[addr], LOW); 
  SPI.transfer(sendBuf, lenMsg);
  digitalWrite(pinMask[addr], HIGH); 
	
}


void sendMove(uint8_t addr, bool home, uint32_t steps)
{
  ++counterMsg;
  uint8_t sendBuf[] = { 	
    uint8_t(0x40), 
    uint8_t(addr*0x10 | (home * 0x01)), 
    uint8_t((counterMsg >> 24) & 0xff), 
    uint8_t((counterMsg >> 16) & 0xff), 
    uint8_t((counterMsg >> 8) & 0xff), 
    uint8_t(counterMsg & 0xff), 
    uint8_t((steps >> 24) & 0xff), 
    uint8_t((steps >> 16) & 0xff), 
    uint8_t((steps >> 8) & 0xff),
    uint8_t(steps & 0xff), 
    uint8_t(0xff)
  };
	uint8_t lenMsg = sizeof(sendBuf)/sizeof(uint8_t);
	sendBuf[0] = sendBuf[0] | ((lenMsg - 1 - 1 - 4 - 1) & 0x0f);
	CRC8 c;
  c.reset();
  for (int i=0; i< lenMsg-1; ++i ) {
    c.add(*(sendBuf+i));
    Serial.print(sendBuf[i], HEX);
    Serial.print(" ");
	}

  uint8_t pinMask[] = { 0x00, PIN_S1, PIN_S2, PIN_S3 };
	sendBuf[lenMsg-1] = c.getCRC();
	Serial.print("crc=");
	Serial.print(sendBuf[lenMsg-1], HEX);
	digitalWrite(pinMask[addr], LOW); 
  SPI.transfer(sendBuf, lenMsg);
  digitalWrite(pinMask[addr], HIGH); 

  //while (digitalRead())
}

void loop (void)
{
  if (Serial.available() > 0) {
    int c = Serial.read();
    if (c > 0) {
      buf[pos++] = c;
      if (c == '\n') {
        if (buf[0] == 'A') {
          Serial.print("\nKonfiguracja: ");
		      sendConf(buf[1]-'0', true, false, 32000, 100, 140);
          pos = 0;
        } else if (buf[0] == 'a') {
          Serial.print("\nKonfiguracja: ");
		      sendConf(buf[1]-'0', false, true, 32000, 100, 200);
          pos = 0;
        }else if (buf[0] == 'B') {
          Serial.print("\nMove Home");
          sendMove(buf[1]-'0', true, 0);
          pos = 0;
        } else if (buf[0] == 'C') {
          Serial.print("\nMove Steps");
          sendMove(buf[1]-'0', false, 1000);
          pos = 0;
        } else if (buf[0] == 'D') {
          Serial.print("\nMove Steps");
          sendMove(buf[1]-'0', false, 100);
          pos = 0;
        }
      } else {
        if (pos > 5) {
          pos = 0;
        }
        Serial.print((char)c);
      }
    }
  }
}
