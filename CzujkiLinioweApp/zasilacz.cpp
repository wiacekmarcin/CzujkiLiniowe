#include "zasilacz.h"


#include <QTime>
#include <QThread>
#include <QSerialPortInfo>
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
    sd(device),
    m_serialPort(nullptr)
{

    runWorker = true;

    connect(this, &SerialWorkerZas::debug, device, &Zasilacz::debug, Qt::QueuedConnection);
    connect(this, &SerialWorkerZas::deviceName, device, &Zasilacz::deviceName, Qt::QueuedConnection);
    connect(this, &SerialWorkerZas::error, device, &Zasilacz::error, Qt::QueuedConnection);
    connect(this, &SerialWorkerZas::kontrolerConfigured, device, &Zasilacz::kontrolerConfigured, Qt::QueuedConnection);
    connect(this, &SerialWorkerZas::kontrolerSerialNo, device, &Zasilacz::kontrolerSerialNo, Qt::QueuedConnection);
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
            connectToSerialJob();
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
    m_serialPort = new QSerialPort();
    m_serialPort->setPort(QSerialPortInfo(portName));

    emit deviceName(m_serialPort->portName());

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        emit error(QString(QObject::tr("Nie mozna otworzyc urzadzenia %1, error: %2")).arg(portName).
                   arg(m_serialPort->errorString()));
        emit kontrolerConfigured(false, Zasilacz::NO_OPEN);
        return false;
    }

    emit kontrolerConfigured(false, Zasilacz::OPEN);
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);

    return true;

}
QList<QStringList> SerialWorkerZas::getComPorts()
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

bool SerialWorkerZas::connectToSerialJob()
{
    if (!sd->connected()) {
        QString vendor, product;
        short found = 0;
        for (const auto & port : getComPorts())
        {
            qDebug() << port;
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
                emit kontrolerConfigured(false, Zasilacz::TO_MANY_FOUND);
            }
            else
                emit kontrolerConfigured(false, Zasilacz::FOUND);

            if (!openDevice(port.at(0))) {
                continue;
            }

            if (!checkIdentJob()) {
                continue;
            }

            sd->setConnected(true);
        }

        if (!found)
            emit kontrolerConfigured(false, Zasilacz::NO_FOUND);
    }
    return sd->connected();
}

bool SerialWorkerZas::checkIdentJob()
{
    QByteArray msg("*IDN?\n", 6);
    DEBUGSER(QString("Sprawdzam identyfikacje [%1]").arg(msg.data()));
    auto s = write(msg, 100, 1000, true);
    if (s.isEmpty()) {
        emit kontrolerConfigured(false, Zasilacz::IDENT_FAILD);
        return false;
    }
    emit kontrolerSerialNo(QString(s));
    emit kontrolerConfigured(true, Zasilacz::ALL_OK);
    return true;
}

QByteArray SerialWorkerZas::write(const QByteArray &currentRequest, int currentWaitWriteTimeout, int currentReadWaitTimeout, bool read)
{
    if (currentRequest.size() > 0) {
        DEBUGSER("Sending bytes....");
        int sendBytes = m_serialPort->write(currentRequest);
        if (!m_serialPort->waitForBytesWritten(currentWaitWriteTimeout)) {
            DEBUGSER(QString("Timeout Write %1").arg(currentWaitWriteTimeout));
            emit error(QString("Nie udało się zapisać do RS zasilacza"));
            return QByteArray();
        }
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
        DEBUGSER(QString("Read %1 [%2]").arg(responseData.size()).arg(responseData.constData()));
        return responseData;
    } else {
        DEBUGSER(QString("Timeout Read %1").arg(currentReadWaitTimeout));
        emit error(QString("Nie udało się odczytać z RS zasilacza"));
        return QByteArray();
    }
}

