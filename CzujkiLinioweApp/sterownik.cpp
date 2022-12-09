#include "sterownik.h"

#include <QTime>
#include <QThread>
#include <QSerialPortInfo>

#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>

#define DEBUGSER(X) debugFun(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))


SerialWorkerSter2::SerialWorkerSter2(Sterownik *device):
    QThread(nullptr),
    actTask(SerialSterownik::TaskExt(SerialSterownik::IDLE, QByteArray())),
    sd(device),
    m_serialPort(nullptr)
{

    runWorker = true;

    connect(this, &SerialWorkerSter2::debug, device, &Sterownik::debug, Qt::QueuedConnection);
    connect(this, &SerialWorkerSter2::deviceName, device, &Sterownik::deviceName, Qt::QueuedConnection);
    connect(this, &SerialWorkerSter2::error, device, &Sterownik::error, Qt::QueuedConnection);
    connect(this, &SerialWorkerSter2::kontrolerConfigured, device, &Sterownik::kontrolerConfigured, Qt::QueuedConnection);
}

SerialWorkerSter2::~SerialWorkerSter2()
{
    mutex.lock();
    runWorker = false;
    actTask = SerialSterownik::TaskExt(SerialSterownik::IDLE, QByteArray());
    newTask.wakeOne();
    mutex.unlock();
    wait(120000);
}

bool SerialWorkerSter2::command(SerialSterownik::TaskId curr, const QByteArray & msg)
{
    DEBUGSER(QString("New command %1").arg(curr));
    {
        const QMutexLocker locker(&mutexRun);
        if (!runWorker)
            return false;
    }
    const QMutexLocker locker(&mutex);
    SerialSterownik::TaskExt ftask(curr, msg);
    futureTask.push_back(ftask);
    newTask.wakeOne();
    return true;
}

void SerialWorkerSter2::setStop()
{
    {
        const QMutexLocker locker(&mutexRun);
        runWorker = false;
    }

    //mutex.lock();
    const QMutexLocker locker(&mutex);
    futureTask.clear();
    SerialSterownik::TaskExt ftask(SerialSterownik::DISCONNECT, QByteArray());
    futureTask.push_back(ftask);
    newTask.wakeOne();
    //mutex.unlock();
    //wait();
}

void SerialWorkerSter2::setReset()
{
    const QMutexLocker locker(&mutex);
    futureTask.clear();
}

void SerialWorkerSter2::run()
{
    mutex.lock();
    short zadanie;
    mutex.unlock();
    bool quit = false;
    DEBUGSER(QString("actTask = %1").arg(actTask.task));
    do {
        mutex.lock();
        if (futureTask.size() == 0) {
            actTask = SerialSterownik::TaskExt( SerialSterownik::IDLE, QByteArray());
            newTask.wait(&mutex);
        } else {
            actTask = futureTask.front();
            futureTask.pop_front();
        }
        zadanie = actTask.task;
        mutex.unlock();
        DEBUGSER(QString("actTask = %1").arg(zadanie));
        switch(zadanie) {
        case SerialSterownik::IDLE:
            break;

        case SerialSterownik::CONNECT:
            connectToSerialJob();
            break;

        //case SerialSterownik::SET_OUTPUT:
        //    sd->setParamJob(actTask);
        //    break;



        case SerialSterownik::DISCONNECT:
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

   DEBUGSER("SerialWorker2::run() quit");
   this->quit();
}

bool SerialWorkerSter2::openDevice(const QString & portName)
{
    m_serialPort = new QSerialPort();
    m_serialPort->setPort(QSerialPortInfo(portName));

    emit deviceName(m_serialPort->portName());

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        emit error(QString(QObject::tr("Nie mozna otworzyc urzadzenia %1, error: %2")).arg(portName).
                   arg(m_serialPort->errorString()));
        emit kontrolerConfigured(false, Sterownik::NO_OPEN);
        return false;
    }

    emit kontrolerConfigured(false, Sterownik::OPEN);
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);

    return true;

}
QList<QStringList> SerialWorkerSter2::getComPorts()
{
    QString description;
    QString manufacturer;
    QString serialNumber;
    QList<QStringList> ports;
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
    return ports;
}

