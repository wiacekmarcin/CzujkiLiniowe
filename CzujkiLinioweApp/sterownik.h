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
#ifndef NOSERIAL
#include <QSerialPort>
#endif
#include "ustawienia.h"
#include "sterownik_worker.h"

#define NEWINTERFACE

struct RuchSilnikaType {
    bool home;
    bool move;
    bool err;
    bool inter;
};


#ifdef NEWINTERFACE

struct PortInterface : QObject {
   Q_SIGNAL void writeData(const QByteArray &);
   Q_SIGNAL void hasReadData(const QByteArray &);
   Q_OBJECT
};


class SterownikFiltrow : public QObject
{
    Q_OBJECT
public:
    explicit SterownikFiltrow(QObject *parent = nullptr);

    bool isRuch();
    void setUstawienia(Sterownik *sd_, const Ustawienia & u);

public slots:
    void setPos(unsigned short pA, unsigned short pB, unsigned short pC);
    void setZero();
    void setPositionDone(short silnik, RuchSilnikaType ruch);
signals:
    void zerowanieFiltrowDone();
    void ukladFiltrowDone();
    void bladFiltrow(QChar filtr, bool zerowanie);
private :
    bool fARuch;
    bool fBRuch;
    bool fCRuch;

    short actPosfA;
    short actPosfB;
    short actPosfC;

    static constexpr short maxPosF = 6;
    static constexpr float stPerPosImp = 1.0 / maxPosF;
    static constexpr short nrSilnikFA = 3;
    static constexpr short nrSilnikFB = 4;
    static constexpr short nrSilnikFC = 5;


    unsigned long impPosFA;
    unsigned long impPosFB;
    unsigned long impPosFC;

    unsigned long speedFA;
    unsigned long speedFB;
    unsigned long speedFC;

    unsigned long speedZerFA;
    unsigned long speedZerFB;
    unsigned long speedZerFC;


    Sterownik *sd;
    QMutex mutex;
    bool setMove;
    bool isSoftReset;
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
    void setParams(short nrSilnika, bool reverse=false, unsigned int maxImpulse=0,
                   unsigned int baseImpulse=0, unsigned int middleImpulse=0);
    void setPositionSilnik(int silnik, bool home, uint32_t steps, uint32_t impTime);
    void setZerowanieUrzadzen(bool ramiona, bool filtry, bool wozek);
    void setReset();
    void setStopMotor(short nrSilnik);
    void setStopMotorAll();
    void setEnableMotor(short nrSilnik, bool enable);
    void setEnableMotorAll(bool enable);
    void setFiltrPos(unsigned short pA, unsigned short pB, unsigned short pC);
    void setFiltrReset();
    QString getProduct();
    QString getVendor();
    QString getSerialNumber();
    bool getConnected() { bool conn = connected(); emit kontrolerConfigured(conn ? ALL_OK : CLOSE); return conn; }
    const QString &portName() const;

    friend class SterownikWriter;

private slots:
    void handleReadyRead();
    void handleTimeout();
#ifndef NOSERIAL    
    void handleError(QSerialPort::SerialPortError error);
#endif

protected:
    bool connected();
    void setConnected(bool connected);
    void setStop();

signals:
    void error(QString s);
    void debug(QString d);
    void paramsDone(int address, bool success, bool silnik);
    void kontrolerConfigured(int state);
    void deviceName(QString name);
    void positionDone(short silnik, RuchSilnikaType ruch);
    void zdarzenieSilnik(short silnik, short zdarzenie);
    void czujkaOn();
    void progressImp(short silnik, unsigned int position);
    void zerowanieFiltrowDone();
    void setUkladFiltrowDone();
    void bladFiltrow(QChar filtr, bool zerowanie);

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
#ifndef NOSERIAL    
    QSerialPort serialPort;
#endif    
    QThread * writeThread;
    QMutex connMutex;
    SterownikWriter writer;
    QTimer m_timer;
    QByteArray receiveData;
    QEventLoop eventLoop;
    SterownikFiltrow filtry;
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
