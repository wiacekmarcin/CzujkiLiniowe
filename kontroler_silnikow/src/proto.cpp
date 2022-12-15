#include "proto.hpp"
#include <SPI.h>
#include "crc8.hpp"

#define DEBUG

#ifdef DEBUG
	#define SD(T) Serial.print(T)
	#define SDN(T) Serial.println(T)
	#define SD2(T,P) Serial.print(T,P)
	#define SDN2(T,P) Serial.println(T,P)

	#define SDP(T, V) SD(T); SD(V)
	#define SDPN(T, V) SD(T); SDN(V)
    #define SPHEX(X) phex(X)
    #define SPRINT(N)	SD("Parse");for (int i=0;i<N;++i){ SPHEX(sendBuff[i]); }SDN("]");
#else
	#define SD(T) 
	#define SDN(T) 
	#define SD2(T,P) 
	#define SDN2(T,P) 

	#define SDP(T, V) 
	#define SDPN(T, V)
    #define SPHEX(X)
    #define SPRINT(N)
#endif

Message::Message():
    lenMsg(0)
    ,cmdMsg(INV_MSG)
    ,crcMsg(0)
    ,addrMsg(0)
    ,options(0)
    ,posData(0)
    ,recvPos(0)
    ,sendPos(0)
{
    clear();
}

void Message::init()
{
  
}

void Message::clear()
{
    for (uint8_t i = 0; i < maxBuff; ++i) {
        recvBuff[i] = 0;
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
    recvBuff[recvPos++] = b;

    if (!startMsg && recvPos == 1) {
        startMsg = true;
        lenMsg = recvBuff[0] & 0x0f;
        cmdMsg = conv(recvBuff[0]);
        return false;
    }

    if (!startMsg) {
        return false;
    }
        

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

Result Message::parse()
{
    CRC8 c;
    Result r;
    r.ok = true;

    c.reset();
    SD("Parse  [");
    for (int i=0; i< lenMsg + restMsgCnt-1; ++i ) {
        SD2(recvBuff[i], HEX);SD(" ");
        c.add(recvBuff[i]);
    }
    SD2(crcMsg, HEX); SDN("]");
   
    if (c.getCRC() != crcMsg) {
        SD("invalid crc ");SD2(crcMsg, HEX);SD("!=");SDN2(c.getCRC(), HEX);
        r.ok = false;
        return r;
    }

    SD("cmdMsg=");SD2(cmdMsg, DEC);SD(" lenMsg");SD2(lenMsg, DEC);SD(" addr=");SDN2(addrMsg, DEC);
    if (cmdMsg == LAST_REQ) {
        return r;
    }

    if (cmdMsg == ECHO_REQ) {
        return r;
    }

    if (cmdMsg == PROGRESS_REQ) {
        return r;
    }
    
    if (cmdMsg == CONF_REQ) {
        r.data.conf.reverse = (options & 0x01) == 0x01;
        r.data.conf.maxStep = toNumber32(dataCmd[0], dataCmd[1], dataCmd[2], dataCmd[3]);
        r.data.conf.baseSteps = toNumber32(dataCmd[4], dataCmd[5], dataCmd[6], dataCmd[7]);
        r.data.conf.middleSteps = toNumber32(dataCmd[8], dataCmd[9], dataCmd[10], dataCmd[11]);
        return r;
    }

    if (cmdMsg == MOVE_REQ) {
        r.data.move.isHome = (options & 0x01);
        if (r.data.move.isHome) 
            r.data.move.position = 0;
        else    
            r.data.move.position = (int32_t)toNumber32(dataCmd[0], dataCmd[1], dataCmd[2], dataCmd[3]);
        r.data.move.speed = toNumber32(dataCmd[4], dataCmd[5], dataCmd[6], dataCmd[7]);    
        return r;
    }
    SDN("Nieznana wiadomosc");
    r.ok = false;
    return r;
}

uint32_t Message::toNumber32(uint8_t n4, uint8_t n3, uint8_t n2, uint8_t n1)
{
    return ((uint32_t) 0) | ( ((uint32_t)n4) << 24 ) | ( ((uint32_t)n3) << 16 ) | ( ((uint32_t)n2) << 8 ) | n1;
}

uint32_t Message::toNumber24(uint8_t n3, uint8_t n2, uint8_t n1)
{
    return ((uint32_t) 0) | ( ((uint32_t)n3) << 16 ) | ( ((uint32_t)n2) << 8 ) | n1;
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
