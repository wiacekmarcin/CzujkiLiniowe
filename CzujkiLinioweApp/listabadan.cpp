#include "listabadan.h"
#include "ui_listabadan.h"
#include "zasilacz.h"
#include <QDate>
#include <QTime>

ListaBadan::ListaBadan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListaBadan),
    procedura(parent),
    badanieWTrakcie(false),
    intCurrAlarm(0)
{
    ui->setupUi(this);

    testyWidget[REPRODUCIBILITY] = testWidget{ui->pOdtwarzalnosc, ui->odtwarzalnoscWyniki, ui->pbOdtwarzalnosc};
    testyWidget[REPEATABILITY] = testWidget{ui->pPowtarzalnosc, ui->powtarzalnoscWyniki, ui->pbPowtarzalnosc};


    ui->odtwarzalnoscWyniki->setVisible(false);
    ui->pbOdtwarzalnosc->setVisible(true);
    ui->powtarzalnoscWyniki->setVisible(false);
    ui->pbPowtarzalnosc->setVisible(true);
    ui->stackedWidget->setCurrentWidget(ui->odtwarzalnoscWidget);

    for (auto val : testyWidget)
    {
        val.button->setVisible(true);
        val.wyniki->setVisible(false);
    }

}

ListaBadan::~ListaBadan()
{
    delete ui;
}

void ListaBadan::startBadanie(short id, ParametryBadania & badanie, const Ustawienia & ust,
                              Zasilacz *zas, Sterownik * ster)
{


    QString nameTest = "Nieznane Badanie";
    if (id < lt.nazwyTestow.size())
        nameTest = lt.nazwyTestow.at(id);
    wyzwalaniePradem = badanie.getWyzwalanieAlarmuPradem();
    wyzwalaniePrzekaznikiem = badanie.getWyzwalanieAlarmuPrzekaznikiem();
    zasilaczZewnetrzny = badanie.getZasilanieCzujekZasilaczZewnetrzny();

    badanieWTrakcie = true;
    if (!procedura.startBadanie(id, nameTest, badanie, ust, zas, ster))
    {
        badanieWTrakcie = false;
        testyWidget[id].wyniki->setVisible(false);
        testyWidget[id].button->setVisible(true);

        return;
    }

    DaneTestu test = procedura.getDane();
    test.setDataZakonczenia(QDate::currentDate().toString("yyyy-MM-dd") + QString(" ") + QTime::currentTime().toString("HH:mm"));
    badanie.setDaneTestu(id, test);
    switch(id){
        case REPRODUCIBILITY:
            badanie.posortuj();
            ui->odtwarzalnoscWyniki->setVisible(true);
            ui->pbOdtwarzalnosc->setVisible(false);
            setDaneTest(test, badanie);
            ui->odtwarzalnoscResult->setText(test.getOk() ? "POZYTYWNY" : QString("NEGATYWNY ") + test.getErrStr());
            ui->stackedWidget->setCurrentWidget(ui->pOdtwarzalnosc);
            ui->tableWidget->item(id, 2)->setText(test.getOsobaWykonujaca());
            ui->tableWidget->item(id, 3)->setText(test.getOk() ? "POZYTYWNY" : "NEGATYWNY");
            ui->tableWidget->item(id, 4)->setText(test.getDataRozpoczecia());
            ui->tableWidget->item(id, 5)->setText(test.getDataZakonczenia());
            ui->tableWidget->item(id, 6)->setText(test.getTemperatura());
            ui->tableWidget->item(id, 7)->setText(test.getWilgotnosc());
            ui->tableWidget->item(id, 8)->setText(test.getCisnienie());
            break;
        case REPEATABILITY:
            ui->powtarzalnoscWyniki->setVisible(true);
            ui->pbPowtarzalnosc->setVisible(false);
            setDaneTest(test, badanie);
            ui->stackedWidget->setCurrentWidget(ui->powtarzalnoscWidget);
            ui->tableWidget->item(id, 2)->setText(test.getOsobaWykonujaca());
            ui->tableWidget->item(id, 3)->setText(test.getOk() ? "POZYTYWNY" : "NEGATYWNY");
            ui->tableWidget->item(id, 4)->setText(test.getDataRozpoczecia());
            ui->tableWidget->item(id, 5)->setText(test.getDataZakonczenia());
            ui->tableWidget->item(id, 6)->setText(test.getTemperatura());
            ui->tableWidget->item(id, 7)->setText(test.getWilgotnosc());
            ui->tableWidget->item(id, 8)->setText(test.getCisnienie());
            break;
    default:
        break;
    }
}

