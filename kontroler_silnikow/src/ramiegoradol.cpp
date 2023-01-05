#include <Arduino.h>
#include "silnik.hpp"
#include "main.h"
#include <TimerOne.h>

#define DEBUGGD
#ifdef DEBUGGD
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

#define PULSE_H digitalWrite(PULSEPIN, HIGH); delayMicroseconds(delayImp); digitalWrite(PULSEPIN, LOW); delayMicroseconds(delayImp);

void Motor::moveHomeGoraDol(uint32_t delayImpOrg)
{
    VSDPN(__FILE__, __LINE__);
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
        for (unsigned short n = 0; n < middleSteps; n++) {
            PULSE_H
        }
        setDirBase(true);
    }
    while(!isKrancowka()) {
        PULSE_H
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS) {
            error = true;
            stopMove(interrupted, move, error, home);
            VSDPN("Err",__LINE__);
            return; 
        }
    }
    for (unsigned short n = 0; n < baseSteps; n++) {
        PULSE_H
    }

    globalPos = 0;
    setDirBase(false);
    for (globalPos = 0; globalPos < (int)middleSteps; globalPos++) {
        PULSE_H
    }
    mstate = IDLE;
    move = false;
    stopMove(interrupted, move, error, home);
}
