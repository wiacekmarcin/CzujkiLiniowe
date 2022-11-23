#include "parametrybadania1.h"
#include "ui_parametrybadania1.h"

ParametryBadania1::ParametryBadania1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametryBadania1)
{
    ui->setupUi(this);
}

ParametryBadania1::~ParametryBadania1()
{
    delete ui;
}

void ParametryBadania1::init(const Ustawienia &u, BadanieDane *badanie)
{
    ui->numerZlecenia->setText(badanie->getNumerZlecenia());
    ui->numerTestu->setText(badanie->getNumerTestu());
    ui->osobaOdpowiedzialna->setText(badanie->getOsobaOdpowiedzialna());
    ui->uwagi->setText(badanie->getUwagi());

    if (badanie->getWewnetrzyZasilacz()) {
        ui->napiecieZasilaniain->setText(QString::number(badanie->getZasilanie_mV()/1000.0, 'g', 3));
        setWewnetrznyZasilacz(true);
    }

    if (badanie->getCentralaZasilanie()) {
        ui->typCentraliSygnalizacji->setText(badanie->getCentralaTyp());
        setWewnetrznyZasilacz(false);
    }

    ui->czasStabilizacjiCzujki->setText(QString::number(badanie->getCzasStabilizacji_ms()/1000.0, 'g', 1));
    if (badanie->getWyzwalanieAlarmuPrzekaznik()) {
        setWyzwolenieAlarmu(true);
    }

    if (badanie->getWyzwalanieAlarmuPrad()) {
        setWyzwolenieAlarmu(false);
        ui->pradAlarmu->setText(QString::number(badanie->getPradWyzwolenia_mA()));
    }

    ui->dlugoscFali->setCurrentText(QString::number(badanie->getDlugoscFaliFiltrow()));
    ui->czasPomiedzyZmianamifiltra->setText(QString::number(badanie->getCzasPomiedzyZmianaTlumenia_ms()/1000.0, 'g', 1));
}

void ParametryBadania1::setWewnetrznyZasilacz(bool inside)
{
    ui->rbAlarmPrzekaznik->setChecked(inside);
    ui->napiecieZasilaniain->setDisabled(!inside);
    ui->rbCentralSupply->setChecked(!inside);
    ui->typCentraliSygnalizacji->setDisabled(inside);
}

void ParametryBadania1::setWyzwolenieAlarmu(bool przekaznik)
{
    ui->rbAlarmPrzekaznik->setChecked(przekaznik);
    ui->rbPrad->setChecked(!przekaznik);
    ui->pradAlarmu->setDisabled(przekaznik);
}
