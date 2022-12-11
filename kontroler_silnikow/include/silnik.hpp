#ifndef __SILNIK_H__
#define __SILNIK_H__

#include <Arduino.h>


typedef enum moveState {
    IDLE,
    HOME_OD_BAZY,
    HOME_DO_BAZY,
    HOME_W_BAZIE,
    HOME_DO_SRODKA,
    MOVE_POS,
} moveStateType;
class Motor
{

public:
    Motor();

    void setStop(bool hard);
    bool moveHome(uint8_t mode);
    bool movePosition(uint32_t pos);

    void init(uint8_t mode);

    void setReverseMotor(bool rev) { reverseMotor = rev; }
    

    void setMaxSteps(uint32_t maxSteps) { this->maxSteps = maxSteps; }
    void setDelayImp(uint16_t delayImp) { this->delayImp = delayImp; }
    void setBaseSteps(uint16_t baseSteps) { this->baseSteps = baseSteps; }
    void setMiddleSteps(uint16_t middleSteps) { this->middleSteps = middleSteps; }

    int32_t getGlobalPos() const { return globalPos; }
    bool isMove() const { return mstate != IDLE; }
    bool isHomeMove() const { return mstate != IDLE && mstate != MOVE_POS; }

    void impulse();

    static constexpr uint8_t KRANCPIN = 2;

    void setDirBase(bool back);

    bool isHome() const { return home; }
    bool isInterrupted() const { return interrupted; }
    int32_t getStepsAll() const { return allSteps; }
    bool isBaseError() const { return baseErr; }

    void setConfiguration() { setConf = true; }

protected:
    bool moveHomePionowa();
    bool moveHomePozioma();
    bool moveHomeKatPionowy();
    bool moveHomeKatPoziomy();
    bool moveFiltrPosiotion();

private:

    static constexpr uint8_t DIRPIN = 8;
    static constexpr uint8_t PULSEPIN = 9;
    static constexpr uint8_t MOVEPIN = 5;

    bool reverseMotor;

    uint32_t maxSteps;
    uint32_t baseSteps;
    volatile uint32_t middleSteps;

    uint32_t delayImp;
    volatile uint8_t impTimer;

    volatile bool isRun;
    volatile int32_t globalPos;
    short diff;
    
    int32_t newPosition;
    volatile bool highlevel;

    moveStateType mstate; 
    volatile uint32_t actSteps;

    bool home;
    bool interrupted;
    uint32_t allSteps;
    bool baseErr;
    bool firstHome;

    bool setConf;
    bool homePion;
};

#endif // __SILNIK_H__
