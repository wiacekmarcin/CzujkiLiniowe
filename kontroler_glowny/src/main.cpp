#include <Arduino.h>
#include <SPI.h>
#include "pins_arduino.h"

#include "crc8.hpp"
#include "protocol.hpp"
#include "spi_proto.hpp"
MessageSerial msg;

#define DEBUG

#define TempPin 22

#include <PinChangeInterrupt.h>
#include <TimerOne.h>
constexpr uint8_t maxNumSter = 9;
constexpr uint8_t maxNumSter2 = 1;
constexpr uint8_t resetPins = 53;
constexpr uint8_t ssPins[maxNumSter]    = {49,47,45,43,41,39,37,35,33};
constexpr uint8_t busyPins[maxNumSter]  = {A15,A14,A13,A12,A11,A10,A9,A8,10};
constexpr uint8_t stopPins[maxNumSter]  = {48,46,44,42,40,38,36,34,32};

SPIMessage motors[maxNumSter];

static void readSerial();
static void configurationLocal();
static void welcomeMsg();
static void configuration();
static void moveSteps();


//przerwanie od timer zeby odczytac wartosc mierzona napiecia i napiecia
volatile bool timeout = false;

static void phex(uint8_t b)
{
    Serial1.print(" ");
    if (b < 16)
        Serial1.print("0");
    Serial1.print(b, HEX);
}


void timerHandler()
{
    //timeout = true;
    //Serial1.println("IRQ T");
}


#define FINISHJOB(N) \
void isrFinishJob##N() \
{\
    digitalWrite(26, HIGH);\
    motors[N].setFinish();\
    digitalWrite(26, LOW);\
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
    pinMode(26, OUTPUT);
    digitalWrite(26, HIGH);

    
    
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

    for (uint8_t p = 0; p < maxNumSter2; ++p) {
        pinMode(ssPins[p], OUTPUT); digitalWrite(ssPins[p], HIGH);
        pinMode(stopPins[p], OUTPUT); digitalWrite(stopPins[p], HIGH);
        pinMode(busyPins[p], INPUT);
        attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(busyPins[p]), funptr[p], FALLING);
        motors[p].init(p+1, ssPins[p], stopPins[p], &msg);
    }

    Serial.begin(115200);
#ifdef DEBUG    
    Serial1.begin(115200);
#endif    
    //delay(1000);

    //Timer1.initialize(10000000);
    //Timer1.attachInterrupt(timerHandler);


    digitalWrite(resetPins, HIGH);
    digitalWrite(26, LOW);
}  // end of setup


uint8_t indexMotor = 0;
uint8_t indexMotorReply = 0;

MessageSerial::Work actWork = MessageSerial::NOP;
bool runLoop = false;
void loop (void)
{

    readSerial();
        
    switch(actWork) {
    case MessageSerial::WELCOME_MSG: welcomeMsg(); break;
    case MessageSerial::CONFIGURATION: configuration(); break; 
    case MessageSerial::CONFIGURATION_LOCAL: configurationLocal(); break; 
    case MessageSerial::MOVE_REQUEST: moveSteps(); break;
    default: break;
    }

    if (timeout) {
        if (indexMotor == 0) {
            motors[indexMotor].sendProgressMsg();

        }
        //else
        //    delayMicroseconds(200);
        
    
        if (++indexMotor == 9) {
            timeout = false;
            indexMotor = 0;
        }
    }

    motors[indexMotorReply++].checkIsDone();

    if (indexMotorReply == 9) {
        indexMotorReply = 0;
    }    
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
    uint8_t motor = msg.getAddress()-1;
    motors[motor].setConfiguration(msg.msg(), msg.len());
    motors[motor].sendConfiguration(true);
    actWork = MessageSerial::NOP;
}

void moveSteps()
{
    uint8_t motor = msg.getAddress()-1;
    motors[motor].moveSteps(msg.msg(), msg.len(), (msg.getOptions() & 0x01) == 0x01);
    actWork = MessageSerial::NOP;
}

void sendProgressMsg(uint8_t index)
{
    motors[index].sendProgressMsg();
}



