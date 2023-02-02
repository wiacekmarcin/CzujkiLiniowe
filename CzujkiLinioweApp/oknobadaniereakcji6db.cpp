#include "oknobadaniereakcji6db.h"
#include "ui_oknobadaniereakcji6db.h"
#include <QDateTime>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "sterownik.h"
#include <QMutexLocker>
#include <QDebug>

OknoBadanieReakcji6dB::OknoBadanieReakcji6dB(unsigned int time1, unsigned int time2,
                           unsigned int dlugoscFali, const double & tlumnienie,
                           const QString & name, const QString & subtitle, const Ustawienia &ust,
                           Sterownik * ster_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoBadanieReakcji6dB),
    tmZmProgressBar(this),
    timeCzujkaOn(time1),
    timeOknoClose(time2),
    posA("0"),
    posB("0"),
    posC("0"),
    tmSterownika(this),
    ster(ster_),
    wynikBadania(false),
    time_sec(0),
    waitForZeroFiltr(false),
    endReject(true),
    pos0A(""),
    pos0B(""),
    pos0C(""),
    czujkaWyzwolona(false)

{
    ui->setupUi(this);

    //qDebug() << __FILE__ << __LINE__ << "tlumienie" << tlumnienie << "Czas1" <<  time1 << "Czas2" <<  time2;
    ui->tlumienie->setText(QString::number(tlumnienie, 'f', 1) + " dB");
    QList<QStringList> tlumienia;
    if (dlugoscFali == 880) {
        tlumienia = ust.getTlumienia880();
    } else if (dlugoscFali == 655) {
        tlumienia = ust.getTlumienia655();
    }

    for (const auto & pos : tlumienia) {
        if (pos.at(0).toDouble() == 0.0) {
            pos0A = pos.at(1);
            pos0B = pos.at(2);
            pos0C = pos.at(3);
            //break;
        }
        if (pos.at(0).toDouble() == tlumnienie) {
            posA = pos.at(1);
            posB = pos.at(2);
            posC = pos.at(3);
            //break;
        }
    }
    //qDebug
    ui->dlugoscFali->setText(QString::number(dlugoscFali) + " nm");
    ui->testName->setText(name);
    if (subtitle.isEmpty()) {
        ui->podTestName->setVisible(false);
    } else {
        ui->podTestName->setText(subtitle);
        ui->podTestName->setVisible(true);
    }

    ui->pbCzujki->setMinimum(0);
    ui->pbCzujki->setMaximum(timeCzujkaOn);
    ui->pbOkna->setMinimum(0);
    ui->pbOkna->setMaximum(timeOknoClose);
    ui->czasCzujki->setText(QString::number(time1) + " s");
    ui->czasOkna->setText(QString::number(time2) + " s");

    tmZmProgressBar.setInterval(1000);

    connect(&tmZmProgressBar, &QTimer::timeout, this, &OknoBadanieReakcji6dB::progressBarUpdate);

    sterResponse = false;
    ster->setFiltrReset();
    tmSterownika.singleShot(5000, this, &OknoBadanieReakcji6dB::timeoutSterownika);

#ifndef TESTVAL
    ui->pbTest->setVisible(false);
#else
    connect(ui->pbTest, &QPushButton::clicked, this, [this]() { this->czujkaOn(); this->flt_setUkladFiltrowDone(); });
    sterResponse = true;
#endif
    adjustSize();
}

OknoBadanieReakcji6dB::~OknoBadanieReakcji6dB()
{
    tmZmProgressBar.stop();
    tmSterownika.stop();
    delete ui;
}

void OknoBadanieReakcji6dB::flt_zerowanieFiltrowDone()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "Rozpoczynam zmiane filtra";
    sterResponse = true;
    tmSterownika.stop();

    ui->a->setText(posA);
    ui->b->setText(posB);
    ui->c->setText(posC);
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "Rozpoczynam zmiane filtra na " << ui->tlumienie->text();
    ster->setFiltrPos(posA.toShort(), posB.toShort(), posC.toShort());
}

