#include "parametrybadania.h"
#include "ui_parametrybadania.h"

ParametryBadania::ParametryBadania(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametryBadania)
{
    ui->setupUi(this);
}

ParametryBadania::~ParametryBadania()
{
    delete ui;
}
