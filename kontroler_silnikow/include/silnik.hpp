#ifndef __SILNIK_H__
#define __SILNIK_H__

#include <Arduino.h>
#define TEST

typedef enum moveState {
    F_HOME_0 = 0,
    F_HOME_1,
    F_HOME_2,
    F_HOME_3,
    F_HOME_4,
    F_HOME_5,
    F_HOME_6,
    IDLE = 9,
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
    
    void init();

    void setStopDef(bool hard);
    bool moveHomeDef(uint32_t delayImp);
    bool movePositionDef(int32_t pos, uint32_t delayImp);
    void impulseDef();
    

    void (Motor::*setStopPtr)(bool);
    bool (Motor::*moveHomePtr)(uint32_t);
    bool (Motor::*movePositionPtr)(int32_t,uint32_t);
    void (Motor::*impulsePtr)(void);

    inline void setStop(bool hard) { (this->*setStopPtr)(hard); }
    inline bool moveHome(uint32_t delayImp) { return (this->*moveHomePtr)(delayImp); }
    inline bool movePosition(int32_t pos, uint32_t delayImp) { return (this->*movePositionPtr)(pos, delayImp); }
    inline void impulse() { (this->*impulsePtr)(); }
    

    void setReverseMotor(bool rev) { reverseMotor = rev; }
    void setMaxSteps(uint32_t maxSteps) { this->maxSteps = maxSteps; }
    void setBaseSteps(uint16_t baseSteps) { this->baseSteps = baseSteps; }
    void setMiddleSteps(uint16_t middleSteps) { this->middleSteps = middleSteps; }
    //void setConfiguration

    int32_t getGlobalPos() const { return globalPos; }
    

    

    bool isHome() const { return home; }
    bool isInterrupted() const { return interrupted; }
    int32_t getStepsAll() const { return allSteps; }
    bool isBaseError() const { return baseErr; }
    void setDirBase(bool back);

    bool isMove() const { return moveP; }
    bool isHomeMove() const { return moveH; }

protected:



    volatile moveStateType mstate; 
    volatile uint32_t actSteps;
    volatile int32_t globalPos;
    bool reverseMotor;

    uint32_t maxSteps;
    uint32_t baseSteps;
    uint32_t middleSteps;

    short diff;
    volatile bool highlevel;
    int32_t newPosition;

    bool home;
    bool interrupted;
    uint32_t allSteps;
    bool baseErr;

    bool moveP;
    bool moveH;
};

#endif // __SILNIK_H__
