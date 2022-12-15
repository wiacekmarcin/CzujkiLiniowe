#include "spi_proto.hpp"
#include "protocol.hpp"

#include<SPI.h>
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

static void phex0(uint8_t b)
{
    SERIALDBG.print(" ");
    if (b < 16)
        SERIALDBG.print("0");
    SERIALDBG.print(b, HEX);
}


SPIMessage::SPIMessage()
: addr (0)
, ssPin(0)
, stopPin(0)
, replyMsg{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
, progressMsg{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
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
    replyMsg[0] = ((MessageSerial::LAST_REQ << 4) & 0xf0) | 15;
    replyMsg[1] = (uint8_t)((addr << 4) & 0xf0);
    c.add(replyMsg[0]);
    c.add(replyMsg[1]);
    for (uint8_t b=0; b<15; ++b)
    {
        replyMsg[2+b] = ((b) << 4) + addr;
        c.add(replyMsg[b+2]);
    }
    replyMsg[17] = c.getCRC();
    replyMsg[18] = 0X0A;
    replyMsg[19] = 0X0A;
    
    
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
    confMsg[1] = (uint8_t)((addr << 4) & 0xf0);
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
if (addr == 5) {
    SERIALDBG.print("M");
    SERIALDBG.print(addr, DEC);
    SERIALDBG.print(" Send Reply Msg : [");
    for (uint8_t s = 0; s < 20; s++) {
        phex(replyMsg[s]);
    }
    SERIALDBG.println("]");
}
#endif 
    uint8_t msgLocal[20];
    memcpy(msgLocal, replyMsg, 20);
    sendSpiMsg(msgLocal, 20);

#ifdef DEBUG
    if (addr == 5) {
    SERIALDBG.print("Recv message [");
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
    phex(msgLocal[10]);
    phex(msgLocal[11]);
    phex(msgLocal[12]);
    phex(msgLocal[13]);
    phex(msgLocal[14]);
    phex(msgLocal[15]);
    phex(msgLocal[16]);
    phex(msgLocal[17]);
    phex(msgLocal[18]);
    phex(msgLocal[19]);
    SERIALDBG.println("]");
    }

#endif
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

void SPIMessage::sendEchoMsg()
{
#ifdef DEBUG
    SERIALDBG.print("M");
    SERIALDBG.print(addr, DEC);
    SERIALDBG.print(" Send ECHO Msg : [");
    for (uint8_t s = 0; s < 3; s++) {
        phex(echoMsg[s]);
    }
    SERIALDBG.println("]");
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
    SERIALDBG.print(addr, DEC);
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

    digitalWrite(SS, LOW);
    digitalWrite(ssPin, LOW);
    delayMicroseconds(40);
    SPI.transfer(bytes, cnt);
    digitalWrite(ssPin, HIGH);
    digitalWrite(SS, HIGH);
}
