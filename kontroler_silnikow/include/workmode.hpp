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
	    KATOWA_POZ = 1, 
	    KOLOWA, 
	    KATOWA_PION,
	    POZIOMA,
	    PIONOWA,
	    UNKNOWN
    } WorkModeEnum;

    WorkMode();
    void init(uint8_t pos);
    ~WorkMode();
    inline bool isDebugMode() const { return debugMode; }
    inline WorkModeEnum getMode() const { return mode; }
    uint32_t getMaxSteps() const;
    uint32_t getDelayImp() const;

protected:
    WorkModeEnum conv2DebugWorkMode(uint8_t pos);
    void print() const;

private:
    WorkModeEnum mode;
    bool debugMode;
};

void debugModeFun(const WorkMode & mode);

#endif