#ifndef STEROWNIK_H
#define STEROWNIK_H

#include <QThread>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include <QByteArray>
#include <QVector>
#include <QTimer>
#include <QSerialPort>


class Sterownik;
class QThread;

namespace SerialSterownik {

typedef enum _task {
    IDLE,
    CONNECT,
    SET_CONF,
    SET_HOME,
    SET_POSITION,
    SET_STOP,
    DISCONNECT
} TaskId;

struct TaskExt
{
    TaskExt() : TaskExt(IDLE, QByteArray()) {}
    TaskExt(TaskId _task, const QByteArray & _msg) :
        task(_task),
        msg(_msg)
    {}

    TaskId task;
    QByteArray msg;
} ;

} //namespace

class SerialWorkerSter2 : public QThread
{
    Q_OBJECT
public:

    explicit SerialWorkerSter2(Sterownik * device);
    ~SerialWorkerSter2();

    bool command(SerialSterownik::TaskId task, const QByteArray & msg);

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

    bool write(const QByteArray & req, int currentWaitWriteTimeout);

    SerialSterownik::TaskId getActTaskId();

    static const char* const mapTask[];

signals:
    void debug(QString);
    void deviceName(QString);
    void error(QString s);
    void kontrolerConfigured(bool success, int state);


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
    SerialSterownik::TaskExt actTask;
    QMutex mutex;
    QWaitCondition newTask;
    QMutex mutexRun;
    bool runWorker;
    Sterownik * sd;
    QSerialPort * m_serialPort;
    QVector<SerialSterownik::TaskExt> futureTask;
};

class Sterownik : public QObject
{
    Q_OBJECT
public:
    explicit Sterownik(QObject *parent = nullptr);
    ~Sterownik();

    typedef enum _statusConn {
        NO_FOUND,
        FOUND,
        TO_MANY_FOUND,
        NO_OPEN,
        OPEN,
        NO_READ,
        IDENT_FAILD,
        IDENT_OK,
        ALL_OK,
        CLOSE,
    } StateConn;

    typedef enum _kindValue {
        VOLTAGE_SET,
        CURRENT_SET,
        VOLTAGE_LIMIT,
        CURRENT_LIMIT,
        VOLTAGE_MEAS,
        CURRENT_MEAS,
        OUTPUT
    } KindValueType;


    void setThread(QThread * trh);
    void connectToDevice();
    void closeDevice(bool waitForDone);

protected:

    bool connected();
    void setConnected(bool connected);
    void setStop();
signals:

    void error(QString s);
    void debug(QString d);
    void kontrolerConfigured(bool success, int state);
    void deviceName(QString name);



protected:
    /********************************** INNE FUNKCJE *************************/


    void debugFun(const QString & fun);

    /*********************  JOBY ******************************************/
protected:
    friend class SerialWorkerSter2;

    /************************ JOBY **********************************/

    void connectToSerialJob();
    void closeDeviceJob();
    void checkIdentJob(const SerialSterownik::TaskExt & actTask);

    /***************************** Inne funkcje zwiazane z wiadaomosciamia ********************/




private:
    /* Ustawienia obiektu */
    QMutex connMutex;
    bool m_connected;
    SerialWorkerSter2 m_worker;

    QTimer readMeas;
    short cntTimeout;
};


#endif // ZASILACZ_H


