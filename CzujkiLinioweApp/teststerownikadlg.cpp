#include "teststerownikadlg.h"
#include "ui_teststerownikadlg.h"
#include "ustawienia.h"
#include "sterownik.h"

#include <QMessageBox>
#include <QTime>

#define SETCONF(N) ui->base##N->setText(QString::number(u->getMotorIloscImpBaza##N())); \
                   ui->delay##N->setText(QString::number(u->getMotorCzasMiedzyImpZerow##N())); \
                   ui->maxSteps##N->setText(QString::number(u->getMotorMaksIloscImp##N())); \
                   ui->obrot##N->setChecked(u->getMotorOdwrocObroty##N()); \
                   ui->ratio##N->setText(QString::number(u->getMotorPrzelozenieImpJedn##N())); \
                   ui->srodekKroki##N->setText(QString::number(u->getMotorIloscImpSrodek##N())); \
                   ui->motorName##N->setText(u->getMotorNazwa##N()); \
                   ui->speed##N->setText(QString::number(u->wyliczPredkosc(u->getMotorPrzelozenieImpJedn##N(),\
                                                                                u->getMotorCzasMiedzyImpZerow##N()))); \
                   ui->speedPos##N->setText(QString::number(u->wyliczPredkosc(u->getMotorPrzelozenieImpJedn##N(),\
                                                         u->getMotorCzasMiedzyImpZerow##N())));\
                   ui->speedWork##N->setText(QString::number(u->wyliczPredkosc(u->getMotorPrzelozenieImpJedn##N(),\
                                                                               u->getMotorCzasMiedzyImpNormal##N()))); \
                   ui->delayWork##N->setText(QString::number(u->getMotorCzasMiedzyImpNormal##N()));


#define SETCONF_ALL SETCONF(1) \
                    SETCONF(2) \
                    SETCONF(3) \
                    SETCONF(4) \
                    SETCONF(5) \
                    SETCONF(6) \
                    SETCONF(7) \
                    SETCONF(8) \
                    SETCONF(9)

