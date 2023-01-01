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



bool Motor::moveHomeGoraDol(uint32_t delayImpOrg)
{
    uint32_t delayImp = delayImpOrg>>1;
    //zakladam, ze lezy na czujniku
    mstate = HOMEPOS;
    VSDPN("Opoznienie=", delayImp);
    VSD("Czujka lezy na czujniku ");
    VSDN(digitalRead(KRANCPIN) == LOW);
    setDirBase(false);

    uint32_t steps = 0;
    while(digitalRead(KRANCPIN) == LOW) {
        digitalWrite(PULSEPIN, HIGH);
        delayMicroseconds(delayImp);
        digitalWrite(PULSEPIN, LOW);
        delayMicroseconds(delayImp);
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS)
            return false; //TODO jakis blad ustawic ?
        VSD("+");    
    }

    while(digitalRead(KRANCPIN) == HIGH) {
        digitalWrite(PULSEPIN, HIGH);
        delayMicroseconds(delayImp);
        digitalWrite(PULSEPIN, LOW);
        delayMicroseconds(delayImp);
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS)
            return false; //TODO jakis blad ustawic ?
        VSD("+");    
    }

    //wyjechalem poza baze - dojazd do bazy
    setDirBase(true);
    steps = 0;
    while(digitalRead(KRANCPIN) == HIGH) {
        digitalWrite(PULSEPIN, HIGH);
        delayMicroseconds(delayImp);
        digitalWrite(PULSEPIN, LOW);
        delayMicroseconds(delayImp);
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS)
            return false; //TODO jakis blad ustawic ?
        VSD("-");
    }
    VSDPN("Ilosc impulsow(50)=",steps);
    VSDPN("Jeszcze odjazd zgodnie konfiguracja:", baseSteps);

    for (unsigned short n = 0; n < baseSteps; n++) {
        digitalWrite(PULSEPIN, HIGH);
        delayMicroseconds(delayImp);
        digitalWrite(PULSEPIN, LOW);
        delayMicroseconds(delayImp);
        VSD(".");
    }
    globalPos = 0;
    setDirBase(false);
    VSDPN("Jazda na srodek:", middleSteps);
    for (globalPos = 0; globalPos < middleSteps; globalPos++) {
        digitalWrite(PULSEPIN, HIGH);
        delayMicroseconds(delayImp);
        digitalWrite(PULSEPIN, LOW);
        delayMicroseconds(delayImp);
        VSD("+");
    }
    VSDN("Koniec");
    mstate = IDLE;
    return true;
}

bool Motor::moveHomeGoraDolFirstTime(uint32_t delayImpOrg)
{
    uint32_t delayImp = delayImpOrg>>1;
    //zakladam, ze lezy na czujniku
    mstate = HOMEPOS;
    VSDPN("Opoznienie=", delayImp);
    
    setDirBase(false);

    uint32_t steps = 0;
    while(digitalRead(KRANCPIN) == LOW) {
        digitalWrite(PULSEPIN, HIGH);
        delayMicroseconds(delayImp);
        digitalWrite(PULSEPIN, LOW);
        delayMicroseconds(delayImp);
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS)
            return false; //TODO jakis blad ustawic ?
        VSD("+");    
    }

    VSDPN("Ilosc impulsow -> =",steps);
    steps = 0;
    while(digitalRead(KRANCPIN) == HIGH) {
        digitalWrite(PULSEPIN, HIGH);
        delayMicroseconds(delayImp);
        digitalWrite(PULSEPIN, LOW);
        delayMicroseconds(delayImp);
        ++steps;
        if (steps > maxSteps || mstate != HOMEPOS)
            return false; //TODO jakis blad ustawic ?
        VSD("+");    
    }
    
    VSDPN("Ilosc impulsow <- =",steps);
    VSDPN("Koniec",steps);
    moveHomePtr = &Motor::moveHomeGoraDol;
    return moveHomeGoraDol(delayImpOrg);    
}