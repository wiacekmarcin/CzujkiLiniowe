#include "silnik.hpp"
#include "proto.hpp"
#include <TimerOne.h>
extern Message msg;

Motor::Motor() :
    reverseMotor(false)
    ,enableAlways(true)
	,maxSteps(100000)
    ,delayImp(1000)
    ,baseSteps(100)
    ,wasHome(false)
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

}

void Motor::init()
{

  	pinMode(ENPIN, OUTPUT);
	//setEnabled(true);
  
  	pinMode(KRANCPIN, INPUT_PULLUP);
	pinMode(DIRPIN, OUTPUT);
	pinMode(PULSEPIN, OUTPUT);
}


inline void Motor::setEnabled(bool en)
{
	digitalWrite(ENPIN, en ? LOW : HIGH);
}

//DIR REV
// 0    0   => 0
// 0    1   => 1
// 1    0   => 1
// 1    1   => 0
inline void Motor::setDir(bool back)
{
	bool out = back ^ reverseMotor;
	digitalWrite(DIRPIN, out);
}

void Motor::setStop()
{
	canMove = false;
}

void Motor::impulse()
{
	highlevel = !highlevel;
	digitalWrite(PULSEPIN, highlevel ? HIGH : LOW);
	globalPos += diff;
	if (globalPos == newPosition || ++actSteps == maxSteps) {
		Timer1.stop();
	}
}


void Motor::moveHome()
{
	bool ret = false;
	isMoveHome = true;

	//if (!enableAlways)
	setEnabled(true);

	setDir(true);
	
	actSteps = 0;
	globalPos = maxSteps;
	newPosition = 0;
	diff = -1;
	Timer1.start();
}

void Motor::movePosition(uint32_t pos) 
{
	isMoveHome = false;
	
	//if (!enableAlways)
	setEnabled(true);

	setDir(true);
	
	actSteps = 0;
	newPosition = pos;
	diff = -1;

	setEnabled(true);
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
		steps = globalPos - pos;
		diff = -1;
		setDir(false);
		Serial.println("right");
	}
	Timer1.start();
}
