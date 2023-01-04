#ifndef __SILNIK_H__
#define __SILNIK_H__

#include <Arduino.h>
#define TEST

#include "workmode.hpp"

#define DEBUGLP
#ifdef DEBUGLP
	#define VSD(T) Serial.print(T);
	#define VSDN(T) Serial.println(T);
	#define VSD2(T,P) Serial.print(T,P);
	#define VSDN2(T,P) Serial.println(T,P);

	#define VSDP(T, V) VSD(T); VSD(V);
	#define VSDPN(T, V) VSD(T); VSDN(V);
#else
	#define VSD(T) 
	#define VSDN(T) 
	#define VSD2(T,P) 
	#define VSDN2(T,P) 

	#define VSDP(T, V) 
	#define VSDPN(T, V) 
#endif

#define DEBUGLPGD
#ifdef DEBUGLPGD
	#define VHSD(T) Serial.print(T);
	#define VHSDN(T) Serial.println(T);
	#define VHSD2(T,P) Serial.print(T,P);
	#define VHSDN2(T,P) Serial.println(T,P);

	#define VHSDP(T, V) VHSD(T); VHSD(V);
	#define VHSDPN(T, V) VHSD(T); VHSDN(V);
#else
	#define VHSD(T) 
	#define VHSDN(T) 
	#define VHSD2(T,P) 
	#define VHSDN2(T,P) 

	#define VHSDP(T, V) 
	#define VHSDPN(T, V) 
#endif




typedef enum moveState {
    IDLE = 0,
    HOMEPOS,
    MOVEPOS,
  

    
} moveStateType;

class Motor
{

public:
    Motor();
    
    void init(WorkMode::WorkModeEnum mode);

    void setStopDef(bool hard);
    bool moveHomeDef(uint32_t delayImp);
    bool movePositionDef(int32_t pos, uint32_t delayImp);
    void impulseDef();

    void setStopGDLP(bool hard);
    bool movePositionGDLP(int32_t pos, uint32_t delayImp);
    void impulseGDLP();
    
    bool moveHomeGoraDol(uint32_t delayImp);
    bool moveHomeGoraDolFirstTime(uint32_t delayImp);
    
    bool moveHomeLewoPrawo(uint32_t delayImp);
    bool moveHomeLewoPrawoFirstTime(uint32_t delayImp);
    

    void (Motor::*setStopPtr)(bool);
    bool (Motor::*moveHomePtr)(uint32_t);
    bool (Motor::*movePositionPtr)(int32_t,uint32_t);
    void (Motor::*impulsePtr)(void);

    inline void setStop(bool hard) { (this->*setStopPtr)(hard); }
    inline bool moveHome(uint32_t delayImp) { return (this->*moveHomePtr)(delayImp); }
    inline bool movePosition(int32_t pos, uint32_t delayImp) { return (this->*movePositionPtr)(pos, delayImp); }
    inline void impulse() { (this->*impulsePtr)(); }
    

    void setSoftStop();
    void setReverseMotor(bool rev) { reverseMotor = rev; }
    void setMaxSteps(uint32_t maxSteps) { this->maxSteps = maxSteps; }
    void setBaseSteps(uint16_t baseSteps) { this->baseSteps = baseSteps; }
    void setMiddleSteps(uint16_t middleSteps) { this->middleSteps = middleSteps; }
    //void setConfiguration

    int32_t getGlobalPos() const { return globalPos; }
    

    bool isHome() const { return home; }
    bool isInterrupted() const { return interrupted; }

    void setDirBase(bool back);


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
    bool move;
    bool interrupted;
    
    uint16_t cntPomSkip;
    uint16_t maxCntSkip;
    bool firstTime;
    bool errorMove;
};
#endif // __SILNIK_H__
