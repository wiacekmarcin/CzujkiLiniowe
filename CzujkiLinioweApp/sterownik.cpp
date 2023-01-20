#include "sterownik.h"
#include <QTime>
#include <QThread>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>
#include <QEventLoop>

#include "serialmessage.h"

#ifdef NEWINTERFACE

#define DEBUGSER(X) debugFun(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))

SterownikFiltrow::SterownikFiltrow(QObject *parent)
    : QObject{parent}
    , fARuch(false)
    , fBRuch(false)
    , fCRuch(false)
    , actPosfA(-1)
    , actPosfB(-1)
    , actPosfC(-1)
    , impPosFA(0)
    , impPosFB(0)
    , impPosFC(0)
    , speedFA(100000)
    , speedFB(100000)
    , speedFC(100000)
    , speedZerFA(100000)
    , speedZerFB(100000)
    , speedZerFC(100000)
    , sd(nullptr)
    , setMove(false)
{

}

bool SterownikFiltrow::isRuch()
{
    QMutexLocker lock(&mutex);
    if (fARuch)
        return true;
    if (fBRuch)
        return true;
    if (fCRuch)
        return true;
    return false;
}

void SterownikFiltrow::setUstawienia(Sterownik *sd_, const Ustawienia &u)
{
    sd = sd_;
    impPosFA = round(stPerPosImp * u.getMotorMaksIloscImp(nrSilnikFA));
    impPosFB = round(stPerPosImp * u.getMotorMaksIloscImp(nrSilnikFB));
    impPosFC = round(stPerPosImp * u.getMotorMaksIloscImp(nrSilnikFC));

    speedFA = u.getMotorCzasMiedzyImpNormal(nrSilnikFA);
    speedFB = u.getMotorCzasMiedzyImpNormal(nrSilnikFB);
    speedFC = u.getMotorCzasMiedzyImpNormal(nrSilnikFC);

    speedZerFA = u.getMotorCzasMiedzyImpZerow(nrSilnikFA);
    speedZerFB = u.getMotorCzasMiedzyImpZerow(nrSilnikFB);
    speedZerFC = u.getMotorCzasMiedzyImpZerow(nrSilnikFC);
}

void SterownikFiltrow::setPos(unsigned short pA, unsigned short pB, unsigned short pC)
{
    //qDebug() << "-------\n" << __FILE__ << __LINE__ << "Uklad Filtrow" << pA << pB << pC;
    if (sd == nullptr)
        return;

    do {
        QMutexLocker lock(&mutex);
        fARuch = pA != actPosfA;
        fBRuch = pB != actPosfB;
        fCRuch = pC != actPosfC;
    } while (false);
    if (pA != actPosfA) {
        //qDebug() << "Ustawiam silnik A na " << pA;
        setMove = true;
        sd->setPositionSilnik(nrSilnikFA, false, pA*impPosFA, speedFA);
        actPosfA = pA;
    }
    if (pB != actPosfB) {
        //qDebug() << "Ustawiam silnik B na " << pB;
        setMove = true;
        sd->setPositionSilnik(nrSilnikFB, false, pB*impPosFB, speedFB);
        actPosfB = pB;
    }
    if (pC != actPosfC) {
        //qDebug() << "Ustawiam silnik C na " << pC;
        setMove = true;
        sd->setPositionSilnik(nrSilnikFC, false, pC*impPosFC, speedFC);
        actPosfC = pC;
    }
    if (!setMove) {
        emit ukladFiltrowDone();
    }
}

void SterownikFiltrow::setZero()
{
    qDebug() << __FILE__ << __LINE__ << "Zerowanie";
    if (sd == nullptr)
        return;
    //if (actPosfA == 0 && actPosfB == 0 && actPosfC == 0) {
    //    emit ukladFiltrowDone();
    //    return;
    //}
    isSoftReset = true;
    do {
        QMutexLocker lock(&mutex);
        fARuch = true;
        fBRuch = true;
        fCRuch = true;
    } while (false);
    //qDebug() << "setPosition";
    setMove = true;
    sd->setPositionSilnik(nrSilnikFA, false, 0, speedZerFA);
    sd->setPositionSilnik(nrSilnikFB, false, 0, speedZerFB);
    sd->setPositionSilnik(nrSilnikFC, false, 0, speedZerFC);
    actPosfA = actPosfB = actPosfC = 0;
}

void SterownikFiltrow::setPositionDone(short silnik, RuchSilnikaType r)
{
    //qDebug() << __FILE__ << __LINE__ << silnik << "home" << r.home << "move" << r.move << "error" << r.err << "interrupt" << r.inter;
    if (silnik != nrSilnikFA &&  silnik != nrSilnikFB && silnik != nrSilnikFC)
        return;

    if (!setMove)
        return;

    //qDebug() << "Filtr " << (silnik == nrSilnikFA ? "A" : (silnik == nrSilnikFC ? "B" : "C")) << (r.home ? "wyzerowany" : "ustawiony") << "Blad" << r.err;
    if (r.err) {
        QMutexLocker lock(&mutex);
        fARuch = false;
        fBRuch = false;
        fCRuch = false;
        emit bladFiltrow((silnik == nrSilnikFA ? 'A' : (silnik == nrSilnikFB ? 'B' : 'C')), r.home);
        return ;
    }

    if (silnik == nrSilnikFA) {
        QMutexLocker lock(&mutex);
        fARuch = r.move;
    } else if (silnik == nrSilnikFB) {
        QMutexLocker lock(&mutex);
        fBRuch = r.move;
    } else if (silnik == nrSilnikFC) {
        QMutexLocker lock(&mutex);
        fCRuch = r.move;
    }

    if (!isRuch()) {
        if (r.home || isSoftReset) {
            //qDebug() << "Zerowanie fitrow zakonczone";
            setMove = false;
            isSoftReset = false;
            emit zerowanieFiltrowDone();
        } else {
            //qDebug() << "Pozycjonowanie filtrow zakonczone";
            setMove = false;
            emit ukladFiltrowDone();
        }
    }
}

