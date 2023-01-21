#include "oknoparametrytestu.h"
#include "ui_oknoparametrytestu.h"
#include "danetestu.h"
#include <QDate>
#include <QTime>
#include <QMessageBox>


#define SETREADONLY(w) w->setReadOnly(true);

OknoParametryTestu::OknoParametryTestu(short nrPomiar_, DaneTestu * test_, const ParametryBadania & badanie_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoParametryTestu),
    test(test_),
    nrPomiar(nrPomiar_),
    badanie(badanie_)
{
    ui->setupUi(this);
    ui->testName->setText(test->getName());
    ui->testName->setReadOnly(true);

    if (nrPomiar > 1) {
        SETREADONLY(ui->osobaWykonujaca);
        SETREADONLY(ui->wilgotnosc);
        SETREADONLY(ui->cisnienie);
        SETREADONLY(ui->temperatura);
        SETREADONLY(ui->dataRozpoczecia);
        SETREADONLY(ui->uwagi);

        ui->dataRozpoczecia->setText(test->getDataRozpoczecia());
        ui->osobaWykonujaca->setText(test->getOsobaWykonujaca());
        ui->wilgotnosc->setText(test->getWilgotnosc());
        ui->cisnienie->setText(test->getCisnienie());
        ui->temperatura->setText(test->getTemperatura());
        ui->uwagi->setPlainText(test->getUwagi());

    } else {
        const QString dataWyk = QDate::currentDate().toString("yyyy-MM-dd") + QString(" ") + QTime::currentTime().toString("HH:mm");
        ui->dataRozpoczecia->setText(dataWyk);
#ifdef DEFVAL
        ui->osobaWykonujaca->setText("Osoba wykonująca Test");
        ui->wilgotnosc->setText("99.3");
        ui->cisnienie->setText("1005");
        ui->temperatura->setText("21.5");
        ui->uwagi->setPlainText("Uwagi");
#endif
    }
    short numCzujek = badanie.getIloscCzujek();
    for (int n = 0 ; n < numCzujek ; n++)
    {
        auto v = badanie.getNumeryCzujki(n, test->getId() != REPRODUCIBILITY);
        QStringList vVariant;
        vVariant << v.first << v.second;
        ui->cbCzujka->addItem(QString::number(n+1), QVariant::fromValue(vVariant));
    }

    ui->eTransmitter->setText(badanie.getNazwaNumerTransmitter());
    ui->eReceiver->setText(badanie.getNazwaNumerReceiver());

    //dodajmy odpowiednie koncowki
    test->setNazwaNumerTransmitter(badanie.getNazwaNumerTransmitter());
    test->setNazwaNumerReceiver(badanie.getNazwaNumerReceiver());
    test->setNazwaTransmitter(badanie.getNazwaTransmitter());
    test->setNazwaReceiver(badanie.getNazwaReceiver());
    test->setNazwaTransmitter_a(badanie.getNazwaTransmitter_a());
    test->setNazwaReceiver_a(badanie.getNazwaReceiver_a());


    connect(ui->cbCzujka, &QComboBox::currentIndexChanged, this, &OknoParametryTestu::changeCzujka);
    NiewspolosiowoscOsUrzadzenie od;
    switch(test->getId())
    {
        case REPRODUCIBILITY:
            ui->lUwagaWyborCzujek->setText(QString("Zamontuj kolejną (%1) czujkę z serii").arg(nrPomiar));
            if (ui->cbCzujka->count() >= nrPomiar) {
                ui->cbCzujka->setCurrentIndex(nrPomiar-1);
                changeCzujka(nrPomiar-1);
            } else {
                ui->cbCzujka->setCurrentIndex(0);
                changeCzujka(0);
            }
            ui->frameSpec->setVisible(false);
            nrCzujkiDoWybrania = nrPomiar;
            break;
        case REPEATABILITY:
            ui->lUwagaWyborCzujek->setText(QString("Wybierz czujkę nr 2 zgodnie z normą"));
            if (ui->cbCzujka->count() >= 2) {
                ui->cbCzujka->setCurrentIndex(1);
                changeCzujka(1);
            } else {
                ui->cbCzujka->setCurrentIndex(0);
                changeCzujka(0);
            }
            ui->frameSpec->setVisible(true);
            ui->frame_powtarzalnosc->setVisible(true);
            ui->frame_niewspolosiowosc->setVisible(false);
            ui->frame_dlugoscDrogiOptycznej->setVisible(false);
            ui->frame_Napiecie->setVisible(false);
            nrCzujkiDoWybrania = 2;
            break;
        break;
        case TOLERANCE_TO_BEAM_MISALIGNMENT:
            ui->lUwagaWyborCzujek->setText(QString("Wybierz czujkę nr 1 zgodnie z normą"));
            ui->cbCzujka->setCurrentIndex(0);
            changeCzujka(0);
            ui->frameSpec->setVisible(true);
            ui->frame_powtarzalnosc->setVisible(false);
            ui->frame_niewspolosiowosc->setVisible(true);
            ui->frame_dlugoscDrogiOptycznej->setVisible(false);
            ui->frame_Napiecie->setVisible(false);
            ui->pionNadajnik->setText(badanie.getMaksKatowaNieWspolPionowaNadajnika());
            ui->poziomNadajnik->setText(badanie.getMaksKatowaNieWspolPoziomaNadajnika());
            ui->pionOdbiornik->setText(badanie.getMaksKatowaNieWspolPionowaOdbiornika());
            ui->poziomOdbiornik->setText(badanie.getMaksKatowaNieWspolPoziomaOdbiornika());
            ui->etTransmitter->setText(badanie.getNazwaNumerTransmitter());
            ui->etReceiver->setText(badanie.getNazwaNumerReceiver());
            nrCzujkiDoWybrania = 1;
            od.nadajnik.pionowo = badanie.getMaksKatowaNieWspolPionowaNadajnika();
            od.nadajnik.poziomo = badanie.getMaksKatowaNieWspolPoziomaNadajnika();
            od.odbiornik.pionowo = badanie.getMaksKatowaNieWspolPionowaOdbiornika();
            od.odbiornik.poziomo = badanie.getMaksKatowaNieWspolPoziomaOdbiornika();
            test->setKatyProducenta(od);
            nrCzujkiDoWybrania = 1;
            break;
        case RAPID_CHANGES_IN_ATTENUATION:
            ui->frameSpec->setVisible(false);
            ui->lUwagaWyborCzujek->setText(QString("Wybierz czujkę nr 1 zgodnie z normą"));
            ui->cbCzujka->setCurrentIndex(0);
            changeCzujka(0);
            nrCzujkiDoWybrania = 1;
            break;
        case OPTICAL_PATH_LENGTH_DEPEDENCE:
            ui->frameSpec->setVisible(true);
            ui->frame_powtarzalnosc->setVisible(false);
            ui->frame_niewspolosiowosc->setVisible(false);
            ui->frame_dlugoscDrogiOptycznej->setVisible(true);
            ui->frame_Napiecie->setVisible(false);
            ui->lUwagaWyborCzujek->setText(QString("Wybierz czujkę nr 1 zgodnie z normą"));
            ui->cbCzujka->setCurrentIndex(0);
            changeCzujka(0);
            ui->rozstawienieMinimalne->setText(badanie.getRozstawienieMinCzujki());
            ui->rozstawienieMaksymalne->setText(badanie.getRozstawienieMaxCzujki());
            nrCzujkiDoWybrania = 1;
            break;
        case STRAY_LIGHT:
            ui->frameSpec->setVisible(false);
            ui->lUwagaWyborCzujek->setText(QString("Wybierz czujkę nr 6 zgodnie z normą"));
            //TODO
            //if (ui->cbCzujka)
            ui->cbCzujka->setCurrentIndex(0);
            changeCzujka(0);
            nrCzujkiDoWybrania = 6;
            break;
        case TOLERANCE_TO_SUPPLY_VOLTAGE:
            ui->frameSpec->setVisible(true);
            ui->frame_powtarzalnosc->setVisible(false);
            ui->frame_niewspolosiowosc->setVisible(false);
            ui->frame_dlugoscDrogiOptycznej->setVisible(true);
            ui->frame_Napiecie->setVisible(true);
            ui->lUwagaWyborCzujek->setText(QString("Wybierz czujkę nr 1 zgodnie z normą"));

            ui->cbCzujka->setCurrentIndex(0);
            changeCzujka(0);
            nrCzujkiDoWybrania = 6;
            break;
        default:
        break;
    }




    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->pbOK_clicked(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    connect(ui->osobaWykonujaca, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });
    connect(ui->wilgotnosc, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });
    connect(ui->cisnienie, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });
    connect(ui->temperatura, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });
    connect(ui->cbCzujka, &QComboBox::currentIndexChanged, this, [this](int) { this->check(); });
    connect(ui->powtarzalnosc_czas, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });
    connect(ui->minVolt, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });
    connect(ui->maxVolt, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });
    check();


}

