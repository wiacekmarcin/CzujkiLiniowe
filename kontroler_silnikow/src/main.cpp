#include <Arduino.h>
#include "pins_arduino.h"

//SPI SLAVE (ARDUINO)
#include<SPI.h>

volatile boolean received;
volatile byte Slavereceived,Slavesend;
#define LEDPIN 9
void setup()
{
  Serial.begin(115200);
  pinMode(LEDPIN,OUTPUT);                 // Setting pin 7 as OUTPUT
  pinMode(MISO,OUTPUT);                   //Sets MISO as OUTPUT (Have to Send data to Master IN 

  SPCR |= _BV(SPE);                       //Turn on SPI in Slave Mode
  received = false;
  SPI.attachInterrupt();                  //Interuupt ON is set for SPI commnucation

  digitalWrite(LEDPIN, HIGH);
  delay(2000);
  digitalWrite(LEDPIN, LOW);
}

ISR (SPI_STC_vect)                        //Inerrrput routine function 
{
  Slavereceived = SPDR;         // Value received from master if store in variable slavereceived
  received = true;                        //Sets received as True 
}

void loop()
{ 
  if(received)                            //Logic to SET LED ON OR OFF depending upon the value recerived from master
  {
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

/*
void setup (void)
{
  pinMode(LEDPIN, OUTPUT);
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  // turn on interrupts
  SPCR |= _BV(SPIE);
  pos = 0;
  process_it = false;
  digitalWrite(LEDPIN, HIGH);
  delay(2000);
  digitalWrite(LEDPIN, LOW);
}  // end of setup

// SPI interrupt routine
ISR (SPI_STC_vect)
{
byte c = SPDR;
  
  // add to buffer if room
  if (pos < sizeof buf)
    {
    buf [pos++] = c;
    
    // example: newline means time to process buffer
    if (c == '\n')
      process_it = true;
      
    }  // end of room available
}

// main loop - wait for flag set in interrupt routine
void loop (void)
{
  if (process_it)
    {
      char c = buf[pos-1];
      pos = 0;
      process_it = false;
      if (c == '0')
        digitalWrite(LEDPIN, LOW);
      else if (c == '1')
        digitalWrite(LEDPIN, HIGH);
    }  // end of flag set 
}  // end of loop
*/

