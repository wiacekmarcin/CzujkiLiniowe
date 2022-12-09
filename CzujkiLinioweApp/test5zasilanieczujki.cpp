#include "test5zasilanieczujki.h"
#include "ui_test5zasilanieczujki.h"
#include "zasilacz.h"
#include <QMessageBox>

Test5ZasilanieCzujki::Test5ZasilanieCzujki(const ParametryBadania &badanie, const DaneTestu &test, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test5ZasilanieCzujki)
{
    ui->setupUi(this);
    ui->testName->setText(test.getName());
    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
    connect(ui->pbDalej, &QPushButton::pressed, this, [this]() { this->accept(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    connect(ui->pbPrzerwij, &QPushButton::pressed, this, [this]() { this->pbCancel_clicked(); });

    ui->testName->setText(test.getName());
    if (badanie.getZasCzujekWbudZasilacz()) {
        ui->info->setText("Czujka została zasilona. Należy ustawić maksymalną czułość na czujce.");
        ui->rodzajZasilania->setText("Zasilacz");
        ui->napiecieUst->setText(QString::number(badanie.getNapiecieZasCzujki_mV()/1000.0, 'f', 3)+QString(" V"));
        ui->napiecieMierz->setText("0.000");
        ui->pradMierz->setText("0.000");
        ui->lTypCentrali->setEnabled(false);
        ui->typCentrali->setEnabled(false);
        ui->lNapiecieUst->setEnabled(true);
        ui->lNapiecieMierz->setEnabled(true);
        ui->lPradMierz->setEnabled(true);
        ui->napiecieUst->setEnabled(true);
        ui->napiecieMierz->setEnabled(true);
        ui->pradMierz->setEnabled(true);
    } else {
        ui->info->setText("Należy załączyć zasilanie czujki z centrali sygnalizacji pożarowej");
        ui->rodzajZasilania->setText("Centrala sygnalizacji pożarowej");
        ui->typCentrali->setText(badanie.getTypCentraliSygnPoz());
        ui->lTypCentrali->setEnabled(true);
        ui->typCentrali->setEnabled(true);
        ui->lNapiecieUst->setEnabled(false);
        ui->lNapiecieMierz->setEnabled(false);
        ui->lPradMierz->setEnabled(false);
        ui->napiecieUst->setEnabled(false);
        ui->napiecieMierz->setEnabled(false);
        ui->pradMierz->setEnabled(false);
    }
}

Test5ZasilanieCzujki::~Test5ZasilanieCzujki()
{
    delete ui;
}

void Test5ZasilanieCzujki::setCurrent_mA(int mA)
{
    ui->pradMierz->setText(QString::number(mA) + QString(" mA"));
}

void Test5ZasilanieCzujki::setVolage_mV(int mV)
{
    ui->napiecieMierz->setText(QString::number(mV/1000.0, 'f', 3)+QString(" V"));
}

void Test5ZasilanieCzujki::value(int kind, int value)
{
    if (kind == Zasilacz::VOLTAGE_MEAS)
        setVolage_mV(value);
    if (kind == Zasilacz::CURRENT_MEAS)
        setCurrent_mA(value);
}

void Test5ZasilanieCzujki::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}