OknoParametryTestu::~OknoParametryTestu()
{
    delete ui;
}

void OknoParametryTestu::check1Pomiar()
{
    //ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
    if (ui->osobaWykonujaca->text().isEmpty()) {
        addError(QString::fromUtf8("Pole 'Osoba Wykonująca test' nie może być puste"), true);
    }
    if (ui->temperatura->text().isEmpty()) {
        addError(QString::fromUtf8("Pole 'Temperatura' nie może być puste"), true);
    } else {
        bool ok;
        double val = ui->temperatura->text().toDouble(&ok);
        if (!ok) {
            addError(QString::fromUtf8("Pole 'Temperatura' zawiera niepoprawną wartość"), true);
        } else if (val < 15 || val > 35 ){
            addError(QString::fromUtf8("Pole 'Temperatura' zawiera wartość niezgodną z normą"), false);
        }
    }

    if (ui->wilgotnosc->text().isEmpty()) {
        addError(QString::fromUtf8("Pole 'Wilgotność' nie może być puste"), true);
    } else {
        bool ok;
        double val = ui->wilgotnosc->text().toDouble(&ok);
        if (!ok) {
            addError(QString::fromUtf8("Pole 'Wilgotność' zawiera niepoprawną wartość"), true);
        } else if (val < 25 || val > 75 ) {
            addError(QString::fromUtf8("Pole 'Wilgotność' zawiera wartość niezgodną z normą"), false);
        }
    }

    if (ui->cisnienie->text().isEmpty()) {
        addError(QString::fromUtf8("Pole 'Ciśnienie' nie może być puste"), true);
    } else {
        bool ok;
        double val = ui->cisnienie->text().toDouble(&ok);
        if (!ok) {
            addError(QString::fromUtf8("Pole 'Ciśnienie' zawiera niepoprawną wartość"), true);
        } else if (val < 860 || val > 1060 ){
            addError(QString::fromUtf8("Pole 'Ciśnienie' zawiera wartość niezgodną z normą"), false);
        }
    }
}

