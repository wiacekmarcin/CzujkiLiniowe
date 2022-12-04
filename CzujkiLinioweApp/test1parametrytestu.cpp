#include "test1parametrytestu.h"
#include "ui_test1parametrytestu.h"

Test1ParametryTestu::Test1ParametryTestu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test1ParametryTestu)
{
    ui->setupUi(this);
}

Test1ParametryTestu::~Test1ParametryTestu()
{
    delete ui;
}
