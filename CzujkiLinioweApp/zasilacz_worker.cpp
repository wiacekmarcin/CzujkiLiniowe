#include "zasilacz_worker.h"
#include "zasilacz.h"

#include <QTime>
#include <QThread>
#ifndef NOSERIAL
#include <QSerialPortInfo>
#endif
//#include "rs232.h"

#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>

#define DEBUGSER(X) debugFun(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))

const char* const SerialWorkerZas::mapTask[] = { "IDLE", "CONNECT", "GET_VOLTAGE","GET_CURRENT","SET_VOLTAGE",
                                              "SET_CURRENT", "SET_VOLTAGE_LIMIT", "SET_CURRENT_LIMIT" ,
                                              "GET_VOLTAGE_LIMIT", "GET_CURRENT_LIMIT",
                                              "GET_VOLTAGE_MEAS", "GET_CURRENT_MEAS" ,
                                              "SET_OUTPUT", "GET_OUTPUT","DISCONNECT" };

SerialWorkerZas::SerialWorkerZas(Zasilacz *device):
    QThread(nullptr),
    actTask(SerialZasilacz::TaskExt(SerialZasilacz::IDLE, QByteArray(), false)),
    sd(device)
#ifndef NOSERIAL    
    ,m_serialPort(nullptr)
#endif    
{

    runWorker = true;

    connect(this, &SerialWorkerZas::debug, device, &Zasilacz::debug, Qt::QueuedConnection);
    connect(this, &SerialWorkerZas::deviceName, device, &Zasilacz::deviceName, Qt::QueuedConnection);
    connect(this, &SerialWorkerZas::error, device, &Zasilacz::error, Qt::QueuedConnection);
    connect(this, &SerialWorkerZas::kontrolerConfigured, device, &Zasilacz::kontrolerConfigured, Qt::QueuedConnection);
    connect(this, &SerialWorkerZas::kontrolerSerialNo, device, &Zasilacz::kontrolerSerialNo, Qt::QueuedConnection);
    connect(this, &SerialWorkerZas::sendMsg, device, &Zasilacz::sendMsg, Qt::QueuedConnection);
    connect(this, &SerialWorkerZas::recvMsg, device, &Zasilacz::recvMsg, Qt::QueuedConnection);
}

SerialWorkerZas::~SerialWorkerZas()
{
    mutex.lock();
    runWorker = false;
    actTask = SerialZasilacz::TaskExt(SerialZasilacz::IDLE, QByteArray(), false);
    newTask.wakeOne();
    mutex.unlock();
    wait(120000);
}

bool SerialWorkerZas::command(SerialZasilacz::TaskId curr, const QByteArray & msg, bool readReply)
{
    DEBUGSER(QString("New command %1").arg(mapTask[curr]));
    {
        const QMutexLocker locker(&mutexRun);
        if (!runWorker)
            return false;
    }
    const QMutexLocker locker(&mutex);
    SerialZasilacz::TaskExt ftask(curr, msg, readReply);
    futureTask.push_back(ftask);
    newTask.wakeOne();
    return true;
}

void SerialWorkerZas::setStop()
{
    {
        const QMutexLocker locker(&mutexRun);
        runWorker = false;
    }

    //mutex.lock();
    const QMutexLocker locker(&mutex);
    futureTask.clear();
    SerialZasilacz::TaskExt ftask(SerialZasilacz::DISCONNECT, QByteArray(), false);
    futureTask.push_back(ftask);
    newTask.wakeOne();
    //mutex.unlock();
    //wait();
}

void SerialWorkerZas::setReset()
{
    const QMutexLocker locker(&mutex);
    futureTask.clear();
}

void SerialWorkerZas::run()
{
    mutex.lock();
    short zadanie;
    mutex.unlock();
    bool quit = false;
    DEBUGSER(QString("actTask = %1").arg(mapTask[actTask.task]));
    do {
        mutex.lock();
        if (futureTask.size() == 0) {
            actTask = SerialZasilacz::TaskExt( SerialZasilacz::IDLE, QByteArray(), false);
            newTask.wait(&mutex);
        } else {
            actTask = futureTask.front();
            futureTask.pop_front();
        }
        zadanie = actTask.task;
        mutex.unlock();
        DEBUGSER(QString("actTask = %1").arg(mapTask[zadanie]));
        switch(zadanie) {
        case SerialZasilacz::IDLE:
            break;

        case SerialZasilacz::CONNECT:
            if (!connectToSerialJob())
                futureTask.clear();
            break;

        case SerialZasilacz::GET_VOLTAGE:
            sd->getVoltageJob(actTask);
            break;

        case SerialZasilacz::GET_CURRENT:
            sd->getCurrentJob(actTask);
            break;

        case SerialZasilacz::GET_VOLTAGE_LIMIT:
            sd->getVoltageLimitJob(actTask);
            break;

        case SerialZasilacz::GET_CURRENT_LIMIT:
            sd->getCurrentLimitJob(actTask);
            break;

        case SerialZasilacz::GET_VOLTAGE_MEAS:
            sd->getVoltageMeasJob(actTask);
            break;

        case SerialZasilacz::GET_CURRENT_MEAS:
            sd->getCurrentMeasJob(actTask);
            break;

        case SerialZasilacz::SET_VOLTAGE:
        case SerialZasilacz::SET_CURRENT:
        case SerialZasilacz::SET_VOLTAGE_LIMIT:
        case SerialZasilacz::SET_CURRENT_LIMIT:
        case SerialZasilacz::SET_OUTPUT:
            sd->setParamJob(actTask);
            break;

        case SerialZasilacz::OTHER:
            write(actTask.msg, 1000, 1000, true, false);
            break;

        case SerialZasilacz::DISCONNECT:
            closeDeviceJob();
            break;
        default:
            break;
        }
        QThread().currentThread()->msleep(10);
        mutexRun.lock();
        quit = !runWorker;
        mutexRun.unlock();
    } while (!quit);

   DEBUGSER("SerialWorker::run() quit");
   this->quit();
}

