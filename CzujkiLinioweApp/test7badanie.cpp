#include "test7badanie.h"
#include "ui_test7badanie.h"
#include <QDateTime>
#include "sterownik.h"

Test7Badanie::Test7Badanie(short /*nrPomiaru*/, const DaneTestu &daneTestu,
                           const ParametryBadania &daneBadania, const Ustawienia &ust,
                           Sterownik * ster_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test7Badanie),
    zmFiltraTmr(this),
    zmProgressBar(this),
    actTlumienie(0.0),
    actTlumPos(0),
    maxTlum(0),
    ster(ster_)

{
    ui->setupUi(this);

    if (daneBadania.getDlugoscFaliFiltrow() == 880) {
        tlumienia = ust.getTlumienia880();
    } else {
        tlumienia = ust.getTlumienia655();
    }
    maxTlum=tlumienia.size();
    ui->dlugoscFali->setText(QString::number(daneBadania.getDlugoscFaliFiltrow()));
    ui->testName->setText(daneTestu.getName());
    ui->czas->setText(QString::number(daneBadania.getCzasPomZmianaTlumenia_s()) + QString(" s"));
    connect(&zmFiltraTmr, &QTimer::timeout, this, &Test7Badanie::uplynalCzasPostojuFiltra);
    czasPostoju = daneBadania.getCzasPomZmianaTlumenia_s();
    zmFiltraTmr.setInterval(daneBadania.getCzasPomZmianaTlumenia_s()*1000);
    //zmFiltraTmr.start();
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(daneBadania.getCzasPomZmianaTlumenia_s());
    connect(&zmProgressBar, &QTimer::timeout, this, &Test7Badanie::progressBarUpdate);
    zmProgressBar.setInterval(1000);
    qDebug() << "__FILE__" << __LINE__;
    ster->setFiltrReset();
}

Test7Badanie::~Test7Badanie()
{
    zmFiltraTmr.stop();
    zmProgressBar.stop();
    delete ui;
}

void Test7Badanie::flt_zerowanieFiltrowDone()
{
    actTlumPos = 0;
    auto pos = tlumienia.at(actTlumPos);
    ui->tlumienie->setText(pos.at(0) + QString(" dB"));
    ui->a->setText(pos.at(1));
    ui->b->setText(pos.at(2));
    ui->c->setText(pos.at(3));
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "Rozpoczynam zmiane filtra";
    ster->setFiltrPos(pos.at(1).toShort(), pos.at(2).toShort(), pos.at(3).toShort());
}

void Test7Badanie::flt_setUkladFiltrowDone()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << "Zmiana filtra";
    zmFiltraTmr.start();
    ui->progressBar->setValue(0);
    zmProgressBar.start();
    ui->pozostal_czas->setText(0);
}

void Test7Badanie::flt_bladFiltrow(QChar filtr, bool zerowanie)
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << "Blad filtrow"
             << filtr << zerowanie;

    zmFiltraTmr.stop();
    ui->progressBar->setValue(0);
    zmProgressBar.stop();
}

void Test7Badanie::uplynalCzasPostojuFiltra()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << ui->progressBar->value() << " timeout 15s " << actTlumPos;
                ;
    zmProgressBar.stop();
    ui->progressBar->setValue(0);
    ui->pozostal_czas->setText(0);
    ++actTlumPos;
    if (actTlumPos == maxTlum) {
        zmFiltraTmr.stop();
        reject();
        return;
    }
    auto pos = tlumienia.at(actTlumPos);
    ui->tlumienie->setText(pos.at(0) + QString(" dB"));
    ui->a->setText(pos.at(1));
    ui->b->setText(pos.at(2));
    ui->c->setText(pos.at(3));
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "Rozpoczynam zmiane filtra";
    ster->setFiltrPos(pos.at(1).toShort(), pos.at(2).toShort(), pos.at(3).toShort());
}


void Test7Badanie::progressBarUpdate()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "Progress update " << ui->progressBar->value();
    ui->progressBar->setValue(ui->progressBar->value() + 1);
    ui->pozostal_czas->setText(QString::number(czasPostoju - ui->progressBar->value()));
}
