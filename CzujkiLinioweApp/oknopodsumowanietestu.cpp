#include "oknopodsumowanietestu.h"
#include "ui_oknopodsumowanietestu.h"
#include <QDateTime>


OknoPodsumowanieTestu::OknoPodsumowanieTestu(DaneTestu &daneTestu, const ParametryBadania & badanie, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoPodsumowanieTestu),
    powtorzPomiar(false)

{
    ui->setupUi(this);
    ui->testName->setText(daneTestu.getName());
    qDebug() << "id" << daneTestu.getId();
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

OknoPodsumowanieTestu::~OknoPodsumowanieTestu()
{
    delete ui;
}
