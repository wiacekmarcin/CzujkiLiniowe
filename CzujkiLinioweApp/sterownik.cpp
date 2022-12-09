#include "sterownik.h"
#include <QTime>
#include <QThread>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>

#define DEBUGSER(X) debugFun(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))

Sterownik::Sterownik(Ustawienia *u, QObject *parent)
    : QObject(parent),
      m_portName(""), m_portNr(-1),
      m_connected(false),  m_writer(this), m_reader(this),
      ust(u)
{


    m_portNr = -1;
#ifdef SERIALLINUX
     m_serialPort = new QSerialPort();
#endif

}

Sterownik::~Sterownik()
{
    setStop();
    m_writer.wait();
    m_reader.wait();
}

void Sterownik::setThread(QThread *thrW, QThread *thrR)
{
    m_writer.moveToThread(thrW);
    m_writer.start();
    m_writer.command(SterownikWriter::IDLE, QByteArray());

    m_reader.moveToThread(thrR);
    //start przy connect
}

void Sterownik::setStop()
{
    m_writer.setReset();
    m_writer.setStop();
    m_reader.setStop();
}

void Sterownik::connectToDevice()
{
    if (connected()) {
        emit kontrolerConfigured(true, ALL_OK);
    } else {
        m_writer.command(SterownikWriter::CONNECT, QByteArray());
    }
}

void Sterownik::setParams()
{


    if (connected()) {
        //insertParams();
    }
}

void Sterownik::setPositionSilnik(int silnik, bool home, unsigned int steps)
{
    if (!connected())
        m_writer.command(SterownikWriter::CONNECT, QByteArray());

    if (home)
        m_writer.command(SterownikWriter::SET_HOME,
                         SerialMessage::setPositionHome(silnik));
    else
        m_writer.command(SterownikWriter::SET_POSITION,
                         SerialMessage::setPosition(silnik,  steps));
}

bool Sterownik::configureDevice(bool wlcmmsg)
{
    if (wlcmmsg) {
        if (!write(SerialMessage::welcomeMsg(), 200))
            return false;

        auto reply = read(1000);
        SerialMessage msg;
        msg.parseCommand(reply);

        if (msg.getParseReply() != SerialMessage::WELCOME_REPLY) {
            emit kontrolerConfigured(false, IDENT_FAILD);
            return false;
        }

        emit kontrolerConfigured(false, IDENT_OK);
    }

    for (short i = 1 ; i < 10; ++i) {
        QByteArray msgBA = SerialMessage::configMotorMsg(
            i, true, ust->getMotorOdwrocObroty(i),
            ust->getMotorMaksIloscImp(i), 0,
            ust->getMotorCzasMiedzyImp(i));

        if (!write(msgBA, 500)) {
            emit kontrolerConfigured(false, PARAMS_FAILD);
            return false;
        }

        auto reply = read(1000);
        SerialMessage msg;
        msg.parseCommand(reply);

        if (msg.getParseReply() != SerialMessage::CONF_REPLY && msg.getSilnik() != i) {
            emit kontrolerConfigured(false, PARAMS_FAILD);
            return false;
        }
    }

    QByteArray msgBA = SerialMessage::configKontrolerMsg();
    if (!write(msgBA, 500)) {
        emit kontrolerConfigured(false, PARAMS_FAILD);
        return false;
    }

    auto reply = read(1000);
    SerialMessage msg;
    msg.parseCommand(reply);

    if (msg.getParseReply() != SerialMessage::CONF_REPLY && msg.isKontroler()) {
        emit kontrolerConfigured(false, PARAMS_FAILD);
        return false;
    }

    emit kontrolerConfigured(true, PARAMS_OK);
    return true;
}

bool Sterownik::connected()
{
    const QMutexLocker locker(&connMutex);
    return m_connected;
}

void Sterownik::setConnected(bool connected)
{
    const QMutexLocker locker(&connMutex);
    m_connected = connected;
}

void Sterownik::closeDeviceJob()
{
    DEBUGSER("CLOSING DEVICE");
    //setStop();
    m_reader.setStop();
    m_writer.setStop();
#ifdef SERIALLINUX
    m_serialPort->close();
#else
    RS232_CloseComport(m_portNr);
    m_portNr = -1;
#endif
    setConnected(false);
    emit kontrolerConfigured(false, CLOSE);
    DEBUGSER("CLOSE DEVICE");
}

