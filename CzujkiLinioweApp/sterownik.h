#ifndef STEROWNIK_H
#define STEROWNIK_H

#include <QThread>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QTimer>
#include <QByteArray>
#include <QVector>
#include <QEventLoop>

#include "serialmessage.h"
#include "ustawienia.h"
#include "sterownik_worker.h"

#define NEWINTERFACE


#ifdef NEWINTERFACE

struct PortInterface : QObject {
   Q_SIGNAL void writeData(const QByteArray &);
   Q_SIGNAL void hasReadData(const QByteArray &);
   Q_OBJECT
};

class SterownikWriter;

class Sterownik : public QObject
{
    Q_OBJECT

public:
    explicit Sterownik(Ustawienia  *u, QObject *parent = nullptr);
    ~Sterownik();

    typedef enum _statusConn {
        NO_FOUND,
        FOUND,
        NO_OPEN,
        OPEN,
        NO_READ,
        IDENT_FAILD,
        IDENT_OK,
        PARAMS_FAILD,
        PARAMS_OK,
        ALL_OK,
        CLOSE,
    } StateConn;

    typedef enum _zdarzenieSilnika {
        M_ACTIVE = 1,
        M_NOACTIVE,
        M_NOCOMM,
        M_NOPINS,
        M_CONFOK,
    } ZdarzenieSilnikaEnum;

    void closeDevice(bool waitForDone);
    void connectToDevice();
    void disconnectDevice();
    void setParams();
    void setPositionSilnik(int silnik, bool home, uint32_t steps, uint32_t impTime);
    void setReset();
    void setStopMotor(short nrSilnik);
    QString getProduct();
    QString getVendor();
    QString getSerialNumber();
    bool getConnected() { bool conn = connected(); emit kontrolerConfigured(conn ? ALL_OK : CLOSE); return conn; }
    const QString &portName() const;

    friend class SterownikWriter;

private slots:
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);


protected:
    bool connected();
    void setConnected(bool connected);
    void setStop();

signals:
    void error(QString s);
    void debug(QString d);
    void setParamsDone(int address, bool success, bool silnik);
    void kontrolerConfigured(int state);
    void deviceName(QString name);
    void setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt);
    void zdarzenieSilnik(short silnik, short zdarzenie);
    void czujkaOn();
    void progressImp(short silnik, unsigned int position);


    void writeData(const QByteArray &d);

protected:
    /********************************** INNE FUNKCJE *************************/
    void debugFun(const QString & fun);

    /************************ JOBY **********************************/
    void connectToSerialJob();
    void closeDeviceJob();
    void configureMotorsJob();

    /***************************** Inne funkcje zwiazane z wiadaomosciamia ********************/

   bool openDevice();
   bool configureDevice();
   void parseMessage(QByteArray &reply);
   bool write(const QByteArray &currentRequest, int waitWrite);
   bool writeAndRead(const QByteArray &currentRequest, int waitWrite, int waitRead);

private:
    QString m_portName;
    bool m_connected;
    Ustawienia * ust;
    PortInterface interface;
    QSerialPort serialPort;
    QThread writeThread, portThread;
    QMutex connMutex;
    SterownikWriter writer;
    QTimer m_timer;
    QByteArray receiveData;
    QEventLoop eventLoop;
};


#else





#define SERIALLINUX

class Sterownik : public QObject
{
    Q_OBJECT
public:
    explicit Sterownik(Ustawienia  *u, QObject *parent = nullptr);
    ~Sterownik();

    typedef enum _statusConn {
        NO_FOUND,
        FOUND,
        NO_OPEN,
        OPEN,
        NO_READ,
        IDENT_FAILD,
        IDENT_OK,
        PARAMS_FAILD,
        PARAMS_OK,
        ALL_OK,
        CLOSE,
    } StateConn;

    typedef enum _zdarzenieSilnika {
        M_ACTIVE = 1,
        M_NOACTIVE,
        M_NOCOMM,
        M_NOPINS,
        M_CONFOK,
    } ZdarzenieSilnikaEnum;

    void setThread(QThread * thrW, QThread *thrR);
    void closeDevice(bool waitForDone);
    void connectToDevice();
    void disconnectDevice();
    void setParams();
    void setPositionSilnik(int silnik, bool home, uint32_t steps, uint32_t impTime);
    void setReset();
    void setStopMotor(short nrSilnik);
    QString getProduct();
    QString getVendor();
    QString getSerialNumber();
    bool getConnected() { bool conn = connected(); emit kontrolerConfigured(conn ? ALL_OK : CLOSE); return conn; }
    const QString &portName() const;

protected:

    bool connected();
    void setConnected(bool connected);
    void setStop();

signals:
    void error(QString s);
    void debug(QString d);
    void setParamsDone(int address, bool success, bool silnik);
    void kontrolerConfigured(int state);
    void deviceName(QString name);
    void setPositionDone(short silnik, bool home, bool success, bool move, unsigned int steps, unsigned int pos);
    void zdarzenieSilnik(short silnik, short zdarzenie);
    void czujkaOn();

protected:
    /********************************** INNE FUNKCJE *************************/
    void debugFun(const QString & fun);

    /*********************  JOBY ******************************************/
protected:
    friend class SterownikWriter;
    friend class SterownikReader;

    /************************ JOBY **********************************/

    void connectToSerialJob();
    void closeDeviceJob();
    void configureMotorsJob();

    /***************************** Inne funkcje zwiazane z wiadaomosciamia ********************/

   bool openDevice();
   bool configureDevice(bool wlcmmsg);
   bool write(const QByteArray & req, int currentWaitWriteTimeout);
   QByteArray read(int currentWaitWriteTimeout);
   QVector<SerialMessage> parseMessage(QByteArray &reply);


private:
    /* Ustawienia obiektu */
    QString m_portName;
    int m_portNr;
    QMutex connMutex;
    bool m_connected;
    SterownikWriter m_writer;
    SterownikReader m_reader;
    bool emitSignal;
    Ustawienia * ust;

#ifdef SERIALLINUX
    QSerialPort * m_serialPort;
#endif

    short m_silnik;
    unsigned int m_steps;
};

#endif

#endif // STEROWNIK_H
