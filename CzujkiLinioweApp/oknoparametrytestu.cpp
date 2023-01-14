#include "oknoparametrytestu.h"
#include "ui_oknoparametrytestu.h"
#include "danetestu.h"
#include <QDate>
#include <QTime>
#include <QMessageBox>


#define SETREADONLY(w) w->setReadOnly(true);

OknoParametryTestu::OknoParametryTestu(short nrPomiar_, DaneTestu * test_, const ParametryBadania & badanie, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoParametryTestu),
    test(test_),
    nrPomiar(nrPomiar_)
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

    ui->ePierwszy->setText(badanie.getNazwaPierwszego());
    ui->eDrugi->setText(badanie.getNazwaDrugiego());
    test->setNazwaPierwszego(badanie.getNazwaPierwszego());
    test->setNazwaDrugiego(badanie.getNazwaDrugiego());

    connect(ui->cbCzujka, &QComboBox::currentIndexChanged, this, &OknoParametryTestu::changeCzujka);
    NiewspolosiowoscOsUrzadzenie od;
    switch(test->getId())
    {
        case REPRODUCIBILITY:
            ui->lUwagaWyborCzujek->setText(QString("Zamontuj kolejną (%1) czujkę z serii").arg(nrPomiar));
            if (ui->cbCzujka->count() > nrPomiar)
                ui->cbCzujka->setCurrentIndex(nrPomiar-1);
            else
                ui->cbCzujka->setCurrentIndex(0);
            ui->frameSpec->setVisible(false);
            nrCzujkiDoWybrania = nrPomiar;
            break;
        case REPEATABILITY:
            ui->lUwagaWyborCzujek->setText(QString("Wybierz czujkę nr 2 zgodnie z normą"));
            if (ui->cbCzujka->count() > 2)
                ui->cbCzujka->setCurrentIndex(1);
            else
                ui->cbCzujka->setCurrentIndex(0);
            ui->frameSpec->setVisible(true);
            ui->frame_powtarzalnosc->setVisible(true);
            ui->frame_niewspolosiowosc->setVisible(false);
            nrCzujkiDoWybrania = 2;
            break;
        break;
        case TOLERANCE_TO_BEAM_MISALIGNMENT:
            ui->lUwagaWyborCzujek->setText(QString("Wybierz czujkę nr 1 zgodnie z normą"));
            ui->cbCzujka->setCurrentIndex(0);
            ui->frameSpec->setVisible(true);
            ui->frame_powtarzalnosc->setVisible(false);
            ui->frame_niewspolosiowosc->setVisible(true);
            ui->pionNadajnik->setText(badanie.getMaksKatowaNieWspolPionowaNadajnika());
            ui->poziomNadajnik->setText(badanie.getMaksKatowaNieWspolPoziomaNadajnika());
            ui->pionOdbiornik->setText(badanie.getMaksKatowaNieWspolPionowaOdbiornika());
            ui->poziomOdbiornik->setText(badanie.getMaksKatowaNieWspolPoziomaOdbiornika());
            ui->etPierwszy->setText(badanie.getNazwaPierwszego());
            ui->etDrugi->setText(badanie.getNazwaDrugiego());
            nrCzujkiDoWybrania = 1;
            od.nadajnik.pionowo = badanie.getMaksKatowaNieWspolPionowaNadajnika();
            od.nadajnik.poziomo = badanie.getMaksKatowaNieWspolPoziomaNadajnika();
            od.odbiornik.pionowo = badanie.getMaksKatowaNieWspolPionowaOdbiornika();
            od.odbiornik.poziomo = badanie.getMaksKatowaNieWspolPoziomaOdbiornika();
            test->setKatyProducenta(od);
            break;
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



    changeCzujka(0);
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
            addError(QString::fromUtf8("Pole 'Wilgotność' zawiera niezgodną z normą"), false);
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
            addError(QString::fromUtf8("Pole 'Ciśnienie' zawiera niezgodną z normą"), false);
        }
    }

}

void OknoParametryTestu::check()
{
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
                } else if (secs < 900 || secs > 3600) {
                    addError(QString::fromUtf8("Czas pomiędzy pierwszymi trzema próbami powinien być w zakresie 15-60 minut [900-3600 sekund]"), false);
                }
            }
        }
    }
    else {
//Todo sprawdzenie czujki
        if (test->sprawdzCzyBadanaCzujka(ui->typPierwszy->text(), ui->typDrugi->text())) {
            ui->errorLab->setText("Wybrana czujka była już badana");
            ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
        }
    }
    ui->errorLab->setText("Dane sa prawidłowe");
    ui->errorLab->setStyleSheet("color : black; font-weight:normal; ");
}

void OknoParametryTestu::pbOK_clicked()
{
    test->setCisnienie(ui->cisnienie->text());
    test->setDataRozpoczecia(ui->dataRozpoczecia->text());
    test->setOsobaWykonujaca(ui->osobaWykonujaca->text());
    test->setWilgotnosc(ui->wilgotnosc->text());
    test->setTemperatura(ui->temperatura->text());
    test->setUwagi(ui->uwagi->toPlainText());
    test->addWybranaCzujka(ui->cbCzujka->currentIndex(), ui->typPierwszy->text(), ui->typDrugi->text());

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
    ui->typPierwszy->setText(sl.first());
    ui->typDrugi->setText(sl.last());
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
        errorsMsg.push_back(QString("<li><span style=\"color:yellow; font-weight:bold;\">%1<span></li>").arg(err));

}
