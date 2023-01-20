#include "sterownik_worker.h"
#include "sterownik.h"
#include <QTime>
#include <QThread>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>

#define DEBUGSER(X) debugFun(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))
#ifndef NEWINTERFACE

SterownikReader::SterownikReader(Sterownik* device):
    QThread(nullptr),
    sd(device)
{
    runWorker = true;
    connect(this, &SterownikReader::debug, device, &Sterownik::debug, Qt::QueuedConnection);
}

SterownikReader::~SterownikReader()
{
    mutex.lock();
    runWorker = false;
    mutex.unlock();
    wait(1000);
}

void SterownikReader::setStop()
{
    const QMutexLocker locker(&mutexRun);
    runWorker = false;
}

void SterownikReader::run()
{
    //qDebug() << "start Recv";
    bool quit;
    QByteArray receiveData;
    do
    {
        mutexRun.lock();
        quit = !runWorker;
        mutexRun.unlock();

        //qDebug() << "read";
        QByteArray responseData = sd->read(500);
        if (responseData.size() > 0) {
            DEBUGSER(QString("[RECV] %1").arg(responseData.toHex(' ').data()));
            receiveData.push_back(responseData);
            sd->parseMessage(receiveData);
        } else {
            QThread::currentThread()->msleep(500);
            mutexRun.lock();
            quit = !runWorker;
            mutexRun.unlock();
            if (quit)
                return;
        }
    } while (!quit);
}

void SterownikReader::debugFun(const QString &s)
{
    emit debug(s);
}

#endif

#define CONN(X) connect(this, &SterownikWriter::X, device, &Sterownik::X, Qt::QueuedConnection)

SterownikWriter::SterownikWriter(Sterownik* device):
    QThread(nullptr)
{
    actTask = IDLE;
    runWorker = true;

    CONN(debug);
    CONN(connectToSerialJob);
    CONN(configureMotorsJob);
    CONN(write);
    CONN(closeDeviceJob);


}

SterownikWriter::~SterownikWriter()
{
    mutex.lock();
    futureTask.clear();
    runWorker = false;
    actTask = IDLE;
    newTask.wakeOne();
    mutex.unlock();
    wait(120000);
}

bool SterownikWriter::command(Task curr, const QByteArray &data)
{
    DEBUGSER(QString("New command %1 [%2]").arg(curr).arg(data.toHex().data()));
    {
        const QMutexLocker locker(&mutexRun);
        if (!runWorker)
            return false;
    }
    const QMutexLocker locker(&mutex);

    futureTask.push_back(QPair<Task,QByteArray>(curr, data));
    newTask.wakeOne();
    return true;
}

void SterownikWriter::setStop()
{
    {
    const QMutexLocker locker(&mutexRun);
    runWorker = false;
    }
    const QMutexLocker locker(&mutex);
    futureTask.clear();
    futureTask.push_back(QPair<Task,QByteArray>(DISCONNECT, QByteArray()));
    newTask.wakeOne();
}

void SterownikWriter::setReset()
{
    const QMutexLocker locker(&mutex);
    futureTask.clear();
}

void SterownikWriter::setStart()
{
    runWorker = true;
}

void SterownikWriter::run()
{
    mutex.lock();
    short zadanie;
    QByteArray msg;
    mutex.unlock();
    bool quit = false;

    do {
        mutex.lock();
        if (futureTask.size() == 0) {
            actTask = IDLE;
            DEBUGSER(QString("No Task"));
            newTask.wait(&mutex);
        } else {
            actTask = futureTask.front().first;
            msg = futureTask.front().second;
            futureTask.pop_front();
        }
        zadanie = actTask;
        mutex.unlock();
        QThread::currentThread()->msleep(50);
        DEBUGSER(QString("actTask = %1").arg(zadanie));
        switch(zadanie) {
        case IDLE:
            break;

        case CONNECT:
            emit connectToSerialJob();
            break;

        case SET_PARAMS:
            emit configureMotorsJob();
            break;

        case SET_HOME:
            emit write(msg, 1000);
            break;

        case SET_POSITION:
            emit write(msg, 1000);
            break;

        case SET_STOP:
            emit write(msg, 1000);
            break;

        case SET_ENABLE:
            emit write(msg, 1000);
            break;

        case DISCONNECT:
            emit closeDeviceJob();
            break;

        case RESET:
            emit write(msg, 1000);
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

void SterownikWriter::debugFun(const QString &s)
{
    emit debug(s);
}

SterownikWriter::Task SterownikWriter::getActTask()
{
    Task ret;
    mutex.lock();
    ret = actTask;
    mutex.unlock();
    return ret;
}

