#include "serialmessage.h"
#include "crc8.h"


#include <QDebug>

SerialMessage::SerialMessage()
{

}

SerialMessage::~SerialMessage()
{

}

QByteArray SerialMessage::welcomeMsg()
{
    return prepareMessage(WELCOME_MSG_REQ, 0, 0, NULL, 0);
}

QByteArray SerialMessage::configMotorMsg(short silnik, bool alwaysEnable, bool reverse, int maxSteps, int baseSteps, int delayStep)
{
    uint8_t addr = silnik & 0x0f;;
    uint8_t opt = 0;

    uint8_t tab[] = {
        (uint8_t) ((alwaysEnable ? 0x02 : 0x00) | (reverse ? 0x01 : 0x00)),
        (uint8_t) ((maxSteps >> 24) & 0xff),
        (uint8_t) ((maxSteps >> 16) & 0xff),
        (uint8_t) ((maxSteps >> 8) & 0xff),
        (uint8_t) (maxSteps  & 0xff),
        (uint8_t) ((baseSteps >> 8) & 0xff),
        (uint8_t) (baseSteps  & 0xff),
        (uint8_t) ((delayStep >> 8) & 0xff),
        (uint8_t) (delayStep  & 0xff)
    };

    return prepareMessage(CONF_REQ, addr, opt, tab, sizeof(tab)/sizeof(uint8_t));
}

QByteArray SerialMessage::setPositionHome(uint8_t addr)
{
    uint8_t tab[4] = { 0x00, 0x00, 0x00, 0x00};
    return prepareMessage(MOVE_REQ, addr, 0x01, tab, 4);
}

QByteArray SerialMessage::setPosition(uint8_t addr, const uint32_t x)
{
    uint8_t tab[4] = {
        (uint8_t) ((x >> 24) & 0xff),
        (uint8_t) ((x >> 16) & 0xff),
        (uint8_t) ((x >> 8) & 0xff),
        (uint8_t) (x & 0xff)
    };

    return prepareMessage(MOVE_REQ, addr, 0x00, tab, 4);
}
/*
QByteArray SerialMessage::resetSterownik(uint8_t addr)
{
    return prepareMessage(RESET_STER_REQ, addr, 0, NULL, 0);
}
*/

bool SerialMessage::checkHead(QByteArray &arr, uint8_t & addr, uint8_t & options, uint8_t & cmd, uint8_t & len,  QByteArray & data)
{
    if (arr.length() == 0) {
        m_parseReply = EMPTY_MESSAGE;
        return false;
    }

    cmd = (((unsigned char)arr[0]) >> 4) & 0x0f;
    len = ((unsigned char)arr[0]) & 0x0f;

    if (cmd == ECHO_CLEAR_REP && len == 0) {
        m_parseReply = CLR_MESSAGE;
        arr.front();
        return false;
    }

    if (arr.size() < len + 3) {
        m_parseReply = INPROGRESS_REPLY;
        return false;
    }

    QByteArray msg = arr.left(len + 3);
    arr.remove(0, len+3);

    addr = ((unsigned char)msg[1]) >> 4;
    options = ((unsigned char)msg[1]) & 0x0f;

    CRC8 crc;
    data = QByteArray(len, Qt::Uninitialized);


    int i = 0;
    for (i = 0; i < msg.length() - 1 && i < len + 2; i++) {
        crc.add(msg.at(i));
        if (i >= 2) {
            data[i-2] = msg.at(i);
        }
    }

    if (i-2 != len) {
        m_parseReply = INVALID_REPLY;
        return false;
    }

    unsigned short msgcrc = msg.back() & 0xff;

    if (crc.getCRC() != msgcrc) {
        m_parseReply = INVALID_REPLY;
        return false;
    }

    return true;
}

bool SerialMessage::parseCommand(QByteArray &arr)
{
    uint8_t cmd;
    uint8_t len;
    uint8_t addr;
    uint8_t options;
    QByteArray data;

    m_parseReply = INVALID_REPLY;
    if (!checkHead(arr, addr, options, cmd, len, data)) {
        return false;
    }

    switch (cmd) {
        case ECHO_CLEAR_REP:
            return true;

        case WELCOME_MSG_REP:
        {
            if (len != 15) {
                return false;
            }
            uint8_t wzorzec[15] = { 'C','Z','U','J','N','I','K','I','L','I','N','I','O','W','E'};
            for (int i = 0; i < 15; ++i) {
                if (wzorzec[i] != data[i]) {
                    return false;
                }
            }

            m_parseReply = WELCOME_REPLY;
            return true;
        }

        case CONF_REP:
        {
            silnik = addr;
            m_parseReply = ((options & 0x01) == 0x01) ? CONF_INT_REPLY : CONF_REPLY;
            return true;
        }
        
        default:
            return false;
    }

    return false;
}

QByteArray SerialMessage::prepareMessage(uint8_t cmd, uint8_t addr, uint8_t options, uint8_t tab[], uint8_t len)
{
    QByteArray arr(len+3,0);
    arr[0] = (unsigned char) ( (cmd << 4) | (len & 0x0f) );
    arr[1] = (unsigned char) ( (addr << 4) | (options & 0x0f) );
    CRC8 c;
    c.add(arr[0]);
    c.add(arr[1]);
    for (int i = 0; i < len; i++) {
        arr[i+2] = tab[i];
        c.add(tab[i]);
    }
    arr[len+2] = c.getCRC();

    QByteArray ret;
    ret.append(arr);
    return ret;
}

SerialMessage::ParseReply SerialMessage::getParseReply() const
{
    return m_parseReply;
}

uint32_t SerialMessage::getNumber(const QByteArray &data)
{
    return ((data[0] & 0xff) << 24) +  ((data[1] & 0xff) << 16) + ((data[2] & 0xff) << 8) + (data[3] & 0xff);
}

int SerialMessage::getSilnik() const
{
    return silnik;
}