Sterownik::Sterownik(Ustawienia *u, QObject *parent)
    : QObject(parent),
      m_portName(""),
      m_connected(false),
      ust(u),
      writer(this),
      filtry(this)
{
    //QThread::currentThread()->setObjectName("mainThread");
    //portThread.setObjectName("portThread");
    //qDebug() << QThread::currentThread();

    //serialPort.moveToThread(&portThread);
    //portThread.start();
    writeThread = new QThread(this);
    writer.moveToThread(writeThread);
    writeThread->start();

    filtry.setUstawienia(this, *u);

    connect(&serialPort, &QSerialPort::readyRead, this, &Sterownik::handleReadyRead);
    connect(&serialPort, &QSerialPort::errorOccurred, this, &Sterownik::handleError);
    connect(&m_timer, &QTimer::timeout, this, &Sterownik::handleTimeout);
    connect(&filtry, &SterownikFiltrow::bladFiltrow, this, &Sterownik::bladFiltrow);
    connect(&filtry, &SterownikFiltrow::ukladFiltrowDone, this, &Sterownik::setUkladFiltrowDone);
    connect(&filtry, &SterownikFiltrow::zerowanieFiltrowDone, this, &Sterownik::zerowanieFiltrowDone);

}

bool Sterownik::write(const QByteArray &currentRequest, int waitWrite)
{
    if (currentRequest.size() > 0)
    {
        //() << "write " << currentRequest.size() << "[" << currentRequest.toHex().toStdString().data() << "]";
        int sendsize = serialPort.write(currentRequest);
        bool ret = serialPort.waitForBytesWritten(waitWrite);
        //qDebug() << ret;
        return ret && sendsize == currentRequest.size();
    }
    return true;
}

bool Sterownik::writeAndRead(const QByteArray &currentRequest, int waitWrite, int waitRead)
{
    if (!write(currentRequest, waitWrite)) {
        DEBUGSER("Write Error");
        return false;
    }

    QTimer timer;
    timer.setSingleShot(true);

    unsigned long befSize = receiveData.size();
    timer.start(waitRead);
    connect( &timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit );
    //qDebug() << "wait for data";
    eventLoop.exec(); //wait for rec data

    if(timer.isActive() && receiveData.size() - befSize) {
        //qDebug() << __FILE__ << __LINE__ << "true";
        return true;
    } else {
        //qDebug() << __FILE__ << __LINE__ << "false";
        return false;
    }
}

Sterownik::~Sterownik()
{
    setConnected(false);
    m_timer.stop();
    writer.setReset();
    writer.setStop();
    writer.quit();
    writer.wait();
    eventLoop.quit();
    writeThread->quit();
    writeThread->wait();
    //QThread::currentThread()->wait();
    //portThread.quit();
    //portThread.wait();
}

void Sterownik::setStop()
{
    writer.setReset();
    writer.setStop();
}

void Sterownik::connectToDevice()
{
    if (connected()) {
        emit kontrolerConfigured(ALL_OK);
    } else {
        writer.setReset();
        writer.setStart();
        writer.start();
        writer.command(SterownikWriter::CONNECT, QByteArray());
        //portThread.start();
    }
}

void Sterownik::disconnectDevice()
{
    if (connected()) {
        writer.command(SterownikWriter::DISCONNECT, QByteArray());
    } else {
        emit kontrolerConfigured(CLOSE);
    }
}

void Sterownik::setParams(short nrSilnika, bool reverse, unsigned int maxImpulse,
                          unsigned int baseImpulse, unsigned int middleImpulse)
{
    if (!connected())
        writer.command(SterownikWriter::CONNECT, QByteArray());
    if (nrSilnika > 0 && nrSilnika < 10) {
        writer.command(SterownikWriter::SET_PARAMS, SerialMessage::configMotorMsg(
                           nrSilnika, reverse, maxImpulse, baseImpulse, middleImpulse));
    } else if (nrSilnika == 0){
        writer.command(SterownikWriter::SET_PARAMS, SerialMessage::configKontrolerMsg());
    }
}

void Sterownik::setPositionSilnik(int silnik, bool home, unsigned int steps, unsigned int impTime)
{
    //qDebug() << __FILE__ << __LINE__ << silnik << home << steps << impTime;
    if (!connected())
        writer.command(SterownikWriter::CONNECT, QByteArray());

    if (home)
        writer.command(SterownikWriter::SET_HOME,
                         SerialMessage::setPositionHome(silnik, impTime));
    else
        writer.command(SterownikWriter::SET_POSITION,
                       SerialMessage::setPosition(silnik,  steps, impTime));
}

void Sterownik::setZerowanieUrzadzen(bool ramiona, bool filtry, bool wozek)
{
    for (short id = 1; id <= 9; ++id) {
        if (ramiona && (id == 1 || id == 2 || id == 8 || id == 9))
            setPositionSilnik(id, true, 0, ust->getMotorCzasMiedzyImpZerow(id));

        if (wozek && (id == 6 ||  id == 7))
            setPositionSilnik(id, true, 0, ust->getMotorCzasMiedzyImpZerow(id));

        if (filtry && (id == 3 || id == 4 || id == 5))
            setPositionSilnik(id, true, 0, ust->getMotorCzasMiedzyImpZerow(id));

    }
}

bool Sterownik::connected()
{
    const QMutexLocker locker(&connMutex);
    return m_connected;
}

void Sterownik::setConnected(bool connected)
{
    const QMutexLocker locker(&connMutex);
    m_connected = connected;
}

void Sterownik::closeDeviceJob()
{
    DEBUGSER("CLOSING DEVICE");
    writer.setStop();
    writer.wait();
    //portThread.quit();

    if (serialPort.isOpen()) {
        serialPort.flush();
        serialPort.clear();
        serialPort.clearError();
        serialPort.close();

    }
    setConnected(false);
    emit kontrolerConfigured(CLOSE);
    emit deviceName("");
    DEBUGSER("CLOSE DEVICE");
}
void Sterownik::setReset()
{
    if (!connected())
        writer.command(SterownikWriter::CONNECT, QByteArray());

    if (connected()) {
        writer.command(SterownikWriter::RESET, SerialMessage::resetSilniki());
    }
}

