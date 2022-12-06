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
constexpr uint8_t resetPin = 49;

constexpr uint8_t ssPins[maxNumSter]    = {31,32,33,34,35,36,37,38,39};
constexpr uint8_t busyPins[maxNumSter]  = {A8,A9,A10,A11,A12,A13,A14,A15,10};
constexpr uint8_t stopPins[maxNumSter]  = {30,29,28,27,26,25,24,23,22};
constexpr uint8_t movePins[maxNumSter]  = {40,41,42,43,44,45,46,47,48};

SPIMessage motors[maxNumSter];

static void readSerial();
static void configurationLocal();
static void welcomeMsg();
static void configuration();
static void moveSteps();
//static void sendProgressMsg(uint8_t index);

//przerwanie od timer zeby odczytac wartosc mierzona napiecia i napiecia
volatile bool timeout = false;

static void phex(uint8_t b)
{
    Serial1.print(" ");
    if (b < 16)
        Serial1.print("0");
    Serial1.print(b, HEX);
}

//volatile short checkProgres = 0;
volatile bool checkProgress = false;
void timerHandler()
{
    //checkProgres = maxNumSter;
    checkProgress = true;
}

volatile uint16_t activeBusy = 0x0000;
volatile uint16_t acceptBusy = 0x01ff;

#define FINISHJOB(N) \
void isrFinishJob##N() \
{\
    activeBusy |= (1 << N) & acceptBusy; \
    acceptBusy |= 1 << N; \
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
    pinMode(resetPin, OUTPUT); 
    digitalWrite(resetPin, LOW);
    delay(50);
    digitalWrite(resetPin, HIGH);
    Serial.begin(115200);
#ifdef DEBUG    
    Serial1.begin(115200);
#endif  

    //delay(1450);
    
    /* piny SPI */
    pinMode(SCK, OUTPUT);
    digitalWrite(SCK, LOW);
    pinMode(MISO, INPUT);
    pinMode(MOSI, OUTPUT);
    digitalWrite(MOSI, LOW);
    // Put SCK, MOSI, SS pins into output mode
    // also put SCK, MOSI into LOW state, and SS into HIGH state.
    // Then put SPI hardware into Master mode and turn SPI on
    SPI.begin ();
    //
    SPI.setClockDivider(SPI_CLOCK_DIV128);

    

    for (uint8_t p = 0; p < maxNumSter; ++p) {
        if (p+1 != 9)
            continue;
        pinMode(ssPins[p], OUTPUT); digitalWrite(ssPins[p], HIGH);
        pinMode(stopPins[p], OUTPUT); digitalWrite(stopPins[p], HIGH);
        pinMode(busyPins[p], INPUT);
        pinMode(movePins[p], INPUT);
        attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(busyPins[p]), funptr[p], RISING);
        motors[p].init(p+1, ssPins[p], stopPins[p], movePins[p], busyPins[p],&msg);
    }

   
    //delay(1000);

    //Timer1.initialize((unsigned long) 500000000);
    //Timer1.attachInterrupt(timerHandler);


    delay(100);
    
}  // end of setup


uint8_t indexMotor = 0;
uint8_t indexMotorReply = 0;

MessageSerial::Work actWork = MessageSerial::NOP;
bool runLoop = false;
uint32_t loopsCnt = 500000;
uint8_t cntIsFinishCnt = 0;
uint8_t FirstLoop = 10;


#define PULSE(P) digitalWrite(P, HIGH); delay(10); digitalWrite(P, LOW); delay(10);
unsigned long prevMillis = 0;
volatile bool sendReplyMsg = false;
void loop (void)
{

    if (activeBusy) {
        for (unsigned int n = 0; n < maxNumSter; ++n) {
            if (bitRead(activeBusy, n)) {
                bitClear(activeBusy, n);
                bitClear(acceptBusy, n);
                motors[n].sendReplyMsg();
            }
        }
    }

    if (checkProgress) {
        checkProgress = false;
        Serial1.println(" Send ECHO/PROGRESS");
        for (unsigned int n = 0; n < maxNumSter; ++n) {
            if (n+1 != 9) {
                //motors[n].sendEchoMsg();
                continue;
            }
            
            motors[n].sendProgressMsg();
        }
    }
    readSerial();
        
    switch(actWork) {
    case MessageSerial::WELCOME_MSG: welcomeMsg(); break;
    case MessageSerial::CONFIGURATION: configuration(); break; 
    case MessageSerial::CONFIGURATION_LOCAL: configurationLocal(); break; 
    case MessageSerial::MOVE_REQUEST: moveSteps(); break;
    default: break;
    }
}


void readSerial()
{
    if (Serial.available())  {
        int c = Serial.read();
        if (msg.check(c)) {
            actWork = msg.getStatusWork();
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
    motors[motor].moveSteps(msg.msg(), msg.len());
    actWork = MessageSerial::NOP;
}



