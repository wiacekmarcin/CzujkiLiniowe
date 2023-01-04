#ifndef STEROWNIK_WORKER_H
#define STEROWNIK_WORKER_H

#include <QThread>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include <QByteArray>
#include <QVector>

#define NEWINTERFACE
#ifdef NEWINTERFACE

class Sterownik;

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
        SET_STOP,
        DISCONNECT,
        RESET,
    } Task;

    explicit SterownikWriter(Sterownik * device);
    ~SterownikWriter();

    bool command(Task task, const QByteArray & data);
    void setStop();
    void setReset();
    Task getActTask();

    void setStart();
signals:
    void debug(QString);
    void connectToSerialJob();
    void configureMotorsJob();
    void write(QByteArray, int writeWait);
    void closeDeviceJob();

protected:
    void run();
    void debugFun(const QString &s);

private:
    Task actTask;
    QMutex mutex;
    QWaitCondition newTask;
    QMutex mutexRun;
    bool runWorker;
    QVector<QPair<Task,QByteArray>> futureTask;
};



#else
#define SERIALLINUX

class Sterownik;

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
        SET_STOP,
        DISCONNECT,
        RESET,
    } Task;

    explicit SterownikWriter(Sterownik * device);
    ~SterownikWriter();

    bool command(Task task, const QByteArray & data);
    void setStop();
    void setReset();
    Task getActTask();

    void setStart();
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
#endif
#endif // STEROWNIK_WORKER_H
