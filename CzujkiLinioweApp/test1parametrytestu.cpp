#include "test1parametrytestu.h"
#include "ui_test1parametrytestu.h"
#include "danetestu.h"
#include <QDate>
#include <QTime>

#define SETREADONLY(w) w->setReadOnly(true);

Test1ParametryTestu::Test1ParametryTestu(const ParametryBadania & badanie, DaneTestu * test_, short nrTestu, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test1ParametryTestu),
    test(test_)
{
    ui->setupUi(this);
    ui->testName->setText(test->getName());
    ui->testName->setReadOnly(true);

    if (nrTestu > 1) {
        SETREADONLY(ui->osobaWykonujaca);
        SETREADONLY(ui->wilgotnosc);
        SETREADONLY(ui->cisnienie);
        SETREADONLY(ui->temperatura);
        SETREADONLY(ui->dataRozpoczecia);
        SETREADONLY(ui->uwagi);

        ui->dataRozpoczecia->setText(test->getRozpoczeto());
        ui->osobaWykonujaca->setText(test->getOsobaWykonujaca());
        ui->wilgotnosc->setText(test->getWilgotnosc());
        ui->cisnienie->setText(test->getCisnienie());
        ui->temperatura->setText(test->getTemperatura());
        ui->uwagi->setPlainText(test->getUwagi());
    } else {
        const QString dataWyk = QDate::currentDate().toString("yyyy-MM-dd") + QString(" ") + QTime::currentTime().toString("HH:mm");
        ui->dataRozpoczecia->setText(dataWyk);
    }
    switch(test->getId())
    {
        case 0:
            ui->lUwagaWyborCzujek->setText(QString("Zamontuj kolejną (%1) czujkę z serii").arg(nrTestu)); break;
        default:
            ui->lUwagaWyborCzujek->setText(QString("Wybierz dowolną czujkę")); break;
    }

    short numCzujek = badanie.getIloscCzujek();
    for (int n = 0 ; n < numCzujek ; n++)
    {
        auto v = badanie.getNumeryCzujki(n);
        QStringList vVariant;
        vVariant << v.first << v.second;
        ui->cbCzujka->addItem(QString::number(n+1), QVariant::fromValue(vVariant));
    }

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->pbOK_clicked(); });
    connect(ui->pbDalej, &QPushButton::pressed, this, [this]() { this->pbOK_clicked(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    connect(ui->pbPrzerwij, &QPushButton::pressed, this, [this]() { this->pbCancel_clicked(); });

    connect(ui->cbCzujka, &QComboBox::currentIndexChanged, this, &Test1ParametryTestu::changeCzujka);
    ui->cbCzujka->setCurrentIndex(0);
}

Test1ParametryTestu::~Test1ParametryTestu()
{
    delete ui;
}

void Test1ParametryTestu::check()
{
    if (ui->osobaWykonujaca->text().isEmpty()) {


    }
    if (ui->wilgotnosc->text().isEmpty()) {


    }
    if (ui->cisnienie->text().isEmpty()) {


    }
    if (ui->temperatura->text().isEmpty()) {


    }
}

void Test1ParametryTestu::pbOK_clicked()
{
    test->setCisnienie(ui->cisnienie->text());
    test->setRozpoczeto(ui->dataRozpoczecia->text());
    test->setOsobaWykonujaca(ui->osobaWykonujaca->text());
    test->setWilgotnosc(ui->dataRozpoczecia->text());
    test->setTemperatura(ui->temperatura->text());
    test->setUwagi(ui->uwagi->toPlainText());
    test->setNumerPierwszy(ui->typPierwszy->text());
    test->setNumerDrugi(ui->typDrugi->text());
    test->addNumerWybranejCzujki(ui->cbCzujka->currentIndex());
    accept();
}

void Test1ParametryTestu::pbCancel_clicked()
{
    reject();
}

void Test1ParametryTestu::changeCzujka(int index)
{
    QVariant v = ui->cbCzujka->itemData(index);
    QStringList sl = v.toStringList();
    ui->typPierwszy->setText(sl.first());
    ui->typDrugi->setText(sl.last());
}
