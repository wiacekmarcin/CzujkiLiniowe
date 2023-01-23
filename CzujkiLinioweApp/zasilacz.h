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

#ifndef NOSERIAL
#include <QSerialPort>
#endif

#include "zasilacz_worker.h"

//#define SERIALLINUX



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
    void setVoltage_mV(const int & mvalue);
    void setCurrent_mA(const int & mvalue);
    void setVoltageLimit_mV(const int & mvalue);
    void setCurrentLimit_mA(const int & mvalue);
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
    bool getConnected() { bool conn = connected(); emit kontrolerConfigured(conn ? ALL_OK : CLOSE); return conn; }
protected:

    bool connected();
    void setConnected(bool connected);
    void setStop();


private slots:
    void timeout100ms();
signals:

    void error(QString s);
    void debug(QString d);
    void kontrolerConfigured(int state);
    void kontrolerSerialNo(QString s);
    void deviceName(QString name);
    void value(int kind, int value);
    void sendMsg(QString msg);
    void recvMsg(QString msg);

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
