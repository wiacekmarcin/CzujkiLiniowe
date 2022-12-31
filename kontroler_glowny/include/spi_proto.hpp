#ifndef SPIMESSAGE_H
#define SPIMESSAGE_H
#include <Arduino.h>
#pragma once

#include "protocol.hpp"

typedef enum _actJob {
    JOB_NOP,
    JOB_CONFIGURATION,
    JOB_PROGRESS,
    JOB_HOME_RETURN,
    JOB_POSITIONING,
    JOB_RESPONSE,
    JOB_ECHO
} actJobType;


class SPIMessage
{
public:
    SPIMessage();
    ~SPIMessage();
    void setDevice(uint8_t id, uint8_t addr);
    bool isActive() const { return present && comunication; };
    void init(const uint8_t stopPin, const uint8_t movePin, const uint8_t busy);

    //void sendReplyMsg();
    void sendProgressMsg();
    void sendEchoMsg();
    void setConfiguration(uint8_t config[], uint8_t confLen);
    void sendConfiguration();
    bool sendConfigurationWithResponse(uint16_t activeBusy);
    void moveSteps(uint8_t *msg, uint8_t len);
    void stop();

    void getReply();
    inline bool isConnected() const { return comunication; }
    inline uint8_t getByte2() const { return ((id << 4) & 0x0f) | (present ? 0x08 : 0x00) | (comunication ? 0x04 : 0x00) | 
                        (busyPinOk ? 0x02 : 0x0) | ((movePinOk && stopPinOk) ? 0x01 : 0x00); }
    bool isMove() { return digitalRead(stopPin) == LOW; }
    void setPins(bool comunication, bool busyPin, bool movePin, bool stopPin);

protected:
    void sendSpiMsg(uint8_t * bytes, uint8_t cnt);
private:
    bool present;
    uint8_t id;
    uint8_t addr;
    uint8_t stopPin;
    uint8_t movePin;
    uint8_t busyPin;
    
    uint8_t progressMsg[3];
    uint8_t echoMsg[3];
    uint8_t confLen;
    uint8_t confMsg[18];

    actJobType actJob;
    bool comunication;
    bool busyPinOk;
    bool movePinOk;
    bool stopPinOk;
};

#endif