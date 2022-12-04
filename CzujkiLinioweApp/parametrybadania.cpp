#include "parametrybadania.h"
#include "ui_parametrybadania.h"

ParametryBadania::ParametryBadania(const Ustawienia & u, DaneBadania *d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametryBadania),
    dane(d),
    checkValidTmr(this)
{
    ui->setupUi(this);
    //ui->error->setVisible(false);
    ui->tabBadanie->init(u, d, ui->error);
    ui->tabCzujki->init(u, d, ui->error);

    connect(&checkValidTmr, &QTimer::timeout, this, &ParametryBadania::checkValid);
    checkValidTmr.setInterval(1000);
    checkValidTmr.start();
}

ParametryBadania::~ParametryBadania()
{
    checkValidTmr.stop();
    delete ui;
}

void ParametryBadania::checkValid()
{
    bool valid = false;
    if (ui->tabWidget->currentWidget() == ui->tabBadanie) {
        if (ui->tabBadanie->check())
            valid = ui->tabCzujki->check();
    } else if (ui->tabWidget->currentWidget() == ui->tabCzujki) {
        if (ui->tabCzujki->check())
            valid = ui->tabBadanie->check();
    }
    ui->pbOK->setEnabled(valid);

}

void ParametryBadania::on_pbOK_clicked()
{
    ui->tabBadanie->save(dane);
    ui->tabCzujki->save(dane);
}

