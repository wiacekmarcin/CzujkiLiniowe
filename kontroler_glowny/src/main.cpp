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
static void getMotorReply(uint8_t index);
static void sendProgressMsg(uint8_t index);

//przerwanie od timer zeby odczytac wartosc mierzona napiecia i napiecia
volatile bool timeout = false;

void phex(uint8_t b)
{
    Serial1.print(" ");
    if (b < 10)
        Serial1.print("0");
    Serial1.print(b, HEX);
}


void timerHandler()
{
    //timeout = true;
    //Serial1.println("IRQ T");
}


//przerwanie od kontrolera po skonczonej pracy 
volatile bool finishJob[maxNumSter] = {false,false,false,false,false,false,false,false,false};

typedef enum _actJob {
    JOB_NOP,
    JOB_CONF,
    JOB_PROGR,
    JOB_HOME_RETURN,
    JOB_POSITIONING,
    JOB_RESPONSE,
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
    if (actJob[N] == JOB_RESPONSE) return; \
    finishJob[N] = true;\
    /*digitalWrite(stopPins[N-1],LOW);*/\
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
    delay(1500);
    
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
    delay(1000);

    Timer1.initialize(10000000);
    Timer1.attachInterrupt(timerHandler);


}  // end of setup


uint8_t kontId = 0;
MeasurJob mjob = MEAS_NOP;
MessageSerial::Work actWork;
uint8_t confTab[9][18];
uint8_t confMsgSize = 0;

uint8_t indexMotor = 0;
uint8_t indexMotorReply = 0;

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
            sendProgressMsg(indexMotor);
            Serial1.print("INDEX=");
            Serial1.println(indexMotor, DEC);
        }
        //else
        //    delayMicroseconds(200);
        
    
        if (++indexMotor == 9) {
            timeout = false;
            indexMotor = 0;
        }
    }

    if (finishJob[indexMotorReply]) {
        Serial1.print("FIN=");
        Serial1.println(indexMotorReply, DEC);
        finishJob[indexMotorReply] = false;
        //if (actJob[indexMotorReply] == JOB_PROGR) {
        //    Serial1.println("PROG");
            //Serial1.println(indexMotorReply, DEC);
            getMotorReply(indexMotorReply);
        //}
        //actJob[indexMotorReply] = JOB_NOP;

        if (mjob == M_SEND_VALS) {
            msg.sendMeasuremnt();
            mjob = MEAS_NOP;
            return;
        }
    }

    if (++indexMotorReply == 9) {
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
    digitalWrite(stopPins[motor],LOW);
#ifdef DEBUG
    Serial1.println("Send Configuration");
#endif  
    
    msg.copyCmd(confTab[motor], msg.len());
    confMsgSize = msg.len();
    msg.sendConfigDoneMsg(msg.getAddress());
    digitalWrite(stopPins[motor],HIGH);      
#ifdef DEBUG        
    Serial1.print("motor = ");
    Serial1.print(motor, DEC);
    Serial1.print(" confMsgSize=");
    Serial1.print(confMsgSize, DEC);
    Serial1.print(" PIN=");
    Serial1.println(ssPins[motor]);
#endif  
                
    digitalWrite(ssPins[motor], LOW); 
    SPI.transfer(confTab[motor], confMsgSize);
    digitalWrite(ssPins[motor], HIGH); 
    digitalWrite(stopPins[motor], HIGH);      
    actWork = MessageSerial::NOP;
    actJob[motor] = JOB_CONF;
}

void moveSteps()
{
    uint8_t motor = msg.getAddress()-1;
    digitalWrite(stopPins[motor],LOW);
    actJob[motor] = ((msg.getOptions() & 0x01) == 0x01) ? JOB_HOME_RETURN : JOB_POSITIONING ; 
#ifdef DEBUG
    Serial1.print("Send steps ");
    Serial1.println(((msg.getOptions() & 0x01) == 0x01) ? "H" : "M");
    Serial1.print("PIN=");
    Serial1.println(ssPins[motor]);
#endif  

    digitalWrite(ssPins[motor], LOW); 
    SPI.transfer(msg.msg(), msg.len());
    digitalWrite(ssPins[motor], HIGH); 
    actWork = MessageSerial::NOP;
    digitalWrite(stopPins[motor],HIGH);
}

void sendProgressMsg(uint8_t index)
{
#ifdef DEBUG
    Serial1.print("Send Progress Msg ID=");
    Serial1.println(index+1, DEC);
#endif  
    uint8_t msgLocal[3] = { 
        ((MessageSerial::PROGRESS_REQ << 4) & 0xf0) ,
        (uint8_t)(((index+1) << 4) & 0xf0),  0x00  };
    CRC8 c;
    c.reset();
    c.add(msgLocal[0]);
    c.add(msgLocal[1]);
    msgLocal[2] = c.getCRC();
#ifdef DEBUG
    Serial1.print("Send msg ");
    phex(msgLocal[0]);
    phex(msgLocal[1]);
    phex(msgLocal[2]);
    Serial1.println("");
#endif   
    actJob[index] = JOB_PROGR;
    digitalWrite(ssPins[index], LOW);
    SPI.transfer(msgLocal, 3);
    digitalWrite(ssPins[index], HIGH); 

}
void getMotorReply(uint8_t index)
{
#ifdef DEBUG
    Serial1.print("Get Reply from ID=");
    Serial1.println(index, DEC);
#endif  
    uint8_t msgLocal[20] = { 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 
                             0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0 };
    msgLocal[0] = (MessageSerial::LAST_REQ << 4) & 0xf0;
    msgLocal[1] = (uint8_t)(((index+1) << 4) & 0xf0);
    CRC8 c;
    c.reset();
    c.add(msgLocal[0]);
    c.add(msgLocal[1]);
    msgLocal[2] = c.getCRC();
#ifdef DEBUG
    Serial1.print("Send msg ");
    phex(msgLocal[0]);
    phex(msgLocal[1]);
    phex(msgLocal[2]);
    phex(msgLocal[3]);
    Serial1.println("");
#endif
    actJob[index] = JOB_RESPONSE;
    digitalWrite(ssPins[index], LOW);
    SPI.transfer(msgLocal, 20);
    digitalWrite(ssPins[index], HIGH); 
#ifdef DEBUG
    Serial1.print("Recv message LEN=");
    Serial1.print(msgLocal[0], DEC);
    Serial1.print(" Msg =");
    phex(msgLocal[0]);
    phex(msgLocal[1]);
    phex(msgLocal[2]);
    phex(msgLocal[3]);
    phex(msgLocal[4]);
    phex(msgLocal[5]);
    phex(msgLocal[6]);
    phex(msgLocal[7]);
    phex(msgLocal[8]);
    phex(msgLocal[9]);
    Serial1.println("");
#endif 
    if (msgLocal[1] < 20)     
        Serial.write(msgLocal + 2, msgLocal[1]);
}

/*

int main_2()
{
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
