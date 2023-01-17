#include <Arduino.h>
#include "silnik.hpp"
#include "main.h"
#include <TimerOne.h>

//#define WOZDEBUG 1

#ifdef WOZDEBUG
#define WSD(X) Serial.print(X)
#define WSDN(X) Serial.println(X)
#define WSDP(T, V) WSD(T); WSD(V)
#define WSDPN(T, V) WSD(T); WSDN(V)
#else
#ifndef WSD
#define WSD(X)
#endif
#ifndef WSDN 
#define WSDN(X)
#endif
#ifndef WSDP
#define WSDP(T, V)
#endif
#ifndef WSDPN
#define WSDPN(T, V)
#endif
#endif

#define PULSE_WH digitalWrite(PULSEPIN, HIGH); delayMicroseconds(delayImp); digitalWrite(PULSEPIN, LOW); delayMicroseconds(delayImp);
#define PULSE_WV digitalWrite(PULSEPIN, HIGH); delayMicroseconds(delayImp); digitalWrite(PULSEPIN, LOW); delayMicroseconds(delayImp);

void Motor::moveHomeWozekGoraDol(uint32_t delayImpOrg)
{
    WSDPN(__FILE__, __LINE__);
    uint32_t delayImp = delayImpOrg>>1;
    home = true;
    move = false;
    error = false;
    interrupted = false;
    mstate = HOMEPOS;
    
    WSDPN("maxSteps ", maxSteps);
    WSDPN("middleSteps ", middleSteps);
    WSDPN("baseSteps ", baseSteps);
    WSDPN("delayImpOrg ", delayImpOrg);
    setDirBase(true);

    uint32_t steps = 0;
    steps = maxSteps >> 3;
    
    WSDPN("Czy jest krancowka?", isKrancowka());
    if (isKrancowka()) {
        setDirBase(false);
        WSDN("Odjazd od bazy");
        for (unsigned short n = 0; n < steps; n++) {
            PULSE_WH
            if (mstate == IDLE) //bylo przerwanie
                return;
        }
        setDirBase(true);

    }
    WSDN("Dojazd do krancowki");
    while(!isKrancowka()) {
        PULSE_WH
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS) {
            error = true;
            WSDPN("Err",__LINE__);
            return; 
        }
        if (mstate == IDLE) //bylo przerwanie
            return;
    }
    WSDPN("Baza", baseSteps);
    for (unsigned short n = 0; n < baseSteps; n++) {
        PULSE_WH
        if (mstate == IDLE) //bylo przerwanie
            return;
    }

    globalPos = 0;
    setDirBase(false);
    WSDPN("Srodek", middleSteps);
    for (globalPos = 0; globalPos < (int)middleSteps; globalPos++) {
        PULSE_WH
        if (mstate == IDLE) //bylo przerwanie
            return;
    }
    mstate = IDLE;
}

void Motor::moveHomeWozekLewoPrawo(uint32_t delayImpOrg)
{
    WSDPN(__FILE__, __LINE__);
    uint32_t delayImp = delayImpOrg>>1;
    home = true;
    move = false;
    error = false;
    interrupted = false;
    mstate = HOMEPOS;

    WSDPN("maxSteps ", maxSteps);
    WSDPN("middleSteps ", middleSteps);
    WSDPN("baseSteps ", baseSteps);
    WSDPN("delayImpOrg ", delayImpOrg);
    setDirBase(true);

    uint32_t steps = 0;
    
    WSDPN("Czy krancowka ? ", isKrancowka());
    if (isKrancowka()) {
        WSDN("Wycofuje...");
        setDirBase(false);
        uint8_t stepsB;
        if (middleSteps) stepsB = middleSteps >> 1; 
        else stepsB = maxSteps >> 2;
        for (unsigned short n = 0; n < stepsB; n++) {
            PULSE_WV
            if (mstate == IDLE) //bylo przerwanie
                return;
        }
        setDirBase(true);
    }
    WSDN("Powrot  ");
    while(!isKrancowka()) {
        PULSE_WV
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS) {
            error = true;
            return; 
        }
        if (mstate == IDLE) //bylo przerwanie
            return;
    }
    for (short i = 0; i < 10; i++) {
        if (!isKrancowka()) {
            WSDN("Blad krancowki...");
            error = true;
            return; 
        }
        if (mstate == IDLE) //bylo przerwanie
            return;
    }
    WSDPN("Wykonane impulsy : ",steps);
    for (unsigned short n = 0; n < baseSteps; n++) {
        PULSE_WV
        if (mstate == IDLE) //bylo przerwanie
            return;
    }

    globalPos = 0;
    setDirBase(false);
    for (globalPos = 0; globalPos < (int)middleSteps; globalPos++) {
        PULSE_WV
        if (mstate == IDLE) //bylo przerwanie
            return;
    }
    mstate = IDLE;
}