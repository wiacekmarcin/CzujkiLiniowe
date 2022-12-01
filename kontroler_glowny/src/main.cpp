#include <Arduino.h>
#include <SPI.h>
#include "pins_arduino.h"

#include "crc8.hpp"
#include "protocol.hpp"
#include "spi_proto.hpp"
MessageSerial msg;

#define DEBUG

#include <PinChangeInterrupt.h>
#include <TimerOne.h>
constexpr uint8_t maxNumSter = 9;
constexpr uint8_t resetPins = 53;
constexpr uint8_t ssPins[maxNumSter]    = {32,33,34,35,36,37,38,39,40};
constexpr uint8_t busyPins[maxNumSter]  = {A8,A9,A10,A11,A12,A13,A14,A15,15};
constexpr uint8_t stopPins[maxNumSter]  = {30,29,28,27,26,25,24,23,22};
constexpr uint8_t movePins[maxNumSter]  = {41,42,43,44,45,46,47,48,49};

SPIMessage motors[maxNumSter];

static void readSerial();
static void configurationLocal();
static void welcomeMsg();
static void configuration();
static void moveSteps();
static void sendProgressMsg(uint8_t index);

//przerwanie od timer zeby odczytac wartosc mierzona napiecia i napiecia
volatile bool timeout = false;

static void phex(uint8_t b)
{
    Serial1.print(" ");
    if (b < 16)
        Serial1.print("0");
    Serial1.print(b, HEX);
}

volatile short checkProgres = 0;
void timerHandler()
{
    checkProgres = maxNumSter;
}


#define FINISHJOB(N) \
void isrFinishJob##N() \
{\
    motors[N].setFinish();\
    Serial1.print("F");\
    Serial1.println(N);\
}

FINISHJOB(0)
FINISHJOB(1)
FINISHJOB(2)
FINISHJOB(3)
FINISHJOB(4)
FINISHJOB(5)
FINISHJOB(6)
FINISHJOB(7)
FINISHJOB(8)


typedef void (*isr_bust_fun)(void);
isr_bust_fun funptr[maxNumSter] = {isrFinishJob0, isrFinishJob1, isrFinishJob2, isrFinishJob3, isrFinishJob4, isrFinishJob5, isrFinishJob6, isrFinishJob7, isrFinishJob8};

void setup (void)
{
    pinMode(resetPins, OUTPUT); 
    digitalWrite(resetPins, LOW);
    delay(50);
    digitalWrite(resetPins, HIGH);
    delay(1450);
    
    // Put SCK, MOSI, SS pins into output mode
    // also put SCK, MOSI into LOW state, and SS into HIGH state.
    // Then put SPI hardware into Master mode and turn SPI on
    SPI.begin ();
    //
    SPI.setClockDivider(SPI_CLOCK_DIV128);

    /* piny SPI */
    pinMode(SCK, OUTPUT);
    digitalWrite(SCK, LOW);
    pinMode(MISO, INPUT);
    pinMode(MOSI, OUTPUT);
    digitalWrite(MOSI, LOW);

    for (uint8_t p = 0; p < maxNumSter; ++p) {
        pinMode(ssPins[p], OUTPUT); digitalWrite(ssPins[p], HIGH);
        pinMode(stopPins[p], OUTPUT); digitalWrite(stopPins[p], HIGH);
        pinMode(busyPins[p], INPUT);
        pinMode(movePins[p], INPUT);
        //attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(busyPins[p]), funptr[p], RISING);
        motors[p].init(p+1, ssPins[p], stopPins[p], movePins[p], busyPins[p],&msg);
    }

    Serial.begin(115200);
#ifdef DEBUG    
    Serial1.begin(115200);
#endif    
    //delay(1000);

    Timer1.initialize(5000000);
    Timer1.attachInterrupt(timerHandler);


    delay(100);
    
}  // end of setup


uint8_t indexMotor = 0;
uint8_t indexMotorReply = 0;

MessageSerial::Work actWork = MessageSerial::NOP;
bool runLoop = false;
uint32_t loopsCnt = 500000;
uint8_t cntIsFinishCnt = 0;
uint8_t FirstLoop = 10;

#define SKIP_MOD 

void readAll() {
    Serial1.print("BUSY:");
    for (uint8_t p = 0 ; p < 9; ++p) {
        Serial1.print(digitalRead(busyPins[p]), DEC);
        Serial1.print(" ");
    }
    Serial1.print("\nMOVE:");
    for (uint8_t m = 0 ; m < 9; ++m) {
        Serial1.print(digitalRead(movePins[m]), DEC);
        Serial1.print(" ");
    }
    Serial1.println();
}


