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
static uint16_t downVal = 1151;
static uint16_t upVal = 1151;

void Motor::moveHomeFiltr(uint32_t delayImpOrg)
{
    FSDPN(__FILE__, __LINE__);
    uint32_t delayImp = delayImpOrg>>1;
    home = true;
    move = true;
    error = false;
    interrupted = false;
    mstate = HOMEPOS;
    setDirBase(true);

    uint32_t steps = 0;
    
    if (isKrancowka()) {
        setDirBase(false);
        for (unsigned short n = 0; n < maxSteps/6; n++) {
            PULSE_F
        }
        setDirBase(true);
    }
    while(!isKrancowka()) {
        PULSE_F
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS) {
            error = true;
            stopMove(interrupted, move, error, home);
            FSDPN("Err",__LINE__);
            return; 
        }
    }
    for (unsigned short n = 0; n < baseSteps; n++) {
        PULSE_F
    }

    globalPos = 0;
    setDirBase(false);
    
    mstate = IDLE;
    move = false;
    stopMove(interrupted, move, error, home);
}

void Motor::movePositionFiltr(int32_t pos, uint32_t delayImpOrg)
{
    highlevel = false;
    uint32_t delayImp = delayImpOrg>>1;
    digitalWrite(PULSEPIN, LOW);
    slowMove = delayImp > 500;
	FSDPN("DelayImp=", delayImpOrg);
	FSDPN("globalPos=", globalPos);
	FSDPN("pos=", pos);
    
    for (short i=0; i<10; ++i) {
        speed[i] = (uint16_t) 10*delayImp/(i+1);
        FSDP("speed[", i);
        FSDPN("]", speed[i]);
    }

	home = false;
	error = false;
	interrupted = false;
    uint32_t d1 = 0, d2 = 0, d3 = 0, d4 = 0;
	setMove(false);
    if (pos == globalPos) {
        mstate = IDLE;
		stopMove(interrupted, move, error, home);
		return;
	} else if (pos > globalPos) {
        d1 = pos - globalPos;
        d2 = globalPos - pos + maxSteps;
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
    } else if (pos < globalPos) {
        d3 = globalPos - pos;
        d4 = pos - globalPos + maxSteps;
        if (d3 > d4) {
            newPosition = pos;
            diff = 1;
            upVal = d4 - downVal;
            moveSteps = d4;
        } else {
            diff -= 1;
            newPosition = pos;
            upVal = d3 - downVal;
            moveSteps = d3;
        }
    }
    setDirBase(diff < 0);
       
    FSDPN("moveSteps=", moveSteps);
    FSDPN("diff=", diff);
    FSDPN("downVal=", downVal);
    FSDPN("upVal=", upVal);
    FSDPN("slowMove=", slowMove);
	
    Timer1.setPeriod(speed[0]);
    prevSpeedIdx = 0;
	mstate = MOVEPOS;
	setMove(true);
    stopMove(interrupted, move, error, home);
    if (slowMove) {
        Timer1.start();
        delayStart = false;
    } else
        delayStart = true;   //uruchomienie zegara dopiero po wyslaniu komendyy
}

//1.2 s dla 180s przy T=60us
//to jest już granica wykonywalności kodu w funkcjach ze zmienna predkoscia
void Motor::impulseFiltr()
{

    if (mstate == IDLE) {
        FSDN("Koniec. mstate=idle");
        Timer1.stop();
        setMove(false);
        error=true;
        stopMove(interrupted, move, error, home);
        delayStart = false;
		return;	 
    }
	
    highlevel = !highlevel;
	digitalWrite(PULSEPIN, highlevel ? HIGH : LOW);
    if (!highlevel) {
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
        stopMove(interrupted, move, error, home);
        FSDN("Koniec.");
        digitalWrite(PULSEPIN, LOW);
        delayStart = false;
        return;
    }

    if (globalPos == maxSteps)
        globalPos = 0;
    else if (globalPos == 0)
        globalPos = maxSteps;
    
    
    if (actSteps == maxSteps/2 + 5) {
		Timer1.stop();
		error = true;
		setMove(false);
        stopMove(interrupted, move, error, home);
		FSDN("Koniec. steps > max/2+5");
        digitalWrite(PULSEPIN, LOW);
        delayStart = false;
	}
}