#include "oknobadaniereakcji6db.h"
#include "ui_oknobadaniereakcji6db.h"
#include <QDateTime>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "sterownik.h"
#include <QMutexLocker>

OknoBadanieReakcji6dB::OknoBadanieReakcji6dB(unsigned int time1, unsigned int time2, const double & tlumnienie, unsigned int dlugoscFali, const double & wartTlum,
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
    pos0C("")

{
    ui->setupUi(this);
    QList<QStringList> tlumienia;
    if (dlugoscFali == 880) {
        tlumienia = ust.getTlumienia880();
    } else {
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
    ui->dlugoscFali->setText(QString::number(dlugoscFali));
    ui->testName->setText(name);
    ui->podTestName->setText(subtitle);

    ui->pbCzujki->setMinimum(0);
    ui->pbCzujki->setMaximum(timeCzujkaOn);
    ui->pbOkna->setMinimum(0);
    ui->pbOkna->setMaximum(timeOknoClose);

    tmZmProgressBar.setInterval(1000);

    connect(&tmZmProgressBar, &QTimer::timeout, this, &OknoBadanieReakcji6dB::progressBarUpdate);

    sterResponse = false;
    ster->setFiltrReset();
    tmSterownika.singleShot(5000, this, &OknoBadanieReakcji6dB::timeoutSterownika);

#ifndef DEFVAL
    ui->pbTest->setVisible(false);
#else
    connect(ui->pbTest, &QPushButton::clicked, this, [this]() { this->czujkaOn(); });
#endif
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
             << "Rozpoczynam zmiane filtra na 6.0 dB";
    ster->setFiltrPos(posA.toShort(), posB.toShort(), posC.toShort());
}

void OknoBadanieReakcji6dB::flt_setUkladFiltrowDone()
{
    qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << __FILE__ << __LINE__ <<
                "Filtr zmieniony";

    if (waitForZeroFiltr) {
        if (endReject)
            reject();
        else
            accept();
        return;
    }

    ui->pbCzujki->setValue(timeCzujkaOn);
    ui->pbOkna->setValue(timeOknoClose);
    tmZmProgressBar.start();
}

void OknoBadanieReakcji6dB::flt_bladFiltrow(QChar filtr, bool zerowanie)
{
    qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << __FILE__ << __LINE__ <<
                "Blad filtrow" << filtr << zerowanie;
    sterResponse = true;
    wynikBadania = false;
    tmSterownika.stop();
    tmZmProgressBar.stop();
    error = QString::fromUtf8("Błąd ustawienia filtra %1").arg(filtr);
    reject();
}

void OknoBadanieReakcji6dB::czujkaOn()
{
    qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << __FILE__ << __LINE__ <<
             "czujka on";
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
        usunTlumnik();
    } else {
        error = QString("Czujka zadziała między %1 a %2 sekundą").arg(timeCzujkaOn).arg(timeOknoClose);
        wynikBadania = false;
        tmSterownika.stop();
        tmZmProgressBar.stop();
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
        error = QString::fromUtf8("Upłynęło %1 sekund bez reakcji czujki").arg(timeOknoClose);
        wynikBadania = false;
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
    reject();
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
