#ifndef STEROWNIK_H
#define STEROWNIK_H

#include <QThread>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include <QByteArray>
#include <QVector>

#include "serialmessage.h"
#include "ustawienia.h"
#include "sterownik_worker.h"

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

    void setThread(QThread * thrW, QThread *thrR);
    void closeDevice(bool waitForDone);
    void connectToDevice();
    void setParams();
    void setPositionSilnik(int silnik, bool home, uint32_t steps = 0);
    void setReset();
    QString getProduct();
    QString getVendor();
    QString getSerialNumber();
protected:

    bool connected();
    void setConnected(bool connected);
    void setStop();

signals:
    void error(QString s);
    void debug(QString d);
    void setParamsDone(int address, bool success, bool silnik);
    void kontrolerConfigured(bool success, int state);
    void deviceName(QString name);
    void setPositionDone(bool home, bool success, unsigned int steps);

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

    /***************************** Inne funkcje zwiazane z wiadaomosciamia ********************/

   bool openDevice();
   bool configureDevice(bool wlcmmsg);
   bool write(const QByteArray & req, int currentWaitWriteTimeout);
   QByteArray read(int currentWaitWriteTimeout);
   SerialMessage parseMessage(QByteArray &reply);


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



#endif // STEROWNIK_H
