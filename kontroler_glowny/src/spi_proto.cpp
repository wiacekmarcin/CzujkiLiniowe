#include "spi_proto.hpp"
#include "protocol.hpp"

#include<Wire.h>
#define DEBUG

#include "main.h"

static void phex(uint8_t b)
{
    SERIALDBG.print(" ");
    if (b < 16)
        SERIALDBG.print("0");
    SERIALDBG.print(b, HEX);
}

SPIMessage::SPIMessage()
: present(false)
, id (0)
, addr (0)
, stopPin(0)
, progressMsg{0, 0, 0}
, echoMsg{0, 0, 0}
, confLen(0)
, confMsg{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
, comunication(false)
, busyPinOk(false)
, movePinOk(false)
, stopPinOk(false)
{
    actJob = JOB_ECHO;
}

SPIMessage::~SPIMessage()
{

}

void SPIMessage::setDevice(uint8_t id, uint8_t addr)
{
    present = true;
    this->id = id;
    this->addr = addr;    
}

void SPIMessage::init(const uint8_t stopPin, const uint8_t movePin, const uint8_t busyPin)
{
    this->stopPin = stopPin;
    this->movePin = movePin;
    this->busyPin = busyPin;

    
    progressMsg[0] = (MessageSerial::PROGRESS_REQ << 4) & 0xf0;
    progressMsg[1] = (uint8_t)((id << 4) & 0xf0);
    CRC8 c;
    c.reset();
    c.add(progressMsg[0]);
    c.add(progressMsg[1]);
    progressMsg[2] = c.getCRC();

    c.reset();
    echoMsg[0] = (MessageSerial::ECHO_REQ << 4) & 0xf0;
    echoMsg[1] = (uint8_t)((id << 4) & 0xf0);
    c.add(echoMsg[0]);
    c.add(echoMsg[1]);
    echoMsg[2] = c.getCRC();

    c.reset();
    confMsg[0] = ((MessageSerial::CONF_REQ << 4) & 0xf0) | 12;
    confMsg[1] = (uint8_t)((id << 4) & 0xf0);
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

void SPIMessage::sendEchoMsg()
{
#ifdef DEBUG
    SERIALDBG.print(" M=");
    SERIALDBG.print(id, DEC);
    SERIALDBG.println(" Send ECHO Msg");
#endif 
    replyMsgSize = 3;
    sendSpiMsg(echoMsg, 3);
}

void SPIMessage::setConfiguration(uint8_t *config, uint8_t len)
{
    memcpy(confMsg, config, len);
    confLen = len;
}

void SPIMessage::sendConfiguration()
{
#ifdef DEBUG
    SERIALDBG.println("Send Configuration ");
    SERIALDBG.print("motor = ");
    SERIALDBG.println(id, DEC);
#endif  
    
    uint8_t sendMsg[confLen];
    memcpy(sendMsg, confMsg, confLen);
    
    replyMsgSize = 3;
    sendSpiMsg(sendMsg, confLen);
}

void SPIMessage::sendEnable(uint8_t *msg, uint8_t len)
{
    #ifdef DEBUG
    SERIALDBG.print("M");
    SERIALDBG.print(addr, DEC);
    SERIALDBG.print(" Send Enable Msg : [");
    for (uint8_t s = 0; s < len; s++) {
        phex(msg[s]);
    }
    SERIALDBG.println("]");
#endif
    replyMsgSize = 3;
    sendSpiMsg(msg, len);  
}

void SPIMessage::stop()
{
#ifdef DEBUG
    SERIALDBG.println("Stop ");
    SERIALDBG.print("motor = ");
    SERIALDBG.print(addr, DEC);
    SERIALDBG.print(" StopPin=");
    SERIALDBG.println(stopPin);
#endif      
    digitalWrite(stopPin, LOW);   
    delayMicroseconds(20);
    digitalWrite(stopPin, HIGH);  
}

void SPIMessage::moveSteps(uint8_t *msg, uint8_t len)
{
#ifdef DEBUG
    SERIALDBG.print("M");
    SERIALDBG.print(addr, DEC);
    SERIALDBG.print(" Send Steps Msg : [");
    for (uint8_t s = 0; s < len; s++) {
        phex(msg[s]);
    }
    SERIALDBG.println("]");
#endif
    replyMsgSize = 3;
    sendSpiMsg(msg, len);  
}

void SPIMessage::sendProgressMsg()
{
    //if (digitalRead(movePin) == HIGH) {
    //    SERIALDBG.print("High na MOVE");
    //    //sendEchoMsg();
    //    return;
    //}
#ifdef DEBUG
    SERIALDBG.print("M");
    SERIALDBG.print(id, DEC);
    SERIALDBG.print(" Send PROGRESS Msg : [");
    for (uint8_t s = 0; s < 3; s++) {
        phex(progressMsg[s]);
    }
    SERIALDBG.println("]");
#endif
    uint8_t sendMsg[3];
    memcpy(sendMsg, progressMsg, 3);
    replyMsgSize = 7;
    sendSpiMsg(sendMsg, 3);
}

void SPIMessage::sendSpiMsg(uint8_t * bytes, uint8_t cnt)
{
#ifdef DEBUG    
    SERIALDBG.print(" Send Msg : [");
    for (uint8_t s = 0; s < cnt; s++) {
        phex(bytes[s]);
    }
    SERIALDBG.println("]");
#endif
    Wire.beginTransmission(addr);
    Wire.write(bytes, cnt);
    unsigned char status = Wire.endTransmission();
#ifdef DEBUG
    if (status!=0) {
        SERIALDBG.print("send Status=");
        SERIALDBG.println(status);
    }
#endif
}

void SPIMessage::getReply()
{
    uint8_t recvBuff[20];
    uint8_t recvPos = 0;

    uint8_t recvS = Wire.requestFrom(addr, replyMsgSize);    // komunikaty sa glownie 3 bajtowe
    while (Wire.available()) { // peripheral may send less than requested
        recvBuff[recvPos++] = Wire.read(); // receive a byte as character
    }
    SERIALDBG.print("recvS=");
    SERIALDBG.println(recvS);
    if (recvPos < 3) {
        SERIALDBG.print("To short Message [");
        SERIALDBG.print(recvPos, DEC);
        SERIALDBG.println("]");
        return;
    }
#ifdef DEBUG    
    SERIALDBG.print("Recv Msg : [");
    for (uint8_t r = 0; r < recvPos; r++) {
        SERIALDBG.print(" ");
        SERIALDBG.print(recvBuff[r], HEX);
    }
    SERIALDBG.println("]");
#endif

    uint8_t tmp = 0;
    while (tmp < recvPos) {
        uint8_t cmd = (recvBuff[tmp+0] >> 4) & 0x0f;
        uint8_t len = recvBuff[tmp+0] & 0x0f;
        uint8_t id = (recvBuff[tmp+1] >> 4) & 0x0f;
        uint8_t opt = recvBuff[tmp+1] & 0x0f;
        if (cmd == MessageSerial::CONF_REP && id == this->id) {
            SERIALDBG.println("Send to PC");
            SERIALOUT.write(recvBuff+tmp, len + 3);
        }

        if (cmd == MessageSerial::MOVE_REP && id == this->id) {
            SERIALDBG.println("Send to PC");
            SERIALOUT.write(recvBuff+tmp, len + 3);
            delay(50);
            sendProgressMsg();
        }

        if (cmd == MessageSerial::PROGRESS_REP && id == this->id) {
            SERIALDBG.println("Send to PC");
            SERIALOUT.write(recvBuff+tmp, len + 3);
        }
        tmp += len+3;
    }
}

void SPIMessage::setPins(bool comunication, bool busyPin, bool movePin, bool stopPin)
{
    this->comunication = comunication;
    this->busyPinOk = busyPin;
    this->movePinOk = movePin;
    this->stopPinOk = stopPin;
}