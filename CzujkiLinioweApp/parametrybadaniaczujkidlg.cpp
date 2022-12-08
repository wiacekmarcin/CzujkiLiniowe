#include "parametrybadaniaczujkidlg.h"
#include "ui_parametrybadaniaczujkidlg.h"

#include <QBrush>

#define DEFVAL

const char* ParametryBadaniaCzujkiDlg::etTypPierwszy[2] = { ParametryBadaniaCzujkiDlg::etTypOdbiornika, ParametryBadaniaCzujkiDlg::etTypNadajnikaOdbiornika };
const char* ParametryBadaniaCzujkiDlg::etTypDrugi[2] = { ParametryBadaniaCzujkiDlg::etTypNadajnika, ParametryBadaniaCzujkiDlg::etTypReflektora };
const char* ParametryBadaniaCzujkiDlg::etPierwszy[2] = { ParametryBadaniaCzujkiDlg::etOdbiornika, ParametryBadaniaCzujkiDlg::etNadajnikaOdbiornika };
const char* ParametryBadaniaCzujkiDlg::etDrugi[2] = { ParametryBadaniaCzujkiDlg::etNadajnika, ParametryBadaniaCzujkiDlg::etReflektora };
const char* ParametryBadaniaCzujkiDlg::etNumerPierwszy[2] = { ParametryBadaniaCzujkiDlg::etNumerOdbiornika, ParametryBadaniaCzujkiDlg::etNumerNadajnikaOdiornika };
const char* ParametryBadaniaCzujkiDlg::etNumerDrugi[2] = { ParametryBadaniaCzujkiDlg::etNumerNadajnika, ParametryBadaniaCzujkiDlg::etNumerReflektora };

ParametryBadaniaCzujkiDlg::ParametryBadaniaCzujkiDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametryBadaniaCzujkiDlg)
    ,testOdtwarzalnosci(false)
{
    ui->setupUi(this);


    switchOdbiornikReflektor(ui->comboBox->currentIndex() == 0);

    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, [this](int id) { this->changeTypUkladu(id); this->check(); });
    connect(ui->producent, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->typPierwszy, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->typDrugi, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->rozstawienieMinimalne, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->rozstawienieMaksymalne, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->pierwszy_ospozioma, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->pierwszy_ospionowa, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->drugi_ospozioma, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->drugi_ospionowa, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
}

ParametryBadaniaCzujkiDlg::~ParametryBadaniaCzujkiDlg()
{
    delete ui;
}

void ParametryBadaniaCzujkiDlg::init(const Ustawienia &u, ParametryBadania *badanie, QLabel *err)
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


    for (short nrCz = 0; nrCz < maxNumCzujek; ++nrCz)
    {
        QLineEdit * n = new QLineEdit(ui->frameCzujki);
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

        auto row = badanie->getNumeryCzujki(nrCz);

        QLineEdit * p = new QLineEdit(ui->frameCzujki);
        p->setObjectName(QString("pierwszyNumer%1").arg(nrCz+1));
        p->setReadOnly(nrCz > 0);
        p->setEnabled(nrCz == 0);
        p->setText(row.first);
        ui->gridLayoutNumerCzujek->addWidget(p, nrCz+1, 1, 1, 1);
        connect(p, &QLineEdit::textChanged, [this, nrCz](const QString &) {
            this->czujkaNrEdited(nrCz);
        });



        QLineEdit * d = new QLineEdit(ui->frameCzujki);
        d->setObjectName(QString("drugiNumer%1").arg(nrCz+1));
        d->setReadOnly(nrCz > 0);
        d->setEnabled(nrCz == 0);
        d->setText(row.second);
        ui->gridLayoutNumerCzujek->addWidget(d, nrCz+1, 2, 1, 1);
        connect(d, &QLineEdit::textChanged, [this, nrCz](const QString &) {
            this->czujkaNrEdited(nrCz);
        });
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

    bool o = badanie->getOdtwarzalnosc();
    showInfoSorted(o);
    ui->typPierwszy->setReadOnly(o);
    ui->producent->setReadOnly(o);
    ui->typDrugi->setReadOnly(o);
    ui->rozstawienieMinimalne->setReadOnly(o);
    ui->rozstawienieMaksymalne->setReadOnly(o);
    ui->pierwszy_ospozioma->setReadOnly(o);
    ui->pierwszy_ospionowa->setReadOnly(o);
    ui->drugi_ospozioma->setReadOnly(o);
    ui->drugi_ospionowa->setReadOnly(o);
    for( auto le : m_numbers ) {
        le.first->setReadOnly(o);
        le.second->setReadOnly(o);
    }

#ifdef DEFVAL
    ui->typPierwszy->setText("Rodzaj nadajnika");
    ui->producent->setText("Producent");
    ui->typDrugi->setText("Rodzaj odbiornika");
    ui->rozstawienieMinimalne->setText("1.0");
    ui->rozstawienieMaksymalne->setText("10.0");
    ui->pierwszy_ospozioma->setText("0.51");
    ui->pierwszy_ospionowa->setText("0.52");
    ui->drugi_ospozioma->setText("0.53");
    ui->drugi_ospionowa->setText("0.54");

    auto cw1 = m_numbers.at(0);
    cw1.first->setText("Nadajnik1");
    cw1.second->setText("Odbiornik1");

    auto cw2 = m_numbers.at(1);
    cw2.first->setText("Nadajnik2");
    cw2.second->setText("Odbiornik2");
#endif


}

