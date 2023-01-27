#ifndef ZASILACZ_WORKER_H
#define ZASILACZ_WORKER_H

#include "ustawienia.h"
#include <QThread>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include <QByteArray>
#include <QVector>
#include <QTimer>

#include <QSerialPort>


//#include "ustawienia.h"

//#define SERIALLINUX

class Zasilacz;

namespace SerialZasilacz {

typedef enum _task {
    IDLE,
    CONNECT,
    GET_VOLTAGE,
    GET_CURRENT,
    SET_VOLTAGE,
    SET_CURRENT,
    SET_VOLTAGE_LIMIT,
    SET_CURRENT_LIMIT,
    GET_VOLTAGE_LIMIT,
    GET_CURRENT_LIMIT,
    GET_VOLTAGE_MEAS,
    GET_CURRENT_MEAS,
    SET_OUTPUT,
    GET_OUTPUT,
    OTHER,
    DISCONNECT,
} TaskId;

struct TaskExt
{
    TaskExt() : TaskExt(IDLE, QByteArray(), false) { }
    TaskExt(TaskId _task, const QByteArray & _msg, bool _waitForRead) :
        task(_task)
        , msg(_msg)
        , waitForRead(_waitForRead) {}
    TaskId task;
    QByteArray msg;
    bool waitForRead;
} ;

} //namespace //serialZasilacz

class SerialWorkerZas : public QThread
{
    Q_OBJECT
public:

    explicit SerialWorkerZas(Zasilacz * device);
    ~SerialWorkerZas();

    bool command(SerialZasilacz::TaskId task, const QByteArray & msg, bool wait);

    /**
     * @brief setStop
     * Zatrzymanie pracy workera
     */
    void setStop();

    /**
     * @brief setReset - usuniecie pozostalych zadan
     */
    void setReset();

    void closeDeviceJob();

    QByteArray write(const QByteArray & req, int currentWaitWriteTimeout,
                     int currentReadWaitTimeout, bool waitForRead,
                     bool emitError = true);

    SerialZasilacz::TaskId getActTaskId();

    static const char* const mapTask[];

signals:
    void debug(QString);
    void deviceName(QString);
    void error(QString s);
    void kontrolerConfigured(int state);
    void kontrolerSerialNo(QString s);
    void sendMsg(QString msg);
    void recvMsg(QString msg);

protected:
    /**
     * @brief run
     * Fukcja workera
     */
    void run();

    void debugFun(const QString &s);

    bool openDevice(const QString & portName);


    QList<QStringList> getComPorts();


    bool connectToSerialJob();
    bool checkIdentJob();

private:
    SerialZasilacz::TaskExt actTask;
    QMutex mutex;
    QWaitCondition newTask;
    QMutex mutexRun;
    bool runWorker;
    Zasilacz * sd;
    QSerialPort * m_serialPort;

    
    QVector<SerialZasilacz::TaskExt> futureTask;
};

#endif // ZASILACZ_WORKER_H
