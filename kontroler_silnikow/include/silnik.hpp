#ifndef __SILNIK_H__
#define __SILNIK_H__

#include <Arduino.h>

class Motor
{

public:
    Motor();

    void setStop();
    void moveHome();
    void movePosition(uint32_t pos);

    void init();

    void setReverseMotor(bool rev) { reverseMotor = rev; }
    void setEnabledAlways(bool en) { enableAlways = en; }

    void setMaxSteps(uint32_t maxSteps) { this->maxSteps = maxSteps; }
    void setDelayImp(uint16_t delayImp) { this->delayImp = delayImp; }
    void setBaseSteps(uint16_t baseSteps) { this->baseSteps = baseSteps; }

    int32_t getGlobalPos() const { return globalPos; }
    bool getIsMoveHome() const { return isMoveHome; }

    void impulse();

    static constexpr uint8_t KRANCPIN = 2;

protected:
    inline void setEnabled(bool en);
    inline void setDir(bool back);

private:
    static constexpr uint8_t ENPIN = 7;
    static constexpr uint8_t DIRPIN = 8;
    static constexpr uint8_t PULSEPIN = 9;

    bool reverseMotor;
    bool enableAlways;

    uint32_t maxSteps;
    uint16_t delayImp;
    uint16_t baseSteps;

    volatile bool isRun;
    int32_t newPosition;
    short diff;

    bool canMove;
    volatile int32_t globalPos;

    bool wasHome;
    volatile bool highlevel;

    bool isMoveHome;
    volatile uint32_t actSteps;
};

#endif // __SILNIK_H__
