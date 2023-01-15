#include "oknomontaz.h"
#include "ui_oknomontaz.h"

#include <QMessageBox>

OknoMontaz::OknoMontaz(const DaneTestu &daneTestu, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoMontaz)
{
    ui->setupUi(this);

    ui->numerNadajnika->setText(daneTestu.getNumerNadajnika());
    ui->numerDrugi->setText(daneTestu.getNumerOdbiornika());
    ui->testName->setText(daneTestu.getName());

    ui->ePierwszy->setText(daneTestu.getNazwaNumerPierwszego());
    ui->eDrugi->setText(daneTestu.getNazwaNumerDrugiego());

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });

}

OknoMontaz::~OknoMontaz()
{
    delete ui;
}

void OknoMontaz::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}
