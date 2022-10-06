#include <Arduino.h>
#include <SPI.h>
#include "pins_arduino.h"

#include "crc8.hpp"
#include "protocol.hpp"

MessageSerial msg;


#include <PinChangeInterrupt.h>
#include <TimerOne.h>
constexpr uint8_t maxNumSter = 9;
constexpr uint8_t resetPins[maxNumSter] = {41,42,43,44,45,46,47,48,49};
constexpr uint8_t ssPins[maxNumSter]    = {32,33,34,35,36,37,38,39,40};
constexpr uint8_t busyPins[maxNumSter]  = {A8,A9,A10,A11,A12,A13,A14,A15,10};
constexpr uint8_t stopPins[maxNumSter]  = {30,29,28,27,26,25,24,23,22};

//przerwanie od timer zeby odczytac wartosc mierzona napiecia i napiecia
volatile bool readMsr = false;
void readMeasurement()
{
    readMsr = true;
}


//przerwanie od kontrolera po skonczonej pracy 
volatile bool finishJob[maxNumSter] = {false,false,false,false,false,false,false,false,false};

typedef enum _actJob {
    JOB_NOP,
    JOB_HOME_RETURN,
    JOB_POSITIONING
} actKontrJob;

actKontrJob actJob[maxNumSter] = {JOB_NOP,JOB_NOP,JOB_NOP,JOB_NOP,JOB_NOP,JOB_NOP,JOB_NOP,JOB_NOP,JOB_NOP};

typedef enum _measurJob {
    MEAS_NOP = 0,
    SEND_CURR,
    WAIT_CURR,
    SEND_VOLT,
    WAIT_VOLT,
    M_SEND_VALS,
} MeasurJob;

#define FINISHJOB(N) \
void isrFinishJob##N() \
{\
    finishJob[N-1] = true;\
}

FINISHJOB(1)
FINISHJOB(2)
FINISHJOB(3)
FINISHJOB(4)
FINISHJOB(5)
FINISHJOB(6)
FINISHJOB(7)
FINISHJOB(8)
FINISHJOB(9)

typedef void (*isr_bust_fun)(void);
isr_bust_fun funptr[maxNumSter] = {isrFinishJob1, isrFinishJob2, isrFinishJob3, isrFinishJob4, isrFinishJob5, isrFinishJob6, isrFinishJob7, isrFinishJob8, isrFinishJob9};

void setup (void)
{
    // Put SCK, MOSI, SS pins into output mode
    // also put SCK, MOSI into LOW state, and SS into HIGH state.
    // Then put SPI hardware into Master mode and turn SPI on
    SPI.begin ();
#if 0    
    SPI.setClockDivider(SPI_CLOCK_DIV128);

    /* piny SPI */
    pinMode(SCK, OUTPUT);
    digitalWrite(SCK, LOW);
    pinMode(MISO, INPUT);
    pinMode(MOSI, OUTPUT);
    digitalWrite(MOSI, LOW);

    for (uint8_t p = 0; p < maxNumSter; ++p) {
        pinMode(resetPins[p], OUTPUT); digitalWrite(resetPins[p], HIGH);
        pinMode(ssPins[p], OUTPUT); digitalWrite(ssPins[p], HIGH);
        pinMode(stopPins[p], OUTPUT); digitalWrite(stopPins[p], HIGH);
        pinMode(busyPins[p], INPUT);
        attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(busyPins[p]), funptr[p], FALLING);
    }
#endif
    Serial.begin(115200);
#if 0    
    Timer1.initialize(100000);
    Timer1.attachInterrupt(readMeasurement);
    Serial1.begin(1200); // to do konfiguracji
#endif  
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

/*
    uint8_t pinMask[] = { 0x00, PIN_S1, PIN_S2, PIN_S3 };
    sendBuf[lenMsg-1] = c.getCRC();
    Serial.print("crc=");
    Serial.print(sendBuf[lenMsg-1], HEX);
    digitalWrite(pinMask[addr], LOW); 
    SPI.transfer(sendBuf, lenMsg);
    digitalWrite(pinMask[addr], HIGH); 
*/    
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
/*
    uint8_t pinMask[] = { 0x00, PIN_S1, PIN_S2, PIN_S3 };
    sendBuf[lenMsg-1] = c.getCRC();
    Serial.print("crc=");
    Serial.print(sendBuf[lenMsg-1], HEX);
    digitalWrite(pinMask[addr], LOW); 
    SPI.transfer(sendBuf, lenMsg);
    digitalWrite(pinMask[addr], HIGH); 
*/
  //while (digitalRead())
}


uint8_t kontId = 0;
MeasurJob mjob = MEAS_NOP;
MessageSerial::Work actWork;
uint8_t confTab[9][12];

unsigned long readLastChar;    
void loop (void)
{
#if 0    
    //przerwanie od konca pracy z sterownika
    if (finishJob[kontId]) {
        finishJob[kontId] = false;
        if (actJob[kontId] == JOB_HOME_RETURN) {
            //TODO send command with steps
        } else if (actJob[kontId] == JOB_POSITIONING) {
            //TODO send command with steps
        }
        actJob[kontId] = JOB_NOP;
    }
    ++kontId;

    //przerwanie od timera
    if (readMsr) {
        readMsr = false;
        mjob = SEND_CURR;
        return;
    }

    if (mjob == SEND_CURR) {
        Serial1.write("CURR?");
        mjob = WAIT_CURR;
        readLastChar = millis();
        return;
    }

    if (mjob == WAIT_CURR) {
        if (Serial1.available()) {
            uint8_t c = Serial1.read();
            //TODO addMsg
            readLastChar = millis();
        } else {
            if (millis() - readLastChar > 50)
            mjob = SEND_VOLT;
        }
        return;
    }
    
    if (mjob == SEND_VOLT) {
        Serial1.write("VOLT?");
        mjob = WAIT_VOLT;
        readLastChar = millis();
        return;
    }

    if (mjob == WAIT_VOLT) {
        if (Serial1.available()) {
            uint8_t c = Serial1.read();
            //TODO addMsg
            readLastChar = millis();
        } else {
            if (millis() - readLastChar > 50)
            mjob = M_SEND_VALS;
        }
        return;
    }

    if (mjob == M_SEND_VALS) {
        //TODO Serial.write(measurentMsg)
        mjob = MEAS_NOP;
        return;
    }
#endif
    if (actWork == MessageSerial::WELCOME_MSG) {
        msg.sendWelcomeMsg();
        actWork = MessageSerial::NOP;
        return;
    }

    if (actWork == MessageSerial::CONFIGURATION) {
        msg.copyCmd(confTab[msg.getAddress()-1], 12);
        msg.sendConfigDoneMsg(msg.getAddress());
        actWork = MessageSerial::NOP;
        return;
    }

    if (Serial.available())  {
        if (msg.check(Serial.read())) {
            actWork = msg.getStatusWork();
            return;
        }
    }
}
