#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#define MAXLENPROTO 18
#include "crc8.hpp"


class MessageSerial 
{
    public:
    MessageSerial();

    typedef enum _work {
        NOP,
        WELCOME_MSG,
        CONFIGURATION,
        CONFIGURATION_LOCAL,
    } Work;

    typedef enum _msg {
        WELCOME_MSG_REQ = 1,
        WELCOME_MSG_REP = 2,

        CONF_REQ = 8,
        CONF_REP = 9,
        MOVE_REQ = 10,
        MOVE_REP = 11,
        LAST_REQ = 12,
        LAST_REP = 13,
        MEASURENT_REP = 14,
        ECHO_CLEAR_REQ = 0,
        ECHO_CLEAR_REP = 15,
        INV_MSG = 16,
    } MsgType;

    bool check(unsigned char c);
    void init();
    Work getStatusWork() const { return actWork; }
    void reset();

    void sendWelcomeMsg();
    void sendConfigDoneMsg(uint8_t addr);
    void sendMeasuremnt(/*uint8_t t1[4], uint8_t t2[4]*/);

    uint8_t getAddress() { return address ; }
    
    void copyCmd(uint8_t *tab, uint8_t len);
protected:

    bool isWork() { 
        return actWork != NOP; 
    }
protected:
    

    bool parseRozkaz();
    void sendMessage(uint8_t cmd, uint8_t addr, uint8_t options, uint8_t *buf, uint8_t len);
    void sendError(const char * addr);
    
    private :
    // rozkaz/dlugosc | 1 byte | 2 byte | 3 byte | 4 byte | crc
    uint8_t data[MAXLENPROTO + 4];

    Work actWork;
    uint8_t posCmd;
    uint8_t rozkaz;
    uint8_t dlugosc;
    uint8_t address;
    CRC8 crc;

};

    

    

#endif /* PROTOCOL_HPP */
