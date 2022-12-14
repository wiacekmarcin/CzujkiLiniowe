#ifndef __PROTO_H__
#define __PROTO_H__

#include <Arduino.h>

typedef enum _MSGType
{
    CZUJKA_ZW_REP       = 1,
    WELCOME_MSG_REQ     = 2,
    WELCOME_MSG_REP     = 3,
    ECHO_REQ            = 4,
    ECHO_REP            = 5,
    PROGRESS_REQ        = 6,
    PROGRESS_REP        = 7,
    CONF_REQ            = 8,
    CONF_REP            = 9,
    MOVE_REQ            = 10, //a
    MOVE_REP            = 11, //b
    RESET_REQ           = 14,
    ECHO_CLEAR_REQ      = 0,
    ECHO_CLEAR_REP      = 15,
    INV_MSG             = 16,
} Msgtype;

typedef struct _result {
    bool ok;
    union
    {
        /* data */
        struct {
            bool reverse;
            uint32_t maxStep;
            uint32_t baseSteps;
            uint32_t middleSteps;
        } conf;

        struct {
            bool isHome;
            int32_t position;
            uint32_t speed;
        } move;

        struct {
            bool enableOff;
        } reset;
    } data;
} Result;



class Message
{
public:
    
    Message();

    void init();

    bool add(uint8_t b);

    void addSteps(bool success, uint32_t steps);

    void copy(volatile uint8_t * tab);

    uint8_t getAddr() { return addrMsg; };

    Result parse();
    Msgtype getMsgCmd() { return cmdMsg; }
    void clear();
protected:
    
    Msgtype conv(uint8_t m);

private:
    uint32_t toNumber32(uint8_t n4, uint8_t n3, uint8_t n2, uint8_t n1);
    uint32_t toNumber24(uint8_t n3, uint8_t n2, uint8_t n1);
    uint16_t toNumber16(uint8_t n2, uint8_t n1);

    bool startMsg;
    uint8_t lenMsg;
    Msgtype cmdMsg;
    uint8_t crcMsg;
    uint8_t addrMsg;
    uint8_t options;
    uint8_t posData;

    static constexpr uint8_t restMsgCnt = 1 + 1 + 1; //LENCMD ADR CRC
    static constexpr uint8_t msgCnt = 15;
    static constexpr uint8_t maxBuff = restMsgCnt + msgCnt;
    uint8_t recvBuff[maxBuff];
    uint8_t recvPos;
    uint8_t sendPos;
    uint8_t dataCmd[15];

  
};
#endif // __PROTO_H__
