#include "test6polaczeniesterownika.h"
#include "ui_test6polaczeniesterownika.h"

Test6PolaczenieSterownika::Test6PolaczenieSterownika(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test6PolaczenieSterownika)
{
    ui->setupUi(this);
}

Test6PolaczenieSterownika::~Test6PolaczenieSterownika()
{
    delete ui;
}
