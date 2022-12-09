#include "zasilacz.h"
#include <QTime>
#include <QThread>
#include <QSerialPortInfo>
//#include "rs232.h"

#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>

#define DEBUGSER(X) debugFun(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))

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

void Zasilacz::setVoltage_mV(const int &mvalue)
{
    QByteArray cmd("VOLT ", 5);
    cmd += convertFloatValue(mvalue, 30000);
    cmd += '\n';
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::SET_VOLTAGE, cmd, false);
}

void Zasilacz::setCurrent_mA(const int &mvalue)
{
    QByteArray cmd("CURR ", 5);
    cmd += convertFloatValue(mvalue, 5000);
    cmd += '\n';
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::SET_CURRENT, cmd, false);
}

void Zasilacz::setVoltageLimit_mV(const int &mvalue)
{
    QByteArray cmd("VOLT:LIM ", 9);
    cmd += convertFloatValue(mvalue, 31000);
    cmd += '\n';
    if (!connected())
        m_worker.command(SerialZasilacz::CONNECT, QByteArray(), false);
    m_worker.command(SerialZasilacz::SET_VOLTAGE_LIMIT, cmd, false);
}

void Zasilacz::setCurrentLimit_mA(const int &mvalue)
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


