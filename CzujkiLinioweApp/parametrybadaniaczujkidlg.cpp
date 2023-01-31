#include "parametrybadaniaczujkidlg.h"
#include "ui_parametrybadaniaczujkidlg.h"

#include <QBrush>

//#define DEFVAL

const char* ParametryBadaniaCzujkiDlg::etTransmitter_a[2] = {
    ParametryBadaniaCzujkiDlg::etNadajnika, ParametryBadaniaCzujkiDlg::etNadajnikaOdbiornika };

const char* ParametryBadaniaCzujkiDlg::etTypTransmitter[2] = {
    ParametryBadaniaCzujkiDlg::etTypNadajnika, ParametryBadaniaCzujkiDlg::etTypNadajnikaOdbiornika };

const char* ParametryBadaniaCzujkiDlg::etNumerTransmitter[2] = {
    ParametryBadaniaCzujkiDlg::etNumerNadajnika, ParametryBadaniaCzujkiDlg::etNumerNadajnikaOdiornika };

const char* ParametryBadaniaCzujkiDlg::etTransmitter[2] = {
    ParametryBadaniaCzujkiDlg::etNadajnik, ParametryBadaniaCzujkiDlg::etNadajnikOdbiornik };

const char* ParametryBadaniaCzujkiDlg::etReceiver_a[2] = {
    ParametryBadaniaCzujkiDlg::etOdbiornika, ParametryBadaniaCzujkiDlg::etReflektora };

const char* ParametryBadaniaCzujkiDlg::etTypReceiver[2] = {
    ParametryBadaniaCzujkiDlg::etTypOdbiornika, ParametryBadaniaCzujkiDlg::etTypReflektora };

const char* ParametryBadaniaCzujkiDlg::etNumerReceiver[2] = {
    ParametryBadaniaCzujkiDlg::etNumerOdbiornika, ParametryBadaniaCzujkiDlg::etNumerReflektora };

const char* ParametryBadaniaCzujkiDlg::etReceiver[2] = {
    ParametryBadaniaCzujkiDlg::etOdbiornik,  ParametryBadaniaCzujkiDlg::etReflektor };

ParametryBadaniaCzujkiDlg::ParametryBadaniaCzujkiDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametryBadaniaCzujkiDlg)
    ,testOdtwarzalnosci(false)
{
    ui->setupUi(this);


    switchOdbiornikReflektor(ui->comboBox->currentIndex() == 0);

    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, [this](int id) { this->changeTypUkladu(id); this->check(); });
    connect(ui->producent, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->typTransmitter, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->typReceiver, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->rozstawienieMinimalne, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->rozstawienieMaksymalne, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->transmitter_ospozioma, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->transmitter_ospionowa, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->receiver_ospozioma, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    connect(ui->receiver_ospionowa, &QLineEdit::textChanged, this, [this](const QString &) { this->check(); });
    adjustSize();
}

ParametryBadaniaCzujkiDlg::~ParametryBadaniaCzujkiDlg()
{
    delete ui;
}

