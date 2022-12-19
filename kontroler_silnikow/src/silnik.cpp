#include "silnik.hpp"
#include "proto.hpp"
#include "main.h"
#include <TimerOne.h>

extern Message msg;

//#define DEBUG

#ifdef DEBUG
	#define SD(T) Serial.print(T);
	#define SDN(T) Serial.println(T);
	#define SD2(T,P) Serial.print(T,P);
	#define SDN2(T,P) Serial.println(T,P);

	#define SDP(T, V) SD(T); SD(V);
	#define SDPN(T, V) SD(T); SDN(V);
#else
	#define SD(T) 
	#define SDN(T) 
	#define SD2(T,P) 
	#define SDN2(T,P) 

	#define SDP(T, V) 
	#define SDPN(T, V) 
#endif

extern void setCreateStopMessageFun();

//TODO
//dla ramiona chodzacych katowo na kierunku góra dól ramię opadnie grawitacyjnie do jednej krańcówki
// dla pozostalych 






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
	,interrupted(false)
	,allSteps(0)
	,baseErr(false)
	,moveP(false)
    ,moveH(false)
{
	
}

void Motor::init()
{
	pinMode(ENPIN, OUTPUT);
	pinMode(DIRPIN, OUTPUT);
	pinMode(PULSEPIN, OUTPUT);
#ifdef TEST	
	pinMode(KRANCPIN, INPUT_PULLUP);
	pinMode(STOPPIN, INPUT_PULLUP);
#else
	pinMode(KRANCPIN, INPUT);
	pinMode(STOPPIN, INPUT);
#endif	
	pinMode(MOVEPIN, OUTPUT);
	
	digitalWrite(MOVEPIN, LOW);
	digitalWrite(ENPIN, LOW);
	
}

volatile char chDir = '>';

//DIR REV
// 0    0   => 0
// 0    1   => 1
// 1    0   => 1
// 1    1   => 0
void Motor::setDirBase(bool back)
{
	bool out = back ^ reverseMotor;
	digitalWrite(DIRPIN, out);
	diff = back ? -1 : 1;
}

/*
void Motor::setStop(bool hard)
{
	if (!hard) {
		SD('S');
		mstate = IDLE;
		interrupted = true;
		digitalWrite(MOVEPIN, LOW);
		setCreateStopMessageFun();
		return;
	}

	SD('H')

	if (mstate == HOME_DO_BAZY) {
		if (baseSteps == 0) {
			globalPos = 0;
			if (middleSteps == 0) {
				Timer1.stop();
				mstate = IDLE;
				digitalWrite(MOVEPIN, LOW);
				setCreateStopMessageFun();
				chDir='K';		
				SDP("K: globalPos=",globalPos);
				SDP(" newPosition=", newPosition);
				SDPN(" actSteps=", actSteps);
				return;							
			} else {
				chDir='M';		
				SDP("M: globalPos=",globalPos);
				SDP(" newPosition=", newPosition);
				SD("=>");
				SD(middleSteps);
				SDP(" actSteps=", actSteps);
				SDN("=>0");
				mstate = HOME_DO_SRODKA;
				newPosition = middleSteps;
				actSteps = 0;
				setDirBase(false);
				diff += 1;
			} // 	
		} else { 
			mstate = HOME_W_BAZIE;
			chDir='b';
			SDP("M: globalPos=",globalPos);
			SD("=>");
			SD(baseSteps);
			SDP(" newPosition=", newPosition);
			SDP("=>0 actSteps=", actSteps);
			SDN("=>0");
			globalPos = baseSteps;
			newPosition = 0;
			actSteps = 0;	
		}
	}
}

void Motor::impulse()
{
	if (impTimer++ % 8 != 0)
		return;

	impTimer = 0;
	if (mstate == IDLE)
		return;	 
	
	highlevel = !highlevel;
	digitalWrite(PULSEPIN, highlevel ? HIGH : LOW);
	globalPos += diff;
	++allSteps;
	++actSteps;

	SD(chDir);

	if (actSteps == maxSteps) {
		mstate = IDLE;
		interrupted = true;
		Timer1.stop();
		SDP("\nI: globalPos=", globalPos);
		SDP(" newPosition=", newPosition);
		SDP(" actSteps=", actSteps);
		SDPN(" diff=", diff);
		digitalWrite(MOVEPIN, LOW);
		setCreateStopMessageFun();
	}
	
	if (globalPos == newPosition) {
		Timer1.stop(); //to moze chwile trwac
		SDP("\nI: globalPos=", globalPos);
		SDP(" newPosition=", newPosition);
		SDP(" actSteps=", actSteps);
		SDPN(" diff=", diff);

		switch(mstate) {
			case HOME_OD_BAZY: 
			{
				chDir='B';		
				SDP("ODBAZY: globalPos=",globalPos);
				SD("=>");
				SD(maxSteps);
				SDP(" newPosition=", newPosition);
				SDN("=>0");
				mstate = HOME_DO_BAZY;
				setDirBase(true);
				actSteps = 0;
				newPosition = 0;
				globalPos = maxSteps;
				diff =-1;
				Timer1.start();	
				break;
			}
			case HOME_W_BAZIE:
			{
				if (middleSteps) {
					diff = 1;
					newPosition = middleSteps;
					SDP("\nWBAZIE:SRODEK: globalPos=", globalPos);
					SDP(" newPosition=", newPosition);
					SDP(" actSteps=", actSteps);
					SDPN(" diff=", diff);
					mstate = HOME_DO_SRODKA;
					Timer1.start();
				} else {
					mstate = IDLE;
					SDP("\nWBAZIE:END: globalPos=", globalPos);
					SDP(" newPosition=", newPosition);
					SDP(" actSteps=", actSteps);
					digitalWrite(MOVEPIN, LOW);
					setCreateStopMessageFun();
				}
				break;
			}
			case HOME_DO_SRODKA:
			{
				mstate = IDLE;
				SDP("\nSRODEK:END: globalPos=", globalPos);
				SDP(" newPosition=", newPosition);
				SDP(" actSteps=", actSteps);
				digitalWrite(MOVEPIN, LOW);
				setCreateStopMessageFun();
				break;
			}
			case MOVE_POS:
			{
				mstate = IDLE;
				SDP("\nSRODEK:END: globalPos=", globalPos);
				SDP(" newPosition=", newPosition);
				SDP(" actSteps=", actSteps);
				digitalWrite(MOVEPIN, LOW);
				setCreateStopMessageFun();
				break;
			}
		}
	}
}
*/

