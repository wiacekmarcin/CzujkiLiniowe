#include <Arduino.h>
#include <Wire.h>


#include "crc8.hpp"
#include "protocol.hpp"
#include "spi_proto.hpp"
MessageSerial msg;

#define DEBUG

#define SERIALOUT Serial2
#define SERIALDBG Serial1

#include <PinChangeInterrupt.h>
#include <TimerOne.h>
constexpr uint8_t maxNumSter = 9;
constexpr uint8_t maxNumSterTEST = 1;
constexpr uint8_t resetPin = 12;

constexpr uint8_t sixPins[maxNumSter]   = {47,44,41,38,35,32,29,26,23}; //czerwony zam. na zielony;
constexpr uint8_t busyPins[maxNumSter]  = {A8,A9,A10,A11,A12,A13,A14,A15,10}; //bialy
constexpr uint8_t stopPins[maxNumSter]  = {46,43,40,37,34,31,28,25,22}; //niebieskie
constexpr uint8_t movePins[maxNumSter]  = {48,45,42,39,36,33,30,27,24}; //czarny

uint8_t statusWord[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SPIMessage motors[maxNumSter];

uint8_t czujkaZwMsg[3] = { 0x10, 0x00, 0x57 };

static void readSerial();
static void configurationLocal();
static void welcomeMsg();
static void configuration();
static void moveSteps();
static void resetRequest();
//static void sendProgressMsg(uint8_t index);

//przerwanie od timer zeby odczytac wartosc mierzona napiecia i napiecia
volatile bool timeout = false;


volatile bool getAllProgress = false;


static void phex(uint8_t b)
{
    SERIALDBG.print(" ");
    if (b < 16)
        SERIALDBG.print("0");
    SERIALDBG.print(b, HEX);
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
    activeBusy |= (1 << N);\
    /*SERIALDBG.print("I");*/\
    /*SERIALDBG.println(N);*/\
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

void sendStopMsg();

void setup (void)
{
    SERIALOUT.begin(115200);
    SERIALDBG.begin(115200);
    SERIALDBG.println("Reset devices");
    pinMode(resetPin, OUTPUT); 
    digitalWrite(resetPin, LOW);
    delay(50);
    digitalWrite(resetPin, HIGH);
    
    
    delay(1500);
    Wire.begin();
    
    activeBusy = 0x0000;
    acceptBusy = 0x01ff;
    SERIALDBG.println("Search devices:");
    unsigned long actTime = millis();
    for (uint8_t p = 0; p < maxNumSter; ++p) {
        int address = 0x31 + p;
        int id = p + 1;
        SERIALDBG.print("Adress:");
        SERIALDBG.println(address, HEX);
        Wire.beginTransmission(address);
        
        byte error = Wire.endTransmission();
        if (error == 0) {
            //found device
            motors[p].setDevice(id, address);
            SERIALDBG.print("Motor ");
            SERIALDBG.print(p+1);
            SERIALDBG.print(" active ");
            SERIALDBG.println(address, HEX);
            pinMode(stopPins[p], OUTPUT); digitalWrite(stopPins[p], HIGH);
            pinMode(busyPins[p], INPUT);
            pinMode(movePins[p], INPUT);
            pinMode(sixPins[p], OUTPUT); digitalWrite(sixPins[p], LOW);
            motors[p].init(stopPins[p], movePins[p], busyPins[p]);
            motors[p].sendEchoMsg();
            actTime = millis();
            SERIALDBG.println("Czekam na BUSY LOW");
            digitalWrite(sixPins[p], HIGH);
            while(digitalRead(busyPins[p]) == HIGH) {
                if (millis() - actTime > 100)
                    break;
            }
            digitalWrite(sixPins[p], LOW);
            if (digitalRead(busyPins[p]) == HIGH) {
                SERIALDBG.println("No Busy");
                //motors[p].setPins(false, false, false);
                continue;
            }
            attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(busyPins[p]), funptr[p], RISING);
            SERIALDBG.print("Stop pin:");
            SERIALDBG.println(stopPins[p], DEC);
            SERIALDBG.print("Move pin:");
            SERIALDBG.println(movePins[p], DEC);

            SERIALDBG.println("Stop Pin LOW");
            SERIALDBG.println("Czekam na move HIGH");
            digitalWrite(stopPins[p], LOW);
            digitalWrite(sixPins[p], LOW);
            while(digitalRead(movePins[p]) == LOW) {
                if (millis() - actTime > 100)
                    break;
            }
            digitalWrite(sixPins[p], HIGH);
            if (digitalRead(movePins[p]) == LOW) {
                SERIALDBG.println("No move/stop");
                //motors[p].setPins(true, false, false);
                continue;
            }
            SERIALDBG.println("Stop pin na HIGH");
            SERIALDBG.println("Czekam na move pin LOW");
            digitalWrite(stopPins[p], HIGH);
            digitalWrite(sixPins[p], LOW);
            while(digitalRead(movePins[p]) == HIGH) {
                if (millis() - actTime > 100)
                    break;
            }
            digitalWrite(sixPins[p], HIGH);
            if (digitalRead(movePins[p]) == HIGH)  {
                SERIALDBG.println("No Move/Stop");
                //motors[p].setPins(true, true, false);
                continue;
            }
                
            
            
        }
    }

    for (uint8_t p = maxNumSter; p < 16; ++p) {
        int address = 0x31 + p;
        int id = p + 1;
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();
        if (error == 0) {
            //found device
            SERIALDBG.print("Found address: ");
            SERIALDBG.println(address, HEX);
        }
    }

    SERIALDBG.println("End search");

    pinMode(2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), sendStopMsg, FALLING);
    

    //Timer1.initialize((unsigned long) 500000000);
    //Timer1.attachInterrupt(timerHandler);


    //Wire.setClock(10000);
    
}  // end of setup


