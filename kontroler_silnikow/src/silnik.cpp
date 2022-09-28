#include "silnik.hpp"
#include "proto.hpp"

extern Message msg;

Motor::Motor() :
    reverseMotor(false)
    ,enableAlways(true)
    ,globalPos(0)
    ,canMove(true)
    ,maxSteps(100000)
    ,delayImp(1000)
    ,baseSteps(100)
    ,wasHome(false)
{

}

void Motor::init()
{

  	pinMode(ENPIN, OUTPUT);
	pinMode(ENPIN2, OUTPUT);
	//setEnabled(true);
  
  	pinMode(KRANCPIN, INPUT_PULLUP);
	pinMode(DIRPIN, OUTPUT);
	pinMode(PULSEPIN, OUTPUT);
	pinMode(DIRPIN2, OUTPUT);
	pinMode(PULSEPIN2, OUTPUT);
}


inline void Motor::setEnabled(bool en)
{
	digitalWrite(ENPIN, en);
	digitalWrite(ENPIN2, en);
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
	digitalWrite(DIRPIN2, out);
}

void Motor::setStop()
{
	canMove = false;
}

bool Motor::moveHome()
{
	bool ret = false;
	//if (!enableAlways)
	setEnabled(true);

	setDir(true);
	
	uint32_t steps = maxSteps;
	digitalWrite(PULSEPIN, LOW);
	digitalWrite(PULSEPIN2, LOW);
	while (steps-- && canMove && digitalRead(10))
	{
		delayMicroseconds(delayImp);
		if (!canMove || !digitalRead(10))
			break;
		digitalWrite(PULSEPIN, HIGH);
		digitalWrite(PULSEPIN2, HIGH);
		delayMicroseconds(delayImp);
		digitalWrite(PULSEPIN, LOW);
		digitalWrite(PULSEPIN2, LOW);
	}

	if (!canMove) {
		steps = baseSteps;
		wasHome = true;
		while (steps--)
		{
			delayMicroseconds(delayImp);
			digitalWrite(PULSEPIN, HIGH);
			digitalWrite(PULSEPIN2, HIGH);
			delayMicroseconds(delayImp);
			digitalWrite(PULSEPIN, LOW);
			digitalWrite(PULSEPIN2, LOW);
		}
		ret = true;
	}	
	globalPos = 0;
	if (!enableAlways)
		setEnabled(false);	
	msg.addSteps(ret, steps);
	return ret;
}

bool Motor::movePosition(uint32_t pos) 
{

	uint32_t steps;
	uint8_t diff = 0;

	if (pos == globalPos) {
		return true;
	} else if (pos > globalPos) {
		steps = pos - globalPos;
		diff = 1;	
		setDir(false);
	} else if (pos < globalPos) {
		steps = globalPos - pos;
		diff = -1;
		setDir(false);
	}

	digitalWrite(PULSEPIN, LOW);
	digitalWrite(PULSEPIN2, LOW);
	while (steps-- && canMove && digitalRead(10) && globalPos > 0 && globalPos < maxSteps)
	{
		delayMicroseconds(delayImp);
		if (!canMove || !digitalRead(10))
			break;
		digitalWrite(PULSEPIN, HIGH);
		digitalWrite(PULSEPIN2, HIGH);
		delayMicroseconds(delayImp);
		digitalWrite(PULSEPIN, LOW);
		digitalWrite(PULSEPIN2, LOW);
		globalPos += diff;
	}
    msg.addSteps(steps == 0, steps);
	return steps == 0;
}
