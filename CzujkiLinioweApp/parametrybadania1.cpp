#include "parametrybadania1.h"
#include "ui_parametrybadania1.h"

ParametryBadania1::ParametryBadania1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametryBadania1)
{
    ui->setupUi(this);
}

ParametryBadania1::~ParametryBadania1()
{
    delete ui;
}

void ParametryBadania1::init(const Ustawienia &u, DaneBadania *badanie, QLabel * err)
{
    errorLabel = err;
    minVolt = u.getMinNapiecieCzujki();
    maxVolt = u.getMaxNapiecieCzujki();

    ui->lunitvoltinside->setText(QString("[V] (%1-%2)").arg(QString::number(minVolt),
                                                            QString::number(maxVolt)));


    minCzasStab = u.getMinCzasStabCzujki();
    maxCzasStab = u.getMaxCzasStabCzujki();
    ui->lunitczasstabilizacji->setText(QString("[s] (%1-%2)").arg(QString::number(minCzasStab),
                                                                  QString::number(maxCzasStab)));

    minPrad= u.getMinPrzekrPradZasCzujki();
    maxPrad = u.getMaxPrzekrPradZasCzujki();
    ui->lunitCurrent->setText(QString("[mA] (%1-%2)").arg(QString::number(minPrad),
                                                             QString::number(maxPrad)));

    minCzasZmianyFiltra = u.getMinCzasPoZmianieFiltra();
    maxCzasZmianyFiltra = u.getMaxCzasPoZmianieFiltra();
    ui->lunitczapomiedzytlumienia->setText(QString("[s] (%1-%2)").arg(QString::number(minCzasZmianyFiltra),
                                                                       QString::number(maxCzasZmianyFiltra)));
    ui->numerZlecenia->setText(badanie->getNumerZlecenia());
    ui->numerTestu->setText(badanie->getNumerTestu());
    ui->osobaOdpowiedzialna->setText(badanie->getOsobaOdpowiedzialna());
    ui->uwagi->setText(badanie->getUwagi());

    if (badanie->getZasCzujekWbudZasilacz()) {
        ui->napiecieZasilaniain->setText(QString::number(badanie->getNapiecieZasCzujki_mV()/1000.0, 'g', 3));
        setWewnetrznyZasilacz(true);
        ui->rbInsideSupply->setChecked(true);
        ui->rbCentralSupply->setChecked(false);
    } else {
        ui->typCentraliSygnalizacji->setText(badanie->getTypCentraliSygnPoz());
        setWewnetrznyZasilacz(false);
        ui->rbInsideSupply->setChecked(false);
        ui->rbCentralSupply->setChecked(true);
    }

    ui->czasStabilizacjiCzujki->setText(QString::number(badanie->getCzasStabilizacjiCzujki_s()));
    bool przekaznik = badanie->getWyzwalanieAlarmuPrzekaznik();
    setWyzwolenieAlarmu(przekaznik);
    ui->rbAlarmPrzekaznik->setChecked(przekaznik);
    ui->rbPrad->setChecked(!przekaznik);
    ui->pradAlarmu->setText(badanie->getPrzekroczeniePraduZasilania_mA());

    ui->czasPomiedzyZmianamifiltra->setText(QString::number(badanie->getCzasPomZmianaTlumenia_s()));
    ui->dlugoscFali->setCurrentText(QString::number(badanie->getDlugoscFaliFiltrow()));
    check();
}

void ParametryBadania1::save(DaneBadania *badanie)
{
    badanie->setNumerZlecenia(ui->numerZlecenia->text());
    badanie->setNumerTestu(ui->numerTestu->text());
    badanie->setOsobaOdpowiedzialna(ui->osobaOdpowiedzialna->text());
    badanie->setUwagi(ui->uwagi->toPlainText());

    if (ui->rbInsideSupply->isChecked()) {
        badanie->setZasCzujekWbudZasilacz(true);
        badanie->setNapiecieZasCzujki_mV(1000*ui->napiecieZasilaniain->text().toInt());
    } else {
        badanie->setZasCzujekWbudZasilacz(false);
        badanie->setTypCentraliSygnPoz(ui->typCentraliSygnalizacji->text());
    }

    ui->czasStabilizacjiCzujki->setText(QString::number(badanie->getCzasStabilizacjiCzujki_s()));
    if (badanie->getWyzwalanieAlarmuPrzekaznik()) {
        setWyzwolenieAlarmu(true);
    } else {
        setWyzwolenieAlarmu(false);
        QString prad = badanie->getPrzekroczeniePraduZasilania_mA();
        if (!prad.isEmpty())
            ui->pradAlarmu->setText(QString::number(prad.toDouble(), 'g', 3));
    }

    badanie->setDlugoscFaliFiltrow(ui->dlugoscFali->currentText().toUInt());
    badanie->setCzasStabilizacjiCzujki_s(ui->czasPomiedzyZmianamifiltra->text().toUInt());
}

