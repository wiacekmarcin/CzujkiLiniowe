#include "parametrybadania2.h"
#include "ui_parametrybadania2.h"

#include <QBrush>

const char* ParametryBadania2::etTypPierwszy[2] = { ParametryBadania2::etTypOdbiornika, ParametryBadania2::etTypNadajnikaOdbiornika };
const char* ParametryBadania2::etTypDrugi[2] = { ParametryBadania2::etTypNadajnika, ParametryBadania2::etTypReflektora };
const char* ParametryBadania2::etPierwszy[2] = { ParametryBadania2::etOdbiornika, ParametryBadania2::etNadajnikaOdbiornika };
const char* ParametryBadania2::etDrugi[2] = { ParametryBadania2::etNadajnika, ParametryBadania2::etReflektora };
const char* ParametryBadania2::etNumerPierwszy[2] = { ParametryBadania2::etNumerOdbiornika, ParametryBadania2::etNumerNadajnikaOdiornika };
const char* ParametryBadania2::etNumerDrugi[2] = { ParametryBadania2::etNumerNadajnika, ParametryBadania2::etNumerReflektora };

ParametryBadania2::ParametryBadania2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametryBadania2)
{
    ui->setupUi(this);


    switchOdbiornikReflektor(ui->comboBox->currentIndex() == 0);
}

ParametryBadania2::~ParametryBadania2()
{
    delete ui;
}

void ParametryBadania2::init(const Ustawienia &u, DaneBadania *badanie, QLabel *err)
{
    (void)u;
    errorLabel = err;
    QPalette palette;
    QBrush brush(QColor(239, 239, 239, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Base, brush);
    QBrush brush1(QColor(255, 255, 255, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush);


    for (short nrCz = 0; nrCz < 7; ++nrCz)
    {
        QLineEdit * n = new QLineEdit(ui->frame);
        n->setObjectName(QString("PorzadkowyNumer%1").arg(nrCz+1));
        n->setPalette(palette);
        n->setFrame(true);
        n->setEchoMode(QLineEdit::Normal);
        n->setAlignment(Qt::AlignCenter);
        n->setReadOnly(true);
        n->setText(QString::number(nrCz+1));
        n->setMinimumSize(QSize(20, 0));
        n->setMaximumSize(QSize(30, 50));
        ui->gridLayoutNumerCzujek->addWidget(n, nrCz+1, 0, 1, 1);

        QLineEdit * p = new QLineEdit(ui->frame);
        p->setObjectName(QString("pierwszyNumer%1").arg(nrCz+1));
        p->setReadOnly(nrCz > 0);
        p->setEnabled(nrCz == 0);
        p->setText(badanie->getNumberFirstCzujkiNominal(nrCz));
        ui->gridLayoutNumerCzujek->addWidget(p, nrCz+1, 1, 1, 1);


        QLineEdit * d = new QLineEdit(ui->frame);
        d->setObjectName(QString("drugiNumer%1").arg(nrCz+1));
        d->setReadOnly(nrCz > 0);
        d->setEnabled(nrCz == 0);
        d->setText(badanie->getNumberSecondCzujkiNominal(nrCz));
        ui->gridLayoutNumerCzujek->addWidget(d, nrCz+1, 2, 1, 1);

        m_numbers.push_back(qMakePair(p,d));
    }

    ui->comboBox->setCurrentIndex(badanie->getSystemOdbiornikNadajnik() ? 0 : 1);
    switchOdbiornikReflektor(badanie->getSystemOdbiornikNadajnik());
    ui->producent->setText(badanie->getProducentCzujki());
    ui->typPierwszy->setText(badanie->getTypPierwszejCzujki());
    ui->typDrugi->setText(badanie->getTypDrugiejCzujki());
    ui->rozstawienieMaksymalne->setText(badanie->getRozstawienieMaxCzujki());
    ui->rozstawienieMinimalne->setText(badanie->getRozstawienieMinCzujki());
    ui->pierwszy_ospionowa->setText(QString::number(badanie->getMaksKatowaNieWspolPionPierwszejCzuj()));
    ui->pierwszy_ospozioma->setText(QString::number(badanie->getMaksKatowaNieWspolPozPierwszejCzuj()));
    ui->drugi_ospionowa->setText(QString::number(badanie->getMaksKatowaNieWspolPionDrugiejCzuj()));
    ui->drugi_ospozioma->setText(QString::number(badanie->getMaksKatowaNieWspolPozDrugiejCzuj()));

}

bool ParametryBadania2::check()
{
    if (ui->producent->text().isEmpty()) {
        errorLabel->setText("Pole 'Producent' nie może być puste");
        return false;
    }

    if (ui->typPierwszy->text().isEmpty()) {
        errorLabel->setText(QString("Pole '%1' nie może być puste").arg(etTypPierwszy[ui->comboBox->currentIndex()]));
        return false;
    }

    if (ui->typDrugi->text().isEmpty()) {
        errorLabel->setText(QString("Pole '%1' nie może być puste").arg(etTypDrugi[ui->comboBox->currentIndex()]));
        return false;
    }

    if (ui->rozstawienieMinimalne->text().isEmpty()) {
        errorLabel->setText(QString("Pole 'Rozstawienie minimalne' nie może być puste"));
        return false;
    } else {
        bool ok;
        ui->rozstawienieMinimalne->text().toDouble(&ok);
        if (!ok) {
            errorLabel->setText(QString("Pole 'Rozstawienie minimalne' nie zawiera poprawnej wartości"));
            return false;
        }
    }

    if (ui->rozstawienieMaksymalne->text().isEmpty()) {
        errorLabel->setText(QString("Pole 'Rozstawienie maksymalne' nie może być puste"));
        return false;
    } else {
        bool ok;
        ui->rozstawienieMaksymalne->text().toDouble(&ok);
        if (!ok) {
            errorLabel->setText(QString("Pole 'Rozstawienie maksymalne' nie zawiera poprawnej wartości"));
            return false;
        }
    }

    if (ui->pierwszy_ospionowa->text().isEmpty()) {
        errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi pionowej dla '%1' nie może być puste").
                            arg(etPierwszy[ui->comboBox->currentIndex()]));
        return false;
    } else {
        bool ok;
        ui->pierwszy_ospionowa->text().toDouble(&ok);
        if (!ok) {
            errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi pionowej dla '%1' musi być liczbą").
                                arg(etPierwszy[ui->comboBox->currentIndex()]));
            return false;
        }
    }

    if (ui->pierwszy_ospozioma->text().isEmpty()) {
        errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi poziomej dla '%1' nie może być puste").
                            arg(etPierwszy[ui->comboBox->currentIndex()]));
        return false;
    } else {
        bool ok;
        ui->pierwszy_ospozioma->text().toDouble(&ok);
        if (!ok) {
            errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi poziomej dla '%1' musi być liczbą").
                                arg(etPierwszy[ui->comboBox->currentIndex()]));
            return false;
        }
    }

    if (ui->drugi_ospionowa->text().isEmpty()) {
        errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi pionowej dla '%1' nie może być puste").
                            arg(etDrugi[ui->comboBox->currentIndex()]));
        return false;
    } else {
        bool ok;
        ui->drugi_ospionowa->text().toDouble(&ok);
        if (!ok) {
            errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi pionowej dla '%1' musi być liczbą").
                                arg(etDrugi[ui->comboBox->currentIndex()]));
            return false;
        }
    }

    if (ui->drugi_ospozioma->text().isEmpty()) {
        errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi poziomej dla '%1' nie może być puste").
                            arg(etDrugi[ui->comboBox->currentIndex()]));
        return false;
    } else {
        bool ok;
        ui->drugi_ospozioma->text().toDouble(&ok);
        if (!ok) {
            errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi poziomej dla '%1' musi być liczbą").
                                arg(etDrugi[ui->comboBox->currentIndex()]));
            return false;
        }
    }

    return true;
}

