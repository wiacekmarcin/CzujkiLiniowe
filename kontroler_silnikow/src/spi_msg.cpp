#include "spi_msg.hpp"
#include <Arduino.h>
#include "proto.hpp"
#include "crc8.hpp"
#include "main.h"
#include "silnik.hpp"

#define DEBUG
//#define EXT_DEBUG
#ifdef DEBUG
	#define SD(T) Serial.print(T)
	#define SDN(T) Serial.println(T)
	#define SD2(T,P) Serial.print(T,P)
	#define SDN2(T,P) Serial.println(T,P)

	#define SDP(T, V) SD(T); SD(V)
	#define SDPN(T, V) SD(T); SDN(V)
    #define SPHEX(X) phex(X)
    #define SPRINT(N)	SD("To Send (");SD(__FILE__);SD(":");SD(__LINE__);SD(") [");for (int i=0;i<N;++i){ SPHEX(sendBuff[i]); }SDN("]");
#else
	#define SD(T) 
	#define SDN(T) 
	#define SD2(T,P) 
	#define SDN2(T,P) 

	#define SDP(T, V) 
	#define SDPN(T, V)
    #define SPHEX(X)
    #define SPRINT(N)
#endif

#ifdef EXT_DEBUG
	#define ESD(T) Serial.print(T)
	#define ESDN(T) Serial.println(T)
	#define ESD2(T,P) Serial.print(T,P)
	#define ESDN2(T,P) Serial.println(T,P)

	#define ESDP(T, V) SD(T); SD(V)
	#define ESDPN(T, V) SD(T); SDN(V)
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

		SD("Mam wiadomosc: ");SDN(msg.getMsgCmd());

		Result status = msg.parse();
		if (!status.ok)
		{
			SDN("CMD:Nie poprawna wiadomosc. BUSY OFF");
			msg.clear();
			setBusy(false);
			continue;
		}

		SDN("Poprawna wiadomosc . BUSY ON");
		setBusy(true);
        address = msg.getAddr();
		ESDPN("ADDR:", address);
		switch (msg.getMsgCmd()) {

		case ECHO_REQ:
		{
			SDN(" CMD:ECHO");
			unsigned long actTime = millis();
			digitalWrite(MOVEPIN, LOW);
			SD("Czekam na stopPin HIGH (movePin na LOW)...");
			while(digitalRead(STOPPIN) == HIGH && (millis() - actTime < 100));
			if (millis() - actTime >= 100) {
				SDN(" Timeout");
				echoRequestFun(false, false);
			} else {
				SDN(" OK");
				SD(" zmiana movepin na HIGH.\nCzekan na stopPin HIGH ....");
				digitalWrite(MOVEPIN, HIGH);
				actTime = millis();
				while(digitalRead(STOPPIN) == LOW && (millis() - actTime < 100));
				if (millis() - actTime >= 100) {
					SDN(" Timeout");
					echoRequestFun(true, false);
				} else {
					SDN(" OK");
					echoRequestFun(true, true);
					attachInterrupt(digitalPinToInterrupt(STOPPIN), setStopSoft, FALLING);
				}
			}
			SDN("BUSY OFF");
			setBusy(false);
			msg.clear();
			break;
		}

		case PROGRESS_REQ:
		{
			SDN(" CMD:PROGRESS");
    		progressRequestFun();
			SDN("BUSY OFF");
            msg.clear();
			setBusy(false);
            
			break;
		}

		case CONF_REQ:
		{
			SDN(" CMD:CONFIGURATION");
			configurationRequest(status);
			SDN("BUSY OFF");
            msg.clear();
            setBusy(false);
			break;
		}

		case MOVE_REQ:
		{
			SDN(" CMD:MOVE");
			moveRequest(status.data.move.isHome, status.data.move.position, status.data.move.speed);
			SDN("BUSY OFF");
			msg.clear();
            setBusy(false);
			break;
		}

		default:
		{
			SDN("CMD:Uknonwn.\nBUSY OFF");
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
	SPRINT(3);
}

void SPIMessage::progressRequestFun()
{
	CRC8 crc;
	crc.restart();
	sendBuff[0] = ((PROGRESS_REP << 4) & 0xf0) | 0x04;
	sendBuff[1] = ((address << 4) & 0xf0) | 0x08;
	if (mot->isMove())
		sendBuff[1] |= 0x04;
	if (mot->isHomeMove())
		sendBuff[1] |= 0x02;
	crc.add(sendBuff[0]);
	crc.add(sendBuff[1]);
	int32_t pos = mot->getGlobalPos();
	SDPN("POS=", pos);
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
	SPRINT(7);
}

void SPIMessage::configurationRequest(Result status)
{
	SDN("CMD:CONF");SDN("Konfiguracja:");
    SDPN("reverse=",status.data.conf.reverse);
	SDPN("max=",status.data.conf.maxStep);
	SDPN("base=",status.data.conf.baseSteps);
	SDPN("middle=",status.data.conf.middleSteps);

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
    SPRINT(3);
}

void SPIMessage::moveRequest(bool isHome, uint32_t steps, uint32_t delayImp)
{
	SD("Ruch. Home = ");SD(isHome ? "Tak" : "Nie");SD(" Kroki = ");SD(steps);SD(" Interwal = ");SDN(delayImp);

    bool wasMove;
    if (isHome)
		wasMove = mot->moveHome(delayImp);
	else
		wasMove = mot->movePosition(delayImp, steps);
	CRC8 crc;
	crc.restart();
	sendBuff[0] = (MOVE_REP << 4) & 0xf0;
	sendBuff[1] = ((address << 4) & 0xf0) | 0x08;
	if (isHome)
		sendBuff[1] += 1;
	crc.add(sendBuff[0]);
	crc.add(sendBuff[1]);
	sendBuff[2] = crc.getCRC();
	sizeSendMsg = 3;
    SPRINT(3);

	//if (!wasMove)
	//	emptyMove(3);
}

void SPIMessage::emptyMove(uint8_t of)
{
	SD("Stop Ruch. Czy home:");SD(mot->isHome() ? "Tak" : "Nie");SD(" Kroki = ");SD(mot->getStepsAll());
    SD(" Pozycja = ");SD(mot->getGlobalPos());SD(" Przerwany");SD(mot->isInterrupted());

	
	CRC8 crc;
	crc.restart();
	sendBuff[0] += 8;
	sendBuff[1+of] = ((MOVE_REP << 4) & 0xf0 ) | 8;
	sendBuff[2+of] = ((address << 4) & 0xf0) | 0x08;
	if (mot->isHome())
		sendBuff[2+of] += 1;
	
	if (mot->isInterrupted())
		sendBuff[2+of] += 2;

	if (mot->isBaseError())
		sendBuff[2+of] += 4;
	uint32_t gp = mot->getGlobalPos();
	uint32_t ap = mot->getStepsAll();
	sendBuff[3+of] = (uint8_t)((gp >> 24) & 0xff);
	sendBuff[4+of] = (uint8_t)((gp >> 16) & 0xff);
	sendBuff[5+of] = (uint8_t)((gp >> 8) & 0xff);
	sendBuff[6+of] = (uint8_t)(gp & 0xff);
	sendBuff[7+of] = (uint8_t)((ap >> 24) & 0xff);
	sendBuff[8+of] = (uint8_t)((ap >> 16) & 0xff);
	sendBuff[9+of] = (uint8_t)((ap >> 8) & 0xff);
	sendBuff[10+of] = (uint8_t)(ap & 0xff);

	for (int i = 1; i < 11 ; ++i)
		crc.add(sendBuff[1+of]);
	sendBuff[12+of] = crc.getCRC();
    SPRINT(13+of);
}