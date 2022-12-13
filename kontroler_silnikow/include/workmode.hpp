#ifndef WORKMODE_H
#define WORKMODE_H

#pragma once

#include <Arduino.h>

class WorkMode
{
public:    
    //dx dx dx dx dx d3 d2 d1
    typedef enum _workModeEnum
    {
	    CHECKKRANC = 0, //brak zworek
	    KATOWA_POZ = 1, //d3 pierwsza od procka
	    KOLOWA = 2, //d2 druga od procka
	    KATOWA_PION = 3, //d3,d2 pierwsza, druga od procka
	    POZIOMA = 4, //d3 trzecia od procka
	    PIONOWA = 7, //wszystkie zworki
	    UNKNOWN = 8
    } WorkModeEnum;

    WorkMode();
    void init();
    ~WorkMode();
    inline bool isDebugMode() const { return debugMode; }
    inline WorkModeEnum getMode() const { return mode; }
    uint32_t getMaxSteps() const;
    uint32_t getDelayImp() const;

protected:
    WorkModeEnum conv2DebugWorkMode(uint8_t d3, uint8_t d2, uint8_t d1);
    void print() const;

private:
    static constexpr uint8_t DEBUGPIN = 4;
    static constexpr uint8_t DBG1 = A1;
    static constexpr uint8_t DBG2 = A2;
    static constexpr uint8_t DBG3 = A3;

    WorkModeEnum mode;
    bool debugMode;
};

void debugModeFun(const WorkMode & mode, uint8_t KRANCPIN, uint8_t DIRPIN, uint8_t PULSEPIN);

#endif