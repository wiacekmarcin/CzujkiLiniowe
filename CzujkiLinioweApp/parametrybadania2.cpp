#include "parametrybadania2.h"
#include "ui_parametrybadania2.h"

ParametryBadania2::ParametryBadania2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametryBadania2)
{
    ui->setupUi(this);
    ui->lcz1->setDisabled(true);
    ui->lcz2->setEnabled(false);
    ui->nadajnik1->setDisabled(true);
    ui->odbiornik1->setDisabled(true);
    ui->reflektor1->setVisible(false);
}

ParametryBadania2::~ParametryBadania2()
{
    delete ui;
}
