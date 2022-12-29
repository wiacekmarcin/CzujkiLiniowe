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
     m_serialPort = nullptr;
#endif

}

Sterownik::~Sterownik()
{
    setStop();
    m_writer.wait();
    m_reader.wait();
#ifdef SERIALLINUX
    if (m_serialPort && m_serialPort->isOpen()) {
        m_serialPort->close();
        delete m_serialPort;
        m_serialPort = nullptr;
    }
#else
    RS232_CloseComport(m_portNr);
    m_portNr = -1;
#endif
    setConnected(false);
}

void Sterownik::setThread(QThread *thrW, QThread *thrR)
{
    m_writer.moveToThread(thrW);
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
        emit kontrolerConfigured(ALL_OK);
    } else {
        m_writer.setReset();
        m_writer.setStart();
        m_writer.start();
        m_writer.command(SterownikWriter::CONNECT, QByteArray());
    }
}

void Sterownik::disconnectDevice()
{
    if (connected()) {
        m_writer.command(SterownikWriter::DISCONNECT, QByteArray());
    } else {
        emit kontrolerConfigured(CLOSE);
    }
}

void Sterownik::setParams()
{


    if (connected()) {
        //insertParams();
    }
}

void Sterownik::setPositionSilnik(int silnik, bool home, unsigned int steps, unsigned int impTime)
{
    if (!connected())
        m_writer.command(SterownikWriter::CONNECT, QByteArray());

    if (home)
        m_writer.command(SterownikWriter::SET_HOME,
                         SerialMessage::setPositionHome(silnik, impTime));
    else
        m_writer.command(SterownikWriter::SET_POSITION,
                         SerialMessage::setPosition(silnik,  steps, impTime));
}

bool Sterownik::configureDevice(bool wlcmmsg)
{
    if (wlcmmsg) {
        if (!write(SerialMessage::welcomeMsg(), 200))
            return false;

        auto reply = read(200);
        SerialMessage msg;
        msg.parseCommand(reply);

        if (msg.getParseReply() != SerialMessage::WELCOME_REPLY) {
            emit error(QString("Invalid Welcome Msg"));
            emit kontrolerConfigured(IDENT_FAILD);
            return false;
        }

        emit kontrolerConfigured(IDENT_OK);
    }

    for (short i = 1 ; i <= 9; ++i) {
        QByteArray msgBA = SerialMessage::configMotorMsg(
            i, ust->getMotorOdwrocObroty(i),
            ust->getMotorMaksIloscImp(i), ust->getMotorIloscImpBaza(i),
            ust->getMotorIloscImpSrodek(i));

        if (!write(msgBA, 500)) {
            emit error(QString("Nie udalo sie zapisac konfiguracji dla %1").arg(i));
            emit kontrolerConfigured(PARAMS_FAILD);
            return false;
        }
        QThread::currentThread()->msleep(100);

        /*
        auto reply = read(100);
        SerialMessage msg;
        msg.parseCommand(reply);

        if (msg.getParseReply() != SerialMessage::CONF_REPLY && msg.getSilnik() != i) {
            emit error(QString("Bledna odpowiedz dla  %1").arg(i));
            emit kontrolerConfigured(PARAMS_FAILD);
            return false;
        }
        emit zdarzenieSilnik(i, msg.getActive() ? M_ACTIVE : M_NOACTIVE);
        */
    }

    QByteArray msgBA = SerialMessage::configKontrolerMsg();
    if (!write(msgBA, 500)) {
        emit error(QString("Nie mozna zapisac konfiguracji dla kontrolera"));
        emit kontrolerConfigured(PARAMS_FAILD);
        return false;
    }

    auto reply = read(100);
    SerialMessage msg;
    msg.parseCommand(reply);

    if (msg.getParseReply() != SerialMessage::CONF_REPLY && !msg.isKontroler()) {
        emit error(QString("Bledna odpowiedz dla kontrolera"));
        emit kontrolerConfigured(PARAMS_FAILD);
        return false;
    }
    for (short s = 0; s < 10; ++s) {
        emit zdarzenieSilnik(s, msg.getActive(s) ? M_ACTIVE : M_NOACTIVE);
    }

    emit kontrolerConfigured(PARAMS_OK);
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
    m_reader.setStop();
    m_writer.setStop();
    m_reader.wait();
    m_writer.wait();

#ifdef SERIALLINUX
    if (m_serialPort) {
        m_serialPort->close();
        delete m_serialPort;
        m_serialPort = nullptr;
    }
#else
    RS232_CloseComport(m_portNr);
    m_portNr = -1;
#endif
    setConnected(false);
    emit kontrolerConfigured(CLOSE);
    emit deviceName("");
    DEBUGSER("CLOSE DEVICE");
}