void ListaBadan::initialTable(const ParametryBadania & badanie)
{
    if (ui->tableWidget->columnCount() < 9)
        ui->tableWidget->setColumnCount(9);

    QTableWidgetItem *itemId = new QTableWidgetItem("ID");
    ui->tableWidget->setHorizontalHeaderItem(0, itemId);
    ui->tableWidget->setColumnWidth(0, 5);
    ui->tableWidget->hideColumn(0);

    QTableWidgetItem *itemName = new QTableWidgetItem(QString::fromUtf8("Nazwa Badania"));
    ui->tableWidget->setHorizontalHeaderItem(1, itemName);
    ui->tableWidget->setColumnWidth(0, 150);

    QTableWidgetItem *itemPerson = new QTableWidgetItem(QString::fromUtf8("Osoba wykonująca test"));
    ui->tableWidget->setHorizontalHeaderItem(2, itemPerson);
    ui->tableWidget->setColumnWidth(0, 120);

    QTableWidgetItem *itemStatus = new QTableWidgetItem(QString::fromUtf8("Wynik"));
    ui->tableWidget->setHorizontalHeaderItem(3, itemStatus);
    ui->tableWidget->setColumnWidth(0, 70);

    QTableWidgetItem *itemBegin = new QTableWidgetItem(QString::fromUtf8("Rozpoczęto"));
    ui->tableWidget->setHorizontalHeaderItem(4, itemBegin);
    ui->tableWidget->setColumnWidth(0, 70);

    QTableWidgetItem *itemEnd = new QTableWidgetItem(QString::fromUtf8("Zakończono"));
    ui->tableWidget->setHorizontalHeaderItem(5, itemEnd);
    ui->tableWidget->setColumnWidth(0, 70);

    QTableWidgetItem *itemTemp = new QTableWidgetItem(QString::fromUtf8("Temp."));
    ui->tableWidget->setHorizontalHeaderItem(6, itemTemp);
    ui->tableWidget->setColumnWidth(0, 50);

    QTableWidgetItem *itemWilg = new QTableWidgetItem(QString::fromUtf8("Wilgotn."));
    ui->tableWidget->setHorizontalHeaderItem(7, itemWilg);
    ui->tableWidget->setColumnWidth(0, 50);

    QTableWidgetItem *itemCisn = new QTableWidgetItem(QString::fromUtf8("Ciśnienie"));
    ui->tableWidget->setHorizontalHeaderItem(8, itemCisn);
    ui->tableWidget->setColumnWidth(0, 60);

    short row = 0;

    if (ui->tableWidget->rowCount() < badanie.getTesty().size())
        ui->tableWidget->setRowCount(badanie.getTesty().size());

    for (const auto & test : badanie.getTesty()) {
        QTableWidgetItem *itemVert = new QTableWidgetItem(QString::number(row+1));
        ui->tableWidget->setVerticalHeaderItem(row, itemVert);

        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(test.getId()));
        ui->tableWidget->setItem(row, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(test.getName());
        ui->tableWidget->setItem(row, 1, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(test.getOsobaWykonujaca());
        ui->tableWidget->setItem(row, 2, item2);

        QTableWidgetItem *item3 = new QTableWidgetItem(test.getWynik());
        ui->tableWidget->setItem(row, 3, item3);

        QTableWidgetItem *item4 = new QTableWidgetItem(test.getDataRozpoczecia());
        ui->tableWidget->setItem(row, 4, item4);

        QTableWidgetItem *item5 = new QTableWidgetItem(test.getDataZakonczenia());
        ui->tableWidget->setItem(row, 5, item5);

        QTableWidgetItem *item6 = new QTableWidgetItem(test.getTemperatura());
        ui->tableWidget->setItem(row, 6, item6);

        QTableWidgetItem *item7 = new QTableWidgetItem(test.getWilgotnosc());
        ui->tableWidget->setItem(row, 7, item7);

        QTableWidgetItem *item8 = new QTableWidgetItem(test.getCisnienie());
        ui->tableWidget->setItem(row, 8, item8);

        ++row;
    }


}



void ListaBadan::flt_zerowanieFiltrowDone()
{
    procedura.flt_zerowanieFiltrowDone();
}

void ListaBadan::flt_setUkladFiltrowDone()
{
    procedura.flt_setUkladFiltrowDone();
}

void ListaBadan::flt_bladFiltrow(QChar filtr, bool zerowanie)
{
    procedura.flt_bladFiltrow(filtr, zerowanie);
}

void ListaBadan::ster_setPositionDone(short silnik, RuchSilnikaType ruch)
//void ListaBadan::ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt)
{
    //procedura.ster_setPositionDone(silnik, home, move, error, interrupt);
    procedura.ster_setPositionDone(silnik, ruch);
}

void ListaBadan::ster_setValue(short silnik, const double & val)
{
    procedura.ster_setValue(silnik, val);
}

void ListaBadan::ster_czujkaOn()
{
    if (badanieWTrakcie && wyzwalaniePrzekaznikiem)
        procedura.czujkaOn();
}

void ListaBadan::zas_value(int kind, int value)
{
    if (!badanieWTrakcie)
        return;

    if (zasilaczZewnetrzny || wyzwalaniePradem)
        procedura.zas_value(kind, value);

    if (wyzwalaniePradem && kind == Zasilacz::CURRENT_MEAS) {
        if (value >= (int)intCurrAlarm)
            procedura.czujkaOn();
    }

}

void ListaBadan::setBadanie(const ParametryBadania &badanie)
{
    ui->tableWidget->clear();
    initialTable(badanie);
}

void ListaBadan::on_tableWidget_cellClicked(int row, int column)
{
    (void)column;
    switch(row)
    {
        case REPRODUCIBILITY:
            ui->stackedWidget->setCurrentIndex(0);
            break;
        case REPEATABILITY:
            ui->stackedWidget->setCurrentIndex(1);
            break;
        case TOLERANCE_TO_BEAM_MISALIGNMENT:
        default:
            ui->stackedWidget->setCurrentIndex(2);
        break;

    }
}

void ListaBadan::setDaneTest(DaneTestu &daneTestu, const ParametryBadania & badanie)
{
    QString pierwszy, drugi;
    pierwszy = daneTestu.getNazwaNumerPierwszego();
    drugi = daneTestu.getNazwaNumerDrugiego();
    if (daneTestu.getId() == REPRODUCIBILITY) {
        QTableWidget * tableParams = ui->odtwarzalnoscTableParams;
        initOdtwarzalnoscTable(pierwszy, drugi);
        ui->stackedWidget->setCurrentWidget(ui->pOdtwarzalnosc);
        tableParams->item(0,0)->setText(QString::number(daneTestu.getCmin(), 'f', 2) + " dB");
        tableParams->item(0,1)->setText("0 %");
        tableParams->item(1,0)->setText(QString::number(daneTestu.getCmax(), 'f', 2) + " dB");
        tableParams->item(1,1)->setText("0 %");
        tableParams->item(2,0)->setText(QString::number(daneTestu.getCrep(), 'f', 2) + " dB");
        tableParams->item(2,1)->setText("0 %");
        tableParams->item(3,0)->setText(QString::number(daneTestu.getCmaxCrep(), 'f', 2));
        if (daneTestu.getCmaxCrep() > 1.33) {
            tableParams->item(3, 0)->setBackground(Qt::red);
        }
        tableParams->item(4,0)->setText(QString::number(daneTestu.getCrepCmin(), 'f', 2));
        if (daneTestu.getCrepCmin() > 1.5) {
            tableParams->item(4, 0)->setBackground(Qt::red);
        }

        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            addOdtwarzalnoscRekord(num, dane.nrCzujki, badanie.getSortedId(dane.nrCzujki-1)+1, dane.numerNadajnika, dane.numerOdbiornika,
                              dane.value_dB, 0, dane.ok, dane.error);
            num++;
        }
    }
}

