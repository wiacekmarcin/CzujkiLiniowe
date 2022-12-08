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
    connect(ui->pbOK, &QPushButton::clicked, this, [this]() { this->pbOK_clicked(); });
    connect(ui->pbOK, &QPushButton::pressed, this, [this]() { this->pbOK_clicked(); });
    connect(ui->pbCancel, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    connect(ui->pbCancel, &QPushButton::pressed, this, [this]() { this->pbCancel_clicked(); });
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

void ParametryBadaniaDlg::pbOK_clicked()
{
    ui->tabBadanie->save(dane);
    ui->tabCzujki->save(dane);
    accept();
}


void ParametryBadaniaDlg::pbCancel_clicked()
{
    qDebug() << "clicked";
    reject();
}




