#include "test3sprawdzenie.h"
#include "danetestu.h"
#include "ui_test3sprawdzenie.h"


//Nazwa testu [Ten ekran nie pojawi się dla badań odwarzalności, powatarzalności, katowych , zasilania
#include "danetestu.h"


#include <QMessageBox>

Test3Sprawdzenie::Test3Sprawdzenie(short nrPomiaru, const DaneTestu & daneTestu,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test3Sprawdzenie)
{
    ui->setupUi(this);
    ui->osobaWykonujacaTest->setText(daneTestu.getOsobaWykonujaca());
    ui->numerNadajnika->setText(daneTestu.getNumerNadajnika(nrPomiaru));
    ui->numerDrugi->setText(daneTestu.getNumerOdbiornika(nrPomiaru));
    ui->testName->setText(daneTestu.getName());

    ui->ePierwszy->setText(daneTestu.getNazwaPierwszego());
    ui->eDrugi->setText(daneTestu.getNazwaDrugiego());

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
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
