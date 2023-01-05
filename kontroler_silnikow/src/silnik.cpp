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
	,error(false)
	,interrupted(false)
	,cntPomSkip(0)
    ,maxCntSkip(0)
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
			setStopPtr = &Motor::setStopGDLP;
    		moveHomePtr = &Motor::moveHomeGoraDol;
   			movePositionPtr = &Motor::movePositionGDLP;
    		impulsePtr = &Motor::impulseGDLP;
			break;
	    case WorkMode::KOLOWA:
			break;
	    case WorkMode::KATOWA_POZ:
			setStopPtr = &Motor::setStopGDLP;
    		moveHomePtr = &Motor::moveHomeLewoPrawo;
   			movePositionPtr = &Motor::movePositionGDLP;
    		impulsePtr = &Motor::impulseGDLP;
			break;
	    case WorkMode::POZIOMA:
	    case WorkMode::PIONOWA:
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
	digitalWrite(MOVEPIN, move ? HIGH : LOW);
	this->move = move;
}

void Motor::setSoftStop()
{
	mstate = IDLE;
	interrupted = true;
	if (move) {
		Timer1.stop();
		setMove(false);
		stopMove(interrupted, move, error, home);
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
	error=true;
	move=false;
	home=true;
	interrupted=false;
	stopMove(interrupted, move, error, home);
}

void Motor::movePositionDef(int32_t, uint32_t) 
{
	Serial.println("Zla fukncja do MOVE");
	error=true;
	move=false;
	home=false;
	interrupted=false;
	stopMove(interrupted, move, error, home);
}

void Motor::setStopGDLP(bool hard)
{
    if (!hard) {
        setSoftStop();
	}
    //nic nie rob
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
		stopMove(interrupted, move, error, home);
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
	actSteps = 0;
	digitalWrite(MOVEPIN, HIGH);
	mstate = MOVEPOS;
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
	stopMove(interrupted, move, error, home);
	Timer1.start();    
}

void Motor::impulseGDLP()
{

    if (mstate == IDLE) {
        Timer1.stop();
        digitalWrite(MOVEPIN, LOW);
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
        mstate = IDLE;
        Timer1.stop();
        setMove(false);
		stopMove(interrupted, move, error, home);
		SDN("Koniec. GlobalPos = newPosition. Move pin na LOW");
        return;
    }


    if (actSteps == maxSteps) {
		mstate = IDLE;
		Timer1.stop();
		error = true;
		setMove(false);
		stopMove(interrupted, move, error, home);
		SDN("Koniec. Osiagnieto max ilosc krokow");
	}
}