#include "spi_proto.hpp"
#include "protocol.hpp"

#include<SPI.h>
#define DEBUG
static void phex(uint8_t b)
{
    Serial1.print(" ");
    if (b < 16)
        Serial1.print("0");
    Serial1.print(b, HEX);
}



SPIMessage::SPIMessage()
: addr (0)
, ssPin(0)
, stopPin(0)
, replyMsg{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
, progressMsg{0, 0, 0}
, echoMsg{0, 0, 0}
, echoRepMsg{0, 0, 0}
, confLen(0)
, confMsg{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
, finishedJob(false)
{
    actJob = JOB_ECHO;
}

SPIMessage::~SPIMessage()
{

}

void SPIMessage::init(const uint8_t addr, const uint8_t ssPin, const uint8_t stopPin, const uint8_t movePin, MessageSerial * msg)
{
    this->addr = addr;
    this->ssPin = ssPin;
    this->stopPin = stopPin;
    this->movePin = movePin;
    this->msg = msg;
    progressMsg[0] = (MessageSerial::PROGRESS_REQ << 4) & 0xf0;
    progressMsg[1] = (uint8_t)((addr << 4) & 0xf0);
    CRC8 c;
    c.reset();
    c.add(progressMsg[0]);
    c.add(progressMsg[1]);
    progressMsg[2] = c.getCRC();

    c.reset();
    replyMsg[0] = (MessageSerial::LAST_REQ << 4) & 0xf0;
    replyMsg[1] = (uint8_t)((addr << 4) & 0xf0);
    c.add(replyMsg[0]);
    c.add(replyMsg[1]);
    replyMsg[2] = c.getCRC();
    for (uint8_t b=3; b<20; ++b)
    {
        replyMsg[b] = b;
    }

    c.reset();
    echoMsg[0] = (MessageSerial::ECHO_CLEAR_REQ << 4) & 0xf0;
    echoMsg[1] = (uint8_t)((addr << 4) & 0xf0);
    c.add(echoMsg[0]);
    c.add(echoMsg[1]);
    echoMsg[2] = c.getCRC();

    c.reset();
    echoRepMsg[0] = (MessageSerial::ECHO_CLEAR_REP << 4) & 0xf0;
    echoRepMsg[1] = (uint8_t)((addr << 4) & 0xf0) | 0x08;
    c.add(echoRepMsg[0]);
    c.add(echoRepMsg[1]);
    echoRepMsg[2] = c.getCRC();

    c.reset();
    confMsg[0] = ((MessageSerial::CONF_REQ << 4) & 0xf0) | 0x09;
    confMsg[1] = (uint8_t)((addr << 4) & 0xf0) | 0x00;
    confMsg[2] = 0;
    confMsg[3] = 0;
    confMsg[4] = 0;
    confMsg[5] = 0x01;
    confMsg[6] = 0x90;
    confMsg[7] = 0;
    confMsg[8] = 0x0a;
    confMsg[9] = 0x03;
    confMsg[10] = 0xe8;

    for (uint8_t z = 0; z < 11; ++z) {
        c.add(confMsg[z]);
    }
    confMsg[11] = c.getCRC();
    confLen = 12;
}

void SPIMessage::sendReplyMsg()
{
    actJob = JOB_RESPONSE;
#ifdef DEBUG
    Serial1.println("Send Reply Msg");
    Serial1.print("motor = ");
    Serial1.print(addr, DEC);
    Serial1.print(" SSPin=");
    Serial1.println(ssPin);
#endif 
    uint8_t msgLocal[20];
    memcpy(msgLocal, replyMsg, 20);
    
    digitalWrite(ssPin, LOW);
    
    SPI.transfer(msgLocal, 20);
    digitalWrite(ssPin, HIGH);    

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
    Serial1.print(" actJob=");
    Serial1.println(actJob);

#endif 
    if (msgLocal[0] == 0x0A && msgLocal[1] < 20) {
#ifdef DEBUG
        Serial1.println("Send Rep msg to PC");
#endif    
        Serial.write(echoMsg, 3);

        //sendConfiguration(false);
    }
}

void SPIMessage::sendEchoMsg()
{
#ifdef DEBUG
    Serial1.println("Send ECHO Msg");
    Serial1.print("motor = ");
    Serial1.print(addr, DEC);
    Serial1.print(" SSPin=");
    Serial1.println(ssPin);
#endif 
    uint8_t msgLocal[3];
    memcpy(msgLocal, echoMsg, 3);
    digitalWrite(ssPin, LOW);
    SPI.transfer(msgLocal, 3);
    digitalWrite(ssPin, HIGH);    

#ifdef DEBUG
    Serial1.print("Recv message LEN=");
    Serial1.print(msgLocal[0], DEC);
    Serial1.print(" Msg =");
    phex(msgLocal[0]);
    phex(msgLocal[1]);
    phex(msgLocal[2]);
    phex(msgLocal[3]);
    phex(msgLocal[4]);
    Serial1.println("");
    Serial1.print(" actJob=");
    Serial1.println(actJob);
#endif

    if (actJob == JOB_ECHO && msgLocal[0] == 0x0A && msgLocal[1] == 0x10) {
#ifdef DEBUG
        Serial1.println("Send msg to PC ECHO");
#endif    
        Serial.write(echoRepMsg, 3);

        sendConfiguration(false);
        actJob = JOB_RESPONSE;
       
    } else 
        actJob = JOB_NOP;
}


void SPIMessage::setConfiguration(uint8_t *config, uint8_t len)
{
    memcpy(confMsg, config, len);
    confLen = len;
}

void SPIMessage::sendConfiguration(bool send2Pc)
{
#ifdef DEBUG
    Serial1.println("Send Configuration ");
    Serial1.print("motor = ");
    Serial1.print(addr, DEC);
    Serial1.print(" SSPin=");
    Serial1.print(ssPin);
    Serial1.print(" StopPin=");
    Serial1.println(stopPin);
#endif  
    digitalWrite(stopPin, LOW);
    uint8_t sendMsg[confLen];
    memcpy(sendMsg, confMsg, confLen);
    actJob = JOB_CONFIGURATION;
    digitalWrite(stopPin, HIGH);

#ifdef DEBUG
    Serial1.print("Send Msg: [");
    for (uint8_t s = 0; s < confLen; s++) {
        phex(sendMsg[s]);
    }
    Serial1.println("]");
#endif
    digitalWrite(ssPin, LOW);
    SPI.transfer(sendMsg, confLen);
    digitalWrite(ssPin, HIGH);
    if (send2Pc)
        msg->sendConfigDoneMsg(addr);  
}

void SPIMessage::stop()
{
#ifdef DEBUG
    Serial1.println("Stop ");
    Serial1.print("motor = ");
    Serial1.print(addr, DEC);
    Serial1.print(" StopPin=");
    Serial1.println(stopPin);
#endif      
    digitalWrite(stopPin, LOW);   
    delayMicroseconds(20);
    digitalWrite(stopPin, HIGH);   
}

void SPIMessage::moveSteps(uint8_t *msg, uint8_t len, bool home)
{
    digitalWrite(stopPin, LOW);
    actJob = home ? JOB_HOME_RETURN : JOB_POSITIONING ; 
    digitalWrite(stopPin, HIGH);
#ifdef DEBUG
    Serial1.print("Send steps ");
    Serial1.print("motor = ");
    Serial1.print(addr, DEC);
    Serial1.print(" SSPin=");
    Serial1.print(ssPin);
    Serial1.print(" StopPin=");
    Serial1.print(stopPin);
    Serial1.println(home ? " Home" : " Move ");
#endif  
    digitalWrite(ssPin, LOW);
    SPI.transfer(msg, len);
    digitalWrite(ssPin, HIGH);
    //msg->
}

void SPIMessage::sendProgressMsg()
{
    if (digitalRead(movePin) == HIGH)
        return;
#ifdef DEBUG
    Serial1.print("Send progres msg ");
    Serial1.print("motor = ");
    Serial1.print(addr, DEC);
    Serial1.print(" SSPin=");
    Serial1.println(ssPin);
#endif
    uint8_t sendMsg[3];
    memcpy(sendMsg, progressMsg, 3);
    actJob = JOB_PROGRESS;
    digitalWrite(ssPin, LOW);
    SPI.transfer(sendMsg, 3);
    digitalWrite(ssPin, HIGH);
}

void SPIMessage::checkIsDone()
{
    if (!finishedJob)
        return;

#ifdef DEBUG
    Serial1.print("CheckIsDone ");
    Serial1.print("motor = ");
    Serial1.print(addr, DEC);
    Serial1.print(" finishJob=");
    Serial1.print(finishedJob);
    Serial1.print(" actJob=");
    Serial1.println(actJob);
#endif

    finishedJob = false;

    if (actJob == JOB_RESPONSE) {
        actJob = JOB_NOP;
        return;
    }

    if (actJob == JOB_ECHO) {
        sendEchoMsg();
        return;
    }

    sendReplyMsg();
}