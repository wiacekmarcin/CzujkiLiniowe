#include "parametrybadaniadlg.h"
#include "ui_parametrybadaniadlg.h"

ParametryBadaniaDlg::ParametryBadaniaDlg(const Ustawienia & u, ParametryBadania *d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametryBadaniaDlg),
    dane(d),
    checkValidTmr(this)
{
    ui->setupUi(this);
    //ui->error->setVisible(false);
    ui->tabBadanie->init(u, d, ui->error);
    ui->tabCzujki->init(u, d, ui->error);

    connect(&checkValidTmr, &QTimer::timeout, this, &ParametryBadaniaDlg::checkValid);
    checkValidTmr.setInterval(1000);
    checkValidTmr.start();
}

ParametryBadaniaDlg::~ParametryBadaniaDlg()
{
    checkValidTmr.stop();
    delete ui;
}

void ParametryBadaniaDlg::checkValid()
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

void ParametryBadaniaDlg::on_pbOK_clicked()
{
    ui->tabBadanie->save(dane);
    ui->tabCzujki->save(dane);
    accept();
}


void ParametryBadaniaDlg::on_pbCancel_clicked()
{
    qDebug() << "clicked";
    reject();
}


void ParametryBadaniaDlg::on_pbCancel_pressed()
{
    qDebug() << "pressed";
    reject();
}


void ParametryBadaniaDlg::on_pbOK_pressed()
{
    on_pbOK_clicked();
}

