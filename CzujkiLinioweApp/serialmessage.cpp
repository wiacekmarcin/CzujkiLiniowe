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

QByteArray SerialMessage::configMotorMsg(short silnik, bool reverse, int maxSteps, int baseSteps, int middleStep)
{
    uint8_t addr = silnik & 0x0f;;
    uint8_t opt = reverse ? 1 : 0;

    uint8_t tab[] = {
        (uint8_t) ((maxSteps >> 24) & 0xff),
        (uint8_t) ((maxSteps >> 16) & 0xff),
        (uint8_t) ((maxSteps >> 8) & 0xff),
        (uint8_t) (maxSteps  & 0xff),
        (uint8_t) ((baseSteps >> 24) & 0xff),
        (uint8_t) ((baseSteps >> 16) & 0xff),
        (uint8_t) ((baseSteps >> 8) & 0xff),
        (uint8_t) (baseSteps  & 0xff),
        (uint8_t) ((middleStep >> 24) & 0xff),
        (uint8_t) ((middleStep >> 16) & 0xff),
        (uint8_t) ((middleStep >> 8) & 0xff),
        (uint8_t) (middleStep  & 0xff),
    };

    return prepareMessage(CONF_REQ, addr, opt, tab, sizeof(tab)/sizeof(uint8_t));
}

QByteArray SerialMessage::configKontrolerMsg()
{
    return prepareMessage(CONF_REQ, 0, 0, nullptr, 0);
}

QByteArray SerialMessage::setPositionHome(uint8_t addr, uint32_t imp)
{
    uint8_t tab[8] = { 0x00, 0x00, 0x00, 0x00,
                       (uint8_t) ((imp >> 24) & 0xff),
                       (uint8_t) ((imp >> 16) & 0xff),
                       (uint8_t) ((imp >> 8) & 0xff),
                       (uint8_t) (imp & 0xff)};
    return prepareMessage(MOVE_REQ, addr, 0x01, tab, 8);
}

QByteArray SerialMessage::setPosition(uint8_t addr, const uint32_t x, uint32_t imp)
{
    uint8_t tab[8] = {
        (uint8_t) ((x >> 24) & 0xff),
        (uint8_t) ((x >> 16) & 0xff),
        (uint8_t) ((x >> 8) & 0xff),
        (uint8_t) (x & 0xff),
        (uint8_t) ((imp >> 24) & 0xff),
        (uint8_t) ((imp >> 16) & 0xff),
        (uint8_t) ((imp >> 8) & 0xff),
        (uint8_t) (imp & 0xff)
    };

    return prepareMessage(MOVE_REQ, addr, 0x00, tab, 8);
}

QByteArray SerialMessage::resetSilniki()
{
    return prepareMessage(RESET_REQ, addrKontrolera, 0, nullptr, 0);
}

QByteArray SerialMessage::stopSilnik(short nrSilnik)
{
    return prepareMessage(RESET_REQ, nrSilnik, 0, nullptr, 0);
}

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
        //arr.front();
        arr.remove(0,1);
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
        errStr = QString("Błędna długość wiadomości %1 != %2").arg(i-2).arg(len);
        m_parseReply = INVALID_REPLY;
        return false;
    }

    //unsigned short msgcrc = msg.back() & 0xff;
    unsigned short msgcrc = msg.at(msg.size()-1) & 0xff;

    if (crc.getCRC() != msgcrc) {
        errStr = QString("Błędne crc %1 != %2").arg((short)crc.getCRC(), 2, 16).arg((short)msgcrc, 2, 16);
        m_parseReply = INVALID_REPLY;
        return false;
    }

    return true;
}

bool SerialMessage::getErrMove() const
{
    return errMove;
}

bool SerialMessage::getStartMove() const
{
    return startMove;
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
            qDebug() << "ECHO_CLEAR_REP";
            return true;
        case CZUJKA_ZW_REP:
            m_parseReply = CZUJKA_ZW_REPLY;
            return true;
        case WELCOME_MSG_REP:
        {
            qDebug() << "WELCOME_MSG_REP";
            if (len != 15) {
                return false;
            }
            uint8_t wzorzec[15] = { 'C','Z','U','J','N','I','K','I','L','I','N','I','O','W','E'};
            for (int i = 0; i < 15; ++i) {
                if (wzorzec[i] != data[i]) {
                    m_parseReply = INVALID_REPLY;
                    return false;
                }
            }

            m_parseReply = WELCOME_REPLY;
            return true;
        }

        case CONF_REP:
        {
            silnik = addr;
            if (silnik == 0) {
                active[0] = (options & 0x08) == 0x08;
                connected[0] = (options & 0x04) == 0x04;
                pinsOK[0] = (options & 0x03) == 0x03;

                m_parseReply = CONF_MEGA_REPLY;
                for (short s = 1; s < 10; ++s) {
                    active[s] = (data[s-1] & 0x08) == 0x08;
                    connected[s] = (data[s-1] & 0x04) == 0x04;
                    pinsOK[s] = (data[s-1] & 0x03) == 0x03;
                    replyConf[s] = false;
                }
                qDebug() << "CONF_MSG_REP addr" << addr << "opt =" << options;
                return true;
            } else if (silnik < 10) {
                replyConf[silnik] = (options & 0x0C) == 0x0C;
                m_parseReply = CONF_REPLY;
            }
            return true;
        }

        case MOVE_REP:
        {
        silnik = addr;

        if (silnik < 1 || silnik > 9) {
            qDebug() << "Nie poprawna wiadomosc";
            m_parseReply = INVALID_REPLY;
            return false;
        }
        bool homeRet = (options & 0x01) == 0x01;
        if (homeRet)
            m_parseReply = MOVEHOME_REPLY;
        else
            m_parseReply = POSITION_REPLY;

        interMove = (options & 0x08) == 0x08;
        startMove = (options & 0x02) == 0x02;
        errMove = (options & 0x04) == 0x04;

        if (data.size() >= 4) {
            steps = getNumber(data);
        } else
            steps = 0;
        return true;
        }

        case PROGRESS_REP:
        {
        silnik = addr;

        if (!(options & 0x08) || silnik < 1 || silnik > 9) {
            qDebug() << "Nie poprawna wiadomosc";
            m_parseReply = INVALID_REPLY;
            return false;
        }
        steps = getNumber(data);
        m_parseReply = PROGRESS_REPLY;
        return true;
        }
        default: {
            qDebug() << "UNKNOWN cmd=" << cmd;
            return false;
        }
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

bool SerialMessage::getInterMove() const
{
    return interMove;
}

const QString &SerialMessage::getError() const
{
    return errStr;
}

bool SerialMessage::getActive(short nr) const
{
    return active[nr];
}

bool SerialMessage::getConnected(short nr) const
{
    return connected[nr];
}

bool SerialMessage::getPinsOk(short nr) const
{
    return pinsOK[nr];
}

bool SerialMessage::getReplyConf(short nr) const
{
    return replyConf[nr];
}

unsigned int SerialMessage::getSteps() const
{
    return steps;
}

int SerialMessage::getSilnik() const
{
    return silnik;
}

