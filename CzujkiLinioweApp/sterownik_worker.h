#ifndef STEROWNIK_WORKER_H
#define STEROWNIK_WORKER_H

#include <QThread>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include <QByteArray>
#include <QVector>

#include "serialmessage.h"
#include "ustawienia.h"

#define SERIALLINUX

class Sterownik;

class SterownikReader : public QThread
{
    Q_OBJECT
public:
    explicit SterownikReader(Sterownik *device);
    ~SterownikReader();

    void setStop();

signals:
    void debug(QString);

protected:
    void run();
    void debugFun(const QString &s);


private:
    QMutex mutex;

    QMutex mutexRun;
    bool runWorker;
    Sterownik * sd;
};

class SterownikWriter : public QThread
{
    Q_OBJECT
public:
    /**
     * Zadania,
     */
    typedef enum _task {
        IDLE,
        CONNECT,
        SET_PARAMS,
        SET_POSITION,
        SET_HOME,
        DISCONNECT,
        RESET,
    } Task;

    explicit SterownikWriter(Sterownik * device);
    ~SterownikWriter();

    bool command(Task task, const QByteArray & data);
    void setStop();
    void setReset();
    Task getActTask();

signals:
    void debug(QString);

protected:
    void run();
    void debugFun(const QString &s);

private:
    Task actTask;
    QMutex mutex;
    QWaitCondition newTask;
    QMutex mutexRun;
    bool runWorker;
    Sterownik * sd;
    QVector<QPair<Task,QByteArray>> futureTask;
};

#endif // STEROWNIK_WORKER_H
