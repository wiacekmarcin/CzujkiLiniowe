#include "silnik.hpp"
#include "proto.hpp"
#include <TimerOne.h>
extern Message msg;

Motor::Motor() :
    reverseMotor(false)
    ,enableAlways(true)
	,maxSteps(1000)
    ,delayImp(1500)
    ,baseSteps(10)
	,isRun(false)
	,newPosition(false)
	,diff(0)
	,canMove(true)
    ,globalPos(0)
	,wasHome(false)
	,highlevel(false)
	,isMoveHome(false)
	,actSteps(0)
{
	cntPrint = 1;
    isMove = false;
}

void Motor::init()
{
	
}


//DIR REV
// 0    0   => 0
// 0    1   => 1
// 1    0   => 1
// 1    1   => 0
void Motor::setDir(bool back)
{
	bool out = back ^ reverseMotor;
	digitalWrite(DIRPIN, out);
}

void Motor::setStop(bool hard)
{
	canMove = false;
	Timer1.stop();
	if (hard) {
		globalPos = 0;
	}
	isRun = false;
	isMove = false;
}

bool Motor::impulse()
{
	highlevel = !highlevel;
	digitalWrite(PULSEPIN, highlevel ? HIGH : LOW);
	globalPos += diff;

	if (globalPos == newPosition || ++actSteps == maxSteps) {
		Timer1.stop();
		isRun = false;
		isMove = false;
		return true;
	}
	return false;
}

void Motor::print()
{
	if (isMove || cntPrint > 0) {
		Serial.print("r= ");
		Serial.print(isRun, DEC);
		Serial.print(" a= ");
		Serial.print(actSteps, DEC);
		Serial.print(" m= ");
		Serial.print(maxSteps, DEC);
		Serial.print(" n= ");
		Serial.print(newPosition, DEC);
		Serial.print(" g= ");
		Serial.println(globalPos, DEC);
	}
	if (!isMove && cntPrint > 0)
		--cntPrint;
	if (isMove)
		cntPrint = 1;
}

void Motor::moveHome()
{
	isMoveHome = true;
	isMove = true;

	setDir(true);
	
	actSteps = 0;
	globalPos = maxSteps;
	newPosition = 0;
	diff = -1;
	Timer1.start();
	isRun = true;
}

void Motor::movePosition(uint32_t pos) 
{
	isMoveHome = false;
	isMove = true;
	
	setDir(true);
	
	actSteps = 0;
	newPosition = pos;
	diff = -1;

	canMove = true;
	Serial.print("1. globalPos");
	Serial.println(globalPos, DEC);


	if (pos == globalPos) {
		Serial.println("pos = globalpos");
		return;
	} else if (pos > globalPos) {
		diff = 1;
		setDir(true);
		Serial.println("left");
	} else if (pos < globalPos) {
		actSteps = globalPos - pos;
		diff = -1;
		setDir(false);
		Serial.println("right");
	}
	Timer1.start();
	isRun = true;
}
