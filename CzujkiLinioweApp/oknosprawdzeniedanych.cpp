#include "oknosprawdzeniedanych.h"
#include "ui_oknosprawdzeniedanych.h"
#include <QMessageBox>

OknoSprawdzenieDanych::OknoSprawdzenieDanych(const DaneTestu & test, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoSprawdzenieDanych)
{
    ui->setupUi(this);

    ui->osobaWykonujacaTest->setText(test.getOsobaWykonujaca());
    ui->numerNadajnika->setText(test.getNumerTransmitter());
    ui->numerReceiver->setText(test.getNumerReceiver());
    ui->temperatura->setText(QString("<html><body>%1 &deg; C</body></html>").arg(test.getTemperatura()));
    ui->cisnienie->setText(QString("%1 mbar").arg(test.getCisnienie()));
    ui->wilgotnosc->setText(QString("%1 %").arg(test.getWilgotnosc()));
    ui->testName->setText(test.getName());

    ui->eTransmitter->setText(test.getNazwaTransmitter());
    ui->eReceiver->setText(test.getNazwaReceiver());

    ui->powtarzalnosc_czas->setVisible(test.getId() == REPEATABILITY);
    ui->ePowtarzalnosc->setVisible(test.getId() == REPEATABILITY);
    ui->powtarzalnosc_czas->setText(QString("%1 s").arg(test.getCzasPowtarzalnosci()));


    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->done(QDialog::Accepted); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    adjustSize();
}

OknoSprawdzenieDanych::~OknoSprawdzenieDanych()
{
    delete ui;
}

static int question(QWidget * parent, const QString & title, const QString & pytanie) {
    QMessageBox messageBox(QMessageBox::Question, title, pytanie, 
                        QMessageBox::Yes | QMessageBox::No, parent);

    messageBox.setButtonText(QMessageBox::Yes, QString::fromUtf8("Tak"));
    messageBox.setButtonText(QMessageBox::No, QString::fromUtf8("Nie"));
    return messageBox.exec();
}


void OknoSprawdzenieDanych::pbCancel_clicked()
{
    int ret = question(this, QString::fromUtf8("Badanie : %1").arg(ui->testName->text()), 
                QString::fromUtf8("Czy napewno chcesz przerwać badanie"));
    if (ret == QMessageBox::Yes)
        done(QDialog::Rejected);
}