void ParametryBadaniaCzujkiDlg::init(bool edit, const Ustawienia &u, ParametryBadania *badanie, QLabel *err)
{
    (void)u;
    errorLabel = err;


    testOdtwarzalnosci = badanie->getTestOdtwarzalnosci();

    if (!testOdtwarzalnosci) {
        createCzujkaTable(badanie);
    } else {
        createCzujkaTableReadOlny(badanie);
    }

    ui->comboBox->setCurrentIndex(badanie->getSystemOdbiornikNadajnik() ? 0 : 1);
    switchOdbiornikReflektor(badanie->getSystemOdbiornikNadajnik());
    ui->producent->setText(badanie->getProducentCzujki());
    ui->typTransmitter->setText(badanie->getTypTransmitter());
    ui->typReceiver->setText(badanie->getTypReceiver());
    ui->rozstawienieMaksymalne->setText(badanie->getRozstawienieMaxCzujki());
    ui->rozstawienieMinimalne->setText(badanie->getRozstawienieMinCzujki());
    ui->transmitter_ospionowa->setText(badanie->getMaksKatowaNieWspolPionowaNadajnika());
    ui->transmitter_ospozioma->setText(badanie->getMaksKatowaNieWspolPoziomaNadajnika());
    ui->receiver_ospionowa->setText(badanie->getMaksKatowaNieWspolPionowaOdbiornika());
    ui->receiver_ospozioma->setText(badanie->getMaksKatowaNieWspolPoziomaOdbiornika());


    showInfoSorted(testOdtwarzalnosci);
    ui->typTransmitter->setReadOnly(testOdtwarzalnosci);
    ui->producent->setReadOnly(testOdtwarzalnosci);
    ui->typReceiver->setReadOnly(testOdtwarzalnosci);
    ui->rozstawienieMinimalne->setReadOnly(testOdtwarzalnosci);
    ui->rozstawienieMaksymalne->setReadOnly(testOdtwarzalnosci);
    ui->transmitter_ospozioma->setReadOnly(testOdtwarzalnosci);
    ui->transmitter_ospionowa->setReadOnly(testOdtwarzalnosci);
    ui->receiver_ospozioma->setReadOnly(testOdtwarzalnosci);
    ui->receiver_ospionowa->setReadOnly(testOdtwarzalnosci);
    ui->comboBox->setEditable(false);

    ui->comboBox->setEditable(!testOdtwarzalnosci);
    ui->lineEdit->setVisible(testOdtwarzalnosci);
    ui->lineEdit->setReadOnly(true);
    ui->comboBox->setVisible(!testOdtwarzalnosci);
    ui->lineEdit->setText(ui->comboBox->currentText());
    ui->headSortowanie->setVisible(testOdtwarzalnosci);

    for( auto le : m_numbers ) {
        le.first->setReadOnly(testOdtwarzalnosci);
        le.second->setReadOnly(testOdtwarzalnosci);
    }

#ifdef DEFVAL
    if (!edit && !testOdtwarzalnosci) {
        ui->typTransmitter->setText("Rodzaj nadajnika");
        ui->producent->setText("Producent");
        ui->typReceiver->setText("Rodzaj odbiornika");
        ui->rozstawienieMinimalne->setText("1.0");
        ui->rozstawienieMaksymalne->setText("10.0");
        ui->transmitter_ospozioma->setText("0.51");
        ui->transmitter_ospionowa->setText("0.52");
        ui->receiver_ospozioma->setText("0.53");
        ui->receiver_ospionowa->setText("0.54");

        auto cw1 = m_numbers.at(0);
        cw1.first->setText("Nadajnik1");
        cw1.second->setText("Odbiornik1");

        cw1 = m_numbers.at(1);
        cw1.first->setText("Nadajnik2");
        cw1.second->setText("Odbiornik2");

        cw1 = m_numbers.at(2);
        cw1.first->setText("Nadajnik3");
        cw1.second->setText("Odbiornik3");

        cw1 = m_numbers.at(3);
        cw1.first->setText("Nadajnik4");
        cw1.second->setText("Odbiornik4");

        cw1 = m_numbers.at(4);
        cw1.first->setText("Nadajnik5");
        cw1.second->setText("Odbiornik5");

        cw1 = m_numbers.at(5);
        cw1.first->setText("Nadajnik6");
        cw1.second->setText("Odbiornik6");

        cw1 = m_numbers.at(6);
        cw1.first->setText("Nadajnik7");
        cw1.second->setText("Odbiornik7");
    }
#endif


}

void ParametryBadaniaCzujkiDlg::configCellLE(QLineEdit * l, const QString & objectName)
{
    l->setObjectName(objectName);
    l->setFrame(true);
    l->setEchoMode(QLineEdit::Normal);
    l->setAlignment(Qt::AlignCenter);
    l->setReadOnly(true);
    l->setMinimumSize(QSize(20, 15));
    l->setMaximumSize(QSize(30, 50));
}

