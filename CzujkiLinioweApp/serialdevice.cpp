#include "serialdevice.h"
#include <QTime>
#include <QThread>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>

#define DEBUGSER(X) debugFun(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))

SerialWorkerReader::SerialWorkerReader(SerialDevice * device):
    QThread(nullptr),
    sd(device)
{
    runWorker = true;
    connect(this, &SerialWorkerReader::debug, device, &SerialDevice::debug, Qt::QueuedConnection);
}

SerialWorkerReader::~SerialWorkerReader()
{
    mutex.lock();
    runWorker = false;
    mutex.unlock();
}

void SerialWorkerReader::setStop()
{
    const QMutexLocker locker(&mutexRun);
    runWorker = false;
}

void SerialWorkerReader::run()
{
    bool quit;
    QByteArray receiveData;
    do
    {
        QByteArray responseData = sd->read(1000);
        DEBUGSER(QString("[RECV] %1").arg(responseData.toHex(' ').data()));
        receiveData.push_back(responseData);
        sd->parseMessage(receiveData);

        mutexRun.lock();
        quit = !runWorker;
        mutexRun.unlock();
    } while (!quit);
}

void SerialWorkerReader::debugFun(const QString &s)
{
    emit debug(s);
}

SerialWorkerWriter::SerialWorkerWriter(SerialDevice * device):
    QThread(nullptr),
    sd(device)
{
    actTask = IDLE;
    runWorker = true;

    connect(this, &SerialWorkerWriter::debug, device, &SerialDevice::debug, Qt::QueuedConnection);
}

SerialWorkerWriter::~SerialWorkerWriter()
{
    mutex.lock();
    runWorker = false;
    actTask = IDLE;
    newTask.wakeOne();
    mutex.unlock();
    wait(120000);
}

bool SerialWorkerWriter::command(Task curr)
{
    DEBUGSER(QString("New command %1").arg(curr));
    {
        const QMutexLocker locker(&mutexRun);
        if (!runWorker)
            return false;
    }
    const QMutexLocker locker(&mutex);

    futureTask.push_back(curr);
    newTask.wakeOne();
    return true;
}

void SerialWorkerWriter::setStop()
{
    {
        const QMutexLocker locker(&mutexRun);
        runWorker = false;    
    }

    //mutex.lock();
    const QMutexLocker locker(&mutex);
    futureTask.clear();
    futureTask.push_back(DISCONNECT);
    newTask.wakeOne();
    //mutex.unlock();
    //wait();
}

void SerialWorkerWriter::setReset()
{
    const QMutexLocker locker(&mutex);
    futureTask.clear();
}

void SerialWorkerWriter::run()
{
    mutex.lock();
    short zadanie = actTask;
    mutex.unlock();
    bool quit = false;
    DEBUGSER(QString("actTask = %1").arg(actTask));
    do {
        mutex.lock();
        if (futureTask.size() == 0) {
            actTask = IDLE;
            newTask.wait(&mutex);
        } else {
            actTask = futureTask.front();
            futureTask.pop_front();
        }
        zadanie = actTask;
        mutex.unlock();

        switch(zadanie) {
        case IDLE:
            break;
            
        case CONNECT:
            sd->connectToSerialJob();
            break;

        case SET_PARAMS:
            sd->setParamsJob();
              break;

        case SET_HOME:
            sd->setHomeJob();
            break;

        case SET_POSITION:
            sd->setPosJob();
            break;

        case DISCONNECT:
            sd->closeDeviceJob();
            break;

        case RESET:
            sd->resetJob();
            break;
        
        default:
            break;
        }
        mutexRun.lock();
        quit = !runWorker;
        mutexRun.unlock();
    } while (!quit);
    
   DEBUGSER("SerialWorker::run() quit");
   this->quit();
}

void SerialWorkerWriter::debugFun(const QString &s)
{
    emit debug(s);
}

SerialWorkerWriter::Task SerialWorkerWriter::getActTask()
{
    Task ret;
    mutex.lock();
    ret = actTask;
    mutex.unlock();
    return ret;
}
/**************************************************************************/

SerialDevice::SerialDevice(Ustawienia *u, QObject *parent)
    : QObject(parent),
      m_portName(""), m_portNr(-1),
      m_connected(false),  m_writer(this), m_reader(this),
      ust(u)
{


    m_portNr = -1;
#ifdef SERIALLINUX
     m_serialPort = new QSerialPort(&m_writer);
#endif

}

