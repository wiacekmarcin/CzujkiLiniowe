#include <Arduino.h>
#include "silnik.hpp"
#include "main.h"
#include <TimerOne.h>

#define PULSE_H digitalWrite(PULSEPIN, HIGH); delayMicroseconds(delayImp); digitalWrite(PULSEPIN, LOW); delayMicroseconds(delayImp);
#define PULSE_V digitalWrite(PULSEPIN, HIGH); delayMicroseconds(delayImp); digitalWrite(PULSEPIN, LOW); delayMicroseconds(delayImp);

void Motor::moveHomeRamieGoraDol(uint32_t delayImpOrg)
{
    VSDPN(__FILE__, __LINE__);
    uint32_t delayImp = delayImpOrg>>1;
    home = true;
    move = false;
    error = false;
    interrupted = false;
    mstate = HOMEPOS;
    setDirBase(true);

    uint32_t steps = 0;
    
    if (isKrancowka()) {
        setDirBase(false);
        for (unsigned short n = 0; n < middleSteps; n++) {
            PULSE_H
            if (mstate == IDLE) //bylo przerwanie
                return;
        }
        setDirBase(true);
    }
    while(!isKrancowka()) {
        PULSE_H
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS) {
            error = true;
            VSDPN("Err",__LINE__);
            return; 
        }
        if (mstate == IDLE) //bylo przerwanie
            return;
    }
    for (unsigned short n = 0; n < baseSteps; n++) {
        PULSE_H
        if (mstate == IDLE) //bylo przerwanie
            return;
    }

    globalPos = 0;
    setDirBase(false);
    for (globalPos = 0; globalPos < (int)middleSteps; globalPos++) {
        PULSE_H
        if (mstate == IDLE) //bylo przerwanie
            return;
    }
    mstate = IDLE;
}

void Motor::moveHomeRamieLewoPrawo(uint32_t delayImpOrg)
{
    uint32_t delayImp = delayImpOrg>>1;
    home = true;
    move = false;
    error = false;
    interrupted = false;
    mstate = HOMEPOS;
    setDirBase(true);

    uint32_t steps = 0;
    
    if (isKrancowka()) {
        setDirBase(false);
        for (unsigned short n = 0; n < middleSteps; n++) {
            PULSE_V
            if (mstate == IDLE) //bylo przerwanie
                return;
        }
        setDirBase(true);
    }
    while(!isKrancowka()) {
        PULSE_V
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS) {
            error = true;
            return; 
        }
        if (mstate == IDLE) //bylo przerwanie
            return;
    }
    for (unsigned short n = 0; n < baseSteps; n++) {
        PULSE_V
        if (mstate == IDLE) //bylo przerwanie
            return;
    }

    globalPos = 0;
    setDirBase(false);
    for (globalPos = 0; globalPos < (int)middleSteps; globalPos++) {
        PULSE_V
        if (mstate == IDLE) //bylo przerwanie
            return;
    }
    mstate = IDLE;
}