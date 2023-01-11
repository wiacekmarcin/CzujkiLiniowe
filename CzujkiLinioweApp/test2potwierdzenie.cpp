#include "test2potwierdzenie.h"
#include "ui_test2potwierdzenie.h"
#include <QMessageBox>

Test2Potwierdzenie::Test2Potwierdzenie(short nrPomiaru, const DaneTestu & test, const ParametryBadania &badanie, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test2Potwierdzenie)
{
    ui->setupUi(this);

    ui->osobaWykonujacaTest->setText(test.getOsobaWykonujaca());
    ui->numerNadajnika->setText(test.getNumerNadajnika(nrPomiaru));
    ui->numerDrugi->setText(test.getNumerOdbiornika(nrPomiaru));
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
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });

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
