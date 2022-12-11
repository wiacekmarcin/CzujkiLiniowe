#include "test0zerowanieurzadzenia.h"
#include "ui_test0zerowanieurzadzenia.h"

Test0ZerowanieUrzadzenia::Test0ZerowanieUrzadzenia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test0ZerowanieUrzadzenia)
{
    ui->setupUi(this);
}

Test0ZerowanieUrzadzenia::~Test0ZerowanieUrzadzenia()
{
    delete ui;
}
