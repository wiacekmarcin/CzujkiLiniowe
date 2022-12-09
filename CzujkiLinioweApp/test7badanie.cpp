#include "test7badanie.h"
#include "ui_test7badanie.h"

Test7Badanie::Test7Badanie(const ParametryBadania &badanie, const DaneTestu &test, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test7Badanie),
    zmFiltraTmr(this),
    actTlumienie(0.0)
{
    ui->setupUi(this);
    ui->testName->setText(test.getName());
    ui->czas->setText(QString::number(badanie.getCzasPomZmianaTlumenia_s()) + QString(" s"));
    connect(&zmFiltraTmr, &QTimer::timeout, this, &Test7Badanie::uplynalCzasPostojuFiltra);
    zmFiltraTmr.setInterval(badanie.getCzasPomZmianaTlumenia_s());
    zmFiltraTmr.start();
}

Test7Badanie::~Test7Badanie()
{
    delete ui;
}

void Test7Badanie::uplynalCzasPostojuFiltra()
{
    actTlumienie += 0.1;
    ui->tlumienie->setText(QString::number(actTlumienie, 'f', 1) + QString(" dB"));
}