void OknoParametryTestu::check()
{
    errorsMsg.clear();
    if (nrPomiar == 1) {
        check1Pomiar();


        if (ui->frame_powtarzalnosc->isVisible()) {
            QString ssecs = ui->powtarzalnosc_czas->text();
            if (ssecs.isEmpty()) {
                addError(QString::fromUtf8("Pole 'Czas pomiędzy pierwszymi trzema próbami' zawiera niepoprawną wartość"), true);
            } else {
                bool ok;
                unsigned int secs = ssecs.toInt(&ok);
                if (!ok) {
                    addError(QString::fromUtf8("Pole 'Czas pomiędzy pierwszymi trzema próbami' zawiera niepoprawną wartość"), true);
                } else if (secs < badanie.getMaksymalnyCzasOczekiwaniaPowtarzalnosc1Test() ||
                           secs > badanie.getMaksymalnyCzasOczekiwaniaPowtarzalnosc1Test()) {
                    addError(QString::fromUtf8("Czas pomiędzy pierwszymi trzema próbami powinien być w zakresie 15-60 minut [900-3600 sekund]"), false);
                }
            }
        } else if (ui->frame_Napiecie->isVisible()) {
            QString sminvolt = ui->minVolt->text();
            if (sminvolt.isEmpty()) {
                addError(QString::fromUtf8("Pole 'Minimalne napięcie zasilania czujki' zawiera niepoprawną wartość"), true);
            } else {
                bool ok;
                unsigned int mvolt = sminvolt.toDouble(&ok);
                if (!ok) {
                    addError(QString::fromUtf8("Pole 'Minimalne napięcie zasilania czujki' zawiera niepoprawną wartość"), true);
                } else if (mvolt < badanie.getMinimalneNapieciaTolerancjaNapiecia() ||
                           mvolt > badanie.getMaksymalneNapieciaTolerancjaNapiecia()) {
                    addError(QString::fromUtf8("Pole 'Minimalne napięcie zasilania czujki' zawiera wartość z poza zakresu"), false);
                }
            }

            QString smaxvolt = ui->maxVolt->text();
            if (smaxvolt.isEmpty()) {
                addError(QString::fromUtf8("Pole 'Maksymalne napięcie zasilania czujki' zawiera niepoprawną wartość"), true);
            } else {
                bool ok;
                unsigned int mvolt = smaxvolt.toDouble(&ok);
                if (!ok) {
                    addError(QString::fromUtf8("Pole 'Miaksymalne napięcie zasilania czujki' zawiera niepoprawną wartość"), true);
                } else if (mvolt < badanie.getMinimalneNapieciaTolerancjaNapiecia() ||
                           mvolt > badanie.getMaksymalneNapieciaTolerancjaNapiecia()) {
                    addError(QString::fromUtf8("Pole 'Maksymalne napięcie zasilania czujki' zawiera wartość z poza zakresu"), false);
                }
            }
        }
    } //1 pomiar
    else {
//Todo sprawdzenie czujki
        if (test->sprawdzCzyBadanaCzujka(ui->typTransmitter->text(), ui->typReceiver->text())) {
            addError(QString::fromUtf8("Wybrana czujka była już badana"), true);
        }
    }

    if (ui->cbCzujka->currentIndex() != nrCzujkiDoWybrania-1) {
        addError(QString::fromUtf8("Zgodnie z normą powinną się wybrać czujkę numer %1").arg(nrCzujkiDoWybrania),
                 test->getId() != REPRODUCIBILITY);
    }
    if (errorsMsg.size() == 0) {
        ui->errorLab->setText("Dane sa prawidłowe");
        ui->errorLab->setStyleSheet("color : black; font-weight:normal; ");
    } else {
        QString h = "<html><body><p style=\"color:red; font-weight:bold;\">Formularz zawiera błędy</p><ul>";
        for (const auto & err : errorsMsg) {
            h += err;
        }
        h += "</ul></body></html>";
        ui->errorLab->setText(h);
    }
}

