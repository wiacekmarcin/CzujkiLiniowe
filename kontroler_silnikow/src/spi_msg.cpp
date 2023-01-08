#include "spi_msg.hpp"
#include <Arduino.h>
#include "proto.hpp"
#include "crc8.hpp"
#include "main.h"
#include "silnik.hpp"

//#define SSDEBUG
//#define EXT_DEBUG
#ifdef SSDEBUG
	#define SSD(T) Serial.print(T)
	#define SSDN(T) Serial.println(T)
	#define SSD2(T,P) Serial.print(T,P)
	#define SSDN2(T,P) Serial.println(T,P)

	#define SSDP(T, V) SSD(T); SSD(V)
	#define SSDPN(T, V) SSD(T); SSDN(V)
    #define SSPHEX(X) phex(X)
    #define SSPRINT(N)	SSD("Add to send (");SSD(__FILE__);SSD(":");SSD(__LINE__);SSD(") [");for (unsigned short i=0;i<(N);++i){ SSPHEX(sendBuff[i]); }SSDN("]");
#else
	#define SSD(T) 
	#define SSDN(T) 
	#define SSD2(T,P) 
	#define SSDN2(T,P) 

	#define SSDP(T, V) 
	#define SSDPN(T, V)
    #define SSPHEX(X)
    #define SSPRINT(N)
#endif

#ifdef EXT_DEBUG
	#define ESD(T) Serial.print(T)
	#define ESDN(T) Serial.println(T)
	#define ESD2(T,P) Serial.print(T,P)
	#define ESDN2(T,P) Serial.println(T,P)

	#define ESDP(T, V) SSD(T); SSD(V)
	#define ESDPN(T, V) SSD(T); SSDN(V)
    #define EPHEX(X) phex(X)
#else
	#define ESD(T) 
	#define ESDN(T) 
	#define ESD2(T,P) 
	#define ESDN2(T,P) 

	#define ESDP(T, V) 
	#define ESDPN(T, V) 
    #define EPHEX(X)
#endif



SPIMessage::SPIMessage()
{
    actProcess = 0;
    skipCharCnt = -1;
    address = 15;
}

void SPIMessage::init(uint8_t id, Motor * mot_)
{
	this->address = 0x30 + id;
    for (uint8_t i = 0; i < maxBuff; ++i)
	{
		recvBuff[2*i] = 0;
		recvBuff[2*i+1] = 0;
		sendBuff[i] = 0;
	}

	sendBuff[0] = 3;
	sendBuff[1] = (ECHO_REP << 4) & 0xf0;
	sendBuff[2] = uint8_t((id << 4) | 0x80);
	CRC8 c;
    c.reset();
    c.add(sendBuff[1]);
	c.add(sendBuff[2]);
    sendBuff[3] = c.getCRC();
	sendBuff[4] = 0;

    msg.init();
    mot = mot_;
}


bool SPIMessage::proceed()
{
    ESD("1.Start Rec/Act:[");ESD(actProcess+1);ESD(",");ESD(recvPos);ESDN("]");
	ESDN("----------");

	while (recvPos > 0 && actProcess < recvPos)
	{
		ESD("2.Act/Rec:[");ESD(actProcess+1);ESD(",");ESD(recvPos);ESD("]=");EPHEX(recvBuff[actProcess]);ESDN("");

		if (!msg.add(recvBuff[actProcess++])) {
			continue;
		}

		SSD("Mam wiadomosc: ");SSDN(msg.getMsgCmd());

		Result status = msg.parse();
		if (!status.ok)
		{
			SSDN("CMD:Nie poprawna wiadomosc. BUSY OFF");
			msg.clear();
			setBusy(false);
			continue;
		}

		SSDN("Poprawna wiadomosc . BUSY ON");
		address = msg.getAddr();
		ESDPN("ADDR:", address);
		setBusy(true);
		bool proceed;
		switch (msg.getMsgCmd()) {

		case ECHO_REQ:
		{
			SSDN(" CMD:ECHO");

			
			pinMode(STOPPIN, OUTPUT);
			digitalWrite(STOPPIN, LOW);
			digitalWrite(MOVEPIN, LOW);

			delayMicroseconds(100);
			echoRequestFun(true, true);

			pinMode(STOPPIN, INPUT);

			SSDN("BUSY OFF");
			setBusy(false);
			digitalWrite(MOVEPIN, LOW);
			msg.clear();
			break;
		}

		case PROGRESS_REQ:
		{
			SSDN(" CMD:PROGRESS");
    		progressRequestFun();
			SSDN("BUSY OFF");
            msg.clear();
			setBusy(false);
            
			break;
		}

		case CONF_REQ:
		{
			SSDN(" CMD:CONFIGURATION");
			configurationRequest(status);
			SSDN("BUSY OFF");
            msg.clear();
            setBusy(false);
			attachInterrupt(digitalPinToInterrupt(STOPPIN), setStopSoft, FALLING);
			break;
		}

		case MOVE_REQ:
		{
			SSDN(" CMD:MOVE");
			moveRequest(status.data.move.isHome, status.data.move.position, status.data.move.speed);
			SSDN("BUSY OFF");
			msg.clear();
            setBusy(false);
			break;
		}

		default:
		{
			SSDN("CMD:Uknonwn.\nBUSY OFF");
			msg.clear();
			setBusy(false);
			break;
		}

		} // switch
	}	  // while

	ESDN("---------");ESD("3. Act/Rec:[");ESD(actProcess);ESD(",");ESD(recvPos);ESD("]");ESD("END.LOOP");

	actProcess = 0;
	recvPos = 0;
	return false;
}

