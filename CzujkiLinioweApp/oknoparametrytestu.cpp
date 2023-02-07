#include "oknoparametrytestu.h"
#include "ui_oknoparametrytestu.h"
#include "danetestu.h"
#include <QDate>
#include <QTime>
#include <QMessageBox>


#define SETREADONLY(w) w->setReadOnly(true);

OknoParametryTestu::OknoParametryTestu(short nrPomiar_, DaneTestu * test_, const ParametryBadania & badanie_,
                                       const Ustawienia & ust, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoParametryTestu),
    test(test_),
    nrPomiar(nrPomiar_),
    badanie(badanie_)
{
    ui->setupUi(this);
    ui->testName->setText(test->getName());
    ui->testName->setReadOnly(true);

    minTemp = ust.getMinimalnaTemperatura();
    maxTemp = ust.getMaksymalnaTemperatura();
    minCisn = ust.getMinimalnaCisnienie();
    maxCisn = ust.getMaksymalnaCisnienie();
    minHumi = ust.getMinimalnaWilgotnosc();
    maxHumi = ust.getMaksymalnaWilgotnosc();
    miniCzasPowt = ust.getMinimalnaWilgotnosc();
    maxiCzasPowt = ust.getMaksymalnyCzasOczekiwaniaPowtarzalnosc1Test();
    minVolt = ust.getMinimalneNapieciaTolerancjaNapiecia();
    maxVolt = ust.getMaksymalneNapieciaTolerancjaNapiecia();

    nrCzujkiDoWybrania2 = 0;
    dwieCzujkiDoWybrania = false;

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
        ui->osobaWykonujaca->setText(badanie.getOsobaOdpowiedzialna());
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
        auto v = badanie.getNumeryCzujki(n);
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


    connect(ui->cbCzujka, qOverload<int>(&QComboBox::currentIndexChanged), this, &OknoParametryTestu::changeCzujka);
    NiewspolosiowoscOsUrzadzenie od;
    switch(test->getId())
    {
        case REPRODUCIBILITY:
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Zamontuj kolejną (%1) czujkę z serii").arg(nrPomiar));
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
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 2 zgodnie z normą"));
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
            ui->powtarzalnosc_czas->setText("900");
            check();
            break;
        break;
        case TOLERANCE_TO_BEAM_MISALIGNMENT:
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 1 zgodnie z normą"));
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
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 1 zgodnie z normą"));
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
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 1 zgodnie z normą"));
            ui->cbCzujka->setCurrentIndex(0);
            changeCzujka(0);
            ui->rozstawienieMinimalne->setText(badanie.getRozstawienieMinCzujki());
            ui->rozstawienieMaksymalne->setText(badanie.getRozstawienieMaxCzujki());
            nrCzujkiDoWybrania = 1;
            break;
        case STRAY_LIGHT:
            ui->frameSpec->setVisible(false);
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 6 zgodnie z normą"));
            if (ui->cbCzujka->count() >= 6) {
                ui->cbCzujka->setCurrentIndex(5);
                changeCzujka(5);
            } else {
                ui->cbCzujka->setCurrentIndex(0);
                changeCzujka(0);
            }
            nrCzujkiDoWybrania = 6;
            break;
        case TOLERANCE_TO_SUPPLY_VOLTAGE:
            ui->frameSpec->setVisible(true);
            ui->frame_powtarzalnosc->setVisible(false);
            ui->frame_niewspolosiowosc->setVisible(false);
            ui->frame_dlugoscDrogiOptycznej->setVisible(false);
            ui->frame_Napiecie->setVisible(true);
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 1 zgodnie z normą"));

            ui->cbCzujka->setCurrentIndex(0);
            changeCzujka(0);
            nrCzujkiDoWybrania = 1;
            break;
        case DRY_HEAT:
            ui->frameSpec->setVisible(false);
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 3 zgodnie z normą"));
            if (ui->cbCzujka->count() >= 3) {
                ui->cbCzujka->setCurrentIndex(2);
                changeCzujka(2);
            } else {
                ui->cbCzujka->setCurrentIndex(0);
                changeCzujka(0);
            }
            nrCzujkiDoWybrania = 3;
            break;
        case COLD:
            ui->frameSpec->setVisible(false);
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 3 zgodnie z normą"));
            if (ui->cbCzujka->count() >= 3) {
                ui->cbCzujka->setCurrentIndex(2);
                changeCzujka(2);
            } else {
                ui->cbCzujka->setCurrentIndex(0);
                changeCzujka(0);
            }
            nrCzujkiDoWybrania = 3;
            break;
        case DAMP_HEAT_STADY_STATE_OPERATIONAL:
            ui->frameSpec->setVisible(false);
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 2 zgodnie z normą"));
            if (ui->cbCzujka->count() >= 2) {
                ui->cbCzujka->setCurrentIndex(1);
                changeCzujka(1);
            } else {
                ui->cbCzujka->setCurrentIndex(0);
                changeCzujka(0);
            }
            nrCzujkiDoWybrania = 2;
            break; 
        case DAMP_HEAT_STADY_STATE_ENDURANCE:
            ui->frameSpec->setVisible(false);
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 2 zgodnie z normą"));
            if (ui->cbCzujka->count() >= 2) {
                ui->cbCzujka->setCurrentIndex(1);
                changeCzujka(1);
            } else {
                ui->cbCzujka->setCurrentIndex(0);
                changeCzujka(0);
            }
            nrCzujkiDoWybrania = 2;
            break; 
        case VIBRATION:
            ui->frameSpec->setVisible(false);
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 7 zgodnie z normą"));
            if (ui->cbCzujka->count() >= 7) {
                ui->cbCzujka->setCurrentIndex(6);
                changeCzujka(6);
            } else {
                ui->cbCzujka->setCurrentIndex(0);
                changeCzujka(0);
            }
            nrCzujkiDoWybrania = 7;
            break;  

        case IMPACT:
            ui->frameSpec->setVisible(false);
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 1 zgodnie z normą"));
            if (ui->cbCzujka->count() >= 1) {
                ui->cbCzujka->setCurrentIndex(0);
                changeCzujka(0);
            } else {
                ui->cbCzujka->setCurrentIndex(0);
                changeCzujka(0);
            }
            nrCzujkiDoWybrania = 7;
            break;

        case ELECTROMAGNETIC_ELEKTROSTATIC_DISCHARGE:
        case ELECTROMAGNETIC_RADIATED_ELEKTROMAGNETIC_FIELDS:
        case ELECTROMAGNETIC_CONDUCTED_DISTURBANCE_INDUCED:
        case ELECTROMAGNETIC_FAST_TRANSIENT_BURSTS:
        case ELECTROMAGNETIC_SLOW_HIGH_ENERGY_VOLTAGE_SURGES:
            ui->frameSpec->setVisible(false);
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 4 lub 6 zgodnie z normą"));
            if (ui->cbCzujka->count() >= 4) {
                ui->cbCzujka->setCurrentIndex(3);
                changeCzujka(3);
            } else {
                ui->cbCzujka->setCurrentIndex(0);
                changeCzujka(0);
            }
            nrCzujkiDoWybrania = 4;
            nrCzujkiDoWybrania2 = 6;
            dwieCzujkiDoWybrania = true;
            break;


        case SULPHUR_DIOXIDE_SO2_CORROSION:
            ui->frameSpec->setVisible(false);
            ui->lUwagaWyborCzujek->setText(QString::fromUtf8("Wybierz czujkę nr 5 zgodnie z normą"));
            if (ui->cbCzujka->count() >= 5) {
                ui->cbCzujka->setCurrentIndex(4);
                changeCzujka(4);
            } else {
                ui->cbCzujka->setCurrentIndex(0);
                changeCzujka(0);
            }
            nrCzujkiDoWybrania = 5;
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
    connect(ui->cbCzujka, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int) { this->check(); });
    connect(ui->powtarzalnosc_czas, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });
    connect(ui->minVolt, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });
    connect(ui->maxVolt, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });
    check();

    adjustSize();
}

