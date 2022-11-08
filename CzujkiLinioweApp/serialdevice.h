#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QThread>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include <QByteArray>
#include <QVector>

#include "serialmessage.h"
#include "ustawienia.h"

#define SERIALLINUX

class SerialDevice;
class QThread;

class SerialWorkerReader : public QThread
{
    Q_OBJECT
public:
    explicit SerialWorkerReader(SerialDevice * device);
    ~SerialWorkerReader();

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
    SerialDevice * sd;
};



/**
 * @brief The SerialWorker class
 * Klasa wątku w którym chodzi sterowanie dozownikime
 * Normalnie wątek jest zawieszony do czasu podania i
 * intrukcji za pomocą funkcji command.
 * @param actTask - aktualne wykonywane zadanie, na IDLE - wątek "wisi"
 * @param mutex - ustawianie zadania chronione mutexem
 * @param newTask - zmienna informujaca, że jest nowe zadanie
 * @param sd - pointer to device
 */
class SerialWorkerWriter : public QThread
{
    Q_OBJECT
public:
    /**
     * Zadania,
     * IDLE -nic nierób, CONNECT - staraj się połączyć z urządzeniem, CONFIGURE - skonfiguruj urządzenie,
     * RESET - resetuj sterownik silników, SET_PARAMS - ustaw parametry sterownika silników,
     * SET_POSITION - ustaw konkretną pozycję dozownika, SET_HOME - ustaw dozownik w pozycji bazowej,
     * SET_ROLETA - ustaw rolete, SET_ROLETA_HOME - zjedz roleta w dol
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

    explicit SerialWorkerWriter(SerialDevice * device);
    ~SerialWorkerWriter();

    /**
     * @brief command
     * Ustawienie nowego zadania
     * @param task - nowe zadanie IDLE, CONNECT, CONFIGURE, RESET, SET_PARAMS, SET_POSITION, SET_HOME, SET_CYCLE,
     * SET_STEPS, SET_ECHO2
     */
    bool command(Task task);

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
    SerialDevice * sd;
    QVector<Task> futureTask;
};


/**
 * @brief The SerialDevice class
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
class SerialDevice : public QObject
{
    Q_OBJECT
public:
    explicit SerialDevice(Ustawienia  *u, QObject *parent = nullptr);
    ~SerialDevice();

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
    void setPositionHome();

    /**
     * @brief setSteps - ustawia ilość kroków dla przesunięcia
     * @param x - ilosc przesuniecia w impulsach os X
     * @param y - ilosc przesuniecia w impulsach os Y
     */
    void setPosition(uint32_t x, uint32_t y);

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
    void setParamsDone(bool success);

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
    void setPositionDone(bool home, bool success);

protected:
    /********************************** INNE FUNKCJE *************************/


    void debugFun(const QString & fun);

    /*********************  JOBY ******************************************/
protected:
    friend class SerialWorkerWriter;
    friend class SerialWorkerReader;

    /************************ JOBY **********************************/

    /**
     * @brief connectToSerialJob - wyszukiwanie i otworzenie sterownika
     */
    void connectToSerialJob();


    /**
     * @brief setParamsJob - ustawia paremetry sterownika silnika
     */
    void setParamsJob();

    /**
     * @brief setHomeJob - ustawia pozycję bazową dla wybranego dozownika
     */
    void setHomeJob();

    /**
     * @brief setPosJob - ustawia żądana pozycję
     */
    void setPosJob();

    /**
     * @brief closeDevice zamyka urzadzenia
     */
    void closeDeviceJob();

    /**
     * @brief resetJob - resetuje kontroler silnikow - zatrzymuje go
    */
    void resetJob();

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
    SerialWorkerWriter m_writer;
    SerialWorkerReader m_reader;
    bool emitSignal;
    Ustawienia * ust;

#ifdef SERIALLINUX
    QSerialPort * m_serialPort;
#endif


};



#endif // SERIALDEVICE_H