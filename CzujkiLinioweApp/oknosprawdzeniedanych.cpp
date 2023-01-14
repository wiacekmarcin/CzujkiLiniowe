#include "oknosprawdzeniedanych.h"
#include "ui_oknosprawdzeniedanych.h"
#include <QMessageBox>

OknoSprawdzenieDanych::OknoSprawdzenieDanych(const DaneTestu & test, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoSprawdzenieDanych)
{
    ui->setupUi(this);

    ui->osobaWykonujacaTest->setText(test.getOsobaWykonujaca());
    ui->numerNadajnika->setText(test.getNumerNadajnika());
    ui->numerDrugi->setText(test.getNumerOdbiornika());
    ui->temperatura->setText(test.getTemperatura());
    ui->cisnienie->setText(test.getCisnienie());
    ui->wilgotnosc->setText(test.getWilgotnosc());
    ui->testName->setText(test.getName());

    ui->ePierwszy->setText(test.getNazwaPierwszego());
    ui->eDrugi->setText(test.getNazwaDrugiego());

    ui->powtarzalnosc_czas->setVisible(test.getId() == REPEATABILITY);
    ui->ePowtarzalnosc->setVisible(test.getId() == REPEATABILITY);
    ui->powtarzalnosc_czas->setText(QString("%1 s").arg(test.getCzasPowtarzalnosci()));


    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });

}

OknoSprawdzenieDanych::~OknoSprawdzenieDanych()
{
    delete ui;
}

void OknoSprawdzenieDanych::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()), "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}
