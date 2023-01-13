#include "test9podsumowanie.h"
#include "ui_test9podsumowanie.h"
#include <QDateTime>


Test9Podsumowanie::Test9Podsumowanie(DaneTestu &daneTestu, const ParametryBadania & badanie, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test9Podsumowanie),
    powtorzPomiar(false)

{
    ui->setupUi(this);
    ui->testName->setText(daneTestu.getName());
    if (daneTestu.getId() == REPRODUCIBILITY) {
        ui->stackedWidget->setCurrentWidget(ui->odtwarzalnosc);
        ui->odtwarzalnosc->valueTest(daneTestu);
        ui->odtwarzalnosc->setDaneTest(true, daneTestu, badanie);
        //badanie->s
    } else if (daneTestu.getId() == REPEATABILITY) {
        ui->stackedWidget->setCurrentWidget(ui->powtarzalnosc);
        ui->powtarzalnosc->valueTest(daneTestu);
        ui->powtarzalnosc->setDaneTest(true, daneTestu, badanie);
    }


    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
}

Test9Podsumowanie::~Test9Podsumowanie()
{
    delete ui;
}
