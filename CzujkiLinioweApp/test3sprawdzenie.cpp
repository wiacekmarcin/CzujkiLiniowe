#include "test3sprawdzenie.h"
#include "ui_test3sprawdzenie.h"

Test3Sprawdzenie::Test3Sprawdzenie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test3Sprawdzenie)
{
    ui->setupUi(this);
}

Test3Sprawdzenie::~Test3Sprawdzenie()
{
    delete ui;
}
