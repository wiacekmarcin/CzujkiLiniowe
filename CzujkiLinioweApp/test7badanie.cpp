#include "test7badanie.h"
#include "ui_test7badanie.h"

Test7Badanie::Test7Badanie(short nrPomiaru, const DaneTestu &daneTestu,
                           const ParametryBadania &daneBadania, const Ustawienia &ust, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test7Badanie),
    zmFiltraTmr(this),
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
    zmFiltraTmr.start();
}

Test7Badanie::~Test7Badanie()
{
    delete ui;
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
