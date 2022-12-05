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
constexpr uint8_t busyPins[maxNumSter]  = {A8,A9,A10,A11,A12,A13,A14,A15,15};
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

volatile short checkProgres = 0;
void timerHandler()
{
    checkProgres = maxNumSter;
}

volatile uint16_t busyLowMaks = 0x0000;


#define FINISHJOB(N) \
void isrFinishJob##N() \
{\
    bitSet(busyLowMaks, N); \
    /*motors[N].setFinish();*/\
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
    pinMode(tempPin, OUTPUT);
    Serial.begin(115200);
#ifdef DEBUG    
    Serial1.begin(115200);
#endif  
    digitalWrite(tempPin, LOW);

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


#define PULSE(P) digitalWrite(P, HIGH); delay(10); digitalWrite(P, LOW); delay(10);
unsigned long prevMillis = 0;
bool sendEchoMsg = false;
void loop (void)
{

    if (busyLowMaks) {
        for (unsigned int n = 0; n < 9; ++n) {
            if (bitRead(busyLowMaks, n)) {
                bitClear(busyLowMaks, n);
                Serial1.print(millis());
                Serial1.print(" IRQ:");
                Serial1.println(n+1);
            }
            if (n + 1 == 9) {
                if (!sendEchoMsg) {
                    sendEchoMsg = true;
                    digitalWrite(tempPin, HIGH);
                    motors[n].sendEchoMsg();
                    digitalWrite(tempPin, LOW);
                }
                else {
                    Serial1.print(millis());
                    Serial1.println(" Odp na echo");
                }
                
            }
        }
    } else {
        if (millis() - prevMillis > 500) {
            Serial1.println("Brak przerwań");
            prevMillis = millis();
        }
    }

    if (checkProgres) {
        Serial1.println("Reset ALL");
        digitalWrite(resetPin, LOW);
        delay(50);
        digitalWrite(resetPin, HIGH);

        //DEBUG;
        checkProgres = 0;
    }


    return ;

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