bool Sterownik::openDevice()
{
#ifdef SERIALLINUX
    m_serialPort->setPort(QSerialPortInfo(m_portName));

    emit deviceName(m_serialPort->portName());

    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->clear();
    m_serialPort->clearError();
    m_serialPort->flush();
    m_serialPort->reset();
    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        emit error(QString(QObject::tr("Nie mozna otworzyc urzadzenia %1, error  %2")).arg(m_portName).
                   arg(m_serialPort->errorString()));
        emit kontrolerConfigured(false, NO_OPEN);
        return false;
    }
    m_serialPort->clear();
    m_serialPort->clearError();
    m_serialPort->flush();
    m_serialPort->reset();
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setReadBufferSize(20);

    QThread::currentThread()->sleep(2);

    emit kontrolerConfigured(false, OPEN);



    QByteArray startBuf(18, '\0');
    DEBUGSER(QString("[%1]").arg(startBuf.toHex(' ').data()));
    write(startBuf, 2500);
    read(100);
    return true;
#else
    DEBUGSER(QString("Otwieram urządzenia %1").arg(m_portName));
    char mode[]={'8','O','1',0};
    if (RS232_OpenComport(m_portNr, 115200, mode, 0)) {
        DEBUGSER(QString("Error open Port"));
        emit kontrolerConfigured(false, NO_OPEN);
        return false;
    }

    RS232_flushRXTX(m_portNr);

    const unsigned char startBuf[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int rs = RS232_SendBuf(m_portNr, (unsigned char*)startBuf, 16);

    if (rs <= 0) {
        RS232_CloseComport(m_portNr);
        m_portNr = -1;
        emit kontrolerConfigured(false, NO_READ);
        return false;
    }
    QThread::currentThread()->msleep(200);
    unsigned char recvBuf[100];
    int recv = RS232_PollComport(m_portNr, recvBuf, 1000);
    (void)recv;

    emit kontrolerConfigured(false, OPEN);
    return true;
#endif
}

void Sterownik::setReset()
{
    /*
    if (connected()) {
        m_writer.command(SterownikWriter::RESET);
        m_writer.command(SterownikWriter::SET_PARAMS);
    }
    */
}

bool Sterownik::write(const QByteArray &currentRequest, int currentWaitWriteTimeout)
{

#ifdef SERIALLINUX
    if (currentRequest.size() > 0) {

        int sendBytes = m_serialPort->write(currentRequest);
        if (!m_serialPort->waitForBytesWritten(currentWaitWriteTimeout)) {
            DEBUGSER(QString("Timeout Write %1").arg(currentWaitWriteTimeout));
            emit error(QString("Timeout Write"));
            return false;
        }
        DEBUGSER(QString("Write %1 bytes [%2]").arg(sendBytes).arg(currentRequest.toHex(' ').constData()));
    }
    return true;
#else

    if (currentRequest.size() > 0)
    {

        DEBUGSER(QString("write [%1]").arg(currentRequest.toHex().constData()));
        QElapsedTimer timer;
        timer.start();
        int sendBytes = RS232_SendBuf(m_portNr, (unsigned char*)currentRequest.constData(), currentRequest.size());
        if (sendBytes == 0) {
            DEBUGSER(QString("Write timeout %1").arg(sendBytes).arg(timer.elapsed()));
            return false;
        }
        DEBUGSER(QString("write %1 bytes [%2 ms]").arg(sendBytes).arg(timer.elapsed()));
        QThread::currentThread()->msleep(currentWaitWriteTimeout);
    }
    return true;
#endif
}

QByteArray Sterownik::read(int currentWaitReadTimeout)
{
    QByteArray responseData ;
#ifdef SERIALLINUX
    if (m_serialPort->waitForReadyRead(currentWaitReadTimeout)) {
        responseData = m_serialPort->readAll();
        while (m_serialPort->waitForReadyRead(10) && currentWaitReadTimeout > 0) {
            QByteArray r = m_serialPort->readAll();
            if (r.size() == 0)
                break;
            responseData += r;
            currentWaitReadTimeout -= 10;
        }
    }
    DEBUGSER(QString("READ %1 bytes [%2]").arg(responseData.size()).arg(responseData.toHex(' ').constData()));
#else
    unsigned char recvBuffor[20];
    //int readTimeout = currentReadWaitTimeout;
    int rc = 0;
    QElapsedTimer timer;
    timer.start();
    unsigned short len;

    //1 znak
    do {
        rc = RS232_PollComport(m_portNr, recvBuffor, 1);
        if (rc > 0) {
            DEBUGSER(QString("recv %1 bytes").arg(rc));
        }
        if (rc == 0) {
            readTimeout -= 50;
            QThread::currentThread()->msleep(50);
        }
        if (recvBuffor[0] == 0xf0)
            continue;
    } while (rc == 0 && readTimeout > 0);
    QByteArray responseData;
    if (rc == 1) {
        responseData.append((const char*)recvBuffor, rc);
        len = recvBuffor[0] & 0x0f;
    }
    else {
        emit error(QString("Timeout"));
        DEBUGSER(QString("Timeout Read %1").arg(currentReadWaitTimeout));
        msg.setTimeoutReply(false);
        return msg;
    }

    //reszta znakow
    do {
        rc = RS232_PollComport(m_portNr, recvBuffor, len+1);
        if (rc > 0) {
            DEBUGSER(QString("recv %1 bytes").arg(rc+1));
        }
        responseData.append((const char*)recvBuffor, rc);
        if (responseData.size() == len + 2)
            break;

        readTimeout -= 50;
        QThread::currentThread()->msleep(50);

    } while(readTimeout > 0);
    unsigned long ms = timer.elapsed();

    if (readTimeout <= 0) {
        emit error(QString("Timeout"));
        DEBUGSER(QString("Timeout Read %1").arg(currentReadWaitTimeout));
        msg.setTimeoutReply(false);
        return msg;
    }
    responseData.append((const char*)recvBuffor, rc);
    DEBUGSER(QString("read [%1] [%2 ms]").arg(responseData.toHex().constData()).arg(ms));
    return parseMessage(responseData);

#endif
    return responseData;
}

