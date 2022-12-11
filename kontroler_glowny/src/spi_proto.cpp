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

static void phex0(uint8_t b)
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
, connected(false)
{
    actJob = JOB_ECHO;
}

SPIMessage::~SPIMessage()
{

}

void SPIMessage::init(const uint8_t addr, const uint8_t ssPin, const uint8_t stopPin, const uint8_t movePin, const uint8_t busyPin, MessageSerial * msg)
{
    this->addr = addr;
    this->ssPin = ssPin;
    this->stopPin = stopPin;
    this->movePin = movePin;
    this->busyPin = busyPin;
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
        replyMsg[b] = ((b-3) << 4) + addr;
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
    confMsg[0] = ((MessageSerial::CONF_REQ << 4) & 0xf0) | 12;
    confMsg[1] = (uint8_t)((addr << 4) & 0xf0) | 0x00;
    confMsg[2] = 0;
    confMsg[3] = 0;
    confMsg[4] = 0;
    confMsg[5] = 0;
    confMsg[6] = 0;
    confMsg[7] = 0;
    confMsg[8] = 0;
    confMsg[9] = 0;
    confMsg[10] = 0;
    confMsg[11] = 0;
    confMsg[12] = 0;
    confMsg[13] = 0;
    confMsg[14] = 0;

    for (uint8_t z = 0; z < 15; ++z) {
        c.add(confMsg[z]);
    }
    confMsg[15] = c.getCRC();
    confLen = 16;
}

void SPIMessage::sendReplyMsg()
{
#ifdef DEBUG
    Serial1.print("M");
    Serial1.print(addr, DEC);
    Serial1.print(" Send Reply Msg : [");
    for (uint8_t s = 0; s < 20; s++) {
        phex(replyMsg[s]);
    }
    Serial1.println("]");
#endif 
    uint8_t msgLocal[20];
    memcpy(msgLocal, replyMsg, 20);
    sendSpiMsg(msgLocal, 20);

#ifdef DEBUG
    Serial1.print("Recv message [");
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
    Serial1.println("]");

#endif
    bool send2Pc = false;
    if (msgLocal[0] == 0x0A && msgLocal[1] == 3 ) {
        if (msgLocal[2] == 0x10 && ((addr << 4) | 8) == msgLocal[3]) {
#ifdef DEBUG
            Serial1.println("Connected");
#endif  
            connected = true;
            return;
        }
        if (msgLocal[2] == 0x90 && ((addr << 4) | 8) == msgLocal[3]) {
#ifdef DEBUG
            Serial1.println("Send Rep msg to PC");
#endif  
            Serial2.write(msgLocal+2, msgLocal[1]);
            return;
        }
    } 
    if (msgLocal[0] == 0x0A && msgLocal[1] == 7 && msgLocal[3] == 0x74 && ((msgLocal[4] >> 4) & 0x0f) == addr) {
#ifdef DEBUG
            Serial1.println("Send Rep msg to PC");
#endif  
            Serial2.write(msgLocal+2, msgLocal[1]);
            return;
    }
}

void SPIMessage::sendEchoMsg()
{
#ifdef DEBUG
    Serial1.print("M");
    Serial1.print(addr, DEC);
    Serial1.print(" Send ECHO Msg : [");
    for (uint8_t s = 0; s < 3; s++) {
        phex(echoMsg[s]);
    }
    Serial1.println("]");
#endif 
    uint8_t msgLocal[3];
    memcpy(msgLocal, echoMsg, 3);
    sendSpiMsg(msgLocal, 3);
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
    Serial1.println(addr, DEC);
#endif  
    digitalWrite(stopPin, LOW);
    uint8_t sendMsg[confLen];
    memcpy(sendMsg, confMsg, confLen);
    digitalWrite(stopPin, HIGH);

#ifdef DEBUG
    Serial1.print("Send Msg: [");
    for (uint8_t s = 0; s < confLen; s++) {
        phex(sendMsg[s]);
    }
    Serial1.println("]");
#endif
    sendSpiMsg(sendMsg, confLen);
    if (send2Pc)
        msg->sendConfigDoneMsg(addr, true);  
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

void SPIMessage::moveSteps(uint8_t *msg, uint8_t len)
{
    digitalWrite(stopPin, LOW);
    delayMicroseconds(20);
    digitalWrite(stopPin, HIGH);
#ifdef DEBUG
    Serial1.print("M");
    Serial1.print(addr, DEC);
    Serial1.print(" Send Steps Msg : [");
    for (uint8_t s = 0; s < len; s++) {
        phex(msg[s]);
    }
    Serial1.println("]");
#endif
    sendSpiMsg(msg, len);  
}

void SPIMessage::sendProgressMsg()
{
    //if (digitalRead(movePin) == HIGH) {
    //    Serial1.print("High na MOVE");
    //    //sendEchoMsg();
    //    return;
    //}
#ifdef DEBUG
    Serial1.print("M");
    Serial1.print(addr, DEC);
    Serial1.print(" Send PROGRESS Msg : [");
    for (uint8_t s = 0; s < 3; s++) {
        phex(progressMsg[s]);
    }
    Serial1.println("]");
#endif
    uint8_t sendMsg[3];
    memcpy(sendMsg, progressMsg, 3);
    sendSpiMsg(sendMsg, 3);
}

void SPIMessage::sendSpiMsg(uint8_t * bytes, uint8_t cnt)
{

    digitalWrite(SS, LOW);
    digitalWrite(ssPin, LOW);
    delayMicroseconds(40);
    SPI.transfer(bytes, cnt);
    digitalWrite(ssPin, HIGH);
    digitalWrite(SS, HIGH);
}