void OknoParametryTestu::pbOK_clicked()
{
    test->setCisnienie(ui->cisnienie->text());
    test->setDataRozpoczecia(ui->dataRozpoczecia->text());
    test->setOsobaWykonujaca(ui->osobaWykonujaca->text());
    test->setWilgotnosc(ui->wilgotnosc->text());
    test->setTemperatura(ui->temperatura->text());
    test->setUwagi(ui->uwagi->toPlainText());
    test->addWybranaCzujka(ui->cbCzujka->currentText().toShort(), ui->typTransmitter->text(), ui->typReceiver->text());

    if(test->getId() == REPEATABILITY)
    {
        QString ssecs = ui->powtarzalnosc_czas->text();
        if (!ssecs.isEmpty()) {
            bool ok;
            unsigned int secs = ssecs.toInt(&ok);
            if (ok && secs >= 900 && secs <= 3600) {
               test->setCzasPowtarzalnosci(secs);
            }
        }
    }
    else if (test->getId() == OPTICAL_PATH_LENGTH_DEPEDENCE) {
        test->setMinimalneRozstawienie(ui->rozstawienieMinimalne->text());
        test->setMaksymalneRozstawienie(ui->rozstawienieMaksymalne->text());
    }
    else if (test->getId() == TOLERANCE_TO_SUPPLY_VOLTAGE) {
        test->setMinimalneNapiecie(ui->minVolt->text());
        test->setMaksymalneNapiecie(ui->maxVolt->text());
    }

    accept();
}

void OknoParametryTestu::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()), "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}

void OknoParametryTestu::changeCzujka(int index)
{
    QVariant v = ui->cbCzujka->itemData(index);
    QStringList sl = v.toStringList();
    ui->typTransmitter->setText(sl.first());
    ui->typReceiver->setText(sl.last());
    if (test->sprawdzCzyBadanaCzujka(sl.first(), sl.last())) {
        ui->errorLab->setText("Wybrana czujka była już podana badaniu");
        ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
    }
}

void OknoParametryTestu::addError(const QString &err, bool prio)
{
    if (prio)
        error = true;
    if (prio)
        errorsMsg.push_front(QString("<li><span style=\"color:red; font-weight:bold;\">%1<span></li>").arg(err));
    else
        errorsMsg.push_back(QString("<li><span style=\"color:maroon; font-weight:bold;\">%1<span></li>").arg(err));

}