void ParametryBadania2::save(DaneBadania *badanie)
{
    badanie->setSystemOdbiornikNadajnik(ui->comboBox->currentIndex() == 0);
    badanie->setProducentCzujki(ui->producent->text());
    badanie->setTypPierwszejCzujki(ui->typPierwszy->text());
    badanie->setTypDrugiejCzujki(ui->typDrugi->text());
    badanie->setRozstawienieMaxCzujki(ui->rozstawienieMaksymalne->text());
    badanie->setRozstawienieMinCzujki(ui->rozstawienieMinimalne->text());
    badanie->setMaksKatowaNieWspolPionPierwszejCzuj(ui->pierwszy_ospionowa->text().toDouble());
    badanie->setMaksKatowaNieWspolPozPierwszejCzuj(ui->pierwszy_ospozioma->text().toDouble());
    badanie->setMaksKatowaNieWspolPionDrugiejCzuj(ui->drugi_ospionowa->text().toDouble());
    badanie->setMaksKatowaNieWspolPozDrugiejCzuj(ui->drugi_ospozioma->text().toDouble());

    badanie->clearNumberCzujki();
    for (short i=0; i<m_numbers.size(); ++i)
    {
        if (m_numbers[i].first->text().isEmpty() && m_numbers[i].second->text().isEmpty())
            continue;
        badanie->addNumberCzujki(m_numbers[i].first->text(), m_numbers[i].second->text());
    }
}

void ParametryBadania2::switchOdbiornikReflektor(bool odbiornik)
{
    short ind = odbiornik ? 0 : 1;
    ui->ltyppierwszy->setText(QString(etTypPierwszy[ind]));
    ui->ltypdrugi->setText(QString(etTypDrugi[ind]));
    ui->lpierwszy->setText(QString(etPierwszy[ind]));
    ui->ldrugi->setText(QString(etDrugi[ind]));
    ui->hpierwszy->setText(QString(etNumerPierwszy[ind]));
    ui->hdrugi->setText(QString(etNumerDrugi[ind]));
}

void ParametryBadania2::on_comboBox_currentIndexChanged(int index)
{
    if (index == 0) {
        switchOdbiornikReflektor(true);
    }
    if (index == 1) {
        switchOdbiornikReflektor(false);
    }
}

