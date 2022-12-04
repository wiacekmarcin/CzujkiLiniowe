#include "test2potwierdzenie.h"
#include "ui_test2potwierdzenie.h"

Test2Potwierdzenie::Test2Potwierdzenie(QWidget *parent) :
    QDialog(parent),
    ui(new Test2Potwierdzenie)
{
    ui->setupUi(this);
}

Test2Potwierdzenie::~Test2Potwierdzenie()
{
    delete ui;
}