bool ParametryBadaniaCzujkiDlg::check()
{
    if (errorLabel == nullptr)
        return true;
    errorLabel->setText("");
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
    bool noempty = false;
    for (short id = m_numbers.size(); id > 0; --id) {
        auto v = m_numbers.at(id-1);
        bool empty = v.first->text().isEmpty() && v.second->text().isEmpty();
        if (!empty)
            noempty = true;
        else {
            if (noempty) {
                errorLabel->setText(QString("Nie prawidłowe dane dla czujek w wierszu %1 w tabeli numery fabryczne").
                                    arg(id));
                return false;
            }
        }
    }

    return true;
}

void ParametryBadaniaCzujkiDlg::save(ParametryBadania *badanie)
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
    short num = 0;
    for (short i=0; i<m_numbers.size(); ++i)
    {

        if (m_numbers[i].first->text().isEmpty() && m_numbers[i].second->text().isEmpty())
            continue;
        ++num;
        badanie->addNumeryCzujki(m_numbers[i].first->text(), m_numbers[i].second->text());
    }
    badanie->setIloscCzujek(num);
}

void ParametryBadaniaCzujkiDlg::switchOdbiornikReflektor(bool odbiornik)
{
    short ind = odbiornik ? 0 : 1;
    ui->ltyppierwszy->setText(QString(etTypPierwszy[ind]));
    ui->ltypdrugi->setText(QString(etTypDrugi[ind]));
    ui->lpierwszy->setText(QString(etPierwszy[ind]));
    ui->ldrugi->setText(QString(etDrugi[ind]));
    ui->hpierwszy->setText(QString(etNumerPierwszy[ind]));
    ui->hdrugi->setText(QString(etNumerDrugi[ind]));
}

void ParametryBadaniaCzujkiDlg::czujkaNrEdited(short id)
{
    bool noempty = false;
    short numCzujek = 0;
    bool actRowEmpty = true;
    for (short i = m_numbers.size(); i > 0; --i) {
        auto v = m_numbers.at(i-1);
        bool empty = v.first->text().isEmpty() && v.second->text().isEmpty();
        if (i-1 == id)
            actRowEmpty = empty;
        if (!empty) {
            noempty = true;
            ++numCzujek;
        } else {
            if (noempty) {
                showError("Nie może być pustych rekordów. Numery czujek należy wprowadzać po kolei.");
                ui->iloscczujek->setText("?");
                return;
            }
        }
    }
    ui->iloscczujek->setText(QString::number(numCzujek));
    showError("");

    if (!actRowEmpty) {
        showInfo7Number(numCzujek != m_numbers.size());
        if (id == m_numbers.size()-1)
            return;
        auto wid = m_numbers.at(id+1);
        wid.first->setEnabled(true);
        wid.second->setEnabled(true);
        wid.first->setReadOnly(false);
        wid.second->setReadOnly(false);
    } else {
        if (id == m_numbers.size()-2)
            return;
        auto nex = m_numbers.at(id+1);
        nex.first->setEnabled(false);
        nex.second->setEnabled(false);
        nex.first->setReadOnly(true);
        nex.second->setReadOnly(true);
    }
}

void ParametryBadaniaCzujkiDlg::showInfo7Number(bool show)
{
    ui->lContext2->setVisible(show);
    ui->lUwaga2->setVisible(show);
}

void ParametryBadaniaCzujkiDlg::showInfoSorted(bool sorted)
{
    testOdtwarzalnosci = sorted;
    if (sorted)
        ui->lcontext->setText("Czujki <u>zostały</u> posortowane według czułości zgodnie z normą");
    else
        ui->lcontext->setText("Czujki <u>zostaną</u> posortowane według czułości po zakończeniu badań odtwarzalności");

}

void ParametryBadaniaCzujkiDlg::showError(const QString &err)
{
    ui->lError->setVisible(!err.isEmpty());
    ui->error->setVisible(!err.isEmpty());
    ui->error->setText(err);
    showInfo7Number(!err.isEmpty());
}

void ParametryBadaniaCzujkiDlg::changeTypUkladu(int index)
{
    if (index == 0) {
        switchOdbiornikReflektor(true);
    }
    if (index == 1) {
        switchOdbiornikReflektor(false);
    }
}


