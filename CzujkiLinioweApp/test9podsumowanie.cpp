#include "test9podsumowanie.h"
#include "ui_test9podsumowanie.h"
#include <QDateTime>


Test9Podsumowanie::Test9Podsumowanie(const DaneTestu &daneTestu, const ParametryBadania & badanie, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test9Podsumowanie),
    powtorzPomiar(false)

{
    ui->setupUi(this);
    ui->testName->setText(daneTestu.getName());

    ui->odtwarzalnosc->setDaneTest(daneTestu, badanie);
    ui->odtwarzalnosc->setPodsumowanie(true);
    //TODO count Crep Cmax, Cmin etc



    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
}

Test9Podsumowanie::~Test9Podsumowanie()
{
    delete ui;
}
