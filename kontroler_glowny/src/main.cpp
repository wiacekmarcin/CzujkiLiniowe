#include <Arduino.h>
#include <SPI.h>
#include "pins_arduino.h"

#include "crc8.hpp"
#include "protocol.hpp"

MessageSerial msg;

#define DEBUG

#include <PinChangeInterrupt.h>
#include <TimerOne.h>
constexpr uint8_t maxNumSter = 9;
constexpr uint8_t resetPins = 53;
constexpr uint8_t ssPins[maxNumSter]    = {49,47,45,43,41,39,37,35,33};
constexpr uint8_t busyPins[maxNumSter]  = {A15,A14,A13,A12,A11,A10,A9,A8,10};
constexpr uint8_t stopPins[maxNumSter]  = {48,46,44,42,40,38,36,34,32};

static void readSerial();
static void configurationLocal();
static void welcomeMsg();
static void configuration();
static void moveSteps();

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
    /*digitalWrite(stopPins[N-1],LOW);*/\
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
    pinMode(resetPins, OUTPUT); 
    digitalWrite(resetPins, LOW);
    delay(50);
    digitalWrite(resetPins, HIGH);
    
    // Put SCK, MOSI, SS pins into output mode
    // also put SCK, MOSI into LOW state, and SS into HIGH state.
    // Then put SPI hardware into Master mode and turn SPI on
    SPI.begin ();
    
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
        attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(busyPins[p]), funptr[p], FALLING);
    }

    Serial.begin(115200);
#ifdef DEBUG    
    Serial1.begin(115200);
#endif    
#if 0    
    Timer1.initialize(100000);
    Timer1.attachInterrupt(readMeasurement);
    Serial1.begin(1200); // to do konfiguracji
#endif  
}  // end of setup

volatile char buf[5];
volatile byte pos = 0;
volatile uint32_t counterMsg = 0;



uint8_t kontId = 0;
MeasurJob mjob = MEAS_NOP;
MessageSerial::Work actWork;
uint8_t confTab[9][18];
uint8_t confMsgSize = 0;

unsigned long readLastChar;    

bool runLoop = false;
void loop (void)
{
    readSerial();
        
    switch(actWork) {
    case MessageSerial::WELCOME_MSG: welcomeMsg(); break;
    case MessageSerial::CONFIGURATION: configuration(); break; 
    case MessageSerial::CONFIGURATION_LOCAL: configurationLocal(); break; 
    case MessageSerial::MOVE_REQUEST moveSteps(); break;
    default: break;
    }







    if (runLoop) {
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

            digitalWrite(ssPins[msg.getAddress()-1], LOW); 
            SPI.transfer(confTab[msg.getAddress()-1], 12);
            digitalWrite(ssPins[msg.getAddress()-1], HIGH); 
            
            //mjob = SEND_CURR;
            mjob = M_SEND_VALS;

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
            msg.sendMeasuremnt();
            mjob = MEAS_NOP;
            return;
        }
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
    runLoop = true; // po co ?
}

void welcomeMsg()
{
#ifdef DEBUG
        Serial1.println("Welcome Msg");
#endif        
        msg.sendWelcomeMsg();
        actWork = MessageSerial::NOP;
        runLoop = false;
        return;
    }
}

void configuration()
{
    uint8_t motor = msg.getAddress()-1;
    digitalWrite(stopPins[motor],LOW);
#ifdef DEBUG
    Serial1.println("Send Configuration");
#endif  
    
    msg.copyCmd(confTab[motor], msg.len());
    confMsgSize = msg.len();
    msg.sendConfigDoneMsg(msg.getAddress());
    digitalWrite(stopPins[motor],HIGH);      
#ifdef DEBUG        
    Serial1.print("PIN=");
    Serial1.println(ssPins[motor]);
#endif  
                
    digitalWrite(ssPins[motor], LOW); 
    SPI.transfer(confTab[motor], confMsgSize);
    digitalWrite(ssPins[motor], HIGH); 
    digitalWrite(stopPins[motor], HIGH);      
    actWork = MessageSerial::NOP;
}

void moveSteps()
{
    uint8_t motor = msg.getAddress()-1;
    digitalWrite(stopPins[motor],LOW);
    actJob[motor] = (msg.getOptions() & 0x01 == 0x01) ? JOB_HOME_RETURN : JOB_POSITIONING ; 
#ifdef DEBUG
    Serial1.print("Send steps ");
    Serial1.println((msg.getOptions() & 0x01 == 0x01) ? "H" : "M");
    Serial1.print("PIN=");
    Serial1.println(ssPins[motor]);
#endif  

    digitalWrite(ssPins[motor], LOW); 
    SPI.transfer(msg.msg(), msg.len());
    digitalWrite(ssPins[motor], HIGH); 
    actWork = MessageSerial::NOP;
    digitalWrite(stopPins[motor],HIGH);
}

/*




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
    sendBuf[0] = sendBuf[0] | ((lenMsg - 1 - 1 - 1) & 0x0f);
    CRC8 c;
    c.reset();
    for (int i=0; i< lenMsg-1; ++i ) {
        c.add(*(sendBuf+i));
#ifdef DEBUG
        Serial1.print(sendBuf[i], HEX);
        Serial1.print(" ");
#endif // DEBUG                
    }

    sendBuf[lenMsg-1] = c.getCRC();
#ifdef DEBUG
    Serial1.print("crc=");
    Serial1.print(sendBuf[lenMsg-1], HEX);
#endif // DEBUG            
    
    digitalWrite(ssPins[addr-1], LOW); 
    SPI.transfer(sendBuf, lenMsg);
    digitalWrite(ssPins[addr-1], HIGH); 
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
    sendBuf[0] = sendBuf[0] | ((lenMsg - 1 - 1 - 1) & 0x0f);
    CRC8 c;
    c.reset();
    for (int i=0; i< lenMsg-1; ++i ) {
        c.add(*(sendBuf+i));
#ifdef DEBUG
        Serial1.print(sendBuf[i], HEX);
        Serial1.print(" ");
#endif // DEBUG        

    }

    sendBuf[lenMsg-1] = c.getCRC();
#ifdef DEBUG
    Serial.print("crc=");
    Serial.print(sendBuf[lenMsg-1], HEX);
#endif // DEBUG    
    digitalWrite(ssPins[addr-1], LOW); 
    SPI.transfer(sendBuf, lenMsg);
    digitalWrite(ssPins[addr-1], HIGH); 

  //while (digitalRead())
}
*/