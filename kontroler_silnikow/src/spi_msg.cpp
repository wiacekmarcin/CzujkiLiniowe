#include "spi_msg.hpp"
#include <Arduino.h>
#include "proto.hpp"
#include "crc8.hpp"
#include "main.h"
#include "SPI.h"
#include "silnik.hpp"

#define DEBUG

#ifdef DEBUG
	#define SD(T) Serial.print(T)
	#define SDN(T) Serial.print(T)
	#define SD2(T,P) Serial.print(T,P)
	#define SDN2(T,P) Serial.print(T,P)

	#define SDP(T, V) SD(T); SD(V)
	#define SDPN(T, V) SD(T); SDN(V)
    #define SPHEX(X) phex(X)
    #define SPRINT(N)	SD("Sending [0A");for (int i=0;i<N;++i){ SPHEX(sendBuff[i]); }SDN("]");
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
	#define ESDN(T) Serial.print(T)
	#define ESD2(T,P) Serial.print(T,P)
	#define ESDN2(T,P) Serial.print(T,P)

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
    skipCharCnt = 0;
    address = 15;
}

void SPIMessage::init(uint8_t mode, Motor * mot_)
{
    pinMode(SS, INPUT);
	pinMode(SCK, INPUT);
	pinMode(MOSI, INPUT);
	pinMode(MISO, OUTPUT);

    for (uint8_t i = 0; i < maxBuff; ++i)
	{
		recvBuff[2*i] = 0;
		recvBuff[2*i+1] = 0;
		sendBuff[i] = 0;
	}

	sendBuff[0] = 3;
	sendBuff[1] = (ECHO_REP << 4) & 0xf0;
	sendBuff[2] = 0xf8; //0xf - adres nie wiadomo 0x80 odpowiedz z mini
	sendBuff[2] |= (uint8_t)mode & 0x07;
	CRC8 c;
    c.reset();
    c.add(sendBuff[1]);
	c.add(sendBuff[2]);
    sendBuff[3] = c.getCRC();

    SPCR |= _BV(SPE);                       //Turn on SPI in Slave Mode
  	SPI.attachInterrupt();   

    msg.init();
    mot = mot_;
}


void SPIMessage::proceed()
{
    ESD("1.Start Rec/Act:[");ESD(actProcess+1);ESD(",");ESD(recvPos);	ESD("]");
	ESDN("----------");

	while (recvPos > 0 && actProcess < recvPos)
	{
		ESD("2.Act/Rec:[");ESD(actProcess+1);ESD(",");ESD(recvPos);ESD("]=");EPHEX(recvBuff[actProcess]);ESDN("");

		if (skipCharCnt >= 0)
		{
			ESD(" Skip");ESD(" (");ESD(skipCharCnt);ESDN(")");

			if (skipCharCnt == 0)
			{
				SDN("END SKIP, BUSY OFF");

				sendPos = 0;
				msg.clear();
				setBusy(false);
			} 
			++actProcess;
			--skipCharCnt;
			continue;
		}

		ESDN(" Proc");

		if (!msg.add(recvBuff[actProcess++])) {
			continue;
		}

		SD("Mam wiadomosc: ");SD(msg.getMsgCmd());SDN(" . BUSY ON");
		setBusy(true);

		Result status = msg.parse();
		if (!status.ok)
		{
			SDN("CMD:Nie poprawna wiadomosc. BUSY OFF");
			msg.clear();
			setBusy(false);
			continue;
		}

        address = msg.getAddr();
		ESDPN("ADDR:", address);
		switch (msg.getMsgCmd()) {
		case LAST_REQ:
		{
    	    SDN(" CMD:LAST");
            SPRINT(20);
			skipCharCnt = 16;
			break;
		}

		case ECHO_REQ:
		{
			SDN(" CMD:REQUEST");
			echoRequestFun();
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
			setBusy(false);
            msg.clear();
			break;
		}

		case CONF_REQ:
		{
			SDN(" CMD:CONFIGURATION");
			configurationRequest(status);
			SDN("BUSY OFF");
			setBusy(false);
            msg.clear();
			break;
		}

		case MOVE_REQ:
		{
			SDN(" CMD:MOVE");
			moveRequest(status.data.move.isHome, status.data.move.position, status.data.move.speed);
			SDN("BUSY OFF");
			setBusy(false);
            msg.clear();
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

	ESD("---------");ESD("3. Act/Rec:[");ESD(actProcess);ESD(",");ESD(recvPos);ESD("]");ESD("END.LOOP");

	actProcess = 0;
	recvPos = 0;
	received = false;
}

void SPIMessage::echoRequestFun()
{
	CRC8 crc;
	crc.restart();
	sendBuff[0] = 3;
	sendBuff[1] = (ECHO_REP << 4) & 0xf0;
	sendBuff[2] = ((address << 4) & 0xf0) | 0x08;
	crc.add(sendBuff[1]);
	crc.add(sendBuff[2]);
	sendBuff[3] = crc.getCRC();

	SPRINT(4);
	sendPos = 0;
}

void SPIMessage::progressRequestFun()
{
	CRC8 crc;
	crc.restart();
	sendBuff[0] = 7;
	sendBuff[1] = ((PROGRESS_REP << 4) & 0xf0) | 0x04;
	sendBuff[2] = ((address << 4) & 0xf0) | 0x08;
	if (mot->isMove())
		sendBuff[2] |= 0x04;
	if (mot->isHomeMove())
		sendBuff[2] |= 0x02;
	crc.add(sendBuff[1]);
	crc.add(sendBuff[2]);
	int32_t pos = mot->getGlobalPos();
	SDPN("POS=", pos);
	sendBuff[3] = (pos >> 24) & 0xff;
	sendBuff[4] = (pos >> 16) & 0xff;
	sendBuff[5] = (pos >> 8) & 0xff;
	sendBuff[6] = (pos)&0xff;
	crc.add(sendBuff[3]);
	crc.add(sendBuff[4]);
	crc.add(sendBuff[5]);
	crc.add(sendBuff[6]);
	sendBuff[7] = crc.getCRC();
	sendPos = 0;

	SPRINT(8);
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
	sendBuff[0] = 3;
	sendBuff[1] = (CONF_REP << 4) & 0xf0;
	sendBuff[2] = ((address << 4) & 0xf0) | 0x08;
	crc.add(sendBuff[1]);
	crc.add(sendBuff[2]);
	sendBuff[3] = crc.getCRC();
	sendPos = 0;
    SPRINT(4);
	msg.clear();

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
	sendBuff[0] = 3;
	sendBuff[1] = (MOVE_REP << 4) & 0xf0;
	sendBuff[2] = ((address << 4) & 0xf0) | 0x08;
	if (isHome)
		sendBuff[2] += 1;
	crc.add(sendBuff[1]);
	crc.add(sendBuff[2]);
	sendBuff[3] = crc.getCRC();

	sendPos = 0;
    SPRINT(4);

	if (!wasMove)
		emptyMove(4);

	msg.clear();

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