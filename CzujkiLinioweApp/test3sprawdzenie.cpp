#include "test3sprawdzenie.h"
#include "danetestu.h"
#include "ui_test3sprawdzenie.h"


//Nazwa testu [Ten ekran nie pojawi się dla badań odwarzalności, powatarzalności, katowych , zasilania
#include "danetestu.h"
#include "parametrybadania.h"

#include <QMessageBox>

Test3Sprawdzenie::Test3Sprawdzenie(const ParametryBadania &badanie, const DaneTestu & test, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test3Sprawdzenie)
{
    ui->setupUi(this);
    ui->osobaWykonujacaTest->setText(test.getOsobaWykonujaca());
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

Test3Sprawdzenie::~Test3Sprawdzenie()
{
    delete ui;
}

void Test3Sprawdzenie::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}