bool ParametryBadania1::check()
{
    errorLabel->setText("");
    if (ui->numerZlecenia->text().isEmpty()) {
        errorLabel->setText("Pole 'Numer zlecenia' nie może być puste");
        return false;
    }
    if (ui->numerTestu->text().isEmpty()) {
        errorLabel->setText("Pole 'Numer testu' nie może być puste");
        return false;
    }
    if (ui->osobaOdpowiedzialna->text().isEmpty()) {
        errorLabel->setText("Pole 'Osoba odpowiedzialna' nie może być puste");
        return false;
    }

    if (!ui->rbInsideSupply->isChecked() && !ui->rbCentralSupply->isChecked()) {
        errorLabel->setText("Należy wybrać jedną z metod zasilania czujek");
        return false;
    }

    if (ui->rbInsideSupply->isChecked()) {
        if (ui->napiecieZasilaniain->text().isEmpty()) {
            errorLabel->setText("Pole dla dla napięcia dla zasilacza wewnętrznego nie może być puste");
            return false;
        }
        bool ok;
        double volt = ui->napiecieZasilaniain->text().toDouble(&ok);
        if (!ok) {
            QString svolt = ui->napiecieZasilaniain->text().replace(".", ",");
            volt = svolt.toDouble(&ok);
        }
        if (!ok) {
            errorLabel->setText("Nie poprawna wartość liczbowa dla napięcia dla zasilacza wewnętrznego");
            return false;
        }

        if (volt < minVolt || volt > maxVolt) {
            errorLabel->setText("Wartość napięcia dla zasilacza wewnętrznego poza dopuszczalnym zakresem");
            return false;
        }
    }

    if (ui->rbCentralSupply->isChecked()) {
        if (ui->typCentraliSygnalizacji->text().isEmpty()) {
            errorLabel->setText("Wartość pola 'typ centrali sygnalizacji pożarowej' nie może być puste");
            return false;
        }
    }

    if (ui->czasStabilizacjiCzujki->text().isEmpty()) {
        errorLabel->setText("Pole 'Czas stabilizacji czujki' nie może być puste");
        return false;
    }
    bool ok;
    unsigned int czas = ui->czasStabilizacjiCzujki->text().toInt(&ok);
    if (!ok || ui->czasStabilizacjiCzujki->text() != QString::number(czas)) {
        errorLabel->setText("Nie poprawna wartość liczbowa dla czasu stabilizacji czujki");
        return false;
    }

    if (czas < minCzasStab || czas > maxCzasStab) {
        errorLabel->setText("Wartość czasu stabilizacji dla czujki poza dopuszczalnym zakresem");
        return false;
    }

    if (!ui->rbAlarmPrzekaznik->isChecked() && !ui->rbPrad->isChecked()) {
        errorLabel->setText("Należy wybrać sposób sygnalizacji alarmu");
        return false;
    }
    
    if (ui->rbPrad->isChecked()) {
        if (ui->pradAlarmu->text().isEmpty()) {
            errorLabel->setText("Pole 'przekroczenie wartości prądu zasilania' nie może być puste");
            return false;
        }
        bool ok;
        double prad = ui->pradAlarmu->text().toDouble(&ok);
        if (!ok) {
            errorLabel->setText("Nie poprawna wartość liczbowa dla przekroczonej wartości prądu zasilania");
            return false;
        }
        if (prad < minPrad || prad > maxPrad) {
            errorLabel->setText("Wartość liczbowa dla przekroczonej wartości prądu zasilania poza zakresem");
            return false;
        }
    }

    if (ui->dlugoscFali->currentText().isEmpty()) {
        errorLabel->setText("Należy wybrać długość fali");
        return false;
    }

    if (ui->czasPomiedzyZmianamifiltra->text().isEmpty()) {
        errorLabel->setText("Pole 'Czas pomiędzy zmianami tłumienia filtra' nie może być puste");
        return false;
    }

    unsigned int czasPostoju = ui->czasPomiedzyZmianamifiltra->text().toInt(&ok);
    if (!ok || ui->czasPomiedzyZmianamifiltra->text() != QString::number(czasPostoju)) {
        errorLabel->setText("Nie poprawna wartość liczbowa dla czasu pomiędzy zmianami tłumienia filtra");
        return false;
    }

    if (czasPostoju < minCzasZmianyFiltra || czasPostoju > maxCzasZmianyFiltra) {
        errorLabel->setText("Wartość czasu pomiędzy zmianami tłumienia filtra poza dopuszczalnym zakresem");
        return false;
    }

    return true;
}

void ParametryBadania1::setWewnetrznyZasilacz(bool inside)
{
    ui->napiecieZasilaniain->setEnabled(inside);
    ui->typCentraliSygnalizacji->setEnabled(!inside);
}

void ParametryBadania1::setWyzwolenieAlarmu(bool przekaznik)
{
    ui->pradAlarmu->setEnabled(!przekaznik);
}

void ParametryBadania1::on_numerZlecenia_editingFinished()
{
    check();
}


void ParametryBadania1::on_numerTestu_editingFinished()
{
    check();
}


void ParametryBadania1::on_osobaOdpowiedzialna_editingFinished()
{
    check();
}


void ParametryBadania1::on_uwagi_textChanged()
{
    check();
}


void ParametryBadania1::on_napiecieZasilaniain_editingFinished()
{
    check();
}


void ParametryBadania1::on_typCentraliSygnalizacji_editingFinished()
{
    check();
}


void ParametryBadania1::on_czasStabilizacjiCzujki_editingFinished()
{
    check();
}


void ParametryBadania1::on_pradAlarmu_editingFinished()
{
    check();
}


void ParametryBadania1::on_dlugoscFali_currentIndexChanged(int index)
{
    (void)index;
    check();
}


void ParametryBadania1::on_czasPomiedzyZmianamifiltra_editingFinished()
{
    check();
}


void ParametryBadania1::on_rbInsideSupply_toggled(bool checked)
{
    setWewnetrznyZasilacz(checked);
    check();
}


void ParametryBadania1::on_rbCentralSupply_toggled(bool checked)
{
    setWewnetrznyZasilacz(!checked);
    check();
}


void ParametryBadania1::on_rbAlarmPrzekaznik_toggled(bool checked)
{
    setWyzwolenieAlarmu(checked);
    check();
}


void ParametryBadania1::on_rbPrad_toggled(bool checked)
{
    setWyzwolenieAlarmu(!checked);
    check();
}

