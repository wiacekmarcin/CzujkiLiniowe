#include "protocol.hpp"
#include <Arduino.h>

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
}

bool MessageSerial::check(unsigned char c)
{
    data[posCmd++] = c;
    data[posCmd] = '\0';
#ifdef DEBUG_SERIAL        
    Serial.print("NOWY ZNAK=");
    Serial.println(c, HEX);
#endif    
    if (posCmd-1 == 0) {    
        crc.restart();
        crc.add(data[0]);
        rozkaz = data[0] >> 4;
        dlugosc = data[0] & 0x0f;
#ifdef DEBUG_SERIAL            
        Serial.print("ROZKAZ=");
        Serial.println(rozkaz,DEC);
        Serial.print("LEN=");
        Serial.println(dlugosc, DEC);
#endif        
        return false;
    }
    if (posCmd-1 == 0) {
        crc.add(data[0]);
        address = (data[1] >> 4) & 0x0f;
        #ifdef DEBUG_SERIAL            
        Serial.print("ADDRR=");
        Serial.println(address);
#endif 
        return false;
    }
    
    if (posCmd == dlugosc + 3) {
        uint8_t c = crc.getCRC();
#ifdef DEBUG_SERIAL            
        Serial.print("CRC=");
        Serial.print(c,HEX);
        Serial.print("==");
        Serial.println(data[posCmd-1],HEX);
#endif        
        if (data[posCmd-1] == c) {
            posCmd = 0;
            bool r = parseRozkaz();
            if (!r) {
                sendError("ZLY ROZKAZ");
#ifdef DEBUG_SERIAL                    
                Serial.println("ZLY ROZKAZ");
#endif                
            }
            return r;
        }
        posCmd = 0;
        sendError("ZLE CRC");
#ifdef DEBUG_SERIAL            
        Serial.print("CRC FAILD");
#endif        
        return false;

    }

    crc.add(data[posCmd-1]);    
    
    
    if (posCmd == MAXLENPROTO) {
        posCmd = 0;
        sendError("ZBYT DUZA WIAD");
#ifdef DEBUG_SERIAL           
        Serial.println("ZBYT DUZA WIADOMOSC");
#endif        
        return false;    
    }
    return false;
}

void MessageSerial::sendMessage(uint8_t addr, uint8_t cmd, uint8_t* buf, uint8_t len)
{
    if (len > 15)
        return;
    
    uint8_t sendData[MAXLENPROTO] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    if (len > 0)
        memcpy(sendData+2, buf, len);
    sendData[0] = cmd << 4 | len ;
    sendData[1] = addr << 4;
    crc.restart();
    crc.add(sendData, len+2);
    sendData[len+2] = crc.getCRC();
    Serial.write(sendData, len+3);
}

bool MessageSerial::parseRozkaz()
{
    
    switch(rozkaz) {
        case WELCOME_REQ:   //get info 
        {                         
            actWork = WELCOME_MSG;
            return true;
        }

        case POSITION_REQ: 
        {
            Serial1.write(data, dlugosc+2);
            actWork=NOP;
            return true;
        }
        case MOVEHOME_REQ:
        {
            Serial1.write(data, dlugosc+2);
            actWork=NOP;
            return true;
        }
        case SET_PARAM_REQ:
        {
            Serial1.write(data, dlugosc+2);
            actWork=NOP;
            return true;
        }
        case NOP_MSG:
        {
            Serial.write(0xF0);
            return true;
        }
        case MEASVALUE_REQ:
        {
            actWork = GET_RADIOVAL;
            return true;
        }
        case MEASUNIT_REQ:
        {
            actWork=NOP;
            return true;
        }
        case RESET_STER_REQ:
        {
            digitalWrite(RESET_NANO, LOW);
            delay(1000);
            digitalWrite(RESET_NANO, HIGH);
            delay(2000);
            sendMessage(RESET_STER_REP, nullptr, 0);  
            actWork=NOP;
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
    sendMessage(WELCOME_REP, sendData, 15);
}

