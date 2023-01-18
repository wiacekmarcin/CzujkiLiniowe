#include "oknobadaniamaksymalnegokata.h"
#include "ui_oknobadaniamaksymalnegokata.h"
#include <QDateTime>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "sterownik.h"
#include <QMutexLocker>

OknoBadaniaMaksymalnegoKata::OknoBadaniaMaksymalnegoKata(short nrSilnika_, const QString &name,
                                 const QString & podtitle,
                                 const double & kat,
                                 const Ustawienia &ust,
                           Sterownik * ster_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoBadaniaMaksymalnegoKata),
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
    ui->maxkat->setText(QString("<html><body>%1 &deg;</body></html>").arg(kat));

    unsigned int speed = ust.predkoscRoboczaImp(nrSilnika);
    unsigned int impulsy = ust.wyliczPozycje(nrSilnika, kat);
    destPos = kat;
    speedMin = ust.wyliczPredkosc(ust.getMotorPrzelozenieImpJedn(nrSilnika), speed);
    ui->speed->setText(QString("<html><body>%1 &deg; / <sub>min<sub></body></html>").arg(speedMin, 3, 'f', 2));

    qDebug() << __FILE__ << __LINE__ << speed << impulsy << "Start ruch";

    ster->setPositionSilnik(nrSilnika, false, impulsy, speed);
    tmSterownika.singleShot(15000, this, &OknoBadaniaMaksymalnegoKata::timeoutSterownika);
    deviceisOk = false;

#ifndef DEFVAL
    ui->pbDalej->setVisible(true);
#else
    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->wynikBadania = true; accept(); });
#endif
}

OknoBadaniaMaksymalnegoKata::~OknoBadaniaMaksymalnegoKata()
{
    tmSterownika.stop();
    delete ui;
}

void OknoBadaniaMaksymalnegoKata::czujkaOn()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << "czujka on";
    tmSterownika.stop();
    wynikBadania = true;
    //error = QString("Czujka zadziała dla kąta : %1").arg(prevVal);
    accept();
}


void OknoBadaniaMaksymalnegoKata::timeoutSterownika()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "hardware timeout ";
    if (deviceisOk)
        return;
    error = QString::fromUtf8("Błąd sprzętowy");
    wynikBadania = false;
    reject();
}

const QString &OknoBadaniaMaksymalnegoKata::getError() const
{
    return error;
}

void OknoBadaniaMaksymalnegoKata::ster_setPositionDone(short silnik, RuchSilnikaType r)
{
    qDebug() << __FILE__ << __LINE__ <<"home" << r.home << "move" << r.move << "err" << r.err << "interrupt" << r.inter;
    if (r.home || silnik != nrSilnika)
        return;
    deviceisOk = true;


    if (r.err) {
        wynikBadania = false;
        error = QString("Błąd ustawienia silnika nr %1").arg(silnik);
        tmSterownika.stop();
        reject();
    }
    if (r.inter) {
        if (abs(destPos - prevVal) > 0.3) {
            error = QString("Ruch został przerwany dla silnika nr %1").arg(silnik);
            wynikBadania = false;
            tmSterownika.stop();
            reject();
        } else {
            wynikBadania = true;
            tmSterownika.stop();
            accept();
        }
    }
    if (!r.move) {
        if (abs(destPos - prevVal) > 0.3) {
            error = QString("Osiągnięta pozycja kątowa (%1) jest zbyt daleka od zadanej (%2)").arg(destPos, 3, 'f', 2).arg(prevVal, 3, 'f', 2);
            wynikBadania = false;
            tmSterownika.stop();
            reject();
        } else {
            wynikBadania = true;
            tmSterownika.stop();
            accept();
        }
    }
}

void OknoBadaniaMaksymalnegoKata::ster_setValue(short silnik, const double &val)
{
    if (silnik != nrSilnika)
        return;

    if (!deviceisOk)
        deviceisOk = true;

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

    if (abs(destPos - prevVal) > 0.5) {
        tmSterownika.stop();
        reject();
    }
}

bool OknoBadaniaMaksymalnegoKata::getWynikBadania() const
{
    return wynikBadania;
}

double OknoBadaniaMaksymalnegoKata::getDegrees() const
{
    return prevVal;
}
