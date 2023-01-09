#include <Arduino.h>
#include "silnik.hpp"
#include "main.h"
#include <TimerOne.h>

#define FLTDEBUG 1
#ifdef FLTDEBUG
#define FSD(X) Serial.print(X)
#define FSDN(X) Serial.println(X)
#define FSDP(T, V) FSD(T); FSD(V)
#define FSDPN(T, V) FSD(T); FSDN(V)
#else
#ifndef FSD
#define FSD(X)
#endif
#ifndef FSDN 
#define FSDN(X)
#endif
#ifndef FSDP
#define FSDP(T, V)
#endif
#ifndef FSDPN
#define FSDPN(T, V)
#endif
#endif

#define PULSE_F digitalWrite(PULSEPIN, HIGH); delayMicroseconds(delayImp); digitalWrite(PULSEPIN, LOW); delayMicroseconds(delayImp);

static uint16_t speed[10];
static uint32_t downVal = 1151;
static uint32_t upVal = 1151;

void Motor::moveHomeFiltr(uint32_t delayImpOrg)
{
    FSDN("Zerowanie");
    uint32_t delayImp = delayImpOrg>>1;
    home = true;
    move = false;
    error = false;
    interrupted = false;

    mstate = HOMEPOS;
    setDirBase(true);

    uint32_t steps = 0;
    FSDPN("Czy krancowka", isKrancowka());
    if (isKrancowka()) {
        FSDN("Odjazd");
        setDirBase(false);
        for (unsigned short n = 0; n < maxSteps/6; n++) {
            PULSE_F
            if (mstate == IDLE) //bylo przerwanie
                return;
        }
        setDirBase(true);
    }
    FSDPN("Czy krancowka", isKrancowka());
    FSDN("Jazda do bazy");
    while(!isKrancowka()) {
        if (mstate == IDLE) //bylo przerwanie
            return;
        PULSE_F
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS) {
            error = true;
            FSDPN("Err",__LINE__);
            return; 
        }
    }
    FSDPN("Dodatkowe pozycjoniowanie", baseSteps);
    for (unsigned short n = 0; n < baseSteps; n++) {
        if (mstate == IDLE) //bylo przerwanie
            return;
        PULSE_F
    }
    globalPos = 0;
    mstate = IDLE;
    move = false;
    FSDPN("Koniec globaPos", globalPos);
    setDirBase(false);
}

void Motor::movePositionFiltr(int32_t pos, uint32_t delayImpOrg)
{
    highlevel = false;
    uint32_t delayImp = delayImpOrg>>1;
    digitalWrite(PULSEPIN, LOW);
    slowMove = delayImp > 500;
	FSDPN("delayImp=", delayImp);
    home = false;
    move = false;
    error = false;
    interrupted = false;
    
    FSD("speed[");
    for (short i=0; i<10; ++i) {
        speed[i] = (uint16_t) (10*delayImp/(i+1));
        FSDP(speed[i],",");
    }
    FSDN("]");

    int32_t d1 = 0, d2 = 0, d3 = 0, d4 = 0;
	setMove(false);
    FSDP("pos=", pos);
    FSDPN(" globalPos=", globalPos);
    if (pos == globalPos) {
        mstate = IDLE;
		return;
	} else if (pos > globalPos) {
        FSDN("Pos > globalPos");
        d1 = pos - globalPos;
        d2 = globalPos - pos + maxSteps;
        FSDP("d1=", d1);FSDPN(" d2=", d2);
        if (d2 > d1) {
            diff = 1;
            newPosition = pos;
            upVal = d1 - downVal;
            moveSteps = d1;
        } else {
            diff = -1;
            newPosition = pos;
            upVal = d2 - downVal;
            moveSteps = d2;
        }
        FSDP("diff=", diff);FSDP(" newPosition=", newPosition);FSDP(" upVal=", upVal);FSDPN(" moveSteps", moveSteps);
    } else if (pos < globalPos) {
        FSDN("Pos < globalPos");
        d3 = globalPos - pos;
        d4 = pos - globalPos + maxSteps;
        FSDP("d3=", d3);FSDPN(" d4=", d4);
        if (d3 > d4) {
            newPosition = pos;
            diff = 1;
            upVal = d4 - downVal;
            moveSteps = d4;
        } else {
            diff = -1;
            newPosition = pos;
            upVal = d3 - downVal;
            moveSteps = d3;
        }
        FSDP("diff=", diff);FSDP(" newPosition=", newPosition);FSDP(" upVal=", upVal);FSDPN(" moveSteps", moveSteps);
    }
    setDirBase(diff < 0);
       
    Timer1.setPeriod(speed[0]);
    prevSpeedIdx = 0;
	mstate = MOVEPOS;
    FSDN("Czekam na ruch");
    
    if (slowMove) {
        Timer1.start();
        delayStart = false;
    } else {
        delayStart = true;   //uruchomienie zegara dopiero po wyslaniu komendyy
        timerActive = false;
    }
    setMove(true);
}

//1.2 s dla 180s przy T=60us
//to jest już granica wykonywalności kodu w funkcjach ze zmienna predkoscia
void Motor::impulseFiltr()
{

    if (mstate == IDLE) {
        FSDN("Koniec. mstate=idle");
        Timer1.stop();
        setMove(false);
        stopMove(true, false, true, false);
        delayStart = false;
		return;	 
    }
	
    highlevel = !highlevel;
	digitalWrite(PULSEPIN, highlevel ? HIGH : LOW);
    if (highlevel) {
        globalPos += diff;
	    ++actSteps;

        uint8_t speedPerIdx;
        if (actSteps < downVal) {
            speedPerIdx = actSteps >> 7;
        } else if (actSteps > upVal) {
            speedPerIdx = 8-((actSteps-upVal) >> 7);
        } else {
            speedPerIdx = 9;
        }
        
        if (speedPerIdx != prevSpeedIdx) {
            prevSpeedIdx = speedPerIdx;
            Timer1.stop();
            Timer1.setPeriod(speed[speedPerIdx]);
            Timer1.start();
        }

        return;
    }

    if (globalPos == newPosition || actSteps >= moveSteps) {
        Timer1.stop();
		setMove(false);
        stopMove(false, false, false, false);
        FSDN("Koniec. OK ");  FSDP("actSteps=", actSteps);; FSDPN("globalPos=", globalPos);
        digitalWrite(PULSEPIN, LOW);
        delayStart = false;
        return;
    }

    if (globalPos == (int)maxSteps)
        globalPos = 0;
    else if (globalPos == 0)
        globalPos = maxSteps;
    
    
    if (actSteps >= maxSteps/2 + 5) {
		Timer1.stop();
		setMove(false);
        stopMove(false, true, false, false);
		FSDN("Koniec. steps > max/2+5");
        digitalWrite(PULSEPIN, LOW);
        delayStart = false;
	}
}