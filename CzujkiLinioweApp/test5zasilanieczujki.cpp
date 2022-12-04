#include "test5zasilanieczujki.h"
#include "ui_test5zasilanieczujki.h"

Test5ZasilanieCzujki::Test5ZasilanieCzujki(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test5ZasilanieCzujki)
{
    ui->setupUi(this);
}

Test5ZasilanieCzujki::~Test5ZasilanieCzujki()
{
    delete ui;
}
