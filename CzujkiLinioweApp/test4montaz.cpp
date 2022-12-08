#include "test4montaz.h"
#include "ui_test4montaz.h"

#include <QMessageBox>

Test4Montaz::Test4Montaz(const ParametryBadania &badanie, const DaneTestu &test, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test4Montaz)
{
    ui->setupUi(this);

    ui->numerPierwszy->setText(test.getNumerPierwszy());
    ui->numerDrugi->setText(test.getNumerDrugi());
    ui->testName->setText(test.getName());

    if (badanie.getSystemOdbiornikNadajnik()) { //odb <-> nad
        ui->ePierwszy->setText("Nadajnik");
        ui->eDrugi->setText("Odbiornik");
    } else { //odb+nad <-> ref
        ui->ePierwszy->setText("Nadajnik+Odbiornik");
        ui->eDrugi->setText("Reflektor");
    }

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
    connect(ui->pbDalej, &QPushButton::pressed, this, [this]() { this->accept(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    connect(ui->pbPrzerwij, &QPushButton::pressed, this, [this]() { this->pbCancel_clicked(); });
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
