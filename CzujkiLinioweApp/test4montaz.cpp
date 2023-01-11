#include "test4montaz.h"
#include "ui_test4montaz.h"

#include <QMessageBox>

Test4Montaz::Test4Montaz(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test4Montaz)
{
    ui->setupUi(this);

    ui->numerPierwszy->setText(daneTestu.getNumerPierwszy(nrPomiaru));
    ui->numerDrugi->setText(daneTestu.getNumerDrugi(nrPomiaru));
    ui->testName->setText(daneTestu.getName());
    ui->info->setText(ui->info->text().replace("[nrCzujki]", QString::number(nrPomiaru)));

    if (daneBadania.getSystemOdbiornikNadajnik()) { //odb <-> nad
        ui->ePierwszy->setText("* Nadajnik");
        ui->eDrugi->setText("* Odbiornik");
    } else { //odb+nad <-> ref
        ui->ePierwszy->setText("* Nadajnik+Odbiornik");
        ui->eDrugi->setText("* Reflektor");
    }

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