void Sterownik::setStopMotor(short nrSilnik)
{
    if (!connected())
        writer.command(SterownikWriter::CONNECT, QByteArray());

    if (connected()) {
        writer.command(SterownikWriter::SET_STOP, SerialMessage::stopSilnik(nrSilnik));
    }
}

void Sterownik::setStopMotorAll()
{
    if (!connected())
        writer.command(SterownikWriter::CONNECT, QByteArray());

    if (connected()) {
        writer.command(SterownikWriter::SET_STOP, SerialMessage::stopSilnikAll());
        //writer.command(SterownikWriter::SET_STOP, SerialMessage::stopSilnik(1));
        //writer.command(SterownikWriter::SET_STOP, SerialMessage::stopSilnik(2));
        //writer.command(SterownikWriter::SET_STOP, SerialMessage::stopSilnik(3));
        //writer.command(SterownikWriter::SET_STOP, SerialMessage::stopSilnik(4));
        //writer.command(SterownikWriter::SET_STOP, SerialMessage::stopSilnik(5));
        //writer.command(SterownikWriter::SET_STOP, SerialMessage::stopSilnik(6));
        //writer.command(SterownikWriter::SET_STOP, SerialMessage::stopSilnik(7));
        //writer.command(SterownikWriter::SET_STOP, SerialMessage::stopSilnik(8));
        //writer.command(SterownikWriter::SET_STOP, SerialMessage::stopSilnik(9));
    }
}

void Sterownik::setEnableMotor(short nrSilnik, bool enable)
{
    if (!connected())
        writer.command(SterownikWriter::CONNECT, QByteArray());

    if (connected()) {
        writer.command(SterownikWriter::SET_ENABLE, SerialMessage::enableSilnik(nrSilnik, enable));
    }
}

void Sterownik::setEnableMotorAll(bool enable)
{
    if (!connected())
        writer.command(SterownikWriter::CONNECT, QByteArray());

    if (connected()) {
        writer.command(SterownikWriter::SET_ENABLE, SerialMessage::enableSilnikAll(enable));
        //writer.command(SterownikWriter::SET_ENABLE, SerialMessage::enableSilnik(1, enable));
        //writer.command(SterownikWriter::SET_ENABLE, SerialMessage::enableSilnik(2, enable));
        //writer.command(SterownikWriter::SET_ENABLE, SerialMessage::enableSilnik(3, enable));
        //writer.command(SterownikWriter::SET_ENABLE, SerialMessage::enableSilnik(4, enable));
        //writer.command(SterownikWriter::SET_ENABLE, SerialMessage::enableSilnik(5, enable));
        //writer.command(SterownikWriter::SET_ENABLE, SerialMessage::enableSilnik(6, enable));
        //writer.command(SterownikWriter::SET_ENABLE, SerialMessage::enableSilnik(7, enable));
        //writer.command(SterownikWriter::SET_ENABLE, SerialMessage::enableSilnik(8, enable));
        //writer.command(SterownikWriter::SET_ENABLE, SerialMessage::enableSilnik(9, enable));
    }
}

void Sterownik::setFiltrPos(unsigned short pA, unsigned short pB, unsigned short pC)
{
    filtry.setPos(pA, pB, pC);
}

void Sterownik::setFiltrReset()
{
    filtry.setZero();
}

void Sterownik::configureMotorsJob()
{
    for (short i = 1 ; i <= 9; ++i) {
        QByteArray msgBA = SerialMessage::configMotorMsg(
            i, ust->getMotorOdwrocObroty(i),
            ust->getMotorMaksIloscImp(i), ust->getMotorIloscImpBaza(i),
            ust->getMotorIloscImpSrodek(i));

        if (!write(msgBA, 500)) {
            emit error(QString("Nie udalo sie zapisac konfiguracji dla %1").arg(i));
            emit kontrolerConfigured(PARAMS_FAILD);
        }
        QThread::currentThread()->msleep(100);
    }
    QByteArray msgBA = SerialMessage::configKontrolerMsg();
    if (!write(msgBA, 500)) {
        emit error(QString("Nie mozna zapisac konfiguracji dla kontrolera"));
        emit kontrolerConfigured(PARAMS_FAILD);
    }
}

bool Sterownik::configureDevice()
{
    auto r1 = writeAndRead(SerialMessage::welcomeMsg(), 1000, 2000);
    if (!r1)
        return false;


    SerialMessage msg;
    msg.parseCommand(receiveData);

    if (msg.getParseReply() != SerialMessage::WELCOME_REPLY) {
        emit error(QString("Invalid Welcome Msg"));
        emit kontrolerConfigured(IDENT_FAILD);
        return false;
    }

    emit kontrolerConfigured(IDENT_OK);

    for (short i = 1 ; i <= 9; ++i) {
        QByteArray msgBA = SerialMessage::configMotorMsg(
            i, ust->getMotorOdwrocObroty(i),
            ust->getMotorMaksIloscImp(i), ust->getMotorIloscImpBaza(i),
            ust->getMotorIloscImpSrodek(i));

        if (!write(msgBA, 500)) {
            emit error(QString("Nie udalo sie zapisac konfiguracji dla %1").arg(i));
            emit kontrolerConfigured(PARAMS_FAILD);
            return false;
        }
    }

    QByteArray msgBA = SerialMessage::configKontrolerMsg();
    auto r2 = writeAndRead(msgBA, 500, 2000);
    if (!r2)
    {
        emit error(QString("Nie mozna zapisac konfiguracji dla kontrolera"));
        emit kontrolerConfigured(PARAMS_FAILD);
        return false;
    }


    SerialMessage msgConf;
    msgConf.parseCommand(receiveData);

    if (msgConf.getParseReply() != SerialMessage::CONF_MEGA_REPLY || !msgConf.isKontroler()) {
        emit error(QString("Bledna odpowiedz dla kontrolera"));
        emit kontrolerConfigured(PARAMS_FAILD);
        return false;
    }
    for (short s = 1; s < 10; ++s) {
        if (msgConf.getActive(s) && msgConf.getConnected(s) && msgConf.getPinsOk(s)) {
            DEBUGSER(QString("Silnik %1 aktywny").arg(s));
            emit zdarzenieSilnik(s, M_ACTIVE);
        } else if (msgConf.getActive(s) && msgConf.getConnected(s) && !msgConf.getPinsOk(s)) {
            DEBUGSER(QString("Silnik %1 brak połęcznia z pinami").arg(s));
            emit zdarzenieSilnik(s, M_NOPINS);
        } else if (msgConf.getActive(s) && !msgConf.getConnected(s)) {
            DEBUGSER(QString("Silnik %1 brak komunikacji i2c").arg(s));
            emit zdarzenieSilnik(s, M_NOCOMM);
        } else {
            DEBUGSER(QString("Silnik %1 brak modulu").arg(s));
            emit zdarzenieSilnik(s, M_NOACTIVE);
        }
    }
    while (receiveData.size() > 0) {
        SerialMessage msg;
        msg.parseCommand(receiveData);
        if (msg.getParseReply() != SerialMessage::CONF_REPLY) {
            emit error(QString("Bledna odpowiedz dla kontrolera"));
            emit kontrolerConfigured(PARAMS_FAILD);
            return false;
        }
        DEBUGSER(QString("Silnik %1 konfiguracja ok").arg(msg.getSilnik()));
        emit zdarzenieSilnik(msg.getSilnik(), M_CONFOK);
    }

    emit kontrolerConfigured(PARAMS_OK);
    return true;
}