void ListaBadan::initOdtwarzalnoscTable(const QString &nadajnik, const QString &odbiornik)
{
    QTableWidget * tablePrzebieg = ui->odtwarzalnoscTablePrzebieg;
    if (tablePrzebieg->columnCount() < 4)
        tablePrzebieg->setColumnCount(4);

    QTableWidgetItem *itemNadajnik = new QTableWidgetItem(nadajnik);
    tablePrzebieg->setHorizontalHeaderItem(0, itemNadajnik);
    tablePrzebieg->setColumnWidth(0, 130);

    QTableWidgetItem *itemOdbiornik = new QTableWidgetItem(odbiornik);
    tablePrzebieg->setHorizontalHeaderItem(1, itemOdbiornik);
    tablePrzebieg->setColumnWidth(1, 130);

    QTableWidgetItem *cn1 = new QTableWidgetItem(QString::fromUtf8("C[n]"));
    tablePrzebieg->setHorizontalHeaderItem(2, cn1);
    tablePrzebieg->setColumnWidth(2, 50);

    QTableWidgetItem *cn2 = new QTableWidgetItem(QString::fromUtf8("C[n]"));
    tablePrzebieg->setHorizontalHeaderItem(3, cn2);
    tablePrzebieg->setColumnWidth(3, 50);


}

