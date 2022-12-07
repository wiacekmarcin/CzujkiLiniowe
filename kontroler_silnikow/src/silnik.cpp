#include "silnik.hpp"
#include "proto.hpp"
#include <TimerOne.h>
extern Message msg;

#define DEBUG
#define SD(T) Serial.print(T);
#define SDN(T) Serial.print(T);
#define SD2(T,P) Serial.print(T,P);
#define SDN2(T,P) Serial.print(T,P);

#define SDP(T, V) SD(T); SD(V);
#define SDPN(T, V) SD(T); SDN(V);

extern void setCreateStopMessageFun();

//TODO
//dla ramiona chodzacych katowo na kierunku góra dól ramię opadnie grawitacyjnie do jednej krańcówki
// dla pozostalych 






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
	,home(false)
	,interrupted(false)
	,allSteps(0)
	,baseErr(false)
	,firstHome(true)
{
	    
}

void Motor::init()
{
	
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
			chDir = '-';
			SDP("\nI: globalPos=", globalPos);
			SDPN(" newPosition=", newPosition);
#endif			
		}
	} else {
#ifdef DEBUG		
		SD('S');
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
	++actSteps;
#ifdef DEBUG
	SD(chDir);
#endif // DEBUG
	
	if (globalPos == newPosition || actSteps == maxSteps) {
		Timer1.stop(); //to moze chwile trwac
#ifdef DEBUG		
		SDP("\nI: globalPos=", globalPos);SDP(" newPosition=", newPosition);SDP(" actSteps=", actSteps);SDPN(" diff=", diff);
#endif		
		if (mstate == HOME_BASE) {
			mstate = HOME_MIDDLE;
			actSteps = 0;
			newPosition = middleSteps;
			setDirBase(false);
#ifdef DEBUG			
			chDir='>';		SDP("HM: globalPos=",globalPos);SDPN("newPosition=", newPosition);
#endif			
		} else if (mstate == MOVE_HOME_BASE) {
			mstate = HOME_NOMINAL;
			actSteps = 0;
			newPosition = 0;
			setDirBase(true);
		}
		else {
#ifdef DEBUG			
			SD("\nE: globalPos=");SD(globalPos);SD("\tnewPosition=");SDN(newPosition);
#endif			
			mstate = IDLE;
			digitalWrite(MOVEPIN, HIGH);
			if (!interrupted && actSteps >= maxSteps)
				interrupted = true;
		}
		Timer1.start();
	}
	
}


bool Motor::moveHome()
{
	if (digitalRead(KRANCPIN) == LOW) {
		if (firstHome) {
			//jestesmy na krancu ale nie wiemy ktorym
			mstate = IDLE;
			baseErr = true;
#ifdef DEBUG			
			chDir = '<';
			SDN("\nI: Stan nie ustalony. Pierwsze uruchomienie");
#endif				
			return false;
		} else {
			mstate = MOVE_HOME_BASE;     // przehodzimy do od razu do ustawienia 
			actSteps = 0;
			newPosition = middleSteps;
			globalPos = 0;
			diff = 1;
			home = true;
			interrupted = false;
			allSteps = 0;
			baseErr = false; 
			firstHome = false;
#ifdef DEBUG			
			chDir = '>';
			SDP("globalPos=", globalPos);
			SDPN(" newPosition=", newPosition);
#endif	
			digitalWrite(MOVEPIN, LOW);
			return true;
		} 
	}

	
	mstate = HOME_NOMINAL;
	actSteps = 0;
	globalPos = maxSteps;
	newPosition = 0;
	diff = -1;
	home = true;
    interrupted = false;
    allSteps = 0;
	baseErr = false; 
	firstHome = false;
	

#ifdef DEBUG
	chDir = '-';
	SD("\nI: globalPos=");
	SD(globalPos);
	SD("\tnewPosition=");
	SD(actSteps);
	SD("\tactSteps=");
	SD(actSteps);
	SD("\tmaxSteps=");
	SD(maxSteps);
	SD("\tdiff=");
	SDN(diff);
#endif	
	digitalWrite(MOVEPIN, LOW);
	return true;
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