bool Sterownik::openDevice()
{
#ifdef SERIALLINUX
    m_serialPort = new QSerialPort();
    if (m_serialPort == nullptr)
        return false;
    m_serialPort->setPort(QSerialPortInfo(m_portName));

    emit deviceName(m_serialPort->portName());

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        emit error(QString(QObject::tr("Nie mozna otworzyc urzadzenia %1, error  %2")).arg(m_portName).
                   arg(m_serialPort->errorString()));
        emit kontrolerConfigured(NO_OPEN);
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

    emit kontrolerConfigured(OPEN);



    QByteArray startBuf(20, '\0');
    DEBUGSER(QString("[%1]").arg(startBuf.toHex(' ').data()));
    write(startBuf, 2500);
    QThread::currentThread()->sleep(1);
    read(1000);
    return true;
#else
    DEBUGSER(QString("Otwieram urządzenia %1").arg(m_portName));
    char mode[]={'8','O','1',0};
    if (RS232_OpenComport(m_portNr, 115200, mode, 0)) {
        DEBUGSER(QString("Error open Port"));
        emit kontrolerConfigured(NO_OPEN);
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

    emit kontrolerConfigured(OPEN);
    return true;
#endif
}

void Sterownik::setReset()
{
    if (connected()) {
        m_writer.command(SterownikWriter::RESET, SerialMessage::resetSilniki());
    }
}

bool Sterownik::write(const QByteArray &currentRequest, int currentWaitWriteTimeout)
{

#ifdef SERIALLINUX
    if (m_serialPort == nullptr)
        return false;
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
    if (m_serialPort == nullptr)
        return QByteArray();
    if (m_serialPort->waitForReadyRead(currentWaitReadTimeout)) {
        responseData = m_serialPort->readAll();
        //qDebug() << "R=" << responseData.size();
        //if (responseData.size() > 0) {
        //    return responseData;
        //}
        while (m_serialPort->waitForReadyRead(10) && currentWaitReadTimeout > 0) {
            QByteArray r = m_serialPort->readAll();
            //qDebug() << "r=" << r.size();
            if (r.size() == 0)
                break;
            responseData += r;
            currentWaitReadTimeout -= 10;
        }
    }
    if (responseData.size() > 0)
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
    return responseData;

#endif
    return responseData;
}

QVector<SerialMessage> Sterownik::parseMessage(QByteArray &reply)
{
    //qDebug() << __FILE__ << __LINE__ << "ParseMessage";
    QVector<SerialMessage> ret;
    QMap<int, QString> errMap;
    errMap[SerialMessage::INVALID_REPLY] = "Nie prawidłowa odpowiedź";
    errMap[SerialMessage::CLR_MESSAGE] = "Wiadomość czyszcząca";
    errMap[SerialMessage::EMPTY_MESSAGE] = "Pusta wiadomość";
    errMap[SerialMessage::INPROGRESS_REPLY] = "Wiadomość przerwana";
    errMap[SerialMessage::WELCOME_REPLY] = "Wiadomość powitalna";
    errMap[SerialMessage::CONF_REPLY] = "Wiadomość konfiguracyjna";
    errMap[SerialMessage::MOVEHOME_REPLY] = "Wiadomość HOME_REPLy";
    errMap[SerialMessage::POSITION_REPLY] = "Wiadomość POSITION_REPLY";
    errMap[SerialMessage::RESET_REPLY] = "Wiadomość RESET_REPLY";
    errMap[SerialMessage::CONF_MEGA_REPLY] = "Wiadomość MEGA_REPLY";
    errMap[SerialMessage::CZUJKA_ZW_REPLY] = "Wiadomość CZUJKA ZW";
    do {
        SerialMessage msg;
        if (!msg.parseCommand(reply)) {
            DEBUGSER(QString("Parse Msg faild %1").arg(errMap[msg.getParseReply()]));
            QString errmsg("Nie poprawna wiadomość:");
            errmsg + errMap[msg.getParseReply()];
            emit error(errmsg);
            if (msg.getParseReply() == SerialMessage::INPROGRESS_REPLY)
                return ret;
            continue;
        }

        DEBUGSER(QString("Parse Msg success %1").arg(errMap[msg.getParseReply()]));

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
            emit zdarzenieSilnik(msg.getSilnik(), msg.getActive(msg.getSilnik())? M_ACTIVE : M_NOACTIVE);
            break;

        case SerialMessage::MOVEHOME_REPLY:
            emit setPositionDone(msg.getSilnik(), true, true, 0);
            break;

        case SerialMessage::POSITION_REPLY:
            emit setPositionDone(msg.getSilnik(), false, true, msg.getSteps());
            break;

        case SerialMessage::CZUJKA_ZW_REPLY:
            emit czujkaOn();

        case SerialMessage::RESET_REPLY:
            break;
        }
        ret.append(msg);
    } while (reply.size());
    return ret;
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
    emit deviceName("");
    m_portName = "";
    m_portNr = -1;
    setConnected(false);
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
        QString vendorId;
        QString productId;

        QString systemLocation = "";
        bool findDevice = false;

        DEBUGSER(QString("Szukam urządzenia"));

        const auto serialPortInfos = QSerialPortInfo::availablePorts();

        for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
            description = serialPortInfo.description();
            manufacturer = serialPortInfo.manufacturer();
            serialNumber = serialPortInfo.serialNumber();
            vendorId = serialPortInfo.hasVendorIdentifier() ? QString::number(serialPortInfo.vendorIdentifier(), 16) : "";
            productId = serialPortInfo.hasProductIdentifier() ? QString::number(serialPortInfo.productIdentifier(), 16) : "";
            DEBUGSER(QString("Znaleziono : [%1] DESC=%2 MANU=%3 SER=%4 VID=%5 PROD=%6").arg(serialPortInfo.portName(),
                     description, manufacturer, serialNumber, vendorId, productId));

            if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
                if ((vendorId == getVendor() && productId == getProduct() && serialNumber == getSerialNumber()) ||
                    (vendorId == "2341" && productId == "42" && serialNumber == "851363038373518041D1")  ||
                    (vendorId == "67b" && productId == "2303")   ) {
                    DEBUGSER(QString("Znaleziono kandydata %1").arg(serialPortInfo.portName()));
                    m_portName = serialPortInfo.portName();
                    emit deviceName(m_portName);
                    emit kontrolerConfigured(FOUND);
                    systemLocation = serialPortInfo.systemLocation();
                    DEBUGSER(QString("Znaleziono urządzenie"));
                    findDevice = true;
                    //break;
                }
            }
        }
        if (!findDevice) {
            emit kontrolerConfigured(NO_FOUND);
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

    if (emitSignal) emit kontrolerConfigured(ALL_OK);
}

QString Sterownik::getProduct()
{
    return ust->getSerialDeviceSterownikProduct();
}

QString Sterownik::getVendor()
{
    return ust->getSerialDeviceSterownikVendor();
}

QString Sterownik::getSerialNumber()
{
    return ust->getSerialDeviceSterownikSerial();
}


