#include "silnik.hpp"
#include "proto.hpp"
#include <TimerOne.h>
extern Message msg;

//#define DEBUG

extern void setCreateStopMessageFun();

Motor::Motor() :
    reverseMotor(false)
	,maxSteps(0)
    ,baseSteps(0)
	,middleSteps(0)
	,delayImp(0)
	,impTimer(0)
	,globalPos(0)
	,diff(0)
	,newPosition(false)
	,highlevel(false)
    ,mstate(IDLE)
	,actSteps(0)
{
	    
}

void Motor::init()
{
	
}

volatile char chDir = '-';

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
	if (hard) {
#ifdef DEBUG		
		Serial.print('K');
#endif		
		if (mstate == HOME_NOMINAL) {
			globalPos = baseSteps;
			mstate = HOME_BASE;
#ifdef DEBUG			
			chDir = '_';
			Serial.print("\nI: globalPos=");
			Serial.print(globalPos);
			Serial.print("\tnewPosition=");
			Serial.println(newPosition);
#endif			
		}
	} else {
#ifdef DEBUG		
		Serial.print('S');
#endif			
		mstate = IDLE;
		interrupted = true;
		digitalWrite(MOVEPIN, HIGH);
		setCreateStopMessageFun();
	}
	
}

void Motor::impulse()
{
	if (impTimer++ % 8)
		return;

	impTimer = 0;
	if (mstate == IDLE)
		return;	 
	
	highlevel = !highlevel;
	digitalWrite(PULSEPIN, highlevel ? HIGH : LOW);
	globalPos += diff;
	++allSteps;
#ifdef DEBUG
	Serial.print(chDir);
#endif // DEBUG
	
	if (globalPos == newPosition || ++actSteps == maxSteps) {
#ifdef DEBUG		
		Serial.print("\nI: globalPos=");
		Serial.print(globalPos);
		Serial.print("\tnewPosition=");
		Serial.print(newPosition);
		Serial.print("\tactSteps=");
		Serial.print(actSteps);
		Serial.print("\tdiff=");
		Serial.println(diff);
#endif		
		if (mstate == HOME_BASE) {
			mstate = HOME_MIDDLE;
			actSteps = 0;
			newPosition = middleSteps;
			setDir(true);
			diff = 1;
#ifdef DEBUG			
			chDir='+';
			Serial.print("HM: globalPos=");
			Serial.print(globalPos);
			Serial.print("newPosition=");
			Serial.println(newPosition);
#endif			
		} else {
#ifdef DEBUG			
			Serial.print("\nE: globalPos=");
			Serial.print(globalPos);
			Serial.print("\tnewPosition=");
			Serial.println(newPosition);
#endif			
			mstate = IDLE;
			digitalWrite(MOVEPIN, HIGH);
			interrupted = actSteps >= maxSteps;
		}
	}
	
}


void Motor::moveHome()
{
	mstate = HOME_NOMINAL;
	actSteps = 0;
	globalPos = maxSteps;
	newPosition = 0;
	diff = -1;
	home = true;
    interrupted = false;
    allSteps = 0;
#ifdef DEBUG
	chDir = '-';
	Serial.print("\nI: globalPos=");
	Serial.print(globalPos);
	Serial.print("\tnewPosition=");
	Serial.print(actSteps);
	Serial.print("\tactSteps=");
	Serial.print(actSteps);
	Serial.print("\tmaxSteps=");
	Serial.print(maxSteps);
	Serial.print("\tdiff=");
	Serial.println(diff);
#endif	
	digitalWrite(MOVEPIN, LOW);
}

bool Motor::movePosition(uint32_t pos) 
{
	actSteps = 0;
	newPosition = pos;
	diff = -1;
	home = false;
    interrupted = false;
    allSteps = 0;

	if (pos == globalPos) {
		return false;
	} else if (pos > globalPos) {
		diff = 1;
#ifdef DEBUG		
		chDir = '+';
#endif		
		setDir(true);
	} else if (pos < globalPos) {
		diff = -1;
#ifdef DEBUG		
		chDir = '-';
#endif		
		setDir(false);
	}
	digitalWrite(MOVEPIN, LOW);
	mstate = MOVE_POS;
	return true;
}
