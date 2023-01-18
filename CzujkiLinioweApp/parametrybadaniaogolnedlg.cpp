#include "parametrybadaniaogolnedlg.h"
#include "ui_parametrybadaniaogolnedlg.h"

//#define DEFVAL

ParametryBadaniaOgolneDlg::ParametryBadaniaOgolneDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametryBadaniaOgolneDlg)
{
    ui->setupUi(this);

    connect(ui->password1, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->password2, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->numerZlecenia, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->numerTestu, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->osobaOdpowiedzialna, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->czasPomiedzyZmianamifiltra, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->napiecieZasilaniain, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->typCentraliSygnalizacji, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->czasStabilizacjiCzujki, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->pradAlarmu, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });


}

ParametryBadaniaOgolneDlg::~ParametryBadaniaOgolneDlg()
{
    delete ui;
}

void ParametryBadaniaOgolneDlg::init(bool edit, const Ustawienia &u, ParametryBadania *badanie, QLabel * err)
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

    if (badanie->getZasilanieCzujekZasilaczZewnetrzny()) {
        ui->napiecieZasilaniain->setText(QString::number(badanie->getNapiecieZasilaniaCzujki_mV()/1000.0, 'g', 3));
        setWewnetrznyZasilacz(true);
        ui->rbInsideSupply->setChecked(true);
        ui->rbCentralSupply->setChecked(false);
    } else {
        ui->typCentraliSygnalizacji->setText(badanie->getZasilanieCzujekTypCentrali());
        setWewnetrznyZasilacz(false);
        ui->rbInsideSupply->setChecked(false);
        ui->rbCentralSupply->setChecked(true);
    }

    ui->czasStabilizacjiCzujki->setText(QString::number(badanie->getCzasStabilizacjiCzujki_s()));
    bool przekaznik = badanie->getWyzwalanieAlarmuPrzekaznikiem();
    setWyzwolenieAlarmu(przekaznik);
    ui->rbAlarmPrzekaznik->setChecked(przekaznik);
    ui->rbPrad->setChecked(!przekaznik);
    ui->pradAlarmu->setText(badanie->getPrzekroczeniePraduZasilania_mA());

    ui->czasPomiedzyZmianamifiltra->setText(QString::number(badanie->getCzasPomZmianaTlumenia_s()));
    ui->dlugoscFali->setCurrentText(QString::number(badanie->getDlugoscFaliFiltrow()));
    check();

    bool o = badanie->getTestOdtwarzalnosci();

    ui->numerZlecenia->setReadOnly(o);
    ui->numerTestu->setReadOnly(o);
    ui->osobaOdpowiedzialna->setReadOnly(o);
    ui->czasPomiedzyZmianamifiltra->setReadOnly(o);
    ui->napiecieZasilaniain->setReadOnly(o);
    ui->typCentraliSygnalizacji->setReadOnly(o);
    ui->czasStabilizacjiCzujki->setReadOnly(o);
    ui->pradAlarmu->setReadOnly(o);
    ui->rbInsideSupply->setCheckable(!o);
    ui->rbPrad->setCheckable(!o);
    ui->dlugoscFali->setEditable(!o);
    ui->dlugoscFaliLineEdit->setVisible(o);
    ui->dlugoscFaliLineEdit->setReadOnly(true);
    ui->dlugoscFali->setVisible(!o);
    ui->dlugoscFaliLineEdit->setText(ui->dlugoscFali->currentText());
    ui->uwagi->setReadOnly(o);


#ifdef DEFVAL
    if (!edit && !o) {
        ui->numerZlecenia->setText("Numer zlecenia");
        ui->numerTestu->setText("Numer Testu");
        ui->osobaOdpowiedzialna->setText("Osoba odpowiedzialna");
        ui->czasPomiedzyZmianamifiltra->setText("10");;
        ui->napiecieZasilaniain->setText("12.0");
        setWewnetrznyZasilacz(true);
        ui->typCentraliSygnalizacji->setText("Centrala sygnalizacji");
        ui->czasStabilizacjiCzujki->setText("15");
        ui->pradAlarmu->setText("50");
        ui->rbInsideSupply->setChecked(true);
        ui->rbPrad->setChecked(true);
        setWyzwolenieAlarmu(false);
    }
#endif
}

void ParametryBadaniaOgolneDlg::save(ParametryBadania *badanie)
{
    badanie->setNumerZlecenia(ui->numerZlecenia->text());
    badanie->setNumerTestu(ui->numerTestu->text());
    badanie->setOsobaOdpowiedzialna(ui->osobaOdpowiedzialna->text());
    badanie->setUwagi(ui->uwagi->toPlainText());

    if (ui->rbInsideSupply->isChecked()) {
        badanie->setZasilanieCzujekZasilaczZewnetrzny(true);
        badanie->setZasilanieCzujekCentrala(false);
        badanie->setNapiecieZasilaniaCzujki_mV(1000*ui->napiecieZasilaniain->text().toDouble());
    } else {
        badanie->setZasilanieCzujekCentrala(true);
        badanie->setZasilanieCzujekZasilaczZewnetrzny(false);
        badanie->setZasilanieCzujekTypCentrali(ui->typCentraliSygnalizacji->text());
    }

    ui->czasStabilizacjiCzujki->setText(QString::number(badanie->getCzasStabilizacjiCzujki_s()));
    if (badanie->getWyzwalanieAlarmuPrzekaznikiem()) {
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

bool ParametryBadaniaOgolneDlg::check()
{
    if (errorLabel == nullptr)
        return true;
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

void ParametryBadaniaOgolneDlg::setWewnetrznyZasilacz(bool inside)
{
    ui->napiecieZasilaniain->setEnabled(inside);
    ui->typCentraliSygnalizacji->setEnabled(!inside);
}

void ParametryBadaniaOgolneDlg::setWyzwolenieAlarmu(bool przekaznik)
{
    ui->pradAlarmu->setEnabled(!przekaznik);
}

void ParametryBadaniaOgolneDlg::on_dlugoscFali_currentIndexChanged(int index)
{
    (void)index;
    check();
}

void ParametryBadaniaOgolneDlg::on_rbInsideSupply_toggled(bool checked)
{
    setWewnetrznyZasilacz(checked);
    check();
}


void ParametryBadaniaOgolneDlg::on_rbCentralSupply_toggled(bool checked)
{
    setWewnetrznyZasilacz(!checked);
    check();
}


void ParametryBadaniaOgolneDlg::on_rbAlarmPrzekaznik_toggled(bool checked)
{
    setWyzwolenieAlarmu(checked);
    check();
}


void ParametryBadaniaOgolneDlg::on_rbPrad_toggled(bool checked)
{
    setWyzwolenieAlarmu(!checked);
    check();
}

