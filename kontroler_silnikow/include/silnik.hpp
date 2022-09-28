#ifndef __SILNIK_H__
#define __SILNIK_H__

#include <Arduino.h>

class Motor
{

public:
    Motor();

    void setStop();
    bool moveHome();
    bool movePosition(uint32_t pos);

    void init();

    void setReverseMotor(bool rev) { reverseMotor = rev; }
    void setEnabledAlways(bool en) { enableAlways = en; }

    void setMaxSteps(uint32_t maxSteps) { this->maxSteps = maxSteps; }
    void setDelayImp(uint16_t delayImp) { this->delayImp = delayImp; }
    void setBaseSteps(uint16_t baseSteps) { this->baseSteps = baseSteps; }

    static constexpr uint8_t KRANCPIN = 2;

protected:
    inline void setEnabled(bool en);
    inline void setDir(bool back);

private:
    static constexpr uint8_t ENPIN = 7;
    static constexpr uint8_t DIRPIN = 8;
    static constexpr uint8_t PULSEPIN = 9;

    static constexpr uint8_t ENPIN2 = 15;
    static constexpr uint8_t DIRPIN2 = 16;
    static constexpr uint8_t PULSEPIN2 = 17;

    bool reverseMotor;
    bool enableAlways;
    uint32_t maxSteps;
    uint16_t delayImp;
    uint16_t baseSteps;

    bool canMove;
    uint32_t globalPos;

    bool wasHome;
};

#endif // __SILNIK_H__