bool Sterownik::openDevice()
{
    serialPort.setPort(QSerialPortInfo(m_portName));
    emit deviceName(serialPort.portName());

    if (!serialPort.open(QIODevice::ReadWrite)) {
        emit error(QString(QObject::tr("Nie mozna otworzyc urzadzenia %1, error  %2")).arg(serialPort.portName()).
                   arg(serialPort.errorString()));
        emit kontrolerConfigured(NO_OPEN);
        return false;
    }
    serialPort.clear();
    serialPort.clearError();
    serialPort.flush();
    //serialPort.reset();
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    serialPort.setBaudRate(QSerialPort::Baud115200);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setReadBufferSize(32);

    QThread::currentThread()->sleep(1);

    emit kontrolerConfigured(OPEN);
    receiveData.clear();
    QByteArray startBuf(20, '\0');
    DEBUGSER(QString("[%1]").arg(startBuf.toHex(' ').data()));
    if (writeAndRead(startBuf, 2500, 2500)) {
        while (receiveData.size()) {
            if (receiveData.front() != (char)0x0f)
                return false;
            receiveData.remove(0, 1);
        }
        return true;
    }
    return false;
}

void Sterownik::parseMessage(QByteArray &reply)
{
    QMap<int, QString> errMap;
    errMap[SerialMessage::INVALID_REPLY] = "Nie prawidłowa odpowiedź";
    errMap[SerialMessage::CLR_MESSAGE] = "Wiadomość czyszcząca";
    errMap[SerialMessage::EMPTY_MESSAGE] = "Pusta wiadomość";
    errMap[SerialMessage::INPROGRESS_REPLY] = "Wiadomość przerwana";
    errMap[SerialMessage::WELCOME_REPLY] = "Wiadomość powitalna";
    errMap[SerialMessage::CONF_REPLY] = "Wiadomość konfiguracyjna";
    errMap[SerialMessage::MOVEHOME_REPLY] = "Wiadomość HOME_REPLY";
    errMap[SerialMessage::POSITION_REPLY] = "Wiadomość POSITION_REPLY";
    errMap[SerialMessage::RESET_REPLY] = "Wiadomość RESET_REPLY";
    errMap[SerialMessage::CONF_MEGA_REPLY] = "Wiadomość MEGA_REPLY";
    errMap[SerialMessage::CZUJKA_ZW_REPLY] = "Wiadomość CZUJKA ZW";
    do {
        SerialMessage msg;
        if (!msg.parseCommand(reply)) {
            if (msg.getParseReply() == SerialMessage::INPROGRESS_REPLY)
                return;
            DEBUGSER(QString("Parse Msg faild %1").arg(errMap[msg.getParseReply()]));
            QString errmsg = QString("Nie poprawna wiadomość: [%1]").arg(msg.getError());
            errmsg + errMap[msg.getParseReply()];
            emit error(errmsg);
            continue;
        }

        DEBUGSER(QString("Parse Msg success %1").arg(errMap[msg.getParseReply()]));
            RuchSilnikaType ruch;
        switch(msg.getParseReply())
        {
        case SerialMessage::INVALID_REPLY:
        case SerialMessage::CLR_MESSAGE:
        case SerialMessage::EMPTY_MESSAGE:
        case SerialMessage::INPROGRESS_REPLY:
        case SerialMessage::WELCOME_REPLY:
        default:
            break;

        case SerialMessage::CONF_REPLY:
            emit paramsDone(msg.getSilnik(), true, false);
            if (msg.getReplyConf(msg.getSilnik()))
                emit zdarzenieSilnik(msg.getSilnik(), M_CONFOK);
            break;

        case SerialMessage::MOVEHOME_REPLY:
            ruch.home = true;
            ruch.move = msg.getStartMove();
            ruch.err = msg.getErrMove();
            ruch.inter = msg.getInterMove();
            filtry.setPositionDone(msg.getSilnik(), ruch);
            emit positionDone(msg.getSilnik(), ruch);
            break;

        case SerialMessage::POSITION_REPLY:
            ruch.home = false;
            ruch.move = msg.getStartMove();
            ruch.err = msg.getErrMove();
            ruch.inter = msg.getInterMove();
            filtry.setPositionDone(msg.getSilnik(), ruch);

            emit positionDone(msg.getSilnik(), ruch);
            break;

        case SerialMessage::CZUJKA_ZW_REPLY:
            emit czujkaOn();

        case SerialMessage::RESET_REPLY:
            break;

        case SerialMessage::PROGRESS_REPLY:
            emit progressImp(msg.getSilnik(), msg.getSteps());
        }
    } while (reply.size());
}

const QString &Sterownik::portName() const
{
    return m_portName;
}