void SPIMessage::echoRequestFun(bool stopOk, bool moveOk)
{
	CRC8 crc;
	crc.restart();
	sendBuff[0] = (ECHO_REP << 4) & 0xf0;
	sendBuff[1] = ((address << 4) & 0xf0) | 0x08;
	if (stopOk)
		sendBuff[1] |= 0x01;
	if (moveOk)
		sendBuff[1] |= 0x02;
	
	crc.add(sendBuff[0]);
	crc.add(sendBuff[1]);
	sendBuff[2] = crc.getCRC();
	sizeSendMsg = 3;
	SSPRINT(3);
}

void SPIMessage::progressRequestFun()
{
	CRC8 crc;
	crc.restart();
	sendBuff[0] = ((PROGRESS_REP << 4) & 0xf0) | 0x04;
	sendBuff[1] = ((address << 4) & 0xf0) | 0x08;

	//if (mot->isMove())
	//	sendBuff[1] |= 0x04;
	//if (mot->isHomeMove())
	//	sendBuff[1] |= 0x02;

	crc.add(sendBuff[0]);
	crc.add(sendBuff[1]);
	int32_t pos = mot->getGlobalPos();
	SSDPN("POS=", pos);
	sendBuff[2] = (pos >> 24) & 0xff;
	sendBuff[3] = (pos >> 16) & 0xff;
	sendBuff[4] = (pos >> 8) & 0xff;
	sendBuff[5] = (pos)&0xff;
	crc.add(sendBuff[2]);
	crc.add(sendBuff[3]);
	crc.add(sendBuff[4]);
	crc.add(sendBuff[5]);
	sendBuff[6] = crc.getCRC();
	sizeSendMsg = 7;
	SSPRINT(7);
}

void SPIMessage::configurationRequest(Result status)
{
	SSDN("CMD:CONF");SSDN("Konfiguracja:");
    SSDPN("reverse=",status.data.conf.reverse);
	SSDPN("max=",status.data.conf.maxStep);
	SSDPN("base=",status.data.conf.baseSteps);
	SSDPN("middle=",status.data.conf.middleSteps);

	mot->setReverseMotor(status.data.conf.reverse);
	mot->setMaxSteps(status.data.conf.maxStep);
	mot->setBaseSteps(status.data.conf.baseSteps);
	mot->setMiddleSteps(status.data.conf.middleSteps);
	//mot->setConfiguration();
	CRC8 crc;
	crc.restart();
	sendBuff[0] = (CONF_REP << 4) & 0xf0;
	sendBuff[1] = ((address << 4) & 0xf0) | 0x08 | 0x04;
	crc.add(sendBuff[0]);
	crc.add(sendBuff[1]);
	sendBuff[2] = crc.getCRC();
	sizeSendMsg = 3;
    SSPRINT(3);
}

void SPIMessage::moveRequest(bool isHome, uint32_t steps, uint32_t delayImp)
{
	SSD("Ruch. Home = ");SSD(isHome ? "Tak" : "Nie");SSD(" Kroki = ");SSD(steps);SSD(" Interwal = ");SSDN(delayImp);

    if (isHome) {
		mot->moveHome(delayImp);
	} else {
		mot->movePosition(steps, delayImp);
	}
	sizeSendMsg = 3;
    SSPRINT(3);
}


void SPIMessage::moveStopRequest(bool interrupted, bool move, bool error, bool home)
{
	SSD("Ruch Stop. home=");SSD(home);SSD(" move=");SSD(move);SSD(" err=");SSD(error);SSD(" inter= ");SSDN(interrupted);
	setBusy(true);
	CRC8 crc;
	crc.restart();
	sendBuff[0] = (MOVE_REP << 4) & 0xf0;
	sendBuff[1] = ((address << 4) & 0xf0);
	if (home)
		sendBuff[1] += 1;
	if (error)
		sendBuff[1] += 2;	
	if (move)
		sendBuff[1] += 4;
	if (interrupted)
		sendBuff[1] += 8;
	crc.add(sendBuff[0]);
	crc.add(sendBuff[1]);
	sendBuff[2] = crc.getCRC();
	sizeSendMsg = 3;
    SSPRINT(3);
	setBusy(false);
}