void loop (void)
{
    for (uint8_t mod = 0; mod < 9 ; ++mod) {
        SKIP_MOD
        //Serial1.print("M");
        //Serial1.print(mod+1);
        //Serial1.println(" SS LOW");
        digitalWrite(ssPins[mod], LOW);
        delayMicroseconds(40);
        if (digitalRead(busyPins[mod]) != LOW) {
            Serial.print("M");
            Serial.print(mod+1);
            Serial.print(" SS LOW - ");
            Serial.println("SS/BUSY LOW ERR\n");
            //delay(5000);
        }
        //Serial.print("BEF:");
        //readAll();
        //delay(2000);
        //Serial.print("AFT2s:");
        //readAll();
        
        //Serial.print("M");
        //Serial.print(mod+1);
        //Serial.println(" SS HIGH");
        digitalWrite(ssPins[mod], HIGH);
        delayMicroseconds(40);
        if (digitalRead(busyPins[mod]) != HIGH) {
            Serial.print("M");
            Serial.print(mod+1);
            Serial.print(" SS LOW - ");
            Serial.println("SS/BUSY HIGH ERR\n");
            //delay(5000);
        }
        //Serial.print("BEF:");
        //readAll();
        //delay(2000);
        //Serial.print("AFT2s:");
        //readAll();
        
        //Serial.print("M");
        //Serial.print(mod+1);
        //Serial.println(" STOP LOW");
        digitalWrite(stopPins[mod], LOW);
        delayMicroseconds(40);
        if (digitalRead(movePins[mod]) != LOW) {
            Serial.print("M");
            Serial.print(mod+1);
            Serial.print(" STOP LOW -");
            Serial.println("STOP/MOVE LOW ERR\n");
            //delay(5000);
        }
        
        //Serial.print("BEF:");
        //readAll();
        //delay(2000);
        //Serial.print("AFT2s:");
        //readAll();
       

        //Serial.print("M");
        //Serial.print(mod+1);
        //Serial.println(" STOP HIGH");
        digitalWrite(stopPins[mod], HIGH);
        delayMicroseconds(40);
        if (digitalRead(movePins[mod]) != HIGH) {
            Serial.print("M");
            Serial.print(mod+1);
            Serial.print(" STOP HIGH - ");
            Serial.println("STOP/MOVE HIGH ERR\n");
            //delay(5000);
        }
        //Serial.print("BEF:");
        //readAll();
        //delay(2000);
        //Serial.print("AFT2s:");
        //readAll();

    }

    return;

    for (uint8_t mod = 0; mod < 9 ; ++mod) {
        SKIP_MOD
        Serial1.print("M");
        Serial1.print(mod+1);
        Serial1.println(" BUSY LOW");
        digitalWrite(ssPins[mod], LOW);
        if (digitalRead(busyPins[mod]) != LOW) {
            //Serial1.print("SS/BUSY LOW ERR\n");
            //delay(5000);
        }
        delay(200);
    }

    for (uint8_t mod = 0; mod < 9 ; ++mod) {
        SKIP_MOD
        Serial1.print("M");
        Serial1.print(mod+1);
        Serial1.println(" BUSY HIGH");
        digitalWrite(ssPins[mod], HIGH);
        if (digitalRead(busyPins[mod]) != HIGH) {
            //Serial1.print("SS/BUSY HIGH ERR\n");
            //delay(5000);
        }
        delay(200);
    }
        
    for (uint8_t mod = 0; mod < 9 ; ++mod) {
        SKIP_MOD
        Serial1.print("M");
        Serial1.print(mod+1);
        Serial1.println(" STOP LOW");
        digitalWrite(stopPins[mod], LOW);
        if (digitalRead(movePins[mod]) != LOW) {
            //Serial1.print("STOP/MOVE LOW ERR\n");
            //delay(5000);
        }
        delay(200);
    }

    for (uint8_t mod = 0; mod < 9 ; ++mod) {
        SKIP_MOD
        Serial1.print("M");
        Serial1.print(mod+1);
        Serial1.println(" STOP HIGH");
        digitalWrite(stopPins[mod], HIGH);
        if (digitalRead(movePins[mod]) != HIGH) {
            //Serial1.print("STOP/MOVE HIGH ERR\n");
            //delay(5000);
        }
        delay(200);
    }



    return;

    if (FirstLoop--) {
        FirstLoop = false;
           Serial.println("Wysylam do 6");
           motors[5].sendEchoMsg(); 
           delay(500);
           motors[5].sendReplyMsg();
           delay(5000);
           Serial.println("Wysylam do 7");
           motors[6].sendEchoMsg(); 
           delay(500);
           motors[6].sendReplyMsg();
           delay(5000);
           Serial.println("Wysylam do 8");
           motors[7].sendEchoMsg(); 
           delay(500);
           motors[7].sendReplyMsg();
           delay(20000);
    }


    return ;
    readSerial();
        
    switch(actWork) {
    case MessageSerial::WELCOME_MSG: welcomeMsg(); break;
    case MessageSerial::CONFIGURATION: configuration(); break; 
    case MessageSerial::CONFIGURATION_LOCAL: configurationLocal(); break; 
    case MessageSerial::MOVE_REQUEST: moveSteps(); break;
    default: break;
    }

    loopsCnt--;
    if (loopsCnt < 10) {
        loopsCnt = 500000;
        Serial1.println("Czekam na komende");
    }

    if (checkProgres > 0) {
        motors[--checkProgres].sendProgressMsg();
    }

    motors[cntIsFinishCnt++ % 9].checkIsDone();
}


void readSerial()
{
    if (Serial.available())  {
        int c = Serial.read();
#ifdef DEBUG
        Serial1.print(c, HEX);
#endif                
        if (msg.check(c)) {
            actWork = msg.getStatusWork();
#ifdef DEBUG
        Serial1.print("\nactWork=");
        Serial1.println(actWork, DEC);
#endif                  
            return;
        }
    }
}

void configurationLocal()
{
#ifdef DEBUG
    Serial1.println("Configuration Local");
#endif        

}

void welcomeMsg()
{
#ifdef DEBUG
    Serial1.println("Welcome Msg");
#endif        
    msg.sendWelcomeMsg();
    actWork = MessageSerial::NOP;
    return;
}

void configuration()
{
#ifdef DEBUG
    Serial1.println("Konfiguracja Msg");
#endif  
    uint8_t motor = msg.getAddress()-1;
    motors[motor].setConfiguration(msg.msg(), msg.len());
    motors[motor].sendConfiguration(true);
    actWork = MessageSerial::NOP;
}

void moveSteps()
{
#ifdef DEBUG
    Serial1.println("Ruch Msg");
#endif 
    uint8_t motor = msg.getAddress()-1;
    motors[motor].moveSteps(msg.msg(), msg.len(), (msg.getOptions() & 0x01) == 0x01);
    actWork = MessageSerial::NOP;
}