uint8_t indexMotor = 0;
uint8_t indexMotorReply = 0;

MessageSerial::Work actWork = MessageSerial::NOP;
bool runLoop = false;
uint32_t loopsCnt = 5000000;
uint8_t cntIsFinishCnt = 0;
uint8_t FirstLoop = 10;


#define PULSE(P) digitalWrite(P, HIGH); delay(10); digitalWrite(P, LOW); delay(10);
unsigned long prevMillis = 0;
volatile bool sendReplyMsg = false;
void loop (void)
{

    if (getAllProgress) {
        for (unsigned int n = 0; n < maxNumSter; ++n) {
            if (motors[n].isActive()) {
                motors[n].sendProgressMsg();
            }
            delayMicroseconds(50);
        }
        getAllProgress = false;
    }


    if (activeBusy) {
        SERIALDBG.print("Active:");
        SERIALDBG.println(activeBusy,HEX);
        for (unsigned int n = 0; n < maxNumSter; ++n) {
            if (bitRead(activeBusy, n)) {
                bitClear(activeBusy, n);
                bitClear(acceptBusy, n); 
                motors[n].getReply();
            }
        }
    }
    

    if (checkProgress) {
        checkProgress = false;
        SERIALDBG.println(" Send PROGRESS");
        for (unsigned int n = 0; n < maxNumSter; ++n) {
            if (motors[n].isActive() && motors[n].isMove()) 
                motors[n].sendProgressMsg();
            delayMicroseconds(50);
        }
    }


    readSerial();
        
    switch(actWork) {
    case MessageSerial::WELCOME_MSG: welcomeMsg(); break;
    case MessageSerial::CONFIGURATION: configuration(); break; 
    case MessageSerial::CONFIGURATION_LOCAL: configurationLocal(); break; 
    case MessageSerial::MOVE_REQUEST: moveSteps(); break;
    case MessageSerial::RESET_REQUEST: resetRequest(); break;
    default:
    break;
    }
}


void readSerial()
{
    if (SERIALOUT.available())  {
        int c = SERIALOUT.read();
        if (msg.check(c)) {
            actWork = msg.getStatusWork();
            return;
        }
    }
}

void welcomeMsg()
{
#ifdef DEBUG
    SERIALDBG.println("Send Welcome Msg");
#endif        
    msg.sendWelcomeMsg();
    actWork = MessageSerial::NOP;
    return;
}

void configurationLocal()
{
#ifdef DEBUG
    SERIALDBG.println("Configuration Local and send configuration");
#endif            
    uint8_t bytes[maxNumSter];
    for (unsigned int n = 0; n < maxNumSter; ++n) 
        bytes[n] = (((n+1) << 4) & 0xf0) |  motors[n].getByte2();
    msg.sendConfigLocalDoneMsg(bytes, maxNumSter);
#ifdef DEBUG
    SERIALDBG.print("Send to PC [");
    for (unsigned int n = 0; n < maxNumSter; ++n) 
        phex(bytes[n]);
    SERIALDBG.println("]");    
#endif            

    delay(200); 
    for (unsigned int n = 0; n < maxNumSter; ++n) {
        if (motors[n].isActive()) {
            motors[n].sendConfiguration();
            delayMicroseconds(10);
        }
    }
    actWork = MessageSerial::NOP;
}

void configuration()
{
#ifdef DEBUG
    SERIALDBG.println("Konfiguracja Msg");
#endif  
    uint8_t motor = msg.getAddress()-1;
    motors[motor].setConfiguration(msg.msg(), msg.len());
    actWork = MessageSerial::NOP;
}

void moveSteps()
{
#ifdef DEBUG
    SERIALDBG.println("Ruch Msg");
#endif 
    uint8_t motor = msg.getAddress()-1;
    motors[motor].moveSteps(msg.msg(), msg.len());
    actWork = MessageSerial::NOP;
}

void resetRequest()
{
#ifdef DEBUG
    SERIALDBG.println("Reset");
#endif     
    digitalWrite(resetPin, LOW);
    delay(1000);
    digitalWrite(resetPin, HIGH);
    delay(1500);
    for (unsigned int n = 0; n < maxNumSter; ++n) {
        if (motors[n].isActive()) {
            motors[n].sendConfiguration();
            delayMicroseconds(10);
        }
    }
    actWork = MessageSerial::NOP;
}

volatile unsigned long prevZw = 0;
void sendStopMsg()
{
    unsigned long actTime = millis();
    if (actTime - prevZw < 10)
        return;
    prevZw = actTime;
    SERIALDBG.println("ID2");
    SERIALOUT.write(czujkaZwMsg, 3);
    SERIALDBG.println("ID2");
    for (unsigned int n = 0; n < maxNumSter; ++n) {
        if (motors[n].isActive())
            digitalWrite(stopPins[n], LOW);
    }
    
    for (unsigned int n = 0; n < maxNumSter; ++n) {
        if (motors[n].isActive()) 
            digitalWrite(stopPins[n], HIGH);
    }
    getAllProgress = true;
}