void ParametryBadaniaCzujkiDlg::configCellLAB(QLabel * l, const QString & objectName, bool s, const QSize & s1, const QSize & s2)
{
    l->setObjectName(objectName);
    l->setFrameStyle(QFrame::Box);
    l->setMargin(3);
    if (s) {
        l->setMinimumSize(s1);
        l->setMaximumSize(s2);
    }
}

void ParametryBadaniaCzujkiDlg::createCzujkaTable(ParametryBadania *badanie)
{
    QPalette palette;
    QBrush brush(QColor(239, 239, 239, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Base, brush);
    QBrush brush1(QColor(255, 255, 255, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush);

    short maxNoEmptyRows = 0;
    for (short nrCz = 0; nrCz < maxNumCzujek; ++nrCz)
    {
        QLineEdit * n = new QLineEdit(ui->frameCzujki);
        configCellLE(n, QString("PorzadkowyNumer%1").arg(nrCz+1));
        n->setPalette(palette);
        n->setText(QString::number(nrCz+1));
        ui->gridLayoutNumerCzujek->addWidget(n, nrCz+1, 0, 1, 1);

        auto row = badanie->getNumeryCzujki(nrCz);

        QLineEdit * p = new QLineEdit(ui->frameCzujki);
        p->setObjectName(QString("transmitterNumer%1").arg(nrCz+1));
        p->setReadOnly(nrCz > 0);
        p->setEnabled(nrCz == 0);
        p->setText(row.first);
        ui->gridLayoutNumerCzujek->addWidget(p, nrCz+1, 1, 1, 1);
        connect(p, &QLineEdit::textChanged, [this, nrCz](const QString &) {
            this->czujkaNrEdited(nrCz);
        });

        QLineEdit * d = new QLineEdit(ui->frameCzujki);
        d->setObjectName(QString("receiverNumer%1").arg(nrCz+1));
        d->setReadOnly(nrCz > 0);
        d->setEnabled(nrCz == 0);
        d->setText(row.second);
        ui->gridLayoutNumerCzujek->addWidget(d, nrCz+1, 2, 1, 1);
        connect(d, &QLineEdit::textChanged, [this, nrCz](const QString &) {
            this->czujkaNrEdited(nrCz);
        });
        if (!row.first.isEmpty() || !row.second.isEmpty()) {
            maxNoEmptyRows = nrCz;
        }
        m_numbers.push_back(qMakePair(p,d));
    }
    for (short n = 0; n <= maxNoEmptyRows; ++n )
        czujkaNrEdited(n);
}

void ParametryBadaniaCzujkiDlg::createCzujkaTableReadOlny(ParametryBadania *badanie)
{
    ui->iloscczujek->setText(QString::number(badanie->getIloscWszystkichCzujek()));
    showInfo7Number(badanie->getIloscWszystkichCzujek() != 7);
    auto wszystkieCzujki = badanie->getDaneBadanCzujek();
    short last = badanie->getIloscWszystkichCzujek() - 1;
    QVector<QStringList> records(badanie->getIloscWszystkichCzujek());
    for (const auto & dane : wszystkieCzujki) {
        if (dane.nrSortCzujki == 0) {
            QStringList row;
            row << QString("-") << dane.numerNadajnika << dane.numerOdbiornika << QString::number(dane.nrCzujki) << "-";
            records[last--] = row;
        } else {
            QStringList row;
            row << QString::number(dane.nrSortCzujki) << dane.numerNadajnika << dane.numerOdbiornika << QString::number(dane.nrCzujki)
                << dane.value_dB + " dB";
            records[dane.nrSortCzujki-1] = row;
        }
    }
    short nrCz = 0;
    for (const auto & row : records)
    {
        qDebug() << row.size();
        if (row.size() == 0)
            continue;
        QLabel * n = new QLabel(ui->frameCzujki);
        configCellLAB(n, QString("PorzadkowyNumerLabel%1").arg(nrCz+1), true, QSize(20, 20), QSize(30,50));
        n->setText(row.at(0));
        ui->gridLayoutNumerCzujek->addWidget(n, nrCz+1, 0, 1, 1);

        QLabel * p = new QLabel(ui->frameCzujki);
        configCellLAB(p, QString("transmitterNumerLabel%1").arg(nrCz+1));
        p->setText(row.at(1));
        ui->gridLayoutNumerCzujek->addWidget(p, nrCz+1, 1, 1, 1);

        QLabel * d = new QLabel(ui->frameCzujki);
        configCellLAB(d, QString("receiverNumerLabel%1").arg(nrCz+1));
        d->setText(row.at(2));
        ui->gridLayoutNumerCzujek->addWidget(d, nrCz+1, 2, 1, 1);

        QLabel * k = new QLabel(ui->frameCzujki);
        configCellLAB(k, QString("numePorzadkowyLabel%1").arg(nrCz+1), true, QSize(20, 20), QSize(30,50));
        k->setText(row.at(3));
        ui->gridLayoutNumerCzujek->addWidget(k, nrCz+1, 3, 1, 1);

        QLabel * t = new QLabel(ui->frameCzujki);
        configCellLAB(t, QString("tlumienieLabel%1").arg(nrCz+1), true, QSize(40, 20), QSize(80,50));
        t->setText(row.at(4));
        ui->gridLayoutNumerCzujek->addWidget(t, nrCz+1, 4, 1, 1);

        ++nrCz;
    }
    QSpacerItem * spacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->gridLayoutNumerCzujek->addItem(spacer, badanie->getIloscWszystkichCzujek()+1, 0, 1, 1);
    ui->error->setVisible(false);
    ui->lError->setVisible(false);
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

    if (ui->typTransmitter->text().isEmpty()) {
        errorLabel->setText(QString("Pole '%1' nie może być puste").arg(etTypTransmitter[ui->comboBox->currentIndex()]));
        return false;
    }

    if (ui->typReceiver->text().isEmpty()) {
        errorLabel->setText(QString("Pole '%1' nie może być puste").arg(etTypReceiver[ui->comboBox->currentIndex()]));
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

    if (ui->transmitter_ospionowa->text().isEmpty()) {
        errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi pionowej dla '%1' nie może być puste").
                            arg(etTransmitter_a[ui->comboBox->currentIndex()]));
        return false;
    } else {
        bool ok;
        ui->transmitter_ospionowa->text().toDouble(&ok);
        if (!ok) {
            errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi pionowej dla '%1' musi być liczbą").
                                arg(etTransmitter_a[ui->comboBox->currentIndex()]));
            return false;
        }
    }

    if (ui->transmitter_ospozioma->text().isEmpty()) {
        errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi poziomej dla '%1' nie może być puste").
                            arg(etTransmitter_a[ui->comboBox->currentIndex()]));
        return false;
    } else {
        bool ok;
        ui->transmitter_ospozioma->text().toDouble(&ok);
        if (!ok) {
            errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi poziomej dla '%1' musi być liczbą").
                                arg(etTransmitter_a[ui->comboBox->currentIndex()]));
            return false;
        }
    }

    if (ui->receiver_ospionowa->text().isEmpty()) {
        errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi pionowej dla '%1' nie może być puste").
                            arg(etReceiver_a[ui->comboBox->currentIndex()]));
        return false;
    } else {
        bool ok;
        ui->receiver_ospionowa->text().toDouble(&ok);
        if (!ok) {
            errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi pionowej dla '%1' musi być liczbą").
                                arg(etReceiver_a[ui->comboBox->currentIndex()]));
            return false;
        }
    }

    if (ui->receiver_ospozioma->text().isEmpty()) {
        errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi poziomej dla '%1' nie może być puste").
                            arg(etReceiver_a[ui->comboBox->currentIndex()]));
        return false;
    } else {
        bool ok;
        ui->receiver_ospozioma->text().toDouble(&ok);
        if (!ok) {
            errorLabel->setText(QString("Pole 'Maksymalna kątowa niewspółosiowość' dla osi poziomej dla '%1' musi być liczbą").
                                arg(etReceiver_a[ui->comboBox->currentIndex()]));
            return false;
        }
    }
    if (!testOdtwarzalnosci) {
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
    }
    return true;
}