void SerialWorkerZas::closeDeviceJob()
{
    DEBUGSER("CLOSING DEVICE");
    //setStop();

    m_serialPort->close();
    sd->setConnected(false);
    emit kontrolerConfigured(false, Zasilacz::CLOSE);
    delete m_serialPort;
    m_serialPort = nullptr;
    DEBUGSER("CLOSE DEVICE");
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

/**************************************************************************/

bool Zasilacz::connected()
{
    const QMutexLocker locker(&connMutex);
    return m_connected;
}

void Zasilacz::setConnected(bool connected)
{
    const QMutexLocker locker(&connMutex);
    m_connected = connected;
}

void Zasilacz::closeDevice(bool waitForDone)
{
    DEBUGSER(QString("close device %1").arg(waitForDone));
    if (waitForDone) {
        m_worker.command(SerialZasilacz::DISCONNECT, QByteArray(), false);
    } else {
        m_worker.setReset();
        m_worker.closeDeviceJob();
    }
}

QString Zasilacz::getProduct()
{
    return u->getSerialDeviceZasilaczProduct();
}

QString Zasilacz::getVendor()
{
    return u->getSerialDeviceZasilaczVendor();
}

void Zasilacz::debugFun(const QString &log)
{
    emit debug(log);
}

Zasilacz::Zasilacz(Ustawienia *ust, QObject *parent):
    QObject(parent),
    m_connected(false),  m_worker(this),
    readMeas(this),
    cntTimeout(9),
    u(ust)
{
    readMeas.setInterval(10000);
    connect(&readMeas, &QTimer::timeout, this, &Zasilacz::timeout100ms);
    readMeas.start();
}

Zasilacz::~Zasilacz()
{
    setStop();
}

void Zasilacz::setStop()
{
    m_worker.setReset();
    m_worker.setStop();
    readMeas.stop();
}

void Zasilacz::timeout100ms()
{
    if (!connected()) {
        cntTimeout = 9;
        return;
    }

    if (++cntTimeout >= 10) {
        cntTimeout = 0;
        getVoltage();
        getCurrent();
        getVoltageLimit();
        getCurrentLimit();
        getOutput();
    }

    getVoltageMeas();
    getCurrentMeas();

}

QByteArray Zasilacz::convertFloatValue(const int &mval, const int & maxval)
{
    QByteArray ret;
    int val = mval;
    int tmp;
    if (val > maxval)
        val = maxval;

    if (val > 10000) {
        tmp = val/10000;
        ret.append('0'+tmp);
        val -= tmp*10000;
    }

    if (val > 1000) {
        tmp = val/1000;
        ret.append('0'+tmp);
        val -= tmp*1000;
    } else
        ret.append('0');

    ret.append('.');

    if (val > 100) {
        tmp = val/100;
        ret.append('0'+tmp);
        val -= tmp*100;
    } else
        ret.append('0');

    if (val > 10) {
        tmp = val/10;
        ret.append('0'+tmp);
        val -= tmp*10;
    } else
        ret.append('0');

    ret.append('0'+val);
    return ret;
}

void Zasilacz::connectToDevice()
{
    if (connected()) {
        emit kontrolerConfigured(true, ALL_OK);
    } else {
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    }
}

void Zasilacz::setThread(QThread *trh)
{
    m_worker.moveToThread(trh);
    m_worker.start();
    m_worker.command(SerialZasilacz::IDLE, QByteArray(), false);
}

void Zasilacz::setVoltage(const int &mvalue)
{
    QByteArray cmd("VOLT ", 5);
    cmd += convertFloatValue(mvalue, 30000);
    cmd += '\n';
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::SET_VOLTAGE, cmd, false);
}

void Zasilacz::setCurrent(const int &mvalue)
{
    QByteArray cmd("CURR ", 5);
    cmd += convertFloatValue(mvalue, 5000);
    cmd += '\n';
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::SET_CURRENT, cmd, false);
}

void Zasilacz::setVoltageLimit(const int &mvalue)
{
    QByteArray cmd("VOLT:LIM ", 9);
    cmd += convertFloatValue(mvalue, 31000);
    cmd += '\n';
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::SET_VOLTAGE_LIMIT, cmd, false);
}

void Zasilacz::setCurrentLimit(const int &mvalue)
{
    QByteArray cmd("CURR:LIM ", 9);
    cmd += convertFloatValue(mvalue, 5100);
    cmd += '\n';
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::SET_CURRENT_LIMIT, cmd, false);
}

void Zasilacz::getVoltage()
{
    QByteArray cmd("VOLT?\n", 6);
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::GET_VOLTAGE, cmd, true);
}

void Zasilacz::getCurrent()
{
    QByteArray cmd("CURR?\n", 6);
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::GET_CURRENT, cmd, true);
}

void Zasilacz::getVoltageLimit()
{
    QByteArray cmd("VOLT:LIM?\n", 10);
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::GET_VOLTAGE_LIMIT, cmd, true);
}

void Zasilacz::getCurrentLimit()
{
    QByteArray cmd("CURR:LIM?\n", 10);
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::GET_CURRENT_LIMIT, cmd, true);
}

