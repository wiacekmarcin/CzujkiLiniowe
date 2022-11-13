#include "proto.hpp"
#include <SPI.h>
#include "crc8.hpp"

void phex(uint8_t b)
{
    Serial.print(" ");
    if (b < 10)
        Serial.print("0");
    Serial.print(b, HEX);
}


Message::Message():
    lenMsg(0)
    ,cmdMsg(INV_MSG)
    ,crcMsg(0)
    ,addrMsg(0)
    ,options(0)
    ,posData(0)
    ,recvPos(0)
    ,sendPos(0)
    ,skipChars(0)
{
    clear();
}

void Message::init()
{
    SPCR |= _BV(SPE);                       //Turn on SPI in Slave Mode
  	SPI.attachInterrupt();     
}

void Message::clear()
{
    for (uint8_t i = 0; i < maxBuff; ++i) {
        recvBuff[i] = 0;
        sendBuff[i] = 0;
    }
    recvPos = 0;
    startMsg = false;
    lenMsg = 0;
    posData = 0;
    sendPos = 0;
    addrMsg = 0;
}

bool Message::add(uint8_t b)
{
    phex(b);
    if (skipChars) {
        --skipChars;
        Serial.println(" Skip");
        return false;
    }else 
    {
        Serial.println(" Proc");
    }
        

    recvBuff[recvPos++] = b;

    if (!startMsg && recvPos == 1) {
        startMsg = true;
        lenMsg = recvBuff[0] & 0x0f;
        cmdMsg = conv(recvBuff[0]);
        return false;
    }

    if (!startMsg)
        return false;

    if (recvPos == 2) {
        addrMsg = (recvBuff[1] >> 4)  & 0x0f;
        options = recvBuff[1] & 0x0f;
        return false;
    }

    if (recvPos == 3) {
        posData = 0;
    }

    if (posData == lenMsg) {
        crcMsg = b;
        return true;
    } else {
        dataCmd[posData++] = b;
        return false;
    }
}

void Message::addSteps(bool success, uint32_t steps)
{
    
    sendBuff[sendPos++]	= success;
    sendBuff[sendPos++]	= 0xff & (steps >> 24);
	sendBuff[sendPos++]	= 0xff & (steps >> 16);
	sendBuff[sendPos++]	= 0xff & (steps >> 8);
	sendBuff[sendPos++]	= 0xff & steps; 
    sendBuff[0] = (sendBuff[0] & 0xf0) | ( 5 & 0x0f );

    CRC8 c;
    c.reset();
    for (int i=0; i<sendPos; ++i )
        c.add(*(sendBuff+i));
    sendBuff[sendPos++] = c.getCRC();
}

void Message::copy(volatile uint8_t * tab)
{
    uint8_t * p = sendBuff;
    for (uint8_t i = 0; i < maxBuff; i++) {
        *(tab++) = *(p++);
    }
}

Result Message::parse()
{
    CRC8 c;
    Result r;
    r.ok = true;

    c.reset();
    Serial.print("Parse  ");
    for (int i=0; i< lenMsg + restMsgCnt-1; ++i ) {
        Serial.print(recvBuff[i], HEX);
        Serial.print(" ");
        c.add(recvBuff[i]);
    }
    Serial.print(" crc=");
    Serial.println(recvBuff[lenMsg + restMsgCnt-1], HEX);
    
    if (c.getCRC() != recvBuff[lenMsg + restMsgCnt-1]) {
        Serial.print("invalid crc=");
        Serial.println(c.getCRC(), HEX);
        r.ok = false;
        return r;
    }
    
    Serial.print("cmdMsg");
    Serial.println(cmdMsg, DEC);
    Serial.print("lenMsg");
    Serial.println(lenMsg, DEC);
    Serial.print("addr");
    Serial.println(addrMsg, DEC);
    if (cmdMsg == LAST_REQ) {
        skipChars = 17;
        return r;
    }

    if (cmdMsg == ECHO_REQ) {
        return r;
    }

    if (cmdMsg == PROGRESS_REQ) {
        return r;
    }
    
    if (cmdMsg == CONF_REQ) {
        r.data.conf.reverse = (dataCmd[0] & 0x01) == 0x01;
        r.data.conf.enableAlways = (dataCmd[0] & 0x02) == 0x02;
        r.data.conf.maxStep = toNumber32(dataCmd[1], dataCmd[2], dataCmd[3], dataCmd[4]);
        r.data.conf.baseSteps = toNumber16(dataCmd[5], dataCmd[6]);
        r.data.conf.delayImp = toNumber16(dataCmd[7], dataCmd[8]);
        return r;
    }

    if (cmdMsg == MOVE_REQ) {
        r.data.move.isHome = (options & 0x01);
        if (r.data.move.isHome) 
            r.data.move.steps = 0;
        else    
            r.data.move.steps = toNumber32(dataCmd[0], dataCmd[1], dataCmd[2], dataCmd[3]);
        return r;
    }

    Serial.println("Nieznana wiadomosc");
    r.ok = false;
    return r;
}

uint32_t Message::toNumber32(uint8_t n4, uint8_t n3, uint8_t n2, uint8_t n1)
{
    return ((uint32_t) 0) | ( ((uint32_t)n4) << 24 ) | ( ((uint32_t)n3) << 16 ) | ( ((uint32_t)n2) << 8 ) | n1;
}

uint16_t Message::toNumber16(uint8_t n2, uint8_t n1)
{
    return ((uint16_t) 0) | ( ((uint32_t)n2) << 8 ) | n1;
}

Msgtype Message::conv(uint8_t m)
{
    switch(m & 0xf0) {
        case ECHO_REQ << 4: return ECHO_REQ;
        case LAST_REQ << 4: return LAST_REQ;
        case CONF_REQ << 4: return CONF_REQ;
        case MOVE_REQ << 4: return MOVE_REQ;
        case PROGRESS_REQ << 4: return PROGRESS_REQ;
        default: return INV_MSG;
    }
}
