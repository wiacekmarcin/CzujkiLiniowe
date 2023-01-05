#ifndef SPIMSG_H
#define SPIMSG_H
#include <Arduino.h>

#include "proto.hpp"
class Motor; 
class SPIMessage 
{
    public:
    SPIMessage();

    void init(uint8_t mode, Motor * mot);
    static constexpr uint8_t maxBuff = 32;
    volatile uint8_t recvBuff[2*maxBuff];
    volatile uint8_t sendBuff[maxBuff];
    volatile uint8_t recvPos;
    volatile uint8_t sendPos;
    uint32_t sizeSendMsg = 0;
    volatile bool sendStopPos = false;
    
    void moveStopRequest(bool interrupted, bool move, bool error, bool home);
    bool proceed();
protected:

    void echoRequestFun(bool stopOk, bool moveOk);
    void progressRequestFun();
    void configurationRequest(Result status);
    void moveRequest(bool isHome, uint32_t steps, uint32_t delay);
    void emptyMove(uint8_t of);

private:
    uint8_t actProcess = 0;
    int8_t skipCharCnt;
    uint8_t address = 15;
    Message msg;
    Motor * mot;
    
};



#endif /* SPIMSG_H */
