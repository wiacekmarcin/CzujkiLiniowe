#include "test7stabilizacjaczujki.h"
#include "ui_test7stabilizacjaczujki.h"

Test7StabilizacjaCzujki::Test7StabilizacjaCzujki(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test7StabilizacjaCzujki)
{
    ui->setupUi(this);
}

Test7StabilizacjaCzujki::~Test7StabilizacjaCzujki()
{
    delete ui;
}
