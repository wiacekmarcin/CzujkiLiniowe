#include "test7badanie.h"
#include "ui_test7badanie.h"

Test7Badanie::Test7Badanie(short nrPomiaru, const DaneTestu &daneTestu,
                           const ParametryBadania &daneBadania, const Ustawienia &ust, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test7Badanie),
    zmFiltraTmr(this),
    zmProgressBar(this),
    actTlumienie(0.0),
    actTlumPos(0),
    maxTlum(0)

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
    zmFiltraTmr.setInterval(daneBadania.getCzasPomZmianaTlumenia_s()*1000);
    //zmFiltraTmr.start();
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(daneBadania.getCzasPomZmianaTlumenia_s());
    connect(&zmProgressBar, &QTimer::timeout, this, &Test7Badanie::progressBarUpdate);
}

Test7Badanie::~Test7Badanie()
{
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
}

void Test7Badanie::flt_setUkladFiltrowDone()
{
    zmFiltraTmr.start();
    ui->progressBar->setValue(0);
    zmProgressBar.start();

}

void Test7Badanie::flt_bladFiltrow(short silnik, bool zerowanie)
{
    zmFiltraTmr.stop();
    ui->progressBar->setValue(0);
    zmProgressBar.stop();
}

void Test7Badanie::uplynalCzasPostojuFiltra()
{
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
}

void Test7Badanie::progressBarUpdate()
{
    ui->progressBar->setValue(ui->progressBar->value() + 1);
}
