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

#define SERIALLINUX

class SterownikSerial;
class QThread;

class SterownikWorkerReader : public QThread
{
    Q_OBJECT
public:
    explicit SterownikWorkerReader(SterownikSerial *device);
    ~SterownikWorkerReader();

    /**
     * @brief setStop
     * Zatrzymanie pracy workera
     */
    void setStop();

signals:
    void debug(QString);

protected:
    /**
     * @brief run
     * Fukcja workera
     */
    void run();

    void debugFun(const QString &s);


private:
    QMutex mutex;

    QMutex mutexRun;
    bool runWorker;
    SterownikSerial * sd;
};

class SterownikWorkerWriter : public QThread
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

    explicit SterownikWorkerWriter(SterownikSerial * device);
    ~SterownikWorkerWriter();

    /**
     * @brief command
     * Ustawienie nowego zadania
     * @param task - nowe zadanie IDLE, CONNECT, CONFIGURE, RESET, SET_PARAMS, SET_POSITION, SET_HOME, SET_CYCLE,
     * SET_STEPS, SET_ECHO2
     */
    bool command(Task task, const QByteArray & data);

    /**
     * @brief setStop
     * Zatrzymanie pracy workera
     */
    void setStop();

    /**
     * @brief setReset - usuniecie pozostalych zadan
     */
    void setReset();


    Task getActTask();

signals:
    void debug(QString);

protected:
    /**
     * @brief run
     * Fukcja workera
     */
    void run();

    void debugFun(const QString &s);


private:
    Task actTask;
    QMutex mutex;
    QWaitCondition newTask;
    QMutex mutexRun;
    bool runWorker;
    SterownikSerial * sd;
    QVector<QPair<Task,QByteArray>> futureTask;
};


/**
 * @brief The Sterownik class
 * Klasa urzadzenia szeregowego, jest odpowiedzialna za sterowanie dozownikiem
 * @param m_portName - nazwa portu szeregowego
 * @param m_portNr - numer portu szeregowego na windowsie np 7 (czyli COM7)
 * @param m_connected - czy urządzenie jest w systemie
 * @param m_configured - czy urządzenie jest skonfigurowane
 * @param m_reverse - konfiguracja obrotów silnika
 * @param m_maxImp - maksymalna ilość impulsów
 * @param m_maxStep - maksymalna ilosc krokow (R - rolety)
 * @param m_worker - obiekt który realizuje komunikację w wątku
 */
class SterownikSerial : public QObject
{
    Q_OBJECT
public:
    explicit SterownikSerial(Ustawienia  *u, QObject *parent = nullptr);
    ~SterownikSerial();

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


    /**
     * @brief setThreads
     * Ustawia wątek do obsługi procesu
     * @param trh
     */
    void setThread(QThread * thrW, QThread *thrR);


    /************ Funkcje sterujące *************************************/
    /**
     * @brief closeDevice zamyka urzadzenia
     * @param waitForDone - czy natychmiast czy poczekac na zakonczenie zadan
     */
    void closeDevice(bool waitForDone);

    /**
     * @brief connectToDevice ustanawia polaczenie z urzadzeniem
     */
    void connectToDevice();

    /**
    * @brief setParams zapamietuje parametry, ktore zostana wpisane do kontrolera w momencie otwarcia
    * @param reverseX czy silnik X chodzi w odwrotnym kierunku
    * @param reverseY czy silnik Y chodzi w odwrotnym kierunku
    * @param reverseR czy silnik R chodzi w odwrotnym kierunku
    * @param maxImpX maksymalna ilosc impulsow dla silnika X
    * @param maxImpY maksymalna ilosc impulsow dla silnika Y
    * @param maxStepX maksymalna ilosc krokow dla silnika X
    * @param maxStepY maksymalna ilosc krokow dla silnika Y
    * @param maxStepR maksymalna ilosc krokow dla silnika R
    * @param minStepR ilosc impulsow wjechania rolety na dol mimo wykrycia czujnika
    * @param speedRolPos predkosco rolety w gore
    * @param speedRolHome predkosco rolety w dol
    */
    void setParams();


    /**
     * @brief setPositionHome - ustawia pozycje bazową dla dozownika
    */
    void setPositionSilnik(int silnik, bool home, uint32_t steps = 0);

    /**
    * @brief setReset
    */
    void setReset();

protected:

    /**
     * @brief connected czy urzadzenie jest podlaczone
     * @return true wszystko OK, false nie podlaczone
     */
    bool connected();

    /**
     * @brief setConnected ustaw podlaczenia
     * @param connected czy podlaczone
     */
    void setConnected(bool connected);

    /**
     * @brief setStop
     * Zatrzymanie pracy workera
     */
    void setStop();

signals:
    /**
     * @brief error wystąpił bład
     * @param s info o błędzie
     */
    void error(QString s);

    /**
     * @brief debug Informacje debugowe
     * @param d
     */
    void debug(QString d);

    /**
     * @brief setParamsDone - ustawianie parametrów zakończone
     * @param success - true powodzenie, false niepowodzenie
     */
    void setParamsDone(int address, bool success, bool silnik);

    /**
     * @brief kontrolerConfigured - znaleziono sterownik i skonfigurowano
     * @param success - czy połączone urzadzenie i jest OK
     * @param state - rodzaj operacji, ktora nie udala sie
     */
    void kontrolerConfigured(bool success, int state);

    /**
     * @brief zwraca nazwe urzadzenia
     * @param name nazwa urzadzenia
     */
    void deviceName(QString name);

    /**
     * @brief setPositionDone - ustawianie pozycji zakończone
     * @param home - czy powrot do bazy true, czy ustawianie pozycji
     * @param success - czy poprawny komunikat
     */
    void setPositionDone(bool home, bool success, unsigned int steps);

protected:
    /********************************** INNE FUNKCJE *************************/


    void debugFun(const QString & fun);

    /*********************  JOBY ******************************************/
protected:
    friend class SterownikWorkerWriter;
    friend class SterownikWorkerReader;

    /************************ JOBY **********************************/

    /**
     * @brief connectToSerialJob - wyszukiwanie i otworzenie sterownika
     */
    void connectToSerialJob();

    /**
     * @brief closeDevice zamyka urzadzenia
     */
    void closeDeviceJob();

    /***************************** Inne funkcje zwiazane z wiadaomosciamia ********************/


    /**
     * @brief openDevice - otwiera urzadzenie
     * @return
     */
    bool openDevice();


    /**
     * @brief configureDeviceJob - konfiguracja sterownika dozowników
     * @return
     */
     bool configureDevice(bool wlcmmsg);

    /**
     * @brief write - zapisz
     * @param req - dane do zapisu
     * @param currentWaitWriteTimeout
     * @param currentReadWaitTimeout
     * @return
     */
    bool write(const QByteArray & req, int currentWaitWriteTimeout);

    QByteArray read(int currentWaitWriteTimeout);

    /**
     * @brief parseMessage
     * @param reply
     * @return
     */
    SerialMessage parseMessage(QByteArray &reply);


private:
    /* Ustawienia obiektu */
    QString m_portName;
    int m_portNr;
    QMutex connMutex;
    bool m_connected;
    SterownikWorkerWriter m_writer;
    SterownikWorkerReader m_reader;
    bool emitSignal;
    Ustawienia * ust;

#ifdef SERIALLINUX
    QSerialPort * m_serialPort;
#endif

    short m_silnik;
    unsigned int m_steps;
};



#endif // STEROWNIK_H