void Zasilacz::getVoltageMeas()
{
    QByteArray cmd("MEAS:VOLT?\n", 11);
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::GET_VOLTAGE_MEAS, cmd, true);
}

void Zasilacz::getCurrentMeas()
{
    QByteArray cmd("MEAS:CURR?\n", 11);
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::GET_CURRENT_MEAS, cmd, true);
}

void Zasilacz::setOutput(bool on)
{
    QByteArray cmd("OUTP ", 5);
    cmd += (on) ? '1' : '0';
    cmd += '\n';
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::SET_OUTPUT, cmd, false);
}

void Zasilacz::getOutput()
{
    QByteArray cmd("OUTP?\n", 6);
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::GET_OUTPUT, cmd, true);
}

void Zasilacz::getValueJob(const SerialZasilacz::TaskExt &actTask, KindValueType kind, const QString & debug, const QString & function)
{
    DEBUGSER(QString("Sprawdzam %1 [%2] [%3]").arg(debug).arg(SerialWorkerZas::mapTask[actTask.task]).arg(actTask.msg.data()));
    auto s = m_worker.write(actTask.msg, 100, 1000, actTask.waitForRead);
    if (s.isEmpty()) {
        emit error(QString("Pusta odpowiedź na %1").arg(function));
        return;
    }
    auto parts = QString(s).split(".");
    if (parts.size() != 2) {
        emit error(QString("Nie prawidłowa odpowiedź [%1] na %2").arg(QString(s)).arg(function));
        return;
    }
    bool ok1, ok2;
    int val = parts[0].toInt(&ok1)*1000 + parts[1].toInt(&ok2);
    if (!ok1 || !ok2) {
        emit error(QString("Nie prawidłowa odpowiedź [%1] na %2").arg(QString(s)).arg(function));
        return;
    }
    emit value(kind, val);
}

void Zasilacz::getVoltageJob(const SerialZasilacz::TaskExt &actTask)
{
    getValueJob(actTask, VOLTAGE_SET, "ustawione napięcie", __PRETTY_FUNCTION__);
}

void Zasilacz::getCurrentJob(const SerialZasilacz::TaskExt &actTask)
{
    getValueJob(actTask, CURRENT_SET, "ustawiony prąd", __PRETTY_FUNCTION__);
}

void Zasilacz::getVoltageLimitJob(const SerialZasilacz::TaskExt &actTask)
{
    getValueJob(actTask, VOLTAGE_LIMIT, "maksymalne napięcie", __PRETTY_FUNCTION__);
}

void Zasilacz::getCurrentLimitJob(const SerialZasilacz::TaskExt &actTask)
{
    getValueJob(actTask, CURRENT_LIMIT, "maksymalny prąd", __PRETTY_FUNCTION__);
}

void Zasilacz::getVoltageMeasJob(const SerialZasilacz::TaskExt &actTask)
{
    getValueJob(actTask, VOLTAGE_MEAS, "mierzone napięcie", __PRETTY_FUNCTION__);
}

void Zasilacz::getCurrentMeasJob(const SerialZasilacz::TaskExt &actTask)
{
    getValueJob(actTask, CURRENT_MEAS, "mierzony prąd", __PRETTY_FUNCTION__);
}

void Zasilacz::setParamJob(const SerialZasilacz::TaskExt &actTask)
{
    DEBUGSER(QString("Zapisuje parametr [%1] [%2]").arg(SerialWorkerZas::mapTask[actTask.task]).arg(actTask.msg.data()));
    auto s = m_worker.write(actTask.msg, 100, 10, actTask.waitForRead);
}

void Zasilacz::getOutputJob(const SerialZasilacz::TaskExt &actTask)
{
    DEBUGSER(QString("Sprawdzam ustawione wyjście [%1] [%2]").arg(SerialWorkerZas::mapTask[actTask.task]).arg(actTask.msg.data()));
    auto s = m_worker.write(actTask.msg, 100, 1000, actTask.waitForRead);
    if (s.isEmpty()) {
        emit error(QString("Pusta odpowiedź na %1").arg(__PRETTY_FUNCTION__));
        return;
    }

    bool ok;
    int val = QString(s).toInt(&ok);
    if (!ok || val != 0 || val != 1) {
        emit error(QString("Nie prawidłowa odpowiedź [%1] na %2").arg(QString(s)).arg(__PRETTY_FUNCTION__));
        return;
    }
    emit value(OUTPUT, val);
}