void ListaBadan::addOdtwarzalnoscRekord(short num, short nrCzujki, short sortPomiar, const QString & numerNadajnika, const QString & numerOdbiornika,
                              const QString & value_dB, const QString & value_perc, bool ok, const QString & error)
{
    int row = num;
    QTableWidget * tablePrzebieg = ui->odtwarzalnoscTablePrzebieg;
    QTableWidgetItem *itemVert = new QTableWidgetItem(QString::number(num+1));
    //ui->tablePrzebieg
    tablePrzebieg->setVerticalHeaderItem(row, itemVert);

    int col = 0;
    QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(nrCzujki));
    ui->tableWidget->setItem(row, col++, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(sortPomiar));
    ui->tableWidget->setItem(row, col++, item0);

    QTableWidgetItem *item2 = new QTableWidgetItem(numerNadajnika);
    tablePrzebieg->setItem(row, col++, item1);

    QTableWidgetItem *item3 = new QTableWidgetItem(numerOdbiornika);
    tablePrzebieg->setItem(row, col++, item2);

    if (ok) {
        QTableWidgetItem *item4 = new QTableWidgetItem(value_dB);
        tablePrzebieg->setItem(row, col++, item3);

        QTableWidgetItem *item5 = new QTableWidgetItem(value_perc);
        tablePrzebieg->setItem(row, col++, item4);
    } else {
        QTableWidgetItem *item3 = new QTableWidgetItem(value_dB);
        tablePrzebieg->setItem(row, col++, item3);

        QTableWidgetItem *item4 = new QTableWidgetItem(error);
        tablePrzebieg->setItem(row, col++, item4);

        for (short e=0; e<col; ++e) {
            tablePrzebieg->item(row, col)->setBackground(Qt::red);
        }
    }
}