void Sterownik::handleReadyRead()
{
    QByteArray input = serialPort.readAll();
    DEBUGSER(QString("Recv %1 [%2]").arg(input.size()).arg(input.toHex().toStdString().data()));
    //qDebug() << "Recv " << input.size() << "[" << input.toHex().toStdString().data() << "]";
    receiveData.append(input);

    if (eventLoop.isRunning()) {
        eventLoop.quit();
    } else {
        parseMessage(receiveData);
    }

    if (!m_timer.isActive())
        m_timer.start(5000);
}

void Sterownik::handleTimeout()
{
    //qDebug() << "no recv data timeout";
}

void Sterownik::handleError(QSerialPort::SerialPortError err)
{
    if (err == QSerialPort::NoError)
        return;
    DEBUGSER(QString("Error %1").arg(err));
    emit error(serialPort.errorString());
}

void Sterownik::closeDevice(bool waitForDone)
{
    DEBUGSER(QString("close device %1").arg(waitForDone));
    if (waitForDone) {
        writer.command(SterownikWriter::DISCONNECT, QByteArray());
    } else {
        writer.setReset();
        closeDeviceJob();
    }
    emit deviceName("");
    m_portName = "";
    setConnected(false);
}

void Sterownik::debugFun(const QString &log)
{
    emit debug(log);
}

void Sterownik::connectToSerialJob()
{
    if (!connected()) {
        QString description;
        QString manufacturer;
        QString serialNumber;
        QString vendorId;
        QString productId;

        QString systemLocation = "";
        bool findDevice = false;

        DEBUGSER(QString("Szukam urządzenia"));

        const auto serialPortInfos = QSerialPortInfo::availablePorts();

        for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
            description = serialPortInfo.description();
            manufacturer = serialPortInfo.manufacturer();
            serialNumber = serialPortInfo.serialNumber();
            vendorId = serialPortInfo.hasVendorIdentifier() ? QString::number(serialPortInfo.vendorIdentifier(), 16) : "";
            productId = serialPortInfo.hasProductIdentifier() ? QString::number(serialPortInfo.productIdentifier(), 16) : "";
            DEBUGSER(QString("Znaleziono : [%1] DESC=%2 MANU=%3 SER=%4 VID=%5 PROD=%6").arg(serialPortInfo.portName(),
                     description, manufacturer, serialNumber, vendorId, productId));

            if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
                if ((vendorId == getVendor() && productId == getProduct() && serialNumber == getSerialNumber())
                    /*|| (vendorId == "2341" && productId == "42" && serialNumber == "851363038373518041D1")*/) {
                    DEBUGSER(QString("Znaleziono kandydata %1").arg(serialPortInfo.portName()));
                    m_portName = serialPortInfo.portName();
                    emit deviceName(m_portName);
                    emit kontrolerConfigured(FOUND);
                    systemLocation = serialPortInfo.systemLocation();
                    DEBUGSER(QString("Znaleziono urządzenie"));
                    findDevice = true;
                    //break;
                }
            }
        }
        if (!findDevice) {
            emit kontrolerConfigured(NO_FOUND);
            return;
        }

        DEBUGSER("openDevice");
        setConnected(openDevice());
    }

    if (connected()) {
        setConnected(configureDevice());
    }

    if (!connected()) {
        closeDevice(false);
    } else {
        //portThread.start();
        writer.setStart();
        emit kontrolerConfigured(ALL_OK);
    }


}

QString Sterownik::getProduct()
{
    return ust->getSerialDeviceSterownikProduct();
}

QString Sterownik::getVendor()
{
    return ust->getSerialDeviceSterownikVendor();
}

QString Sterownik::getSerialNumber()
{
    return ust->getSerialDeviceSterownikSerial();
}


#else


#define DEBUGSER(X) debugFun(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))

Sterownik::Sterownik(Ustawienia *u, QObject *parent)
    : QObject(parent),
      portName(""), m_portNr(-1),
      m_connected(false),  m_writer(this), m_reader(this),
      ust(u)
{


    m_portNr = -1;
#ifdef SERIALLINUX
     m_serialPort = nullptr;
#endif

}

Sterownik::~Sterownik()
{
    setStop();
    m_writer.wait();
    m_reader.wait();
#ifdef SERIALLINUX
    if (m_serialPort && m_serialPort->isOpen()) {
        m_serialPort->close();
        delete m_serialPort;
        m_serialPort = nullptr;
    }
#else
    RS232_CloseComport(m_portNr);
    m_portNr = -1;
#endif
    setConnected(false);
}

void Sterownik::setThread(QThread *thrW, QThread *thrR)
{
    m_writer.moveToThread(thrW);
    m_writer.command(SterownikWriter::IDLE, QByteArray());

    m_reader.moveToThread(thrR);
    //start przy connect
}

void Sterownik::setStop()
{
    m_writer.setReset();
    m_writer.setStop();
    m_reader.setStop();

}

void Sterownik::connectToDevice()
{
    if (connected()) {
        emit kontrolerConfigured(ALL_OK);
    } else {
        m_writer.setReset();
        m_writer.setStart();
        m_writer.start();
        m_writer.command(SterownikWriter::CONNECT, QByteArray());
    }
}

void Sterownik::disconnectDevice()
{
    if (connected()) {
        m_writer.command(SterownikWriter::DISCONNECT, QByteArray());
    } else {
        emit kontrolerConfigured(CLOSE);
    }
}

void Sterownik::setParams()
{
    if (!connected())
        m_writer.command(SterownikWriter::CONNECT, QByteArray());

    if (connected()) {
        m_writer.command(SterownikWriter::SET_PARAMS, QByteArray());
    }
}

void Sterownik::setPositionSilnik(int silnik, bool home, unsigned int steps, unsigned int impTime)
{
    if (!connected())
        m_writer.command(SterownikWriter::CONNECT, QByteArray());

    if (home)
        m_writer.command(SterownikWriter::SET_HOME,
                         SerialMessage::setPositionHome(silnik, impTime));
    else
        m_writer.command(SterownikWriter::SET_POSITION,
                         SerialMessage::setPosition(silnik,  steps, impTime));
}