SerialMessage Sterownik::parseMessage(QByteArray &reply)
{
    SerialMessage msg;
    if (!msg.parseCommand(reply)) {
        DEBUGSER(QString("Parse Msg faild %1").arg(msg.getParseReply()));
        return msg;
    }
    DEBUGSER(QString("Parse Msg success %1").arg(msg.getParseReply()));
    switch(msg.getParseReply())
    {
    case SerialMessage::INVALID_REPLY:
    case SerialMessage::CLR_MESSAGE:
    case SerialMessage::EMPTY_MESSAGE:
    case SerialMessage::INPROGRESS_REPLY:
    case SerialMessage::WELCOME_REPLY:
    default:
        break;

    case SerialMessage::CONF_REPLY:
        emit setParamsDone(msg.getSilnik(), true, false);
        break;

    case SerialMessage::MOVEHOME_REPLY:
        emit setPositionDone(true, true, 0);
        break;

    case SerialMessage::POSITION_REPLY:
        emit setPositionDone(false, true, msg.getSteps());
        break;

    case SerialMessage::RESET_REPLY:
        break;

    case SerialMessage::CONF_INT_REPLY:
        emit setParamsDone(msg.getSilnik(), true, true);
        break;
    }
    return msg;
}

void Sterownik::closeDevice(bool waitForDone)
{
    DEBUGSER(QString("close device %1").arg(waitForDone));
    if (waitForDone) {
        m_writer.command(SterownikWriter::DISCONNECT, QByteArray());
        m_reader.setStop();
    } else {
        m_writer.setReset();
        closeDeviceJob();
    }
}

void Sterownik::debugFun(const QString &log)
{
    emit debug(log);
}

void Sterownik::connectToSerialJob()
{
    if (!connected()) {
        QString description;
        QString manufacturer;
        QString serialNumber;

        QString systemLocation = "";
        bool findDevice = false;

        DEBUGSER(QString("Szukam urządzenia"));

        const auto serialPortInfos = QSerialPortInfo::availablePorts();

        for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
            description = serialPortInfo.description();
            manufacturer = serialPortInfo.manufacturer();
            serialNumber = serialPortInfo.serialNumber();
            DEBUGSER(QString("Znaleziono : [%1] DESC ").arg(serialPortInfo.portName()) + description + QString(" MENU ") + manufacturer + QString(" SERIAL") + serialNumber);

            if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
                auto vendorId = serialPortInfo.vendorIdentifier();
                auto productId = serialPortInfo.productIdentifier();
                DEBUGSER(QString("Znaleziono kandydata %1 %2").arg(vendorId).arg(productId));
                if (vendorId == 6790 && productId == 29987 /* && serialNumber == serialNumberKontroler */) {
                    m_portName = serialPortInfo.portName();
                    emit deviceName(m_portName);
                    emit kontrolerConfigured(false, FOUND);
                    systemLocation = serialPortInfo.systemLocation();
                    DEBUGSER(QString("Znaleziono urządzenie"));
                    findDevice = true;
                    //break;
                }
            }
        }
        if (!findDevice) {
            emit kontrolerConfigured(false, NO_FOUND);
            return;
        }

    #ifndef SERIALLINUX
        m_portNr = -1;
        //GetComPortUsb(bufPortName,"1B4F","9206");
        m_portNr = RS232_GetPortnr(m_portName.toStdString().c_str());

        if (m_portNr == -1)
            return;
    #endif
        DEBUGSER("openDevice");
        setConnected(openDevice());
    }

    if (connected()) {
        setConnected(configureDevice(true));
    }

    if (!connected()) {
        closeDevice(false);
    } else {
       m_reader.start();
    }

    if (emitSignal) emit kontrolerConfigured(true, ALL_OK);
}