/*
bool Motor::moveHome(uint8_t mode)
{
	bool ret = false;
	actSteps = 0;
	globalPos = 0;
	newPosition = 0;
	diff = -1;
	home = true;
	interrupted = false;
	allSteps = 0;
	baseErr = false; 
#ifdef DEBUG 
	SDN("Ruch do bazy");
#endif	

	switch(mode) {
		case PIONOWA:
			ret = moveHomePionowa(); break;
		case POZIOMA:
			ret = moveHomePozioma(); break;
		case KATOWA_PION:
			ret = moveHomeKatPionowy(); break;
		case KATOWA_POZ:
			ret = moveHomeKatPoziomy(); break;
		default:
			break;
	}
	if (!ret) {
#ifdef DEBUG 
		SD("Brak ruchu....");
#endif		
		return false;
	}
#ifdef DEBUG
		SDN("Ruch");
		if (mstate == HOME_OD_BAZY)
			chDir = 'O';
		else if (mstate == HOME_DO_BAZY)
			chDir = 'B';
		else
			chDir = '?';
#endif		


	Timer1.start();
	return true;
}

bool Motor::movePosition(uint8_t mode, uint32_t pos) 
{

	if (mode == KOLOWA)
		moveFiltrPosiotion();
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
		setDirBase(true);
	} else if (pos < globalPos) {
		diff = -1;
#ifdef DEBUG		
		chDir = '-';
#endif		
		setDirBase(false);
	}
	digitalWrite(MOVEPIN, LOW);
	mstate = MOVE_POS;
	return true;
}

bool Motor::moveHomePionowa()
{
	digitalWrite(MOVEPIN, HIGH);
	if (digitalRead(KRANCPIN) == LOW) {
		setDirBase(false);
		actSteps = 0;
		globalPos = 0;
		newPosition = 10000;
		diff = +1;
		mstate = HOME_OD_BAZY;
	} else {
		setDirBase(true);
		actSteps = 0;
		globalPos = maxSteps;
		newPosition = 0;
		diff = -1;
		mstate = HOME_DO_BAZY;
	}
	return true;
}

bool Motor::moveHomePozioma() 
{
	if (digitalRead(KRANCPIN) == LOW) 
		return false;
	setDirBase(true);
	actSteps = 0;
	globalPos = maxSteps;
	newPosition = 0;
	diff = -1;
	mstate = HOME_DO_BAZY;
	digitalWrite(MOVEPIN, HIGH);
	return true;
}

bool Motor::moveHomeKatPionowy()
{
	digitalWrite(MOVEPIN, HIGH);
	if (digitalRead(KRANCPIN) == LOW) {
		setDirBase(false);
		actSteps = 0;
		globalPos = 0;
		newPosition = 50;
		diff = +1;
		mstate = HOME_OD_BAZY;
	} else {
		setDirBase(true);
		actSteps = 0;
		globalPos = maxSteps;
		newPosition = 0;
		diff = -1;
		mstate = HOME_DO_BAZY;
	}
	return true;
}

bool Motor::moveHomeKatPoziomy()
{
	if (digitalRead(KRANCPIN) == LOW) 
		return false;
	setDirBase(true);
	actSteps = 0;
	globalPos = maxSteps;
	newPosition = 0;
	diff = -1;
	mstate = HOME_DO_BAZY;
	digitalWrite(MOVEPIN, HIGH);
	return true;
}

bool Motor::moveFiltrPosiotion()
{
	return true;
}*/