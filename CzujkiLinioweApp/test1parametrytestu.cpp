#include "test1parametrytestu.h"
#include "ui_test1parametrytestu.h"
#include "danetestu.h"
#include <QDate>
#include <QTime>
#include <QMessageBox>


#define SETREADONLY(w) w->setReadOnly(true);

Test1ParametryTestu::Test1ParametryTestu(short nrPomiar_, DaneTestu * test_, const ParametryBadania & badanie, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test1ParametryTestu),
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
    switch(test->getId())
    {
        case REPRODUCIBILITY:
            ui->lUwagaWyborCzujek->setText(QString("Zamontuj kolejną (%1) czujkę z serii").arg(nrPomiar)); break;
        default:
            ui->lUwagaWyborCzujek->setText(QString("Wybierz dowolną czujkę")); break;
    }

    short numCzujek = badanie.getIloscCzujek();
    for (int n = 0 ; n < numCzujek ; n++)
    {
        auto v = badanie.getNumeryCzujki(test->getId() != REPRODUCIBILITY, n);
        QStringList vVariant;
        vVariant << v.first << v.second;
        ui->cbCzujka->addItem(QString::number(n+1), QVariant::fromValue(vVariant));
    }

    if (badanie.getSystemOdbiornikNadajnik()) { //odb <-> nad
        ui->ePierwszy->setText("Nadajnik");
        ui->eDrugi->setText("Odbiornik");
    } else { //odb+nad <-> ref
        ui->ePierwszy->setText("Nadajnik+Odbiornik");
        ui->eDrugi->setText("Reflektor");
    }

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->pbOK_clicked(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    connect(ui->osobaWykonujaca, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });
    connect(ui->wilgotnosc, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });
    connect(ui->cisnienie, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });
    connect(ui->temperatura, &QLineEdit::textEdited, this, [this](const QString &) { this->check(); });

    connect(ui->cbCzujka, &QComboBox::currentIndexChanged, this, &Test1ParametryTestu::changeCzujka);
    ui->cbCzujka->setCurrentIndex(0);
    changeCzujka(0);
    check();


}

Test1ParametryTestu::~Test1ParametryTestu()
{
    delete ui;
}

void Test1ParametryTestu::check()
{
    if (nrPomiar == 1) {
        if (ui->osobaWykonujaca->text().isEmpty()) {
            ui->errorLab->setText("Pole 'Osoba Wykonująca test' nie może być puste");
            ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
            return;
        }

        if (ui->temperatura->text().isEmpty()) {
            ui->errorLab->setText("Pole 'Temperatura' nie może być puste");
            ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
            return;
        } else {
            bool ok;
            double val = ui->temperatura->text().toDouble(&ok);
            if (!ok) {
                ui->errorLab->setText("Pole 'Temperatura' zawiera niepoprawną wartość");
                ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
                return;
            } else if (val < 15 || val > 35 ){
                ui->errorLab->setText("Pole 'Temperatura' zawiera niepoprawną wartość");
                ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
                return;
            }
        }

        if (ui->wilgotnosc->text().isEmpty()) {
            ui->errorLab->setText("Pole 'Wilgotność' nie może być puste");
            ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
            return;
        } else {
            bool ok;
            double val = ui->wilgotnosc->text().toDouble(&ok);
            if (!ok) {
                ui->errorLab->setText("Pole 'Wilgotność' zawiera niepoprawną wartość");
                ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
                return;
            } else if (val < 25 || val > 75 ){
                ui->errorLab->setText("Pole 'Wilgotność' zawiera niepoprawną wartość");
                ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
                return;
            }
        }

        if (ui->cisnienie->text().isEmpty()) {
            ui->errorLab->setText("Pole 'Ciśnienie' nie może być puste");
            ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
            return;
        } else {
            bool ok;
            double val = ui->cisnienie->text().toDouble(&ok);
            if (!ok) {
                ui->errorLab->setText("Pole 'Ciśnienie' zawiera niepoprawną wartość");
                ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
                return;
            } else if (val < 860 || val > 1060 ){
                ui->errorLab->setText("Pole 'Ciśnienie' zawiera niepoprawną wartość");
                ui->errorLab->setStyleSheet("color : red; font-weight:bold; ");
                return;
            }
        }
    } else {
//Todo sprawdzenie czujki
    }
    ui->errorLab->setText("Dane sa prawidłowe");
    ui->errorLab->setStyleSheet("color : black; font-weight:normal; ");
}

void Test1ParametryTestu::pbOK_clicked()
{
    test->setCisnienie(ui->cisnienie->text());
    test->setDataRozpoczecia(ui->dataRozpoczecia->text());
    test->setOsobaWykonujaca(ui->osobaWykonujaca->text());
    test->setWilgotnosc(ui->wilgotnosc->text());
    test->setTemperatura(ui->temperatura->text());
    test->setUwagi(ui->uwagi->toPlainText());
    test->addWybranaCzujka(ui->typPierwszy->text(), ui->typDrugi->text());
    accept();
}

void Test1ParametryTestu::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()), "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}

void Test1ParametryTestu::changeCzujka(int index)
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