OknoParametryTestu::~OknoParametryTestu()
{
    delete ui;
}

void OknoParametryTestu::check1Pomiar()
{
    iloscKrytycznychBledow = 0;
    //ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
    if (ui->osobaWykonujaca->text().isEmpty()) {
        addError(QString::fromUtf8("Pole 'Osoba Wykonująca test' nie może być puste"), true);
        ++iloscKrytycznychBledow;
    }
    if (ui->temperatura->text().isEmpty()) {
        addError(QString::fromUtf8("Pole 'Temperatura' nie może być puste"), true);
        ++iloscKrytycznychBledow;
    } else {
        bool ok;
        double val = ui->temperatura->text().toDouble(&ok);
        if (!ok) {
            addError(QString::fromUtf8("Pole 'Temperatura' zawiera niepoprawną wartość"), true);
            ++iloscKrytycznychBledow;
        } else if (val < minTemp || val > maxTemp ){
            addError(QString::fromUtf8("Pole 'Temperatura' zawiera wartość niezgodną z normą"), false);
        }
    }

    if (ui->wilgotnosc->text().isEmpty()) {
        addError(QString::fromUtf8("Pole 'Wilgotność' nie może być puste"), true);
        ++iloscKrytycznychBledow;
    } else {
        bool ok;
        double val = ui->wilgotnosc->text().toDouble(&ok);
        if (!ok) {
            addError(QString::fromUtf8("Pole 'Wilgotność' zawiera niepoprawną wartość"), true);
            ++iloscKrytycznychBledow;
        } else if (val < minHumi || val > maxHumi ) {
            addError(QString::fromUtf8("Pole 'Wilgotność' zawiera wartość niezgodną z normą"), false);
        }
    }

    if (ui->cisnienie->text().isEmpty()) {
        addError(QString::fromUtf8("Pole 'Ciśnienie' nie może być puste"), true);
        ++iloscKrytycznychBledow;
    } else {
        bool ok;
        double val = ui->cisnienie->text().toDouble(&ok);
        if (!ok) {
            addError(QString::fromUtf8("Pole 'Ciśnienie' zawiera niepoprawną wartość"), true);
            ++iloscKrytycznychBledow;
        } else if (val < minCisn || val > maxCisn ){
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
                ++iloscKrytycznychBledow;
            } else {
                bool ok;
                unsigned int secs = ssecs.toInt(&ok);
                if (!ok) {
                    addError(QString::fromUtf8("Pole 'Czas pomiędzy pierwszymi trzema próbami' zawiera niepoprawną wartość"), true);
                    ++iloscKrytycznychBledow;
                } else if (secs < miniCzasPowt ||
                           secs > maxiCzasPowt) {
                    addError(QString::fromUtf8("Czas pomiędzy pierwszymi trzema próbami powinien być w zakresie 15-60 minut [900-3600 sekund]"), false);
                }
            }
        } else if (ui->frame_Napiecie->isVisible()) {
            QString sminvolt = ui->minVolt->text();
            if (sminvolt.isEmpty()) {
                addError(QString::fromUtf8("Pole 'Minimalne napięcie zasilania czujki' zawiera niepoprawną wartość"), true);
                ++iloscKrytycznychBledow;
            } else {
                bool ok;
                unsigned int mvolt = sminvolt.toDouble(&ok);
                if (!ok) {
                    addError(QString::fromUtf8("Pole 'Minimalne napięcie zasilania czujki' zawiera niepoprawną wartość"), true);
                    ++iloscKrytycznychBledow;
                } else if (mvolt < minVolt ||
                           mvolt > maxVolt) {
                    addError(QString::fromUtf8("Pole 'Minimalne napięcie zasilania czujki' zawiera wartość z poza zakresu"), false);
                }
            }

            QString smaxvolt = ui->maxVolt->text();
            if (smaxvolt.isEmpty()) {
                addError(QString::fromUtf8("Pole 'Maksymalne napięcie zasilania czujki' zawiera niepoprawną wartość"), true);
                ++iloscKrytycznychBledow;
            } else {
                bool ok;
                unsigned int mvolt = smaxvolt.toDouble(&ok);
                if (!ok) {
                    addError(QString::fromUtf8("Pole 'Miaksymalne napięcie zasilania czujki' zawiera niepoprawną wartość"), true);
                    ++iloscKrytycznychBledow;
                } else if (mvolt < minVolt ||
                           mvolt > maxVolt) {
                    addError(QString::fromUtf8("Pole 'Maksymalne napięcie zasilania czujki' zawiera wartość z poza zakresu"), false);
                } else {

                }
            }
        }
    } //1 pomiar
    else {
//Todo sprawdzenie czujki
        if (test->sprawdzCzyBadanaCzujka(ui->typTransmitter->text(), ui->typReceiver->text())) {
            addError(QString::fromUtf8("Wybrana czujka była już badana"), true);
            ui->pbDalej->setEnabled(false);
        } else {
            ui->pbDalej->setEnabled(true);
        }
    }

    if (dwieCzujkiDoWybrania) {
        if (ui->cbCzujka->currentIndex() != nrCzujkiDoWybrania-1 &&
                ui->cbCzujka->currentIndex() != nrCzujkiDoWybrania2-1) {
            addError(QString::fromUtf8("Zgodnie z normą powinną się wybrać czujkę numer %1 lub %2").
                     arg(nrCzujkiDoWybrania).
                     arg(nrCzujkiDoWybrania2),
                     test->getId() != REPRODUCIBILITY);
        }
    } else {
        if (ui->cbCzujka->currentIndex() != nrCzujkiDoWybrania-1) {
            addError(QString::fromUtf8("Zgodnie z normą powinną się wybrać czujkę numer %1").arg(nrCzujkiDoWybrania),
                     test->getId() != REPRODUCIBILITY);
        }
    }

    ui->pbDalej->setEnabled(iloscKrytycznychBledow == 0);
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
    else if (test->getId() == STRAY_LIGHT ||
             test->getId() == DRY_HEAT ||
             test->getId() == COLD ||
             test->getId() == DAMP_HEAT_STADY_STATE_OPERATIONAL ||
             test->getId() == DAMP_HEAT_STADY_STATE_ENDURANCE ||
             test->getId() == VIBRATION ||
             test->getId() == IMPACT ||
             test->getId() == SULPHUR_DIOXIDE_SO2_CORROSION ||
             test->getId() == ELECTROMAGNETIC_ELEKTROSTATIC_DISCHARGE ||
             test->getId() == ELECTROMAGNETIC_RADIATED_ELEKTROMAGNETIC_FIELDS ||
             test->getId() == ELECTROMAGNETIC_CONDUCTED_DISTURBANCE_INDUCED ||
             test->getId() == ELECTROMAGNETIC_FAST_TRANSIENT_BURSTS ||
             test->getId() == ELECTROMAGNETIC_SLOW_HIGH_ENERGY_VOLTAGE_SURGES
             )
    {
        test->setDanePomiarowe(badanie.getDaneDlaCzujki(ui->typTransmitter->text(), ui->typReceiver->text()));
    }
    test->addWybranaCzujka(ui->cbCzujka->currentText().toShort(), ui->typTransmitter->text(), ui->typReceiver->text());
    
    done(QDialog::Accepted);
}

static int question(QWidget * parent, const QString & title, const QString & pytanie) {
    QMessageBox messageBox(QMessageBox::Question, title, pytanie, 
                        QMessageBox::Yes | QMessageBox::No, parent);

    messageBox.setButtonText(QMessageBox::Yes, QString::fromUtf8("Tak"));
    messageBox.setButtonText(QMessageBox::No, QString::fromUtf8("Nie"));
    return messageBox.exec();
}

void OknoParametryTestu::pbCancel_clicked()
{
    int ret = question(this, QString::fromUtf8("Badanie : %1").arg(ui->testName->text()), 
                            QString::fromUtf8("Czy napewno chcesz przerwać badanie"));
    if (ret == QMessageBox::Yes)
        done(QDialog::Rejected);
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
        errorsMsg.push_front(QString::fromUtf8("<li><span style=\"color:red; font-weight:bold;\">%1<span></li>").arg(err));
    else
        errorsMsg.push_back(QString::fromUtf8("<li><span style=\"color:maroon; font-weight:bold;\">%1<span></li>").arg(err));

}

