#include "spi_proto.hpp"
#include "protocol.hpp"

#include<SPI.h>

static void phex(uint8_t b)
{
    Serial1.print(" ");
    if (b < 10)
        Serial1.print("0");
    Serial1.print(b, HEX);
}



SPIMessage::SPIMessage()
: addr (0)
, ssPin(0)
, stopPin(0)
, replyMsg({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})
, progressMsg({0, 0, 0})
, confMsg({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})
, finishedJob(false)
, actJob(JOB_NOP)
{
    
}

SPIMessage::~SPIMessage()
{

}

void SPIMessage::init(const uint8_t addr, const uint8_t ssPin, const uint8_t stopPin, MessageSerial * msg)
{
    this->addr = addr;
    this->ssPin = ssPin;
    this->stopPin = stopPin;
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
}

void SPIMessage::sendReplyMsg()
{
#ifdef DEBUG
    Serial1.println("Send Reply Msg");
    Serial1.print("motor = ");
    Serial1.print(addr, DEC);
    Serial1.print(" SSPin=");
    Serial1.print(ssPins);
#endif 
    uint8_t msgLocal[20];
    memcpy(replyMsg, msgLocal, 20);
    actJob = JOB_RESPONSE;
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
#endif 
    if (msgLocal[0] = 0x00 && msgLocal[1] < 20) {
#ifdef DEBUG
        Serial1.println("Send msg to PC");
#endif    
        Serial.write(msgLocal + 2, msgLocal[1]);
    }
}


void SPIMessage::setConfiguration(uint8_t *config, uint8_t len)
{
    memcpy(confMsg, config, len);
}

void SPIMessage::sendConfiguration()
{
#ifdef DEBUG
    Serial1.println("Send Configuration ");
    Serial1.print("motor = ");
    Serial1.print(addr, DEC);
    Serial1.print(" SSPin=");
    Serial1.print(ssPins);
    Serial1.print(" StopPin=");
    Serial1.println(stopPin);
#endif  
    digitalWrite(stopPin, LOW);
    uint8_t sendMsg[confLen];
    memcpy(progressMsg, sendMsg, confLen);
    actJob = JOB_CONFIGURATION;
    digitalWrite(stopPin, HIGH);
    digitalWrite(ssPin, LOW);
    SPI.transfer(sendMsg, confLen);
    digitalWrite(ssPin, HIGH);
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
    Serial1.print(ssPins);
    Serial1.print(" StopPin=");
    Serial1.print(stopPin);
    Serial1.print(((msg.getOptions() & 0x01) == 0x01) ? " Home" : "Move ");
    Serial1.print(" PIN=");
    Serial1.println(ssPins[motor]);
#endif  
    digitalWrite(ssPin, LOW);
    SPI.transfer(msg, len);
    digitalWrite(ssPin, HIGH);
    //msg->
}

void SPIMessage::sendProgressMsg()
{
#ifdef DEBUG
    Serial1.print("Send progres msg ");
    Serial1.print("motor = ");
    Serial1.print(addr, DEC);
    Serial1.print(" SSPin=");
    Serial1.print(ssPins);
#endif
    uint8_t sendMsg[3];
    memcpy(progressMsg, sendMsg, 3);
    actJob = JOB_PROGRESS;
    digitalWrite(ssPin, LOW);
    SPI.transfer(sendMsg, 3);
    digitalWrite(ssPin, HIGH);
}

void SPIMessage::checkIsDone()
{
    if (!finishedJob)
        return;

    finishedJob = false;

    if (actJob != JOB_RESPONSE)
        sendReplyMsg();
    
    actJob = JOB_NOP;
}