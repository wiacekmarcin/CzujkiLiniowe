#include "oknozasilaniaczujki.h"
#include "ui_oknozasilaniaczujki.h"
#include "zasilacz.h"
#include <QMessageBox>

OknoZasilaniaCzujki::OknoZasilaniaCzujki(bool maksCzulosc, const DaneTestu &daneTestu,
                                         const ParametryBadania &daneBadania, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoZasilaniaCzujki)
{
    ui->setupUi(this);
    ui->testName->setText(daneTestu.getName());

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });

    ui->testName->setText(daneTestu.getName());
    if (maksCzulosc) {
        ui->info->setText(ui->info->text().replace("[CZULOSC]", "maksymalną czułość"));
        ui->info2->setText(ui->info2->text().replace("[CZULOSC]", "najwyższą czułość"));
    } else {
        ui->info->setText(ui->info->text().replace("[CZULOSC]", "minimalną czułość"));
        ui->info2->setText(ui->info2->text().replace("[CZULOSC]", "najniższą czułość"));
    }

    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny()) {
        ui->infoZasilanie->setText("Zasilanie czujki zostało włączone");
        ui->rodzajZasilania->setText("Zasilacz");
        ui->napiecieUst->setText(QString::number(daneBadania.getNapiecieZasilaniaCzujki_mV()/1000.0, 'f', 3)+QString(" V"));
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
    } else if (daneBadania.getZasilanieCzujekCentrala()) {
        ui->infoZasilanie->setText("Proszę załączyć zasilanie czujki z centrali sygnalizacji pożarowej");
        ui->rodzajZasilania->setText("Centrala sygnalizacji pożarowej");
        ui->typCentrali->setText(daneBadania. getZasilanieCzujekTypCentrali());
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

OknoZasilaniaCzujki::~OknoZasilaniaCzujki()
{
    delete ui;
}

void OknoZasilaniaCzujki::setCurrent_mA(int mA)
{
    ui->pradMierz->setText(QString::number(mA) + QString(" mA"));
}

void OknoZasilaniaCzujki::setVolage_mV(int mV)
{
    ui->napiecieMierz->setText(QString::number(mV/1000.0, 'f', 3)+QString(" V"));
}

void OknoZasilaniaCzujki::value(int kind, int value)
{
    if (kind == Zasilacz::VOLTAGE_MEAS)
        setVolage_mV(value);
    if (kind == Zasilacz::CURRENT_MEAS)
        setCurrent_mA(value);
}

void OknoZasilaniaCzujki::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}