#define CONN(N) connect(ui->pbHome_##N, &QPushButton::clicked, this, [this](){ this->pbHome_clicked(N, this->ui->delay##N->text()); }); \
                connect(ui->pbUstawPos_##N, &QPushButton::clicked, this, [this](){ \
                                                                this->pbUstawPos_clicked(N, ui->posX_##N->text(), \
                                                                this->ui->ratio##N->text(), \
                                                                this->ui->speedPos##N->text(),\
                                                                this->ui->srodekKroki##N->text(),\
                                                                this->ui->maxSteps##N->text()); });\
                connect(ui->delay##N, &QLineEdit::editingFinished, this, [this]() { \
                                                              this->changeTimeZerowania(ui->delay##N->text(), \
                                                                    this->ui->ratio##N->text(), \
                                                                    this->ui->speed##N); });\
                connect(ui->delayWork##N, &QLineEdit::editingFinished, this, [this]() { \
                                                              this->changeTimeZerowania(ui->delayWork##N->text(), \
                                                                    this->ui->ratio##N->text(), \
                                                                    this->ui->speedWork##N); }); \
                connect(ui->ratio##N, &QLineEdit::editingFinished, this, [this]() { \
                                                              this->changeTimeZerowania(ui->delay##N->text(), \
                                                                    this->ui->ratio##N->text(), \
                                                                    this->ui->speed##N); \
                                                              this->changeTimeZerowania(ui->delayWork##N->text(), \
                                                                    this->ui->ratio##N->text(), \
                                                                    this->ui->speedWork##N); });


#define CONN_ALL CONN(1) \
                 CONN(2) \
                 CONN(3) \
                 CONN(4) \
                 CONN(5) \
                 CONN(6) \
                 CONN(7) \
                 CONN(8) \
                 CONN(9)

#define CONN_STER(F) connect(sdv, &Sterownik::F , this, &TestSterownikaDlg::sd_##F)


#define ADDICONS(N) QHBoxLayout * horizontalLayoutfr##N = new QHBoxLayout(ui->fr##N); \
                    horizontalLayoutfr##N->setSpacing(2);\
                    horizontalLayoutfr##N->setObjectName(QString("horizontalLayoutfr_%1").arg(N));\
                    horizontalLayoutfr##N->setContentsMargins(2, 2, 2, 2);\
                    QLabel * iconStatus##N = new QLabel(ui->fr##N);\
                    iconStatus##N->setObjectName(QString("iconOK%1").arg(N));\
                    iconStatus##N->setMinimumSize(QSize(15, 15)); \
                    iconStatus##N->setMaximumSize(QSize(15, 15)); \
                    iconStatus##N->setStyleSheet("background-color:red");\
                    horizontalLayoutfr##N->addWidget(iconStatus##N);\
                    ikonyStatusu[N] = iconStatus##N;

#define ADDICONS_ALL    ADDICONS(1) \
                        ADDICONS(2) \
                        ADDICONS(3) \
                        ADDICONS(4) \
                        ADDICONS(5) \
                        ADDICONS(6) \
                        ADDICONS(7) \
                        ADDICONS(8) \
                        ADDICONS(9)

#define ICONMOVE(N) ikonyRuchu[N] = qMakePair(ui->homeStatus##N, ui->posStatus##N); pozycja[N] = ui->actPos_##N;

#define ADDMOVEICONS_ALL    ICONMOVE(1) \
                            ICONMOVE(2) \
                            ICONMOVE(3) \
                            ICONMOVE(4) \
                            ICONMOVE(5) \
                            ICONMOVE(6) \
                            ICONMOVE(7) \
                            ICONMOVE(8) \
                            ICONMOVE(9)

TestSterownikaDlg::TestSterownikaDlg(Ustawienia *ust, Sterownik *sdv, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestSterownikaDlg),
    u(ust),
    sd(sdv)
{
    ui->setupUi(this);
    SETCONF_ALL

    CONN_ALL

    connect(ui->pbHomeAll, &QPushButton::clicked, this, &TestSterownikaDlg::pbHomeAll_clicked);
    connect(ui->pbSetConfiguration, &QPushButton::clicked, this, &TestSterownikaDlg::pbSetConfiguration_clicked);
    connect(ui->pbConnect, &QPushButton::clicked, this, &TestSterownikaDlg::pbConnect_clicked);
    connect(ui->pbDisconnect,&QPushButton::clicked, this, &TestSterownikaDlg::pbDisconnect_clicked);
    connect(ui->pbReset, &QPushButton::clicked, this, &TestSterownikaDlg::pbResett_clicked);
    connect(ui->pbStop, &QPushButton::clicked, this, &TestSterownikaDlg::pbStop_clicked);
    connect(ui->pbWylicz, &QPushButton::clicked, this, &TestSterownikaDlg::pbWyliczImpulsy_clicked);


    ADDICONS_ALL

    ADDMOVEICONS_ALL

    ui->iconKonf->setStyleSheet("background-color:red");
    ui->iconAuth->setStyleSheet("background-color:red");
    ui->iconOpen->setStyleSheet("background-color:red");
    ui->iconFound->setStyleSheet("background-color:red");

    if (sdv->getConnected()) {
        addDebug("Urządzenie podłączone");
        ui->frameMove->setEnabled(true);
        ui->iconKonf->setStyleSheet("background-color:green");
        ui->iconAuth->setStyleSheet("background-color:green");
        ui->iconOpen->setStyleSheet("background-color:green");
        ui->iconFound->setStyleSheet("background-color:green");
        ui->namePort->setText(sdv->portName());
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);
        ui->pbReset->setEnabled(true);
        ui->pbSetConf->setEnabled(true);
        addDebug("Wysyłam zapytanie o konfiguracje");
        sd->setParams(0);
        for (short i = 1; i < 10; ++i) {
            ikonyStatusu[i]->setStyleSheet("background-color:green");
        }
    } else {
        addDebug("Urządzenie nie podłączone");
        ui->frameMove->setEnabled(false);
        ui->pbDisconnect->setEnabled(false);
        ui->pbConnect->setEnabled(true);
        ui->pbReset->setEnabled(false);
        ui->pbSetConf->setEnabled(false);
        for (short i = 1; i < 10; ++i) {
            ikonyStatusu[i]->setStyleSheet("background-color:gray");
            ikonyRuchu[i].first->setStyleSheet("background-color:gray");
            ikonyRuchu[i].second->setStyleSheet("background-color:gray");
        }
    }
    //resize(1800, 1000);
    adjustSize();
}

TestSterownikaDlg::~TestSterownikaDlg()
{
    delete ui;
}

#define WRITECONF(N) u->setMotorIloscImpBaza##N(ui->base##N->text()); \
                     u->setMotorCzasMiedzyImpZerow##N(ui->delay##N->text()); \
                     u->setMotorMaksIloscImp##N(ui->maxSteps##N->text()); \
                     u->setMotorOdwrocObroty##N(ui->obrot##N->isChecked()); \
                     u->setMotorPrzelozenieImpJedn##N(ui->ratio##N->text()); \
                     u->setMotorIloscImpSrodek##N(ui->srodekKroki##N->text()); \
                     u->setMotorNazwa##N(ui->motorName##N->text()); \
                     u->setMotorCzasMiedzyImpNormal##N(ui->delayWork##N->text());

#define WRITECONF_ALL WRITECONF(1) \
                      WRITECONF(2) \
                      WRITECONF(3) \
                      WRITECONF(4) \
                      WRITECONF(5) \
                      WRITECONF(6) \
                      WRITECONF(7) \
                      WRITECONF(8) \
                      WRITECONF(9)

void TestSterownikaDlg::pbSetConfiguration_clicked()
{
    addDebug("Zapisuje konfiguracje do pliku");
    WRITECONF_ALL
}

void TestSterownikaDlg::sd_deviceName(QString name)
{
    ui->namePort->setText(name);
}

void TestSterownikaDlg::addDebug(const QString & dbg)
{
    ui->debug->appendPlainText(QString("[%1] %2").arg(QTime::currentTime().toString("HH:mm:ss.zzz"), dbg));
}

void TestSterownikaDlg::sd_kontrolerConfigured(int state)
{
    bool conn = false;
    switch(state) {
    case Sterownik::NO_FOUND:
        ui->iconKonf->setStyleSheet("background-color:gray");
        ui->iconAuth->setStyleSheet("background-color:gray");
        ui->iconOpen->setStyleSheet("background-color:gray");
        ui->iconFound->setStyleSheet("background-color:red");
        addDebug(QString("Nie znaleziono kontrolera"));
        break;
    case Sterownik::FOUND:
        ui->iconFound->setStyleSheet("background-color:green");
        addDebug(QString("Znaleziono kontroler"));
        break;
    case Sterownik::NO_OPEN:
    case Sterownik::NO_READ:
        ui->iconOpen->setStyleSheet("background-color:red");
        ui->iconKonf->setStyleSheet("background-color:gray");
        ui->iconAuth->setStyleSheet("background-color:gray");
        addDebug(QString("Błąd odczytu kontrolera"));
        break;
    case Sterownik::IDENT_FAILD:
        ui->iconAuth->setStyleSheet("background-color:red");
        addDebug(QString("Błąd identyfikacji"));
        break;
    case Sterownik::OPEN:
        ui->iconOpen->setStyleSheet("background-color:green");
        addDebug(QString("Port kontrolera prawidłowo otwarty"));
        break;
    case Sterownik::IDENT_OK:
        ui->iconAuth->setStyleSheet("background-color:green");
        addDebug(QString("Autoryzacja kontrolera prawidłowa"));
        break;
    case Sterownik::PARAMS_OK:
    case Sterownik::ALL_OK:
        ui->iconKonf->setStyleSheet("background-color:green");
        addDebug(QString("Połączenie z kontrolerem OK"));
        conn = true;
        for (short i = 1; i < 10; ++i) {
            ikonyRuchu[i].first->setStyleSheet("background-color:yellow");
            ikonyRuchu[i].second->setStyleSheet("background-color:green");
        }
        ui->pbReset->setEnabled(true);
        ui->pbSetConf->setEnabled(true);
        break;
    case Sterownik::PARAMS_FAILD:
        ui->iconKonf->setStyleSheet("background-color:red");
        addDebug(QString("Nie udało się skonfigurować kontrolera"));
        break;
    case Sterownik::CLOSE:
        addDebug(QString("Port kontrolera zamknięty"));
        ui->iconKonf->setStyleSheet("background-color:gray");
        ui->iconAuth->setStyleSheet("background-color:gray");
        ui->iconOpen->setStyleSheet("background-color:gray");
        ui->iconFound->setStyleSheet("background-color:gray");
        ui->pbReset->setEnabled(false);
        ui->pbSetConf->setEnabled(false);
        for (short i = 1; i < 10; ++i) {
            ikonyStatusu[i]->setStyleSheet("background-color:gray");
            ikonyRuchu[i].first->setStyleSheet("background-color:gray");
            ikonyRuchu[i].second->setStyleSheet("background-color:gray");
        }
        break;
    default:
        break;
    }

    ui->frameMove->setEnabled(conn);
    ui->pbConnect->setEnabled(!conn);
    ui->pbDisconnect->setEnabled(conn);
}

void TestSterownikaDlg::sd_setParamsDone(bool success)
{
    addDebug(QString("%1 się ustawić parametry konfigurcjia").arg(success ? "Udało" : "Nie udało" ));
    ui->iconKonf->setStyleSheet("background-color:green");
}

void TestSterownikaDlg::sd_disconnect()
{
    addDebug("Urzadzenie rozłączone");
    ui->iconKonf->setStyleSheet("background-color:red");
}

void TestSterownikaDlg::sd_setZdarzenieSilnik(short silnik, short zdarzenie)
{
    if (silnik == 0)
        return;
    switch(zdarzenie) {
    case Sterownik::M_ACTIVE:
        addDebug(QString("Silnik %1 aktywny").arg(silnik));
        ikonyStatusu[silnik]->setStyleSheet("background-color:orange");

        break;
    case Sterownik::M_NOCOMM:
        addDebug(QString("Brak komunikacji z silnikiem %1").arg(silnik));
        ikonyStatusu[silnik]->setStyleSheet("background-color:blue");
        break;
    case Sterownik::M_NOPINS:
        addDebug(QString("Problemy z pinami dla silnika %1").arg(silnik));
        ikonyStatusu[silnik]->setStyleSheet("background-color:yellow");
        break;
    case Sterownik::M_CONFOK:
        ikonyStatusu[silnik]->setStyleSheet("background-color:green");
        break;
    case Sterownik::M_NOACTIVE:
    default: {
        addDebug(QString("Silnik %1 nieaktywny").arg(silnik));
        ikonyStatusu[silnik]->setStyleSheet("background-color:red");
        break;
    }
    }
}

void TestSterownikaDlg::sd_czujkaOn(bool /*hardware*/)
{
    addDebug("Wyzwolona czujka");
    ui->czujkaOn->setStyleSheet("background-color:green");
    QTimer tm;
    tm.singleShot(1000, [this](){ this->ui->czujkaOn->setStyleSheet("background-color:black");});
}

void TestSterownikaDlg::sd_setValue(short silnik, const double &val)
{
    addDebug(QString("Ruch silnika [%1] => %2").arg(silnik).arg(val));
    pozycja[silnik]->setText(QString::number(val));
}

void TestSterownikaDlg::sd_setPositionDone(short silnik, RuchSilnikaType r)
//void TestSterownikaDlg::sd_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt)
{
    addDebug(QString("[Silnik %1] %2 <%3> {Przerwanie:%4 Błąd:%5").arg(silnik).
             arg(r.move ? "rozpoczął ruch" : "zakończył ruch").
             arg(r.home ? "Powrót do bazy" : "Ustawiona pozycja").
             arg(r.inter ? "T" : "N").
             arg(r.err ? "T" : "N"));

    if (silnik < 1 || silnik > 9)
        return;

    // w ruchu
    if (r.move) {
        ikonyRuchu[silnik].first->setStyleSheet("background-color:blue");
        if (!r.home)
            ikonyRuchu[silnik].second->setStyleSheet("background-color:blue");
    }

    if (r.home) {
        if (!r.err) {
            addDebug(QString("Zerowanie silnika %1 zakończyło się poprawnie").arg(silnik));
            ikonyRuchu[silnik].first->setStyleSheet("background-color:green");
        } else {
            addDebug(QString("Błąd zerowania silnika %1").arg(silnik));
            ikonyRuchu[silnik].first->setStyleSheet("background-color:red");
        }
    } else {
        if (!r.err) {
            if (r.move) {
                ikonyRuchu[silnik].second->setStyleSheet("background-color:blue");
                addDebug(QString("Rozpoczynam ustawianie pozycji silnika %1").arg(silnik));
            } else {
                ikonyRuchu[silnik].second->setStyleSheet("background-color:green");
                addDebug(QString("Ustawianie pozycji silnika %1 zakończyło się poprawnie").arg(silnik));
            }
        } else {
            ikonyRuchu[silnik].second->setStyleSheet("background-color:red");
            addDebug(QString("Ustawianie pozycji silnika %1 zakończyło się błędem").arg(silnik));
        }
    }
}

void TestSterownikaDlg::pbConnect_clicked()
{
    ui->debug->clear();
    addDebug(QString("Szukam urządzenia vendor=%1 product=%2").arg(sd->getVendor(), sd->getProduct()));
    sd->connectToDevice();
}

void TestSterownikaDlg::pbDisconnect_clicked()
{
    addDebug(QString("Rozłączenie na życzenie"));
    sd->disconnectDevice();
    ui->iconKonf->setStyleSheet("background-color:red");
    ui->iconAuth->setStyleSheet("background-color:red");
    ui->iconOpen->setStyleSheet("background-color:red");
    ui->iconFound->setStyleSheet("background-color:red");
}

void TestSterownikaDlg::pbResett_clicked()
{
    addDebug(QString("Reset na życzenie"));
    sd->setReset();
}

void TestSterownikaDlg::pbStop_clicked()
{
    addDebug(QString("Zatrzymuje wszystko"));
    sd->setStopMotor(1);
    sd->setStopMotor(2);
    sd->setStopMotor(3);
    sd->setStopMotor(4);
    sd->setStopMotor(5);
    sd->setStopMotor(6);
    sd->setStopMotor(7);
    sd->setStopMotor(8);
    sd->setStopMotor(9);
}

#define KONF(N) sd->setParams(N, ui->obrot##N->isChecked(), ui->maxSteps##N->text().toUInt(), ui->base##N->text().toUInt(), ui->srodekKroki##N->text().toUInt())
void TestSterownikaDlg::pbLoadConf_clicked()
{
    addDebug("Ładowanie konfiguracji");
    KONF(1);
    KONF(2);
    KONF(3);
    KONF(4);
    KONF(5);
    KONF(6);
    KONF(7);
    KONF(8);
    KONF(9);
    sd->setParams(0);
}

void TestSterownikaDlg::pbWyliczImpulsy_clicked()
{
    if (ui->predkoscdoobliczen->text().isEmpty()) {
        ui->wyliczoneimpulsy->setText("-");
        return ;
    }
    bool ok;
    double val = ui->predkoscdoobliczen->text().toDouble(&ok);
    if (!ok) {
        ui->wyliczoneimpulsy->setText("-");
        return ;
    }
    if (val == 0) {
        ui->wyliczoneimpulsy->setText("-");
        return ;
    }
    double ratio = u->getMotorPrzelozenieImpJedn(ui->cbSilnikSpeed->currentIndex()+1);
    if (ratio == 0) {
        ui->wyliczoneimpulsy->setText("-");
        return ;
    }
    double ilosc_impulsow_per_1min = (val/ratio);

    double czasImp = 60*1000000/ilosc_impulsow_per_1min;
    ui->wyliczoneimpulsy->setText(QString::number(czasImp, 'f', 2));

}

void TestSterownikaDlg::pbHome_clicked(int silnik, const QString & impTime)
{
    addDebug(QString("Żadanie zerowania dla silnika %1 dT=%2").arg(silnik).arg(impTime));
    ikonyRuchu[silnik].first->setStyleSheet("background-color:blue");
    bool ok;
    unsigned int val = impTime.toUInt(&ok);
    if (!ok)
        val = 10000;
    sd->setPositionSilnik(silnik, true, 0, val);
}

void TestSterownikaDlg::pbHomeAll_clicked()
{
    for (int i=1; i<10; ++i) {
        QLineEdit * e = nullptr;
        switch(i) {
            case 1: e = ui->delay1; break;
            case 2: e = ui->delay2; break;
            case 3: e = ui->delay3; break;
            case 4: e = ui->delay4; break;
            case 5: e = ui->delay5; break;
            case 6: e = ui->delay6; break;
            case 7: e = ui->delay7; break;
            case 8: e = ui->delay8; break;
            case 9: e = ui->delay9; break;
        default: break;
        }
        if (e)
            pbHome_clicked(i, e->text());
    }
}

void TestSterownikaDlg::pbUstawPos_clicked(int silnik, const QString &x, const QString &ratio,
                                           const QString & speed, const QString & middleImp,
                                           const QString & maxImp)
{
    addDebug(QString("Ustaw pozycje silnik %1 l=%2[*/mm] speed=%3[*/min mm/min] (ratio=%4, middleImp=%5)").
                     arg(silnik).arg(x).arg(speed).arg(ratio).arg(middleImp));
    if (x.isEmpty() || ratio.isEmpty() || speed.isEmpty() || middleImp.isEmpty())
        return;


    bool ok1, ok2, ok3, ok4, ok5;
    double valx = x.toDouble(&ok1);
    double valratio = ratio.toDouble(&ok2);
    double valspeed = speed.toDouble(&ok3);
    unsigned int valmiddleImp = middleImp.toUInt(&ok4);
    unsigned int valmaxImp = maxImp.toUInt(&ok5);
    if (!ok1 || !ok2 || !ok3 || !ok4 || !ok5)
        return ;

    ikonyRuchu[silnik].second->setStyleSheet("background-color:gray");
    unsigned long impSpeed = u->wyliczImp(valratio, valspeed);
    unsigned long impPos = u->wyliczPozycje(silnik, valmiddleImp, valmaxImp, valratio, valx);
    addDebug(QString("Wysyłam żadanie ruchu pos=%1 speed=%2").arg(impPos).arg(impSpeed));
    sd->setPositionSilnik(silnik, false, impPos, impSpeed);
}

void TestSterownikaDlg::changeTimeZerowania(const QString &val, const QString &ratio, QLabel *result)
{
    if (val.isEmpty() || ratio.isEmpty())
        return;
    bool ok;
    double dval = val.toDouble(&ok);
    if (!ok)
        return;
    double dratio = ratio.toDouble(&ok);
    if (!ok)
        return;
    result->setText(QString::number(u->wyliczPredkosc(dratio, dval)));
}