bool SerialWorkerZas::openDevice(const QString & portName)
{
#ifndef NOSERIAL    
    m_serialPort = new QSerialPort();
    m_serialPort->setPort(QSerialPortInfo(portName));

    emit deviceName(m_serialPort->portName());

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        emit error(QString(QObject::tr("Nie mozna otworzyc urzadzenia %1, error: %2")).arg(portName).
                   arg(m_serialPort->errorString()));
        emit kontrolerConfigured(Zasilacz::NO_OPEN);
        return false;
    }

    emit kontrolerConfigured(Zasilacz::OPEN);
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
#endif
    return true;

}
QList<QStringList> SerialWorkerZas::getComPorts()
{
    QString description;
    QString manufacturer;
    QString serialNumber;
    QList<QStringList> ports;
#ifndef NOSERIAL    
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : "")
             << (!manufacturer.isEmpty() ? manufacturer : "")
             << (!serialNumber.isEmpty() ? serialNumber : "")
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : "")
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : "");

        ports.append(list);
    }
#endif    
    return ports;
}

bool SerialWorkerZas::connectToSerialJob()
{
    if (!sd->connected()) {
        QString vendor, product;
        short found = 0;
        for (const auto & port : getComPorts())
        {
            //qDebug() << port;
            if (port.count() != 7)
                continue;

            vendor = port.at(5);
            product = port.at(6);

            if (vendor.isEmpty() || product.isEmpty())
                continue;

            if (vendor != sd->getVendor() /*"67b"*/ || product != sd->getProduct() /*"23a3"*/)
                continue;

            ++found;
            if (found > 1) {
                emit kontrolerConfigured(Zasilacz::TO_MANY_FOUND);
            }
            else
                emit kontrolerConfigured(Zasilacz::FOUND);

            if (!openDevice(port.at(0))) {
                continue;
            }

            if (!checkIdentJob()) {
                continue;
            }

            sd->setConnected(true);
        }

        if (!found)
            emit kontrolerConfigured(Zasilacz::NO_FOUND);
    }
    return sd->connected();
}

bool SerialWorkerZas::checkIdentJob()
{
    QByteArray msg("*IDN?\n", 6);
    DEBUGSER(QString("Sprawdzam identyfikacje [%1]").arg(msg.data()));
    auto s = write(msg, 100, 1000, true);
    if (s.isEmpty()) {
        emit kontrolerConfigured(Zasilacz::IDENT_FAILD);
        return false;
    }
    emit kontrolerSerialNo(QString(s));
    emit kontrolerConfigured(Zasilacz::ALL_OK);
    return true;
}

QByteArray SerialWorkerZas::write(const QByteArray &currentRequest, int currentWaitWriteTimeout,
                                  int currentReadWaitTimeout, bool read, bool emitError)
{
#ifndef NOSERIAL
    if (currentRequest.size() > 0) {
        DEBUGSER("Sending bytes....");
        
        int sendBytes = m_serialPort->write(currentRequest);
        if (!m_serialPort->waitForBytesWritten(currentWaitWriteTimeout)) {
            DEBUGSER(QString("Timeout Write %1").arg(currentWaitWriteTimeout));
            emit error(QString("Nie udało się zapisać do RS zasilacza"));
            return QByteArray();
        }
        emit sendMsg(currentRequest.constData());
        DEBUGSER(QString("Write %1 bytes [%2]").arg(sendBytes).arg(currentRequest.constData()));
    }

    if (!read)
        return QByteArray();

    QByteArray responseData;
    DEBUGSER(QString("Wait for read"));
    if (m_serialPort->waitForReadyRead(currentReadWaitTimeout)) {
        responseData = m_serialPort->readAll();
        while (m_serialPort->waitForReadyRead(10))
            responseData += m_serialPort->readAll();
        emit recvMsg(responseData.constData());
        DEBUGSER(QString("Read %1 [%2]").arg(responseData.size()).arg(responseData.constData()));
        return responseData;
    } else {
        DEBUGSER(QString("Timeout Read %1").arg(currentReadWaitTimeout));
        if (emitError)
            emit error(QString("Nie udało się odczytać z RS zasilacza"));
        return QByteArray();
    }
#else 
    return QByteArray();
#endif    
}

void SerialWorkerZas::closeDeviceJob()
{
    DEBUGSER("CLOSING DEVICE");
    //setStop();
#ifndef NOSERIAL
    m_serialPort->close();
    sd->setConnected(false);
    emit kontrolerConfigured(Zasilacz::CLOSE);
    delete m_serialPort;
    m_serialPort = nullptr;
    DEBUGSER("CLOSE DEVICE");
#endif    
}

void SerialWorkerZas::debugFun(const QString &s)
{
    emit debug(s);
}

SerialZasilacz::TaskId SerialWorkerZas::getActTaskId()
{
    SerialZasilacz::TaskId ret;
    mutex.lock();
    ret = actTask.task;
    mutex.unlock();
    return ret;
}
