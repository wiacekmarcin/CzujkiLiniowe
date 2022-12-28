#include "spi_proto.hpp"
#include "protocol.hpp"

#include<Wire.h>
#define DEBUG

#define SERIALOUT Serial
#define SERIALDBG Serial1

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
, connected(false)
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

/*
void SPIMessage::sendReplyMsg()
{
#ifdef DEBUG
    SERIALDBG.print("M");
    SERIALDBG.print(addr, DEC);
    SERIALDBG.print(" Send Reply Msg : [");
    for (uint8_t s = 0; s < 20; s++) {
        phex(replyMsg[s]);
    }
    SERIALDBG.println("]");
#endif 
    uint8_t msgLocal[20];
    memcpy(msgLocal, replyMsg, 20);
    sendSpiMsg(msgLocal, 20);


    bool send2Pc = false;
    if (msgLocal[0] == 0x0A && msgLocal[1] == 3 ) {
        if (msgLocal[2] == 0x10 && ((addr << 4) | 8) == msgLocal[3]) {
#ifdef DEBUG
            //SERIALDBG.println("Connected");
#endif  
            connected = true;
            return;
        }
        if (msgLocal[2] == 0x90 && ((addr << 4) | 8) == msgLocal[3]) {
#ifdef DEBUG
            //SERIALDBG.println("Send Rep msg to PC");
#endif  
            SERIALOUT.write(msgLocal+2, msgLocal[1]);
            return;
        }
    } 
    if (msgLocal[0] == 0x0A && msgLocal[1] == 7 && msgLocal[3] == 0x74 && ((msgLocal[4] >> 4) & 0x0f) == addr) {
#ifdef DEBUG
            //SERIALDBG.println("Send Rep msg to PC");
#endif  
            SERIALOUT.write(msgLocal+2, msgLocal[1]);
            return;
    }
}
*/

void SPIMessage::sendEchoMsg()
{
#ifdef DEBUG
    SERIALDBG.print("M");
    SERIALDBG.print(id, DEC);
    SERIALDBG.print(" Send ECHO Msg : [");
    for (uint8_t s = 0; s < 3; s++) {
        phex(echoMsg[s]);
    }
    SERIALDBG.println("]");
#endif 
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
    SERIALDBG.println(addr, DEC);
#endif  
    digitalWrite(stopPin, LOW);
    uint8_t sendMsg[confLen];
    memcpy(sendMsg, confMsg, confLen);
    digitalWrite(stopPin, HIGH);

#ifdef DEBUG
    SERIALDBG.print("Send Msg: [");
    for (uint8_t s = 0; s < confLen; s++) {
        phex(sendMsg[s]);
    }
    SERIALDBG.println("]");
#endif
    sendSpiMsg(sendMsg, confLen);
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
    digitalWrite(stopPin, LOW);
    delayMicroseconds(20);
    digitalWrite(stopPin, HIGH);
#ifdef DEBUG
    SERIALDBG.print("M");
    SERIALDBG.print(addr, DEC);
    SERIALDBG.print(" Send Steps Msg : [");
    for (uint8_t s = 0; s < len; s++) {
        phex(msg[s]);
    }
    SERIALDBG.println("]");
#endif
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
    SERIALDBG.print("send Status=");
    SERIALDBG.println(status);
#endif
}

void SPIMessage::getReply()
{
    uint8_t recvBuff[20];
    uint8_t recvPos = 0;

    Wire.requestFrom(addr, (uint8_t)3);    // komunikaty sa glownie 3 bajtowe
    while (Wire.available() && recvPos < 3) { // peripheral may send less than requested
        recvBuff[recvPos++] = Wire.read(); // receive a byte as character
    }
    if (recvPos < 3) {
        SERIALDBG.print("To short Message [");
        SERIALDBG.print(recvPos, DEC);
        SERIALDBG.println("]");
        return;
    }
        
    uint8_t cmd = (recvBuff[0] >> 4) & 0x0f;
    uint8_t len = recvBuff[0] & 0x0f;
    uint8_t id = (recvBuff[1] >> 4) & 0x0f;
    uint8_t opt = recvBuff[1] & 0x0f;
    if (len > 0) {
        Wire.requestFrom(addr, (uint8_t)(len+1));    // 
        while (Wire.available() && recvPos < 3 + len) { // peripheral may send less than requested
            recvBuff[recvPos++] = Wire.read(); // receive a byte as character
        }
    }

    if (recvPos < 3 + len) {
        SERIALDBG.println("To short Message 2");
        return;
    }
    
#ifdef DEBUG    
    SERIALDBG.print(" Recv Msg : [");
    for (uint8_t r = 0; r < recvPos; r++) {
        SERIALDBG.print(" ");
        SERIALDBG.print(recvBuff[r], HEX);
    }
    SERIALDBG.println("]");
#endif

    if (cmd == MessageSerial::ECHO_REP && id == this->id) {
        connected = true;
        optEcho = opt;
    }
    
    if (cmd == MessageSerial::CONF_REP && id == this->id) {
        SERIALOUT.write(recvBuff, len + 3);
    }
}
