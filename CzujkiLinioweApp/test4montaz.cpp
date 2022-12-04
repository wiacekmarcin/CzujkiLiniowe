#include "test4montaz.h"
#include "ui_test4montaz.h"

Test4Montaz::Test4Montaz(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test4Montaz)
{
    ui->setupUi(this);
}

Test4Montaz::~Test4Montaz()
{
    delete ui;
}
