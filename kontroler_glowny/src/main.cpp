#include <Arduino.h>
#include <SPI.h>
#include "pins_arduino.h"

#define PIN_P1 46
#define PIN_P2 44
#define PIN_P3 42
#define PIN_RES 40

//wiadomosc od mastera
//ustawianie konfiguracji
 
 //1b |typ wiadomosci 
 //2b | 00 - nope, 01 - ustaw konfigruacje, 10 - ruch, 11 - powrot do bazy
 //3-8 dlugosc wiadomosci 64 bajty max

//typ wiadomosci
 //00b - nope (dlugosc 0)
 //01b - konfiguracja
 //10b - ustaw pozycje 5 bajtow
 //11b - ustaw pozycje bazowa

 //numer silnika - caly bajt tutaj ignorujemy - poprostu przepiszemy wiadomosc
 //3b - kierunek obrotow lewy czy prawy
 //4b czy trzymac en - miedzy ruchami
 
 //5b | dlugosc wiadomosci
 //6b |
 //7b |
 //8b |




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