void OknoBadanieReakcji6dB::flt_setUkladFiltrowDone()
{
    qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << __FILE__ << __LINE__ <<
                "Filtr zmieniony";
    if (waitForZeroFiltr) {
        qDebug() << "Wychodze z " << endReject;
        if (endReject)
            done(QDialog::Rejected);
        else
            done(QDialog::Accepted);
        return;
    }


    ui->pbCzujki->setValue(timeCzujkaOn);
    ui->pbOkna->setValue(timeOknoClose);
    tmZmProgressBar.start();
}

void OknoBadanieReakcji6dB::flt_bladFiltrow(QChar filtr, bool zerowanie)
{
    (void)zerowanie;
    qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << __FILE__ << __LINE__ <<
                "Blad filtrow" << filtr << zerowanie;
    sterResponse = true;
    wynikBadania = false;
    tmSterownika.stop();
    tmZmProgressBar.stop();
    error = QString::fromUtf8("Błąd ustawienia filtra %1").arg(filtr);
    done(QDialog::Rejected);
}

void OknoBadanieReakcji6dB::czujkaOn()
{
    qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << __FILE__ << __LINE__ <<
             "czujka on";
    if (czujkaWyzwolona)
        return;
    czujkaWyzwolona = true;
    tmZmProgressBar.stop();
    tmSterownika.stop();
    wynikBadania = true;
    error = "";
    if (time_sec <= timeCzujkaOn) {
        error = "";
        wynikBadania = true;
        tmSterownika.stop();
        tmZmProgressBar.stop();
        endReject = false;
        qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << __FILE__ << __LINE__ <<
                 "Prawidlowe dzialanie";
        usunTlumnik();
    } else {
        error = QString::fromUtf8("Czujka zadziała między %1 a %2 sekundą").arg(timeCzujkaOn).arg(timeOknoClose);
        wynikBadania = false;
        tmSterownika.stop();
        tmZmProgressBar.stop();
        qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << __FILE__ << __LINE__ <<
                "Po czasie";
        endReject = true;
        usunTlumnik();
    }
}

void OknoBadanieReakcji6dB::progressBarUpdate()
{
    ++time_sec;
    if (time_sec <= timeCzujkaOn) {
        ui->pbCzujki->setValue(timeCzujkaOn - time_sec);
        ui->pozostal_czasCzujki->setText(QString::number(timeCzujkaOn - time_sec));
    }
    if (time_sec <= timeOknoClose) {
        ui->pbOkna->setValue(timeOknoClose - time_sec);
        ui->pozostaly_czasokna->setText(QString::number(timeOknoClose - time_sec));
    } else if (waitForZeroFiltr) {
        return;
    } else {
        qDebug() << "Uplynal czas do wyzwolenia czujki";
        error = QString::fromUtf8("Czujka nie zareagowała po upływie %1 sekund").arg(timeOknoClose);
        wynikBadania = false;
        tmSterownika.stop();
        tmZmProgressBar.stop();
        endReject = true;
        usunTlumnik();
    }
}

void OknoBadanieReakcji6dB::timeoutSterownika()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "hardware timeout ";
    if (sterResponse)
        return;
    error = QString::fromUtf8("Błąd stanowiska");
    wynikBadania = false;
    tmZmProgressBar.stop();
    done(QDialog::Rejected);
}

const QString &OknoBadanieReakcji6dB::getError() const
{
    return error;
}

void OknoBadanieReakcji6dB::usunTlumnik()
{
    waitForZeroFiltr = true;
    tmZmProgressBar.stop();
    tmSterownika.stop();
    ui->a->setText(pos0A);
    ui->b->setText(pos0B);
    ui->c->setText(pos0C);

    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "Rozpoczynam zmiane filtra na 0.0 dB";
    ster->setFiltrPos(pos0A.toShort(), pos0B.toShort(), pos0C.toShort());
}

bool OknoBadanieReakcji6dB::getWynikBadania() const
{
    return wynikBadania;
}
