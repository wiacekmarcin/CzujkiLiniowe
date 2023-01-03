#include <Arduino.h>
#include "silnik.hpp"
#include "main.h"
#include <TimerOne.h>

#define PULSE_V digitalWrite(PULSEPIN, HIGH); delayMicroseconds(delayImp); digitalWrite(PULSEPIN, LOW); delayMicroseconds(delayImp);


bool Motor::moveHomeLewoPrawo(uint32_t delayImpOrg)
{
    VSDPN(__FILE__, __LINE__);
    uint32_t delayImp = delayImpOrg>>1;
    home = true;
    move = true;
    interrupted = false;
    mstate = HOMEPOS;
    setDirBase(true);

    uint32_t steps = 0;
    
    if (isKrancowka()) {
        setDirBase(false);
        for (unsigned short n = 0; n < middleSteps; n++) {
            PULSE_V
        }
        setDirBase(true);
    }
    while(!isKrancowka()) {
        PULSE_V
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS) {
            VSDPN("Err",__LINE__);
            return false; 
        }
    }
    for (unsigned short n = 0; n < baseSteps; n++) {
        PULSE_V
    }

    globalPos = 0;
    setDirBase(false);
    for (globalPos = 0; globalPos < (int)middleSteps; globalPos++) {
        PULSE_V
    }
    mstate = IDLE;
    return true;
}

bool Motor::moveHomeLewoPrawoFirstTime(uint32_t delayImpOrg)
{
    VSDPN(__FILE__, __LINE__);
    uint32_t delayImp = (delayImpOrg>>1)*10; 
    uint32_t steps = 0;    
    interrupted = false;
    mstate = HOMEPOS;
    VSDPN("Opoznienie=", delayImp);
    VSDPN("Czujka=", isKrancowka());
    
    setDirBase(true);
    VSDN("Jazda do bazy");
    while(!isKrancowka()) {
        PULSE_V
        ++steps;
        if (steps > maxSteps) {
            moveHomePtr = &Motor::moveHomeLewoPrawo;
            VSDPN("Err",__LINE__);
            return false; 
        }
        if (mstate != HOMEPOS) {
            moveHomePtr = &Motor::moveHomeLewoPrawo;
            VSDPN("Err",__LINE__);
            return false; 
        }
        VSD("<");if (steps % 80 == 0) VSDN();    
    }

    VSDPN("Ilosc impulsow |<- =",steps);
    VSDN("Jazda do drugiej krancowki");
    steps = 0;

    setDirBase(false);
    for (unsigned short n = 0; n < 50 || isKrancowka(); n++) {
        PULSE_V;
        VSD(">");
        ++steps;
    }
    VSDN();

    while(!isKrancowka()) {
        PULSE_V
        ++steps;
        if (steps > maxSteps) {
            moveHomePtr = &Motor::moveHomeLewoPrawo;
            VSDPN("Err",__LINE__);
            return false; 
        }
        if (mstate != HOMEPOS) {
            moveHomePtr = &Motor::moveHomeLewoPrawo;
            VSDPN("Err",__LINE__);
            return false; 
        }
        VSD(">");if (steps % 80 == 0) VSDN();    
    }
    
    VSDPN("Ilosc impulsow ->| =",steps);
    VSDN("Powrot do bazy");
    steps = 0;
    setDirBase(true);
    for (unsigned short n = 0; n < 50 || isKrancowka(); n++) {
        PULSE_V
        VSD("<");
        ++steps;
    }
    VSDN();

    while(!isKrancowka()) {
        PULSE_V
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS) {
            moveHomePtr = &Motor::moveHomeLewoPrawo;
            VSDPN("Err",__LINE__);
            return false; 
        }
        VSD("<");if (steps % 80 == 0) VSDN();        
    }

    VSDPN("Ilosc impulsow |<- =",steps);
    VSDPN("Dodatkowe bazowanie ", baseSteps);
    for (unsigned short n = 0; n < baseSteps; n++) {
        PULSE_V
        VSD("b");if (n % 80 == 0) VSDN();        
    }
    VSDN();

    globalPos = 0;
    setDirBase(false);
    VSDPN("Jazda na srodek ", middleSteps);
    for (globalPos = 0; globalPos < (int)middleSteps; globalPos++) {
        PULSE_V
        VSD("s");if (globalPos % 80 == 0) VSDN();        
    }
    VSDN();
    VSDN("Koniec pierwszego razu");
    mstate = IDLE;
    moveHomePtr = &Motor::moveHomeLewoPrawo;
    return true;   
}