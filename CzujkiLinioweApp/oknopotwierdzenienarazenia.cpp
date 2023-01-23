#include "oknopotwierdzenienarazenia.h"
#include "danetestu.h"
#include "ui_oknopotwierdzenienarazenia.h"


//Nazwa testu [Ten ekran nie pojawi się dla badań odwarzalności, powatarzalności, katowych , zasilania
#include "danetestu.h"


#include <QMessageBox>

OknoPotwierdzenieNarazenia::OknoPotwierdzenieNarazenia(const DaneTestu & daneTestu,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoPotwierdzenieNarazenia)
{
    ui->setupUi(this);
    ui->osobaWykonujacaTest->setText(daneTestu.getOsobaWykonujaca());
    ui->numerNadajnika->setText(daneTestu.getNumerTransmitter());
    ui->numerReceiver->setText(daneTestu.getNumerReceiver());
    ui->testName->setText(daneTestu.getName());

    ui->eTransmitter->setText(daneTestu.getNazwaNumerTransmitter());
    ui->eReceiver->setText(daneTestu.getNazwaNumerReceiver());

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->done(QDialog::Accepted); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
}

OknoPotwierdzenieNarazenia::~OknoPotwierdzenieNarazenia()
{
    delete ui;
}

void OknoPotwierdzenieNarazenia::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        done(QDialog::Rejected);
}
