#ifndef ZASILACZ_H
#define ZASILACZ_H

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
class QThread;

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
    DISCONNECT,
} TaskId;

struct TaskExt
{
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

    QByteArray write(const QByteArray & req, int currentWaitWriteTimeout, int currentReadWaitTimeout, bool waitForRead);

    SerialZasilacz::TaskId getActTaskId();

    static const char* const mapTask[];

signals:
    void debug(QString);
    void deviceName(QString);
    void error(QString s);
    void kontrolerConfigured(bool success, int state);
    void kontrolerSerialNo(QString s);

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

class Zasilacz : public QObject
{
    Q_OBJECT
public:
    explicit Zasilacz(Ustawienia * ust, QObject *parent = nullptr);
    ~Zasilacz();

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
    void setVoltage(const int & mvalue);
    void setCurrent(const int & mvalue);
    void setVoltageLimit(const int & mvalue);
    void setCurrentLimit(const int & mvalue);
    void getVoltage();
    void getCurrent();
    void getVoltageLimit();
    void getCurrentLimit();
    void getVoltageMeas();
    void getCurrentMeas();
    void setOutput(bool on);
    void getOutput();
    void closeDevice(bool waitForDone);
    QString getProduct();
    QString getVendor();
protected:

    bool connected();
    void setConnected(bool connected);
    void setStop();


private slots:
    void timeout100ms();
signals:

    void error(QString s);
    void debug(QString d);
    void kontrolerConfigured(bool success, int state);
    void kontrolerSerialNo(QString s);
    void deviceName(QString name);
    void value(int kind, int value);


protected:
    /********************************** INNE FUNKCJE *************************/


    void debugFun(const QString & fun);
    QByteArray convertFloatValue(const int &mval, const int &maxval);

    /*********************  JOBY ******************************************/
protected:
    friend class SerialWorkerZas;

    /************************ JOBY **********************************/

    void connectToSerialJob();
    void closeDeviceJob();
    void checkIdentJob(const SerialZasilacz::TaskExt & actTask);
    void getVoltageJob(const SerialZasilacz::TaskExt & actTask);
    void getCurrentJob(const SerialZasilacz::TaskExt & actTask);
    void getVoltageLimitJob(const SerialZasilacz::TaskExt & actTask);
    void getCurrentLimitJob(const SerialZasilacz::TaskExt & actTask);
    void getVoltageMeasJob(const SerialZasilacz::TaskExt & actTask);
    void getCurrentMeasJob(const SerialZasilacz::TaskExt & actTask);
    void setParamJob(const SerialZasilacz::TaskExt & actTask);
    void getOutputJob(const SerialZasilacz::TaskExt & actTask);

    /***************************** Inne funkcje zwiazane z wiadaomosciamia ********************/





    void getValueJob(const SerialZasilacz::TaskExt &actTask, KindValueType kind, const QString &debug, const QString &function);


private:
    /* Ustawienia obiektu */
    QMutex connMutex;
    bool m_connected;
    SerialWorkerZas m_worker;

    QTimer readMeas;
    short cntTimeout;

    Ustawienia * u;
};


#endif // ZASILACZ_H
