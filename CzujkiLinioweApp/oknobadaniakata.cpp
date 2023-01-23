#include "oknobadaniakata.h"
#include "ui_oknobadaniakata.h"
#include <QDateTime>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "sterownik.h"
#include <QMutexLocker>

OknoBadaniaKata::OknoBadaniaKata(short nrSilnika_, const QString &name,
                                 const QString & podtitle,
                                 const QString & kat,
                                 const Ustawienia &ust,
                           Sterownik * ster_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoBadaniaKata),
    tmSterownika(this),
    ster(ster_),
    wynikBadania(false),
    destPos(0),
    nrSilnika(nrSilnika_),
    prevVal(0)

{
    qDebug() << __FILE__ << __LINE__ << "kat" << kat;
    ui->setupUi(this);
    //this->setwi
    ui->testName->setText(name);
    if (podtitle.isEmpty()) {
        ui->nazwaPodTestu->setVisible(false);
    } else {
        ui->nazwaPodTestu->setText(podtitle);
        ui->nazwaPodTestu->setVisible(true);
    }
    ui->katProducenta->setText(QString("<html><body>%1 &deg;</body></html>").arg(kat));

    unsigned int speed = ust.predkoscRoboczaImp(nrSilnika);
    unsigned int impulsy = ust.wyliczPozycje(nrSilnika, kat.toDouble());
    destPos = kat.toDouble();
    speedMin = ust.wyliczPredkosc(ust.getMotorPrzelozenieImpJedn(nrSilnika), speed);
    ui->speed->setText(QString("<html><body>%1 &deg; / <sub>min<sub></body></html>").arg(speedMin, 3, 'f', 2));

    ster->setPositionSilnik(nrSilnika, false, impulsy, speed);
    qDebug() << __FILE__ << __LINE__ << "impulsy" << impulsy << speed;
    tmSterownika.singleShot(5000, this, &OknoBadaniaKata::timeoutSterownika);
    deviceisOk = false;

#ifndef DEFVAL
    ui->frameDebug->setVisible(false);
#else
    moveTimer = new QTimer(this);
    ui->frameDebug->setVisible(true);
    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->wynikBadania = true; this->prevVal = this->destPos; accept(); });
    connect(ui->pbCzujkaOn, &QPushButton::clicked, this, [this]() {this->czujkaOn(); });
    connect(ui->pbStartRuchu, &QPushButton::clicked, this, [this]() {this->testValue(); });
#endif
}

OknoBadaniaKata::~OknoBadaniaKata()
{
#ifdef DEFVAL
    moveTimer->stop();
    delete moveTimer;
#endif
    tmSterownika.stop();
    delete ui;
}

void OknoBadaniaKata::czujkaOn()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << "czujka on";
    tmSterownika.stop();
    wynikBadania = false;
    error = QString("Czujka zadziała dla kąta : %1").arg(prevVal);
    reject();
}

#ifdef DEFVAL
void OknoBadaniaKata::testValue()
{
    valBeg = 0.0;
    valEnd = destPos;
    valAct = valBeg;
    valPer100ms = 0.3/60/5;
    moveTimer->setInterval(200);
    moveTimer->start();
    this->ster_setPositionDone(this->nrSilnika, RuchSilnikaType{false, true, false, false});
    connect(moveTimer, &QTimer::timeout, this, [this]() {this->valAct+=this->valPer100ms;
                    this->ster_setValue(this->nrSilnika, this->valAct);
                    if (abs(this->valAct) > abs(this->destPos)) {
                        this->ster_setPositionDone(this->nrSilnika, RuchSilnikaType{false, false, false, false});
                        moveTimer->stop();
                    }
                    });

}
#endif

void OknoBadaniaKata::timeoutSterownika()
{
    //qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             //<< "hardware timeout ";
    if (deviceisOk)
        return;
    error = QString::fromUtf8("Błąd stanowiska");
    wynikBadania = false;
    reject();
}

bool OknoBadaniaKata::getWynikBadania() const
{
    return wynikBadania;
}

double OknoBadaniaKata::getDegrees() const
{
    return prevVal;
}

const QString &OknoBadaniaKata::getError() const
{
    return error;
}

void OknoBadaniaKata::ster_setPositionDone(short silnik, RuchSilnikaType r)
//void OknoBadaniaKata::ster_setPositionDone(short silnik, bool home, bool move, bool err, bool interrupt)
{
    if (r.home || silnik != nrSilnika)
        return;
    deviceisOk = true;


    if (r.err) {
        wynikBadania = false;
        error = QString("Błąd ustawienia silnika nr %1").arg(silnik);
        reject();
    }
    if (r.inter) {
        wynikBadania = false;
        error = QString("Ruch został przerwany dla silnika nr %1").arg(silnik);
        reject();
    }
    if (!r.move) {
        prevVal = destPos;
        wynikBadania = true;
        error = "";
        accept();
    }
}

void OknoBadaniaKata::ster_setValue(short silnik, const double &val)
{
    if (silnik != nrSilnika)
        return;

    if (prevVal == val)
        return;
    prevVal = val;
    ui->kat->setText(QString("<html><body>%1 &deg;</body></html>").arg(val, 4, 'f', 2));
    int dt;
    if (speedMin)
        dt = 60.0*(abs(destPos-val))/speedMin;
    else
        dt = 0;

    ui->szacowanyczas->setText(QString("%1 s").arg(dt));

}