void Sterownik::configureMotorsJob()
{
    for (short i = 1 ; i <= 9; ++i) {
        QByteArray msgBA = SerialMessage::configMotorMsg(
            i, ust->getMotorOdwrocObroty(i),
            ust->getMotorMaksIloscImp(i), ust->getMotorIloscImpBaza(i),
            ust->getMotorIloscImpSrodek(i));

        if (!write(msgBA, 500)) {
            emit error(QString("Nie udalo sie zapisac konfiguracji dla %1").arg(i));
            emit kontrolerConfigured(PARAMS_FAILD);
        }
        QThread::currentThread()->msleep(100);
    }
    QByteArray msgBA = SerialMessage::configKontrolerMsg();
    if (!write(msgBA, 500)) {
        emit error(QString("Nie mozna zapisac konfiguracji dla kontrolera"));
        emit kontrolerConfigured(PARAMS_FAILD);
    }
}

bool Sterownik::configureDevice(bool wlcmmsg)
{
    if (wlcmmsg) {
        if (!write(SerialMessage::welcomeMsg(), 200))
            return false;
        m_serialPort->waitForBytesWritten(1000);
        m_serialPort->waitForReadyRead(1000);
        auto reply = read(200);
        SerialMessage msg;
        msg.parseCommand(reply);

        if (msg.getParseReply() != SerialMessage::WELCOME_REPLY) {
            emit error(QString("Invalid Welcome Msg"));
            emit kontrolerConfigured(IDENT_FAILD);
            return false;
        }

        emit kontrolerConfigured(IDENT_OK);
    }

    for (short i = 1 ; i <= 9; ++i) {
        QByteArray msgBA = SerialMessage::configMotorMsg(
            i, ust->getMotorOdwrocObroty(i),
            ust->getMotorMaksIloscImp(i), ust->getMotorIloscImpBaza(i),
            ust->getMotorIloscImpSrodek(i));

        if (!write(msgBA, 500)) {
            emit error(QString("Nie udalo sie zapisac konfiguracji dla %1").arg(i));
            emit kontrolerConfigured(PARAMS_FAILD);
            return false;
        }
        m_serialPort->waitForBytesWritten(1000);
        m_serialPort->waitForReadyRead(1000);
    }

    QByteArray msgBA = SerialMessage::configKontrolerMsg();
    if (!write(msgBA, 500)) {
        emit error(QString("Nie mozna zapisac konfiguracji dla kontrolera"));
        emit kontrolerConfigured(PARAMS_FAILD);
        return false;
    }
    m_serialPort->waitForBytesWritten(1000);
    m_serialPort->waitForReadyRead(1000);

    auto reply = read(200);
    SerialMessage msg;
    msg.parseCommand(reply);

    if (msg.getParseReply() != SerialMessage::CONF_MEGA_REPLY || !msg.isKontroler()) {
        emit error(QString("Bledna odpowiedz dla kontrolera"));
        emit kontrolerConfigured(PARAMS_FAILD);
        return false;
    }
    for (short s = 1; s < 10; ++s) {
        if (msg.getActive(s) && msg.getConnected(s) && msg.getPinsOk(s)) {
            emit zdarzenieSilnik(s, M_ACTIVE);
        } else if (msg.getActive(s) && msg.getConnected(s) && !msg.getPinsOk(s)) {
            emit zdarzenieSilnik(s, M_NOPINS);
        } else if (msg.getActive(s) && !msg.getConnected(s)) {
            emit zdarzenieSilnik(s, M_NOCOMM);
        } else
            emit zdarzenieSilnik(s, M_NOACTIVE);
    }
    while (reply.size() > 0) {
        SerialMessage msg;
        msg.parseCommand(reply);
        if (msg.getParseReply() != SerialMessage::CONF_REPLY) {
            emit error(QString("Bledna odpowiedz dla kontrolera"));
            emit kontrolerConfigured(PARAMS_FAILD);
            return false;
        }
        emit zdarzenieSilnik(msg.getSilnik(), M_CONFOK);
    }

    emit kontrolerConfigured(PARAMS_OK);
    return true;
}

bool Sterownik::connected()
{
    const QMutexLocker locker(&connMutex);
    return m_connected;
}

void Sterownik::setConnected(bool connected)
{
    const QMutexLocker locker(&connMutex);
    m_connected = connected;
}

void Sterownik::closeDeviceJob()
{
    DEBUGSER("CLOSING DEVICE");
    m_reader.setStop();
    m_writer.setStop();
    m_reader.wait();
    m_writer.wait();

#ifdef SERIALLINUX
    if (m_serialPort) {
        m_serialPort->flush();
        m_serialPort->clear();
        m_serialPort->clearError();
        m_serialPort->close();
        delete m_serialPort;
        m_serialPort = nullptr;
    }
#else
    RS232_CloseComport(m_portNr);
    m_portNr = -1;
#endif
    setConnected(false);
    emit kontrolerConfigured(CLOSE);
    emit deviceName("");
    DEBUGSER("CLOSE DEVICE");
}

bool Sterownik::openDevice()
{
#ifdef SERIALLINUX
    m_serialPort = new QSerialPort();
    if (m_serialPort == nullptr)
        return false;
    m_serialPort->setPort(QSerialPortInfo(m_portName));

    emit deviceName(m_serialPort->portName());

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        emit error(QString(QObject::tr("Nie mozna otworzyc urzadzenia %1, error  %2")).arg(m_portName).
                   arg(m_serialPort->errorString()));
        emit kontrolerConfigured(NO_OPEN);
        return false;
    }
    m_serialPort->clear();
    m_serialPort->clearError();
    m_serialPort->flush();
    //m_serialPort->reset();
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setReadBufferSize(32);

    QThread::currentThread()->sleep(1);

    emit kontrolerConfigured(OPEN);



    QByteArray startBuf(20, '\0');
    //DEBUGSER(QString("[%1]").arg(startBuf.toHex(' ').data()));
    write(startBuf, 2500);
    QThread::currentThread()->sleep(1);
    read(1000);
    return true;
