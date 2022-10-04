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
        ECHO_CLEAR_REQ = 15,
        ECHO_CLEAR_REP = 0,
        INV_MSG = 16,
    } MsgType;

    bool check(unsigned char c);
    void init();
    Work getStatusWork() const { return actWork; }
    void reset();

    
protected:

    bool isWork() { 
        return actWork != NOP; 
    }
protected:
    

    bool parseRozkaz();
    void sendMessage(uint8_t addr, uint8_t cmd, uint8_t *buf, uint8_t len);
    void sendError()
    
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
