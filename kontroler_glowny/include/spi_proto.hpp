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
    void init(const uint8_t addr, const uint8_t ssPin, const uint8_t stopPin, const uint8_t movePin, const uint8_t busy, MessageSerial * msg);

    void sendReplyMsg();
    void sendProgressMsg();
    void sendEchoMsg();
    void setConfiguration(uint8_t config[], uint8_t confLen);
    void sendConfiguration();
    bool sendConfigurationWithResponse(uint16_t activeBusy);
    void moveSteps(uint8_t *msg, uint8_t len);
    void stop();
    inline bool isConnected() const { return connected; }
    inline uint8_t getByte2() const { return confMsg[1] | (connected ? 0x80 : 0x00); }

protected:
    void sendSpiMsg(uint8_t * bytes, uint8_t cnt);
private:
    uint8_t addr;
    uint8_t ssPin;
    uint8_t stopPin;
    uint8_t movePin;
    uint8_t busyPin;
    uint8_t replyMsg[20];
    uint8_t progressMsg[3];
    uint8_t echoMsg[3];
    uint8_t echoRepMsg[3];
    uint8_t confLen;
    uint8_t confMsg[18];
    MessageSerial * msg;
    actJobType actJob;
    bool connected;
};

#endif