#else
    DEBUGSER(QString("Otwieram urządzenia %1").arg(portName));
    char mode[]={'8','O','1',0};
    if (RS232_OpenComport(m_portNr, 115200, mode, 0)) {
        DEBUGSER(QString("Error open Port"));
        emit kontrolerConfigured(NO_OPEN);
        return false;
    }

    RS232_flushRXTX(m_portNr);

    const unsigned char startBuf[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int rs = RS232_SendBuf(m_portNr, (unsigned char*)startBuf, 16);

    if (rs <= 0) {
        RS232_CloseComport(m_portNr);
        m_portNr = -1;
        emit kontrolerConfigured(false, NO_READ);
        return false;
    }
    QThread::currentThread()->msleep(200);
    unsigned char recvBuf[100];
    int recv = RS232_PollComport(m_portNr, recvBuf, 1000);
    (void)recv;

    emit kontrolerConfigured(OPEN);
    return true;
#endif
}

void Sterownik::setReset()
{
    if (!connected())
        m_writer.command(SterownikWriter::CONNECT, QByteArray());

    if (connected()) {
        m_writer.command(SterownikWriter::RESET, SerialMessage::resetSilniki());
    }
}

void Sterownik::setStopMotor(short nrSilnik)
{
    if (!connected())
        m_writer.command(SterownikWriter::CONNECT, QByteArray());

    if (connected()) {
        m_writer.command(SterownikWriter::SET_STOP, SerialMessage::stopSilnik(nrSilnik));
    }
}

bool Sterownik::write(const QByteArray &currentRequest, int currentWaitWriteTimeout)
{

#ifdef SERIALLINUX
    if (m_serialPort == nullptr)
        return false;
    if (currentRequest.size() > 0) {

        int sendBytes = m_serialPort->write(currentRequest);
        if (!m_serialPort->waitForBytesWritten(currentWaitWriteTimeout)) {
            DEBUGSER(QString("Timeout Write %1").arg(currentWaitWriteTimeout));
            emit error(QString("Timeout Write"));
            return false;
        }
        DEBUGSER(QString("Write %1 bytes [%2]").arg(sendBytes).arg(currentRequest.toHex(' ').constData()));
    }
    return true;
#else

    if (currentRequest.size() > 0)
    {

        DEBUGSER(QString("write [%1]").arg(currentRequest.toHex().constData()));
        QElapsedTimer timer;
        timer.start();
        int sendBytes = RS232_SendBuf(m_portNr, (unsigned char*)currentRequest.constData(), currentRequest.size());
        if (sendBytes == 0) {
            DEBUGSER(QString("Write timeout %1").arg(sendBytes).arg(timer.elapsed()));
            return false;
        }
        DEBUGSER(QString("write %1 bytes [%2 ms]").arg(sendBytes).arg(timer.elapsed()));
        QThread::currentThread()->msleep(currentWaitWriteTimeout);
    }
    return true;
#endif
}

QByteArray Sterownik::read(int currentWaitReadTimeout)
{
    QByteArray responseData ;
#ifdef SERIALLINUX
    if (m_serialPort == nullptr)
        return QByteArray();
    if (m_serialPort->bytesAvailable() > 0) {
    //if (m_serialPort->waitForReadyRead(currentWaitReadTimeout)) {
        responseData = m_serialPort->readAll();
        //qDebug() << "R=" << responseData.size();
        //if (responseData.size() > 0) {
        //    return responseData;
        //}
        while (m_serialPort->waitForReadyRead(10) && currentWaitReadTimeout > 0) {
            QByteArray r = m_serialPort->readAll();
            //qDebug() << "r=" << r.size();
            if (r.size() == 0)
                break;
            responseData += r;
            currentWaitReadTimeout -= 10;
        }
    } else {
        m_serialPort->clear(QSerialPort::Direction::Input);
    }
    if (responseData.size() > 0)
        DEBUGSER(QString("READ %1 bytes [%2]").arg(responseData.size()).arg(responseData.toHex(' ').constData()));

#else
    unsigned char recvBuffor[20];
    //int readTimeout = currentReadWaitTimeout;
    int rc = 0;
    QElapsedTimer timer;
    timer.start();
    unsigned short len;

    //1 znak
    do {
        rc = RS232_PollComport(m_portNr, recvBuffor, 1);
        if (rc > 0) {
            DEBUGSER(QString("recv %1 bytes").arg(rc));
        }
        if (rc == 0) {
            readTimeout -= 50;
            QThread::currentThread()->msleep(50);
        }
        if (recvBuffor[0] == 0xf0)
            continue;
    } while (rc == 0 && readTimeout > 0);
    QByteArray responseData;
    if (rc == 1) {
        responseData.append((const char*)recvBuffor, rc);
        len = recvBuffor[0] & 0x0f;
    }
    else {
        emit error(QString("Timeout"));
        DEBUGSER(QString("Timeout Read %1").arg(currentReadWaitTimeout));
        msg.setTimeoutReply(false);
        return msg;
    }

    //reszta znakow
    do {
        rc = RS232_PollComport(m_portNr, recvBuffor, len+1);
        if (rc > 0) {
            DEBUGSER(QString("recv %1 bytes").arg(rc+1));
        }
        responseData.append((const char*)recvBuffor, rc);
        if (responseData.size() == len + 2)
            break;

        readTimeout -= 50;
        QThread::currentThread()->msleep(50);

    } while(readTimeout > 0);
    unsigned long ms = timer.elapsed();

    if (readTimeout <= 0) {
        emit error(QString("Timeout"));
        DEBUGSER(QString("Timeout Read %1").arg(currentReadWaitTimeout));
        msg.setTimeoutReply(false);
        return msg;
    }
    responseData.append((const char*)recvBuffor, rc);
    DEBUGSER(QString("read [%1] [%2 ms]").arg(responseData.toHex().constData()).arg(ms));
    return responseData;

#endif
    return responseData;
}

