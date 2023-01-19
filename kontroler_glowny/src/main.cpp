#include <Arduino.h>
#include <Wire.h>


#include "crc8.hpp"
#include "protocol.hpp"
#include "spi_proto.hpp"
#include "main.h"

MessageSerial msg;

#define DEBUG

#define CZUJKAPIN 2

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
uint8_t czujkaZwContMsg1[3] = { 0x10, 0x03, 0x5E };
uint8_t czujkaZwContMsg0[3] = { 0x10, 0x02, 0x59 };
static void readSerial();
static void configurationLocal();
static void welcomeMsg();
static void configuration();
static void moveSteps();
static void resetRequest();
static void stopRequest();
static void stopAllRequest();
static void enableRequest(bool all, bool enable);

static void checkPins(uint8_t p);

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

void sendZwCzujkiMsg();

void setup (void)
{
    SERIALOUT.begin(115200);
    SERIALDBG.begin(115200);
    SERIALDBG.println("Zerowanie urzadzen");
    pinMode(resetPin, OUTPUT); 
    digitalWrite(resetPin, LOW);
    delay(50);
    digitalWrite(resetPin, HIGH);
    
    
    delay(1500);
    Wire.begin();
    
    activeBusy = 0x0000;
    SERIALDBG.println("Szukam urzadzen...");
    unsigned long actTime = millis();
    for (int8_t p = -1; p < maxNumSter; ++p) {
        int address = 0x31 + p;
        int id = p + 1;
        SERIALDBG.println("------------");
        SERIALDBG.print("Adres:");
        SERIALDBG.print(address, HEX);
        Wire.beginTransmission(address);
        
        byte error = Wire.endTransmission();
        if (error == 0) {
            if (id == 0 && id > maxNumSter) {
                SERIALDBG.println(" : znaleziono urzadzenie, ale ze zlym adresem");
                continue;
            }

            //found device
            motors[p].setDevice(id, address);
            motors[p].init(stopPins[p], movePins[p], busyPins[p]);
            checkPins(p);

            pinMode(stopPins[p], OUTPUT); digitalWrite(stopPins[p], LOW);
            pinMode(busyPins[p], INPUT);
            pinMode(movePins[p], INPUT);
            pinMode(sixPins[p], OUTPUT); digitalWrite(sixPins[p], LOW);
            attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(busyPins[p]), funptr[p], RISING);
        } else {
            SERIALDBG.println(" Brak urzadzenia");
        }
    }

    for (uint8_t p = maxNumSter; p < 16; ++p) {
        int address = 0x31 + p;
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
    attachInterrupt(digitalPinToInterrupt(2), sendZwCzujkiMsg, FALLING);
    

    Timer1.initialize((unsigned long) 500000UL);
    Timer1.attachInterrupt(timerHandler);


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
unsigned long prevMls=0;
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
        SERIALDBG.println(activeBusy,BIN);
        for (unsigned int n = 0; n < maxNumSter; ++n) {
            if (bitRead(activeBusy, n)) {
                bitClear(activeBusy, n);
                motors[n].getReply();
            }
        }
        SERIALOUT.flush();
    }
    

    if (checkProgress) {
        checkProgress = false;
        for (unsigned int n = 0; n < maxNumSter; ++n) {
            if (motors[n].isActive() && motors[n].isMove()) {
                SERIALDBG.print("Send PROGRESS:");
                SERIALDBG.println(n+1);
                motors[n].sendProgressMsg();
            }

            delayMicroseconds(50);
        }
        if (digitalRead(CZUJKAPIN) == LOW) {
            SERIALOUT.write(czujkaZwContMsg0, 3);
        } else {
            //SERIALOUT.write(czujkaZwContMsg1, 3);
        }

    }


    readSerial();
        
    switch(actWork) {
    case MessageSerial::WELCOME_MSG: welcomeMsg(); break;
    case MessageSerial::CONFIGURATION: configuration(); break; 
    case MessageSerial::CONFIGURATION_LOCAL: configurationLocal(); break; 
    case MessageSerial::MOVE_REQUEST: moveSteps(); break;
    case MessageSerial::RESET_REQUEST: resetRequest(); break;
    case MessageSerial::STOP_REQUEST: stopRequest(); break;
    case MessageSerial::STOPALL_REQUEST: stopAllRequest(); break;
    case MessageSerial::ENABLE_OFF: enableRequest(false, false); break;
    case MessageSerial::ENABLE_ON: enableRequest(false, true); break;
    case MessageSerial::ENABLE_ALL_OFF: enableRequest(true, false); break;
    case MessageSerial::ENABLE_ALL_ON: enableRequest(true, true); break;

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

void stopRequest()
{
#ifdef DEBUG
    SERIALDBG.println("Stop");
#endif     
    uint8_t motor = msg.getAddress()-1;
    motors[motor].stop();
    actWork = MessageSerial::NOP;
}

void stopAllRequest()
{
#ifdef DEBUG
    SERIALDBG.println("Stop All");
#endif     
    for (unsigned int n = 0; n < maxNumSter; ++n) {
        if (motors[n].isActive())
            digitalWrite(stopPins[n], LOW);
    }
    
    for (unsigned int n = 0; n < maxNumSter; ++n) {
        if (motors[n].isActive()) 
            digitalWrite(stopPins[n], HIGH);
    }
    actWork = MessageSerial::NOP;
}

void enableRequest(bool all, bool en)
{
#ifdef DEBUG
    SERIALDBG.print("ENABLE REQUEST");
    SERIALDBG.print(" all");
    SERIALDBG.print( all);
    SERIALDBG.print(" enable");
    SERIALDBG.println( en);
#endif 
    if (all) {
        for (unsigned int n = 0; n < maxNumSter; ++n) {
            motors[n].sendEnable(en);
    }
    } else {
        uint8_t motor = msg.getAddress()-1; 
        motors[motor].sendEnable(en);
    }
    actWork = MessageSerial::NOP;
}

volatile unsigned long prevZw = 0;
void sendZwCzujkiMsg()
{
    unsigned long actTime = millis();
    if (actTime - prevZw < 200)
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

void checkPins(uint8_t p)
{
    motors[p].sendEchoMsg();
    pinMode(busyPins[p], INPUT);
    pinMode(stopPins[p], INPUT_PULLUP);
    pinMode(movePins[p], INPUT_PULLUP);
    pinMode(sixPins[p], INPUT_PULLUP);

    unsigned long actTime = millis();
    while(digitalRead(busyPins[p]) == HIGH) {
        if (millis() - actTime > 100)
            break;
    }

    if (digitalRead(busyPins[p]) == HIGH) {
        SERIALDBG.println(". No Busy LOW");
        motors[p].setPins(false, false, false, false);
        return;
    }
    SERIALDBG.print(" Busy OK. ");

    delayMicroseconds(100);
    bool stopPinOk = digitalRead(stopPins[p] == LOW);
    bool movePinOk = digitalRead(movePins[p] == LOW);
    bool sixPinOk = digitalRead(sixPins[p] == LOW);
    delayMicroseconds(100);
    SERIALDBG.print(" stop=");
    SERIALDBG.print(stopPinOk);
    SERIALDBG.print(" move=");
    SERIALDBG.print(movePinOk);
    SERIALDBG.print(" six=");
    SERIALDBG.println(sixPinOk);

    actTime = millis();
    while (digitalRead(busyPins[p]) == LOW) {
        if (millis() - actTime > 500) {
            SERIALDBG.println(". No Busy HIGH");
            motors[p].setPins(false, false, movePinOk, stopPinOk);
            return;
        }
    }
     
    if (digitalRead(busyPins[p]) == HIGH)
    {
        uint8_t recvPos = 0;
        uint8_t recvBuff[3];
        uint8_t address = 0x31+p;
        Wire.requestFrom(address, (uint8_t)3);    
        while (Wire.available() && recvPos < 3) { 
            recvBuff[recvPos++] = Wire.read(); 
        }
        if (recvPos < 3) {
            SERIALDBG.println(" Blad Komunikacji");
            motors[p].setPins(false, true, movePinOk, stopPinOk);
            return;
        }

        if (recvBuff[0] == 0x50 && ((recvBuff[1] >> 4) & 0x0f)+ 0x30 == address) {
            SERIALDBG.println(" Komunikacja OK. Wszystko OK");
            motors[p].setPins(true, true, movePinOk, stopPinOk);
        } else {
            SERIALDBG.println(" Nie poprawne dane");
            SERIALDBG.print("[");
            SERIALDBG.print(recvBuff[0], HEX);
            SERIALDBG.print(" ");
            SERIALDBG.print(recvBuff[1], HEX);
            SERIALDBG.print(" ");
            SERIALDBG.print(recvBuff[2], HEX);
            SERIALDBG.println("]");
            motors[p].setPins(false, true, movePinOk, stopPinOk);
        }
    }
}