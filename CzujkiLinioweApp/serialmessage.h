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
        PROGRESS_REQ = 6,
        PROGRESS_REP = 7,
        CONF_MSG_REQ = 8,
        CONF_MSG_REP = 9,
        MOVE_MSG_REQ = 10,
        MOVE_MSG_REP = 11,
        LAST_REQ = 12,
        LAST_REP = 13,
        RESET_REQ = 14,
        ECHO_CLEAR_REQ = 0,
        ECHO_CLEAR_REP = 15,
        INV_MSG = 16,

    } CMD;

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
        CONF_MEGA_REPLY,
    } ParseReply;

    /**
     * @brief welcomeMsg - wiadomość powitalna
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray welcomeMsg();


    static QByteArray configMotorMsg(short silnik, bool reverse, int maxSteps, int baseSteps, int middleStep);
    static QByteArray configKontrolerMsg();
    /**
     * @brief setPositionHome
     * @param addr - adres kontrolera 1-9 silnik
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray setPositionHome(uint8_t addr, uint32_t imp);

    /**
     * @brief setPosition
     * @param addr - adres kontrolera 1-9 silnik
     * @param x - pozycja w krokach do ustawienia
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray setPosition(uint8_t addr, uint32_t x, uint32_t imp);

    static QByteArray resetSilniki();



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
    bool isKontroler() { return silnik == 0;}

    unsigned int getSteps() const;

    bool getActive(short nr) const;

protected:

    bool checkHead(QByteArray &arr, uint8_t &addr, uint8_t &options, uint8_t & cmd, uint8_t & len, QByteArray & data);
    static QByteArray prepareMessage(uint8_t cmd, uint8_t addr, uint8_t options, uint8_t tab[], uint8_t len);
    static uint32_t getNumber(const QByteArray &data);

private:
    ParseReply m_parseReply;


    int silnik;
    unsigned int steps;
    bool active[10]; // kontroler + 9 silnikow

    static constexpr short addrKontrolera = 10;

};

#endif // SERIALMESSAGE_H