QVector<SerialMessage> Sterownik::parseMessage(QByteArray &reply)
{
    //qDebug() << __FILE__ << __LINE__ << "ParseMessage";
    QVector<SerialMessage> ret;
    QMap<int, QString> errMap;
    errMap[SerialMessage::INVALID_REPLY] = "Nie prawidłowa odpowiedź";
    errMap[SerialMessage::CLR_MESSAGE] = "Wiadomość czyszcząca";
    errMap[SerialMessage::EMPTY_MESSAGE] = "Pusta wiadomość";
    errMap[SerialMessage::INPROGRESS_REPLY] = "Wiadomość przerwana";
    errMap[SerialMessage::WELCOME_REPLY] = "Wiadomość powitalna";
    errMap[SerialMessage::CONF_REPLY] = "Wiadomość konfiguracyjna";
    errMap[SerialMessage::MOVEHOME_REPLY] = "Wiadomość HOME_REPLY";
    errMap[SerialMessage::POSITION_REPLY] = "Wiadomość POSITION_REPLY";
    errMap[SerialMessage::RESET_REPLY] = "Wiadomość RESET_REPLY";
    errMap[SerialMessage::CONF_MEGA_REPLY] = "Wiadomość MEGA_REPLY";
    errMap[SerialMessage::CZUJKA_ZW_REPLY] = "Wiadomość CZUJKA ZW";
    do {
        SerialMessage msg;
        if (!msg.parseCommand(reply)) {
            DEBUGSER(QString("Parse Msg faild %1").arg(errMap[msg.getParseReply()]));
            QString errmsg("Nie poprawna wiadomość:");
            errmsg + errMap[msg.getParseReply()];
            emit error(errmsg);
            if (msg.getParseReply() == SerialMessage::INPROGRESS_REPLY)
                return ret;
            continue;
        }

        DEBUGSER(QString("Parse Msg success %1").arg(errMap[msg.getParseReply()]));

        switch(msg.getParseReply())
        {
        case SerialMessage::INVALID_REPLY:
        case SerialMessage::CLR_MESSAGE:
        case SerialMessage::EMPTY_MESSAGE:
        case SerialMessage::INPROGRESS_REPLY:
        case SerialMessage::WELCOME_REPLY:
        default:
            break;

        case SerialMessage::CONF_REPLY:
            emit setParamsDone(msg.getSilnik(), true, false);
            if (msg.getReplyConf(msg.getSilnik()))
                emit zdarzenieSilnik(msg.getSilnik(), M_CONFOK);
            break;

        case SerialMessage::MOVEHOME_REPLY:
            emit setPositionDone(msg.getSilnik(), true, msg.getErrMove(), msg.getStartMove(), 0, 0);
            break;

        case SerialMessage::POSITION_REPLY:
            emit setPositionDone(msg.getSilnik(), false, msg.getErrMove(), msg.getStartMove(), msg.getSteps(), 0);
            break;

        case SerialMessage::CZUJKA_ZW_REPLY:
            emit czujkaOn();

        case SerialMessage::RESET_REPLY:
            break;
        }
        ret.append(msg);
    } while (reply.size());
    return ret;
}

const QString &Sterownik::portName() const
{
    return portName;
}

void Sterownik::closeDevice(bool waitForDone)
{
    DEBUGSER(QString("close device %1").arg(waitForDone));
    if (waitForDone) {
        m_writer.command(SterownikWriter::DISCONNECT, QByteArray());
        m_reader.setStop();
    } else {
        m_writer.setReset();
        closeDeviceJob();
    }
    emit deviceName("");
    portName = "";
    m_portNr = -1;
    setConnected(false);
}

void Sterownik::debugFun(const QString &log)
{
    emit debug(log);
}

void Sterownik::connectToSerialJob()
{
    if (!connected()) {
        QString description;
        QString manufacturer;
        QString serialNumber;
        QString vendorId;
        QString productId;

        QString systemLocation = "";
        bool findDevice = false;

        DEBUGSER(QString("Szukam urządzenia"));

        const auto serialPortInfos = QSerialPortInfo::availablePorts();

        for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
            description = serialPortInfo.description();
            manufacturer = serialPortInfo.manufacturer();
            serialNumber = serialPortInfo.serialNumber();
            vendorId = serialPortInfo.hasVendorIdentifier() ? QString::number(serialPortInfo.vendorIdentifier(), 16) : "";
            productId = serialPortInfo.hasProductIdentifier() ? QString::number(serialPortInfo.productIdentifier(), 16) : "";
            DEBUGSER(QString("Znaleziono : [%1] DESC=%2 MANU=%3 SER=%4 VID=%5 PROD=%6").arg(serialPortInfo.portName(),
                     description, manufacturer, serialNumber, vendorId, productId));

            if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
                if ((vendorId == getVendor() && productId == getProduct() && serialNumber == getSerialNumber())
                    /*|| (vendorId == "2341" && productId == "42" && serialNumber == "851363038373518041D1")*/) {
                    DEBUGSER(QString("Znaleziono kandydata %1").arg(serialPortInfo.portName()));
                    portName = serialPortInfo.portName();
                    emit deviceName(portName);
                    emit kontrolerConfigured(FOUND);
                    systemLocation = serialPortInfo.systemLocation();
                    DEBUGSER(QString("Znaleziono urządzenie"));
                    findDevice = true;
                    //break;
                }
            }
        }
        if (!findDevice) {
            emit kontrolerConfigured(NO_FOUND);
            return;
        }

    #ifndef SERIALLINUX
        m_portNr = -1;
        //GetComPortUsb(bufPortName,"1B4F","9206");
        m_portNr = RS232_GetPortnr(m_portName.toStdString().c_str());

        if (m_portNr == -1)
            return;
    #endif
        DEBUGSER("openDevice");
        setConnected(openDevice());
    }

    if (connected()) {
        setConnected(configureDevice(true));
    }

    if (!connected()) {
        closeDevice(false);
    } else {
       m_reader.start();
    }

    if (emitSignal) emit kontrolerConfigured(ALL_OK);
}

QString Sterownik::getProduct()
{
    return ust->getSerialDeviceSterownikProduct();
}

QString Sterownik::getVendor()
{
    return ust->getSerialDeviceSterownikVendor();
}

QString Sterownik::getSerialNumber()
{
    return ust->getSerialDeviceSterownikSerial();
}

#endif