bool SerialWorkerSter2::connectToSerialJob()
{
    if (!sd->connected()) {
        QString vendor, product;
        short found = 0;
        for (const auto & port : getComPorts())
        {
            if (port.count() != 7)
                continue;

            vendor = port.at(5);
            product = port.at(6);

            if (vendor.isEmpty() || product.isEmpty())
                continue;

            if (vendor != "67b" || product != "2303")
                continue;

            ++found;
            if (found > 1) {
                emit kontrolerConfigured(false, Sterownik::TO_MANY_FOUND);
            }
            else
                emit kontrolerConfigured(false, Sterownik::FOUND);

            if (!openDevice(port.at(0))) {
                continue;
            }

            if (!checkIdentJob()) {
                continue;
            }

            sd->setConnected(true);
        }

        if (!found)
            emit kontrolerConfigured(false, Sterownik::NO_FOUND);
    }
    return sd->connected();
}

bool SerialWorkerSter2::checkIdentJob()
{
    QByteArray msg("*IDN?\n", 6);
    DEBUGSER(QString("Sprawdzam identyfikacje [%1]").arg(msg.data()));
    write(msg, 100);

    QString s;
    //TODO add read...

    QByteArray responseData;
    DEBUGSER(QString("Wait for read"));
    if (m_serialPort->waitForReadyRead(1000)) {
        responseData = m_serialPort->readAll();
        while (m_serialPort->waitForReadyRead(10))
            responseData += m_serialPort->readAll();
        DEBUGSER(QString("Read %1 [%2]").arg(responseData.size()).arg(responseData.constData()));
    } else {
        DEBUGSER(QString("Timeout Read %1").arg(100));
        emit error(QString("Nie udało się odczytać z RS kontrolera"));
    }

    if (responseData.isEmpty()) {
        emit kontrolerConfigured(false, Sterownik::IDENT_FAILD);
        return false;
    }
    emit kontrolerConfigured(true, Sterownik::ALL_OK);
    return true;
}

bool SerialWorkerSter2::write(const QByteArray &currentRequest, int currentWaitWriteTimeout)
{
    if (currentRequest.size() > 0) {
        DEBUGSER("Sending bytes....");
        int sendBytes = m_serialPort->write(currentRequest);
        if (!m_serialPort->waitForBytesWritten(currentWaitWriteTimeout)) {
            DEBUGSER(QString("Timeout Write %1").arg(currentWaitWriteTimeout));
            emit error(QString("Nie udało się zapisać do RS Sterownika"));
            return false;
        }
        DEBUGSER(QString("Write %1 bytes [%2]").arg(sendBytes).arg(currentRequest.constData()));
    }
    return true;
}

void SerialWorkerSter2::closeDeviceJob()
{
    DEBUGSER("CLOSING DEVICE");
    //setStop();

    m_serialPort->close();
    sd->setConnected(false);
    emit kontrolerConfigured(false, Sterownik::CLOSE);
    delete m_serialPort;
    m_serialPort = nullptr;
    DEBUGSER("CLOSE DEVICE");
}

void SerialWorkerSter2::debugFun(const QString &s)
{
    emit debug(s);
}

SerialSterownik::TaskId SerialWorkerSter2::getActTaskId()
{
    SerialSterownik::TaskId ret;
    mutex.lock();
    ret = actTask.task;
    mutex.unlock();
    return ret;
}

/**************************************************************************/

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

void Sterownik::closeDevice(bool waitForDone)
{
    DEBUGSER(QString("close device %1").arg(waitForDone));
    if (waitForDone) {
        m_worker.command(SerialSterownik::DISCONNECT, QByteArray());
    } else {
        m_worker.setReset();
        m_worker.closeDeviceJob();
    }
}

void Sterownik::debugFun(const QString &log)
{
    emit debug(log);
}

Sterownik::Sterownik(QObject *parent):
    QObject(parent),
    m_connected(false),  m_worker(this)
{

}

Sterownik::~Sterownik()
{
    setStop();
}

void Sterownik::setStop()
{
    m_worker.setReset();
    m_worker.setStop();
}

void Sterownik::connectToDevice()
{
    if (connected()) {
        emit kontrolerConfigured(true, ALL_OK);
    } else {
        m_worker.command(SerialSterownik::CONNECT, QByteArray());
    }
}

void Sterownik::setThread(QThread *trh)
{
    m_worker.moveToThread(trh);
    m_worker.start();
    m_worker.command(SerialSterownik::IDLE, QByteArray());
}



