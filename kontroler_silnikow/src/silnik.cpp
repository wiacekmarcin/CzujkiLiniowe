#include "silnik.hpp"
#include "proto.hpp"
#include "main.h"
#include <TimerOne.h>

extern Message msg;
#include "workmode.hpp"

Motor::Motor() :
    mstate(IDLE) 
    ,actSteps(0)
    ,globalPos(0)
    ,reverseMotor(false)
	,maxSteps(0)
    ,baseSteps(0)
	,middleSteps(0)
	,diff(0)
	,highlevel(false)
	,newPosition(false)
	,home(false)
	,move(false)
	,cntPomSkip(0)
    ,maxCntSkip(0)
	,slowMove(true) // domyslnie
	,delayStart(false)
	,reverseHome(false)
{
	setStopPtr = &Motor::setStopDef;
    moveHomePtr = &Motor::moveHomeDef;
    movePositionPtr = &Motor::movePositionDef;
    impulsePtr = &Motor::impulseDef;
}

void Motor::init(WorkMode::WorkModeEnum mode)
{
	switch(mode) {
		case WorkMode::KATOWA_PION:
			setStopPtr = &Motor::setStopDef;
    		moveHomePtr = &Motor::moveHomeRamieGoraDol;
   			movePositionPtr = &Motor::movePositionGDLP;
    		impulsePtr = &Motor::impulseGDLP;
			break;
	    case WorkMode::KOLOWA:
			setStopPtr = &Motor::setStopDef;
			moveHomePtr = &Motor::moveHomeFiltr;
			movePositionPtr = &Motor::movePositionFiltr;
			impulsePtr = &Motor::impulseFiltr;
			break;
	    case WorkMode::KATOWA_POZ:
			setStopPtr = &Motor::setStopDef;
    		moveHomePtr = &Motor::moveHomeRamieLewoPrawo;
   			movePositionPtr = &Motor::movePositionGDLP;
    		impulsePtr = &Motor::impulseGDLP;
			break;
	    case WorkMode::POZIOMA:
			setStopPtr = &Motor::setStopDef;
    		moveHomePtr = &Motor::moveHomeWozekLewoPrawo;
   			movePositionPtr = &Motor::movePositionGDLP;
    		impulsePtr = &Motor::impulseGDLP;
			break;
	    case WorkMode::PIONOWA:
			setStopPtr = &Motor::setStopDef;
    		moveHomePtr = &Motor::moveHomeWozekGoraDol;
   			movePositionPtr = &Motor::movePositionGDLP;
    		impulsePtr = &Motor::impulseGDLP;
			break;
		default:
		break;
	}
}

void Motor::setDirBase(bool back)
{
	bool out = back ^ reverseMotor;
	digitalWrite(DIRPIN, out);
	diff = back ? -1 : 1;
}

void Motor::setMove(bool move)
{
	if (slowMove)
		digitalWrite(MOVEPIN, move ? HIGH : LOW);
	this->move = move;
	mstate = move ? MOVEPOS : IDLE;
	if (move) actSteps = 0;
}

void Motor::setSoftStop()
{
	mstate = IDLE;
	interrupted = true;
	if (move && !home) {
		Timer1.stop();
		setMove(false);
		stopMove(true, false, true, false);
	}
	
	return;
}

void Motor::setStopDef(bool hard)
{
	if (!hard) {
		setSoftStop();
		return;
	}
}

void Motor::impulseDef()
{
	
}

void Motor::moveHomeDef(uint32_t)
{
	Serial.println("Zla fukncja do HOME");
	interrupted = false;
	move=false;
	home=true;
	stopMove(interrupted, true, move, home);
}

void Motor::movePositionDef(int32_t, uint32_t) 
{
	Serial.println("Zla fukncja do MOVE");
	interrupted = false;
	move=false;
	home=false;
	stopMove(interrupted, true, move, home);
}

void Motor::movePositionGDLP(int32_t pos, uint32_t delayImpOrg)
{
    highlevel = false;
    uint32_t delayImp = delayImpOrg >> 1;
    digitalWrite(PULSEPIN, LOW);
	VHSDPN("DelayImp=", delayImpOrg);
	VHSDPN("DelayImpHalf=", delayImp);
	VHSDPN("globalPos=", globalPos);
	VHSDPN("pos=", pos);
	home = false;
	error = false;
	interrupted = false;
	setMove(false);
    if (pos == globalPos) {
        mstate = IDLE;
		return;
	} else if (pos > globalPos) {
		diff = 1;
		setDirBase(false);
	} else if (pos < globalPos) {
		diff = -1;
		setDirBase(true);
	}
    setMove(true);
	VHSDPN("globalPos", globalPos);
	VHSDPN("pos", pos);
	newPosition = pos;
	
	if (delayImp < 15000000) {
        cntPomSkip = 0;
        Timer1.setPeriod(delayImp);
        maxCntSkip = 0;
    } else {
        cntPomSkip = 0;
        maxCntSkip = round(delayImp / 15000000) + 1;
        VHSDPN("maxCntSkip", maxCntSkip);
		VHSDPN("period", round(delayImp / maxCntSkip));
		Timer1.setPeriod(round(delayImp / maxCntSkip));
    }
	Timer1.start();    
}

void Motor::impulseGDLP()
{

    if (mstate == IDLE) {
        Timer1.stop();
        setMove(false);
        stopMove(true, false, true, false);
		return;	  
    }
	
	highlevel = !highlevel;
	digitalWrite(PULSEPIN, highlevel ? HIGH : LOW);
    if (!highlevel)
        return;

    if (maxCntSkip > 0) {
        if ( cntPomSkip++ < maxCntSkip)
            return;
        cntPomSkip = 0;
    }
	
    globalPos += diff;
	++actSteps;
	VHSDP(globalPos, " +");
	VHSDN(actSteps);

    if (globalPos == newPosition) {
        Timer1.stop();
        setMove(false);
		stopMove(false, false, false, false);
		SDN("Koniec. GlobalPos = newPosition. Move pin na LOW");
        return;
    }


    if (actSteps == maxSteps) {
		Timer1.stop();
		setMove(false);
		stopMove(false, true, false, false);
		SDN("Koniec. Osiagnieto max ilosc krokow");
	}
}