#include "filtr.hpp"

#include "TimerOne.h"

#define DEBUG

#ifdef DEBUG
#define SD(X) Serial.println(X)
#define SDN(X) Serial.println(X)
#else
#define SD(X) 
#define SDN(X)
#endif
Filtr::Filtr() :
    Motor()
{
    mstate = IDLE;
    impPerObrot = 0;
}

void Filtr::setStop(bool hard)
{
    if (!hard) {
        //raczej nie powinno się przytrawic
        Timer1.stop();
    } else {
        Timer1.stop();
        SD("STOP ");
        SDN(mstate);
        switch(mstate) {
            case F_HOME_0: 
                actSteps = 0;
                mstate = F_HOME_1;
                break;
            case F_HOME_1:
                mstate = F_HOME_2; 
                temp1 = actSteps;
                actSteps = 0;
                break;
            case F_HOME_2:
                mstate = F_HOME_3; 
                temp2 = actSteps;
                actSteps = 0;
                break;
            case F_HOME_4:
                mstate = F_HOME_5; 
                temp3 = actSteps;
                actSteps = 0;
                break;
            case F_HOME_5:
                mstate = F_HOME_6; 
                temp4 = actSteps;
                actSteps = 0;
                globalPos = middleSteps;
                diff = -1;
                break;
            default: break;
        }
        Timer1.start();
    }
}

void Filtr::impulse()
{
    highlevel = !highlevel;
	digitalWrite(PULSEPIN, highlevel ? HIGH : LOW);
	globalPos += diff;
	++actSteps;

    switch(mstate) {
        case F_HOME_0:
            if (actSteps == maxSteps) {
                Timer1.stop();
                err = true;
                //sendError();
            }
            break;
        case F_HOME_3:
            if (digitalRead(KRANCPIN) == HIGH) {
                mstate = F_HOME_4;
                setDirBase(true);
            }
            break;
        case F_HOME_6:
            if (globalPos == 0) {
                Timer1.stop();
                err = false;
                //sentDone();
                impPerObrot = ((int32_t)temp1 + temp2 + temp3 + temp4)/4;
            }
            break;
        default: break;
    };
} 

bool Filtr::moveHome(uint32_t delayImp)
{
    SDN("Zerowanie");
    mstate = F_HOME_0;
    actSteps = 0;
    globalPos = 0;
    diff = 1;
    setDirBase(false);
    moveH = true;
    Timer1.stop();
	Timer1.setPeriod(delayImp)	;
	Timer1.start();

    return true;
}

bool Filtr::movePosition(int32_t pos, uint32_t delayImp)
{
    if (pos == globalPos)
        return false;
    Timer1.stop();
	Timer1.setPeriod(delayImp)	;
	Timer1.start();
    return true;
}