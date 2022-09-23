#include <Arduino.h>
#include <SPI.h>
#include "pins_arduino.h"

#define PIN_P1 46
#define PIN_P2 44
#define PIN_P3 42
#define PIN_RES 40

void setup (void)
{
  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();
  //SPI.setClockDivider(SPI_CLOCK_DIV128);
  
  Serial.begin(9600);

  pinMode(PIN_P1, OUTPUT);
  pinMode(PIN_P2, OUTPUT);
  pinMode(PIN_P3, OUTPUT);
  pinMode(PIN_RES, OUTPUT);

  digitalWrite(PIN_RES, LOW);
  digitalWrite(PIN_P1, LOW);
  digitalWrite(PIN_P2, LOW);
  digitalWrite(PIN_P3, LOW);
  delay(1000);
  digitalWrite(PIN_RES, HIGH);
  delay(1000);
  digitalWrite(PIN_P1, HIGH);
  delay(1000);
  digitalWrite(PIN_P2, HIGH);
  delay(1000);
  digitalWrite(PIN_P3, HIGH);
  delay(1000);
}  // end of setup

volatile char buf[5];
volatile byte pos = 0;

void loop (void)
{
  if (Serial.available() > 0) {
    int c = Serial.read();
    if (c > 0) {
      buf[pos++] = c;
      if (c == '\n') {
        if (buf[0] == 'A') {
          Serial.print("\nTransfer=");
          Serial.print(buf[1]);
          Serial.println("Read=");
          digitalWrite(PIN_P1, LOW); 
          Serial.println(SPI.transfer (buf[1]), HEX);
          digitalWrite(PIN_P1, HIGH);
          pos = 0;
        } else if (buf[0] == 'B') {
          Serial.print("\nTransfer=");
          Serial.print(buf[1]);
          Serial.println("Read=");
          digitalWrite(PIN_P2, LOW); 
          Serial.println(SPI.transfer (buf[1]), HEX);
          digitalWrite(PIN_P2, HIGH);
          pos = 0;
        } else if (buf[0] == 'C') {
          Serial.print("\nTransfer=");
          Serial.print(buf[1]);
          Serial.println("Read=");
          digitalWrite(PIN_P3, LOW); 
          Serial.println(SPI.transfer (buf[1]), HEX);
          digitalWrite(PIN_P3, HIGH);
          pos = 0;
        } else if (buf[0] == 'R') {
          Serial.println("\nReset");
          digitalWrite(PIN_RES, LOW);
          delay(100);
          digitalWrite(PIN_RES, HIGH);
          pos = 0;
          return;
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
