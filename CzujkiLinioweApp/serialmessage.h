#ifndef SERIALMESSAGE_H
#define SERIALMESSAGE_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>



/**
 * @brief The SerialMessage class
 * Klasa reprezuntująca wiadomość wysyłaną do sterownika dozowników
 * NOP_REQ i NOP_REP - pusta wiadomość, WELCOME_REQ i WELCOME_REP -
 * wiadomość powitalna, SET_PARAM_REQ i SET_PARAM_REP - ustawianie
 * parametrów, POSITION_REQ i POSITION_REP - żądanie ustawienia
 * pozycji, ECHO_REQ i ECHO_REP - wiadomość echo, MOVEHOME_REQ i
 * MOVEHOME_REP - ustawianie pozycji bazowej, RESET_REQ i RESET_REP
 * - reset sterownika silnika, ERROR_REP - bład
 *
 * @param m_parseReply - odpowiedż po sparsowaniu
 */
class SerialMessage
{

public:

    explicit SerialMessage();

    ~SerialMessage();

    /**
     * Typy wiadomości
     */
    typedef enum _cmd {
        WELCOME_MSG_REQ = 1,
        WELCOME_MSG_REP = 2,

        CONF_REQ = 8,
        CONF_REP = 9,
        MOVE_REQ = 10,
        MOVE_REP = 11,
        LAST_REQ = 12,
        LAST_REP = 13,
        MEASURENT_REP = 14,
        ECHO_CLEAR_REQ = 0,
        ECHO_CLEAR_REP = 15,
        INV_MSG = 16,

    } CMD;

    typedef enum _status {
        START_XY,
        START_X,
        END_X,
        START_Y,
        END_Y,
        END_XY,
        START_R,
        END_R,
        ERROR_XY,
        ERROR_R
    } StatusWork;



    /**
     * Możliwe statusy odpowiedzi
     */
    typedef enum _action {
        INVALID_REPLY = -4,
        CLR_MESSAGE = -2,
        EMPTY_MESSAGE = -1,
        INPROGRESS_REPLY = 0,
        WELCOME_REPLY = 1,
        CONF_REPLY,
        MOVEHOME_REPLY,
        POSITION_REPLY,
        RESET_REPLY,
        CONF_INT_REPLY
    } ParseReply;

    /**
     * @brief welcomeMsg - wiadomość powitalna
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray welcomeMsg();


    static QByteArray configMotorMsg(short silnik, bool en, bool reverse, int maxSteps, int baseSteps, int delayStep);

    /**
     * @brief setPositionHome
     * @param addr - adres kontrolera 1-9 silnik
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray setPositionHome(uint8_t addr);

    /**
     * @brief setPosition
     * @param addr - adres kontrolera 1-9 silnik
     * @param x - pozycja w krokach do ustawienia
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray setPosition(uint8_t addr, uint32_t x);

    static QByteArray resetSterownik(uint8_t addr);
    

    /**
     * @brief getParseReply zwraca typ sparsowanej wiadomości
     * @return
     */
    ParseReply getParseReply() const;

    /**
     * @brief parseCommand - parsuje komendę
     * @param arr - bajty do sparsowania
     * @return - wynik czy poprawna instrukcja
     */
    bool parseCommand(QByteArray &arr);

    /**
     * @brief isInvalidReply
     * @brief isInProgressReply
     * @brief isTimeoutWriteReply;
     * @brief isTimeoutReadReply;
     */
    bool isInvalidReply();
    bool isInProgressReply();



    int getSilnik() const;

protected:

    bool checkHead(QByteArray &arr, uint8_t &addr, uint8_t &options, uint8_t & cmd, uint8_t & len, QByteArray & data);
    static QByteArray prepareMessage(uint8_t cmd, uint8_t addr, uint8_t options, uint8_t tab[], uint8_t len);
    static uint32_t getNumber(const QByteArray &data);

private:
    ParseReply m_parseReply;


    int silnik;


};

#endif // SERIALMESSAGE_H