void ParametryBadaniaCzujkiDlg::save(ParametryBadania *badanie)
{
    if (testOdtwarzalnosci)
        return;
    badanie->setSystemOdbiornikNadajnik(ui->comboBox->currentIndex() == 0);
    badanie->setProducentCzujki(ui->producent->text());
    badanie->setTypTransmitter(ui->typTransmitter->text());
    badanie->setTypReceiver(ui->typReceiver->text());
    badanie->setRozstawienieMaxCzujki(ui->rozstawienieMaksymalne->text());
    badanie->setRozstawienieMinCzujki(ui->rozstawienieMinimalne->text());
    badanie->setMaksKatowaNieWspolPionowaNadajnika(ui->transmitter_ospionowa->text());
    badanie->setMaksKatowaNieWspolPoziomaNadajnika(ui->transmitter_ospozioma->text());
    badanie->setMaksKatowaNieWspolPionowaOdbiornika(ui->receiver_ospionowa->text());
    badanie->setMaksKatowaNieWspolPoziomaOdbiornika(ui->receiver_ospozioma->text());
    badanie->setNazwaTransmitter(etTransmitter[ui->comboBox->currentIndex()]);
    badanie->setNazwaReceiver(etReceiver[ui->comboBox->currentIndex()]);
    badanie->setNazwaTransmitter_a(etTransmitter_a[ui->comboBox->currentIndex()]);
    badanie->setNazwaReceiver_a(etReceiver_a[ui->comboBox->currentIndex()]);
    badanie->setNazwaNumerTransmitter(etNumerTransmitter[ui->comboBox->currentIndex()]);
    badanie->setNazwaNumerReceiver(etNumerReceiver[ui->comboBox->currentIndex()]);


    badanie->wyczyscCzujki();
    short num = 0;
    for (short i=0; i<m_numbers.size(); ++i)
    {

        if (m_numbers[i].first->text().isEmpty() && m_numbers[i].second->text().isEmpty())
            continue;

        if (badanie->dodajCzujki(m_numbers[i].first->text(), m_numbers[i].second->text()))
            ++num;
    }
    badanie->setIloscWszystkichCzujek(num);
}

