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
	,isRun(false)
	,newPosition(false)
	,diff(0)
	,highlevel(false)
	,isMoveHome(false)
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
	if () {
		highlevel = !highlevel;
		digitalWrite(PULSEPIN, highlevel ? HIGH : LOW);
		globalPos += diff;
		if (globalPos == newPosition) {

		}
	}
}


bool Motor::moveHome()
{
	bool ret = false;
	isMoveHome = true;

	//if (!enableAlways)
	setEnabled(true);

	setDir(true);
	
	uint32_t steps = maxSteps;
	digitalWrite(PULSEPIN, LOW);
	while (steps-- && canMove && digitalRead(10))
	{
		delayMicroseconds(delayImp);
		if (!canMove || !digitalRead(SS))
			break;
		digitalWrite(PULSEPIN, HIGH);
		delayMicroseconds(delayImp);
		digitalWrite(PULSEPIN, LOW);
	}

	if (!canMove) {
		steps = baseSteps;
		wasHome = true;
		while (steps--)
		{
			delayMicroseconds(delayImp);
			digitalWrite(PULSEPIN, HIGH);
			delayMicroseconds(delayImp);
			digitalWrite(PULSEPIN, LOW);
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
	isMoveHome = false;
	uint32_t steps;
	uint8_t diff = 0;

	setEnabled(true);
	canMove = true;
	Serial.print("1. globalPos");
	Serial.println(globalPos, DEC);
	if (pos == globalPos) {
		Serial.println("pos = globalpos");
		return true;
	} else if (pos > globalPos) {
		steps = pos - globalPos;
		diff = 1;	
		setDir(true);
		Serial.println("left");
	} else if (pos < globalPos) {
		steps = globalPos - pos;
		diff = -1;
		setDir(false);
		Serial.println("right");
	}

	digitalWrite(PULSEPIN, LOW);
	Serial.print("1. Steps = ");
	Serial.println(steps, DEC);
	steps += 1; //dodaje zero i 1 dekremetacja licznika 
	while (--steps && globalPos >= 0 && globalPos < maxSteps)
	{
		delayMicroseconds(delayImp);
		if (!canMove || !digitalRead(SS)) {
			Serial.println("break");
			break;
		}
		digitalWrite(PULSEPIN, HIGH);
		delayMicroseconds(delayImp);
		digitalWrite(PULSEPIN, LOW);
		globalPos += diff;
	}
	Serial.print("2. Steps");
	Serial.println(steps, DEC);
	Serial.print("2. globalPos");
	Serial.println(globalPos, DEC);
    msg.addSteps(steps == 0, steps);
	if (!enableAlways)
		setEnabled(false);	
	return steps == 0;
}
