#include "protocol.hpp"
#include <Arduino.h>

#define DEBUG_SERIAL

MessageSerial::MessageSerial() 
{
    posCmd = 0;
    rozkaz = 0;
    dlugosc = 0;
    crc.restart();
}

void MessageSerial::init() 
{
    Serial.begin(115200); 
    reset();
}

void MessageSerial::reset()
{
    posCmd = 0;
    rozkaz = 0;
    dlugosc = 0;
    crc.restart();
}

bool MessageSerial::check(unsigned char c)
{
    data[posCmd++] = c;
    data[posCmd] = '\0';
#ifdef DEBUG_SERIAL        
    Serial1.print("NOWY ZNAK=");
    Serial1.println(c, HEX);
#endif    
    if (posCmd-1 == 0) {    
        crc.restart();
        crc.add(data[0]);
        rozkaz = data[0] >> 4;
        dlugosc = data[0] & 0x0f;
#ifdef DEBUG_SERIAL            
        Serial1.print("ROZKAZ=");
        Serial1.println(rozkaz,DEC);
        Serial1.print("LEN=");
        Serial1.println(dlugosc, DEC);
#endif
        if (rozkaz == ECHO_CLEAR_REQ && dlugosc == 0) {
            reset();
            return true;
        }
        return false;
    }

    if (posCmd-2 == 0) {
        address = (data[1] >> 4) & 0x0f;
        options = data[1] & 0x0f;
        crc.add(data[1]);
#ifdef DEBUG_SERIAL            
        Serial1.print("ADDRR=");
        Serial1.println(address);
#endif 
        return false;
    }
    
    if (posCmd == dlugosc + 3) {
        uint8_t c = crc.getCRC();
#ifdef DEBUG_SERIAL            
        Serial1.print("CRC=");
        Serial1.print(c,HEX);
        Serial1.print("==");
        Serial1.println(data[posCmd-1],HEX);
#endif        
        if (data[posCmd-1] == c) {
            posCmd = 0;
            bool r = parseRozkaz();
            if (!r) {
                reset();
                sendError("ZLY ROZKAZ");
#ifdef DEBUG_SERIAL                    
                Serial1.println("ZLY ROZKAZ");
#endif                
            }
            return r;
        }
        reset();
        posCmd = 0;
        sendError("ZLE CRC");
#ifdef DEBUG_SERIAL            
        Serial1.print("CRC FAILD");
#endif        
        return false;

    }

    crc.add(data[posCmd-1]);    
    
    
    if (posCmd == MAXLENPROTO) {
        posCmd = 0;
        sendError("ZBYT DUZA WIAD");
#ifdef DEBUG_SERIAL           
        Serial1.println("ZBYT DUZA WIADOMOSC");
#endif        
        return false;    
    }
    return false;
}

void MessageSerial::sendMessage(uint8_t cmd, uint8_t addr, uint8_t options, uint8_t* buf, uint8_t len)
{
    if (len > 15)
        return;
    
    uint8_t sendData[MAXLENPROTO] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    if (len > 0)
        memcpy(sendData+2, buf, len);
    sendData[0] = (cmd << 4) | len ;
    sendData[1] = (addr << 4) | (options &0x0f);
    crc.restart();
    crc.add(sendData, len+2);
    sendData[len+2] = crc.getCRC();
    Serial.write(sendData, len+3);
}

bool MessageSerial::parseRozkaz()
{
    
    switch(rozkaz) {
        case WELCOME_MSG_REQ:   //get info 
        {
            actWork = WELCOME_MSG;
            return true;
        }

        case ECHO_CLEAR_REQ:
        {
            Serial.write(ECHO_CLEAR_REP);
            actWork = NOP;
            return true;
        }

        case CONF_REQ:
        {
            Serial1.println("konf");
            Serial1.println(address, DEC);        
            if (address == 0) {
                Serial1.println("konf local");        
                actWork = CONFIGURATION_LOCAL;
            }
            else if (address < 10) {
                Serial1.print("konf - ");
                Serial1.println(address, DEC);
                actWork = CONFIGURATION;
            }
            else
                actWork = NOP;
            return true;
        }

        case MOVE_REQ:
        {
            actWork = MOVE_REQUEST;
            return true;
        }
        default:
            break;

    }
    rozkaz = 0;
    return false;
}

void MessageSerial::sendWelcomeMsg()
{
                                //1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  
    uint8_t sendData[15] = {'C','Z','U','J','N','I','K','I','L','I','N','I','O','W', 'E'};
    sendMessage(WELCOME_MSG_REP, 0x00, 0x00, sendData, 15);
}

void MessageSerial::sendError(const char * err)
{
    uint8_t sendData[15];
    uint8_t i;
    for (i = 0; i < 15 && err[i]; ++i) {
        sendData[i] = err[i];
    }
    sendMessage(ECHO_CLEAR_REP, 0x00, 0x01, sendData, i);
}

void MessageSerial::sendConfigDoneMsg(uint8_t addr)
{
    sendMessage(CONF_REP, addr, 0x00, nullptr, 0);
}

void  MessageSerial::sendMeasuremnt(/*uint8_t t1[4], uint8_t t2[4]*/)
{
    uint8_t sendData[] = {'0', '0', '1', '.', '0', ';', '0', '0', '2', '.', '0', ';'};
    sendMessage(MEASURENT_REP, 0x10, 0x00, sendData, sizeof(sendData)/sizeof(uint8_t));
}


void MessageSerial::copyCmd(uint8_t *tab, uint8_t len)
{
    for (uint8_t i = 0; i < len; ++i)
    {
        tab[i] = data[i];
    }
}