void ParametryBadaniaCzujkiDlg::switchOdbiornikReflektor(bool odbiornik)
{
    short ind = odbiornik ? 0 : 1;
    ui->ltypTransmitter->setText(QString(etTypTransmitter[ind]));
    ui->ltypReceiver->setText(QString(etTypReceiver[ind]));
    ui->lTransmitter->setText(QString(etTransmitter_a[ind]));
    ui->lReceiver->setText(QString(etReceiver_a[ind]));
    ui->hnumerTransmitter->setText(QString(etNumerTransmitter[ind]));
    ui->hnumerReceiver->setText(QString(etNumerReceiver[ind]));

}

void ParametryBadaniaCzujkiDlg::czujkaNrEdited(short id)
{
    qDebug() << "id " << id;
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

    showInfo7Number(numCzujek != 7);
    if (!actRowEmpty) {

        if (id == m_numbers.size()-1)
            return;
        auto wid = m_numbers.at(id+1);
        wid.first->setEnabled(true);
        wid.second->setEnabled(true);
        wid.first->setReadOnly(false);
        wid.second->setReadOnly(false);
    } else {
        if (id == 6) {
            //auto nex = m_numbers.at(id);
            //nex.first->setEnabled(false);
            //nex.second->setEnabled(false);
            //nex.first->setReadOnly(true);
            //nex.second->setReadOnly(true);
        } else {
            auto nex = m_numbers.at(id+1);
            nex.first->setEnabled(false);
            nex.second->setEnabled(false);
            nex.first->setReadOnly(true);
            nex.second->setReadOnly(true);
        }
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


