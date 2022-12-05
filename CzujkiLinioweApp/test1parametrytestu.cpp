#include "test1parametrytestu.h"
#include "ui_test1parametrytestu.h"

Test1ParametryTestu::Test1ParametryTestu(DaneBadania * badanie, short TestId, short CzujkaId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test1ParametryTestu)
{
    ui->setupUi(this);

    //ui->testName->setText(badanie->)
}

Test1ParametryTestu::~Test1ParametryTestu()
{
    delete ui;
}
