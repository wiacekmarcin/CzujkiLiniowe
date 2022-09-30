#ifndef __PROTO_H__
#define __PROTO_H__

#include <Arduino.h>

typedef enum _MSGType
{
    ECHO_REQ = 0,
    ECHO_REP = 1,
    CONF_REQ = 2,
    CONF_REP = 3,
    MOVE_REQ = 4,
    MOVE_REP = 5,
    LAST_REQ = 0xe,
    LAST_REP = 0xf,
    INV_MSG
} Msgtype;

typedef struct _result {
    bool ok;
    union
    {
        /* data */
        struct {
            bool reverse;
            bool enableAlways;
            uint32_t maxStep;
            uint16_t delayImp;
            uint16_t baseSteps;
        } conf;

        struct {
            bool isHome;
            uint32_t steps;
        } move;
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

    Result parse();
    Msgtype getMsgCmd() { return cmdMsg; }
    void clear();
protected:
    
    Msgtype conv(uint8_t m);

private:
    uint32_t toNumber32(uint8_t n4, uint8_t n3, uint8_t n2, uint8_t n1);
    uint16_t toNumber16(uint8_t n2, uint8_t n1);

    bool startMsg;
    uint8_t lenMsg;
    Msgtype cmdMsg;
    uint8_t crcMsg;
    uint8_t addrMsg;
    uint8_t posData;

    static constexpr uint8_t restMsgCnt = 1 + 1 + 4 + 1; //LENCMD ADR LICZNIK CRC
    static constexpr uint8_t msgCnt = 15;
    static constexpr uint8_t maxBuff = restMsgCnt + msgCnt;
    uint8_t recvBuff[maxBuff];
    uint8_t sendBuff[maxBuff];
    uint8_t recvPos;
    uint8_t sendPos;
    uint8_t dataCmd[15];

    
    
};
#endif // __PROTO_H__