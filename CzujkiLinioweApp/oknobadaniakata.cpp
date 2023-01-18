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
    ui->setupUi(this);
    ui->testName->setText(name);
    ui->nazwaPodTestu->setText(podtitle);
    ui->katProducenta->setText(QString("<html><body>%1 &deg;</body></html>").arg(kat));

    unsigned int speed = ust.predkoscRoboczaImp(nrSilnika);
    unsigned int impulsy = ust.wyliczPozycje(nrSilnika, kat.toDouble());
    destPos = kat.toDouble();
    speedMin = ust.wyliczPredkosc(ust.getMotorPrzelozenieImpJedn(nrSilnika), speed);
    ui->speed->setText(QString("<html><body>%1 &deg; / <sub>min<sub></body></html>").arg(speedMin, 3, 'f', 2));

    ster->setPositionSilnik(nrSilnika, false, impulsy, speed);
    tmSterownika.singleShot(5000, this, &OknoBadaniaKata::timeoutSterownika);
    deviceisOk = false;

#ifndef DEFVAL
    ui->pbDalej->setVisible(true);
#else
    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->wynikBadania = true; accept(); });
#endif
}

OknoBadaniaKata::~OknoBadaniaKata()
{
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


void OknoBadaniaKata::timeoutSterownika()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "hardware timeout ";
    if (deviceisOk)
        return;
    error = QString::fromUtf8("Błąd stanowiska");
    wynikBadania = false;
    reject();
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
        error = QString("Ruch został przerwany dla silnika nr %1").arg(silnik);
        reject();
    }
    if (!r.move) {
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
        dt = 60.0*(destPos-val)/speedMin;
    else
        dt = 0;
    qDebug() << destPos << val << (destPos-val) << (60.0*(destPos-val)/speedMin);
    ui->szacowanyczas->setText(QString("%1 s").arg(dt));

}

bool OknoBadaniaKata::getWynikBadania() const
{
    return wynikBadania;
}