SerialDevice::~SerialDevice()
{
    setStop();
    m_writer.wait();
    m_reader.wait();
}

void SerialDevice::setThread(QThread *thrW, QThread *thrR)
{
    m_writer.moveToThread(thrW);
    m_writer.start();
    m_writer.command(SerialWorkerWriter::IDLE);

    m_reader.moveToThread(thrR);
    //start przy connect
}

void SerialDevice::setStop()
{
    m_writer.setReset();
    m_writer.setStop();
    m_reader.setStop();
}

void SerialDevice::connectToDevice()
{
    if (connected()) {
        emit kontrolerConfigured(true, ALL_OK);
    } else {
        m_writer.command(SerialWorkerWriter::CONNECT);
    }
}

void SerialDevice::setParams()
{


    if (connected()) {
        //insertParams();
    }
}

void SerialDevice::setPositionHome()
{
    if (!connected())
        m_writer.command(SerialWorkerWriter::CONNECT);
    m_writer.command(SerialWorkerWriter::SET_HOME);
}

void SerialDevice::setPosition(uint32_t x, uint32_t y)
{
    if (!connected()) {
        m_writer.command(SerialWorkerWriter::CONNECT);
        m_writer.command(SerialWorkerWriter::SET_HOME);
    }
    m_writer.command(SerialWorkerWriter::SET_POSITION);
}

bool SerialDevice::configureDevice(bool wlcmmsg)
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
            i, ust->getEnableSilnik(i), ust->getLeftRotationSilnik(i),
            ust->getMaxStepsSilnik(i).toInt(), ust->getBaseStepsSilnik(i).toInt(),
            ust->getDelayusSilnik(i).toInt());

        if (!write(msgBA, 200))
            return false;

        auto reply = read(1000);
        SerialMessage msg;
        msg.parseCommand(reply);

        if (msg.getParseReply() != SerialMessage::CONF_REPLY && msg.getSilnik() != i) {
            emit kontrolerConfigured(false, PARAMS_FAILD);
            return false;
        }
    }

    emit kontrolerConfigured(true, PARAMS_OK);
    return true;
}

void SerialDevice::setParamsJob()
{

}


void SerialDevice::setHomeJob()
{
    DEBUGSER(QString("Ustaw pozycje startowa dla czujnika"));
}

void SerialDevice::setPosJob()
{

}

bool SerialDevice::connected()
{
    const QMutexLocker locker(&connMutex);
    return m_connected;
}

void SerialDevice::setConnected(bool connected)
{
    const QMutexLocker locker(&connMutex);
    m_connected = connected;
}

void SerialDevice::closeDeviceJob()
{
    DEBUGSER("CLOSING DEVICE");
    //setStop();
    m_reader.setStop();
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

void SerialDevice::resetJob()
{
    DEBUGSER(QString("Resetuje urzadzenie"));
    //auto s = write(SerialMessage::resetSterownik(0), 100);
    //if (s.getParseReply() != SerialMessage::RESET_REPLY) {
    //    return;
    //}
}

bool SerialDevice::openDevice()
{
#ifdef SERIALLINUX
    m_serialPort->setPort(QSerialPortInfo(m_portName));

    emit deviceName(m_serialPort->portName());

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        emit error(QString(QObject::tr("Nie mozna otworzyc urzadzenia %1, error  %2")).arg(m_portName).
                   arg(m_serialPort->errorString()));
        emit kontrolerConfigured(false, NO_OPEN);
        return false;
    }

    emit kontrolerConfigured(false, OPEN);
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);

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

SerialMessage SerialDevice::parseMessage(QByteArray &reply)
{
    SerialMessage msg;
    msg.parseCommand(reply);
    return msg;
}

void SerialDevice::setReset()
{
    if (connected()) {
        m_writer.command(SerialWorkerWriter::RESET);
        m_writer.command(SerialWorkerWriter::SET_PARAMS);
    }
}

bool SerialDevice::write(const QByteArray &currentRequest, int currentWaitWriteTimeout)
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

QByteArray SerialDevice::read(int currentWaitReadTimeout)
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

void SerialDevice::closeDevice(bool waitForDone)
{
    DEBUGSER(QString("close device %1").arg(waitForDone));
    if (waitForDone) {
        m_writer.command(SerialWorkerWriter::DISCONNECT);
    } else {
        m_writer.setReset();
        closeDeviceJob();
    }
}

void SerialDevice::debugFun(const QString &log)
{
    emit debug(log);
}

void SerialDevice::connectToSerialJob()
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


