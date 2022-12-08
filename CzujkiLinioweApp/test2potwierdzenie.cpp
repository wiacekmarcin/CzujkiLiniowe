#include "test2potwierdzenie.h"
#include "ui_test2potwierdzenie.h"
#include <QMessageBox>

Test2Potwierdzenie::Test2Potwierdzenie(const ParametryBadania &badanie, const DaneTestu & test, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test2Potwierdzenie)
{
    ui->setupUi(this);

    ui->osobaWykonujacaTest->setText(test.getOsobaWykonujaca());
    ui->numerPierwszy->setText(test.getNumerPierwszy());
    ui->numerDrugi->setText(test.getNumerDrugi());
    ui->temperatura->setText(test.getTemperatura());
    ui->cisnienie->setText(test.getCisnienie());
    ui->wilgotnosc->setText(test.getWilgotnosc());
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

Test2Potwierdzenie::~Test2Potwierdzenie()
{
    delete ui;
}

void Test2Potwierdzenie::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()), "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}
