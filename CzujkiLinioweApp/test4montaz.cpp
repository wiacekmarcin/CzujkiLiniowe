#include "test4montaz.h"
#include "ui_test4montaz.h"

#include <QMessageBox>

Test4Montaz::Test4Montaz(short nrPomiaru, const DaneTestu &daneTestu, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test4Montaz)
{
    ui->setupUi(this);

    ui->numerNadajnika->setText(daneTestu.getNumerNadajnika(nrPomiaru));
    ui->numerDrugi->setText(daneTestu.getNumerOdbiornika(nrPomiaru));
    ui->testName->setText(daneTestu.getName());
    ui->info->setText(ui->info->text().replace("[nrCzujki]", QString::number(nrPomiaru)));
    ui->ePierwszy->setText(daneTestu.getNazwaPierwszego());
    ui->eDrugi->setText(daneTestu.getNazwaDrugiego());

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });

}

Test4Montaz::~Test4Montaz()
{
    delete ui;
}

void Test4Montaz::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}
