#ifndef __SILNIK_H__
#define __SILNIK_H__

#include <Arduino.h>
#include <TimerOne.h>
#define TEST

#include "workmode.hpp"

//#define DEBUGLP
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

//#define DEBUGLPGD
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
    void moveHomeDef(uint32_t delayImp);
    void movePositionDef(int32_t pos, uint32_t delayImp);
    void impulseDef();

    void movePositionGDLP(int32_t pos, uint32_t delayImp);
    void impulseGDLP();
    
    void moveHomeRamieGoraDol(uint32_t delayImp);
    void moveHomeRamieLewoPrawo(uint32_t delayImp);
    void moveHomeWozekGoraDol(uint32_t delayImp);
    void moveHomeWozekLewoPrawo(uint32_t delayImp);

    void moveHomeFiltr(uint32_t delayImpOrg);  
    void movePositionFiltr(int32_t pos, uint32_t delayImpOrg); 
    void impulseFiltr(); 

    void (Motor::*setStopPtr)(bool);
    void (Motor::*moveHomePtr)(uint32_t);
    void (Motor::*movePositionPtr)(int32_t,uint32_t);
    void (Motor::*impulsePtr)(void);

    inline void setStop(bool hard) { (this->*setStopPtr)(hard); }
    inline void moveHome(uint32_t delayImp) { (this->*moveHomePtr)(delayImp); }
    inline void movePosition(int32_t pos, uint32_t delayImp) { (this->*movePositionPtr)(pos, delayImp); }
    inline void impulse() { (this->*impulsePtr)(); }
    

    void setSoftStop();
    void setReverseMotor(bool rev) { reverseMotor = rev; }
    void setMaxSteps(uint32_t maxSteps) { this->maxSteps = maxSteps; }
    void setBaseSteps(uint16_t baseSteps) { this->baseSteps = baseSteps; }
    void setMiddleSteps(uint16_t middleSteps) { this->middleSteps = middleSteps; }
    //void setConfiguration

    int32_t getGlobalPos() const { return globalPos; }
    

    bool isHome() const { return home; }
    bool isMove() const { return move; }
    bool isError() const { return error; }
    bool isInterrupted() const { return interrupted; }

    void setDirBase(bool back);

    inline void startImpulse() { if (!timerActive && move && delayStart) { timerActive = true; Timer1.start(); } }

protected:

    void setMove(bool move);

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

    bool home;   //czy zerowanie
    bool move;   //czy ruch na timerze
    bool error;  //blad glownie zerowania, na timer jest zglaszany osobno
    volatile bool interrupted; //czy bylo przerwanie - glownie dla zerowania - na timerze jest zglaszany osobno
    
    uint16_t cntPomSkip;
    uint16_t maxCntSkip;
    bool firstTime;
    volatile uint8_t prevSpeedIdx; //poprzedni index predkosci przy przyspieszaniu i zwalnaniu
    uint16_t moveSteps;            //ilosc krokow przy filtrach - inny algorytm porusznia
    bool slowMove;                 //czy wolny ruch dla filtrow - jezeli tak bedzie progress
    bool delayStart;
    volatile bool timerActive;    //czy start po wiadomosci (request i reply trwaja kilka ms na ISR)
};
#endif // __SILNIK_H__
