#ifndef __SILNIK_H__
#define __SILNIK_H__

#include <Arduino.h>

/**
 * silniki przy filtrach 
 * 640 w bazie
 * 30 400 krokow
 * 
 */





class Motor
{

public:
    Motor();

    void setStop(bool hard);
    void moveHome();
    void movePosition(uint32_t pos);

    void init();

    void setReverseMotor(bool rev) { reverseMotor = rev; }
    

    void setMaxSteps(uint32_t maxSteps) { this->maxSteps = maxSteps; }
    void setDelayImp(uint16_t delayImp) { this->delayImp = delayImp; }
    void setBaseSteps(uint16_t baseSteps) { this->baseSteps = baseSteps; }

    int32_t getGlobalPos() const { return globalPos; }
    bool getIsMoveHome() const { return isMoveHome; }

    bool impulse();

    void print();

    static constexpr uint8_t KRANCPIN = 2;

    void setDir(bool back);

private:

    static constexpr uint8_t DIRPIN = 8;
    static constexpr uint8_t PULSEPIN = 9;
    static constexpr uint8_t MOVEPIN = 5;

    bool reverseMotor;
    bool enableAlways;

    uint32_t maxSteps;
    uint16_t delayImp;
    uint16_t baseSteps;

    volatile bool isRun;
    int32_t newPosition;
    short diff;

    volatile int32_t globalPos;

    bool wasHome;
    volatile bool highlevel;

    bool isMoveHome;
    volatile uint32_t actSteps;

    uint8_t cntPrint;
    bool isMove;

};

#endif // __SILNIK_H__
