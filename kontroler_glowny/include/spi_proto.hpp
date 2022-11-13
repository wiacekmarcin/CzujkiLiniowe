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
} actJobType;


class SPIMessage
{
public:
    SPIMessage();
    ~SPIMessage();
    void init(const uint8_t addr, const uint8_t ssPin, const uint8_t stopPin, MessageSerial * msg);

    void sendReplyMsg();
    void sendProgressMsg();
    void setConfiguration(uint8_t config[], uint8_t confLen);
    void sendConfiguration();
    void moveSteps(uint8_t *msg, uint8_t len, bool home);
    void stop();
    void checkIsDone();
    inline volatile void setFinish() { finishedJob = true; }
private:
    uint8_t addr;
    uint8_t ssPin;
    uint8_t stopPin;
    uint8_t replyMsg[20];
    uint8_t progressMsg[3];
    static constexpr uint8_t confLen = 18;
    uint8_t confMsg[confLen];
    bool finishedJob;
    MessageSerial * msg;
    actJobType actJob;
};

#endif