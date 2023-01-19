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
    if (id == -1)
        return;
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
    test.setWykonany(true);
    badanie.setDaneTestu(id, test);
    if (test.getId() == REPRODUCIBILITY)
        badanie.posortuj();
    setUkonczoneBadanie(test.getId(), badanie);
    ui->stackedWidget->setCurrentWidget(testyWidget[test.getId()].page);


}

void ListaBadan::setUkonczoneBadanie(short id, const ParametryBadania & badanie)
{
    const DaneTestu & test = badanie.getTesty()[id];
    if (test.getWykonany()) {
        testyWidget[test.getId()].button->setVisible(false);
        testyWidget[test.getId()].wyniki->setVisible(true);
        ui->tableWidget->item(id, 2)->setText(test.getOsobaWykonujaca());
        ui->tableWidget->item(id, 3)->setText(test.getOk() ? "POZYTYWNY" : "NEGATYWNY");
        ui->tableWidget->item(id, 4)->setText(test.getDataRozpoczecia());
        ui->tableWidget->item(id, 5)->setText(test.getDataZakonczenia());
        ui->tableWidget->item(id, 6)->setText(test.getTemperatura());
        ui->tableWidget->item(id, 7)->setText(test.getWilgotnosc());
        ui->tableWidget->item(id, 8)->setText(test.getCisnienie());

        setDaneTest(test, badanie);
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
    for (auto val : testyWidget)
    {
        val.button->setVisible(true);
        val.wyniki->setVisible(false);
    }
    initialTable(badanie);
    const auto & keys = badanie.getTesty().keys();
    for (const auto & k : keys) {
        setUkonczoneBadanie(k, badanie);
    }
    ui->stackedWidget->setCurrentWidget(testyWidget[0].page);
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
        case RAPID_CHANGES_IN_ATTENUATION:
        default:
            ui->stackedWidget->setCurrentIndex(2);
        break;

    }
}


#define ADDITEM(T,S,R,C) do { QTableWidgetItem *item = new QTableWidgetItem(S); T->setItem(R,C,item); } while(false)
#define ADDHEADITEM(T,S,R,W) do { QTableWidgetItem *item = new QTableWidgetItem(S); T->setHorizontalHeaderItem(R, item); T->setColumnWidth(0, W); } while(false)
#define ADDVHEADITEM(T,S,R) do { QTableWidgetItem *item = new QTableWidgetItem(S); T->setVerticalHeaderItem(R, item); } while(false)

void ListaBadan::setDaneTest(const DaneTestu &daneTestu, const ParametryBadania & badanie)
{
    QString pierwszy, drugi;
    pierwszy = daneTestu.getNazwaNumerPierwszego();
    drugi = daneTestu.getNazwaNumerDrugiego();

    if (daneTestu.getId() == REPRODUCIBILITY) {
        QTableWidget * tableParams = ui->odtwarzalnoscTableParams;
        tableParams->clear();

        ADDVHEADITEM(tableParams, "Cmin", 0);
        ADDITEM(tableParams, QString::number(daneTestu.getCmin(), 'f', 2) + " dB", 0, 0);
        ADDITEM(tableParams, "0 %" , 0, 1);

        ADDVHEADITEM(tableParams, "Cmax", 1);
        ADDITEM(tableParams, QString::number(daneTestu.getCmax(), 'f', 2) + " dB", 1, 0);
        ADDITEM(tableParams, "0 %" , 1, 1);

        ADDVHEADITEM(tableParams, "Crep", 2);
        ADDITEM(tableParams, QString::number(daneTestu.getCrep(), 'f', 2) + " dB", 2, 0);
        ADDITEM(tableParams, "0 %" , 2, 1);

        ADDVHEADITEM(tableParams, "Cmax/Crep", 3);
        ADDITEM(tableParams, QString::number(daneTestu.getCmaxCrep(), 'f', 2), 3, 0);

        ADDVHEADITEM(tableParams, "Crep/Cmax", 4);
        ADDITEM(tableParams, QString::number(daneTestu.getCrepCmin(), 'f', 2), 4, 0);

        initOdtwarzalnoscTable(pierwszy, drugi);
        if (daneTestu.getCmaxCrep() > badanie.getOdtwarzalnoscCmaxCrep()) {
            tableParams->item(3, 0)->setBackground(Qt::red);
        }

        if (daneTestu.getCrepCmin() > badanie.getOdtwarzalnoscCrepCmin()) {
            tableParams->item(4, 0)->setBackground(Qt::red);
        }

        if (daneTestu.getOk()) {
            ui->odtwarzalnoscResult->setText("POZYTYWNY");
        } else {
            ui->odtwarzalnoscResult->setText(QString("NEGATYWNY - %1").arg(daneTestu.getErrStr()));
        }

        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            addOdtwarzalnoscRekord(num, dane.nrCzujki, badanie.getSortedId(dane.nrCzujki-1),
                                   dane.numerNadajnika, dane.numerOdbiornika,
                              dane.value_dB, "0", dane.ok, dane.error);
            num++;
        }
    } else if (daneTestu.getId() == REPEATABILITY) {

        initPowtarzalnoscTable();

        QTableWidget * tableCzujka = ui->powarzalnosctableCzujka;
        int col = 0;
        //ADDHEADITEM(tableCzujka, "Nr czujki", col++, 50);
        ADDHEADITEM(tableCzujka, badanie.getNazwaNumerPierwszego(), col++, 150);
        ADDHEADITEM(tableCzujka, badanie.getNazwaNumerDrugiego(), col++, 150);
        col = 0;
        //ADDITEM(tableCzujka, daneTestu.getNumerCzujki(), 0, col++);
        ADDITEM(tableCzujka, daneTestu.getNumerNadajnika(), 0, col++);
        ADDITEM(tableCzujka, daneTestu.getNumerOdbiornika(), 0, col++);

        QTableWidget * tableParams = ui->powtarzalnoscTableParams;
        ADDVHEADITEM(tableParams, "Cmin", 0);
        ADDITEM(tableParams, QString::number(daneTestu.getCmin(), 'f', 2) + " dB", 0, 0);
        ADDITEM(tableParams, "0 %" , 0, 1);

        ADDVHEADITEM(tableParams, "Cmax", 1);
        ADDITEM(tableParams, QString::number(daneTestu.getCmax(), 'f', 2) + " dB", 1, 0);
        ADDITEM(tableParams, "0 %" , 1, 1);

        ADDVHEADITEM(tableParams, "Cmax/Cmin", 2);
        ADDITEM(tableParams, QString::number(daneTestu.getCmaxCmin(), 'f', 2), 2, 0);

        if (daneTestu.getCmaxCmin() > badanie.getPowtarzalnoscCmaxCmin()) {
            tableParams->item(2, 0)->setBackground(Qt::red);
        }

        if (daneTestu.getOk()) {
            ui->powtarzalnoscResult->setText("POZYTYWNY");
        } else {
            ui->powtarzalnoscResult->setText(QString("NEGATYWNY - %1").arg(daneTestu.getErrStr()));
        }

        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            addPowtarzalnoscRekord(num, dane.value_dB, "0", dane.ok, dane.error);
            num++;
        }
    }
}

void ListaBadan::initOdtwarzalnoscTable(const QString &nadajnik, const QString &odbiornik)
{
    QTableWidget * tablePrzebieg = ui->odtwarzalnoscTablePrzebieg;
    if (tablePrzebieg->columnCount() != 7)
        tablePrzebieg->setColumnCount(7);

    int col = 0;
    QTableWidgetItem *nrCzujki = new QTableWidgetItem("Nr czujki");
    tablePrzebieg->setHorizontalHeaderItem(col, nrCzujki);
    tablePrzebieg->setColumnWidth(col, 50);
    ++col;

    QTableWidgetItem *kolCzujki = new QTableWidgetItem("Kol. pomiarów");
    tablePrzebieg->setHorizontalHeaderItem(col, kolCzujki);
    tablePrzebieg->setColumnWidth(col, 50);
    ++col;

    QTableWidgetItem *itemNadajnik = new QTableWidgetItem(nadajnik);
    tablePrzebieg->setHorizontalHeaderItem(col, itemNadajnik);
    tablePrzebieg->setColumnWidth(col, 130);
    ++col;

    QTableWidgetItem *itemOdbiornik = new QTableWidgetItem(odbiornik);
    tablePrzebieg->setHorizontalHeaderItem(col, itemOdbiornik);
    tablePrzebieg->setColumnWidth(col, 130);
    ++col;

    QTableWidgetItem *cn1 = new QTableWidgetItem(QString::fromUtf8("C[n] dB"));
    tablePrzebieg->setHorizontalHeaderItem(col, cn1);
    tablePrzebieg->setColumnWidth(col, 50);
    ++col;

    QTableWidgetItem *cn2 = new QTableWidgetItem(QString::fromUtf8("C[n] %"));
    tablePrzebieg->setHorizontalHeaderItem(col, cn2);
    tablePrzebieg->setColumnWidth(col, 50);
    ++col;

    QTableWidgetItem *itemUwagi = new QTableWidgetItem("Uwagi");
    tablePrzebieg->setHorizontalHeaderItem(col, itemUwagi);
    tablePrzebieg->setColumnWidth(col, 130);
    ++col;

    tablePrzebieg->adjustSize();

    QTableWidget * tableParametry = ui->odtwarzalnoscTableParams;

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
    tablePrzebieg->setItem(row, col++, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(sortPomiar));
    tablePrzebieg->setItem(row, col++, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem(numerNadajnika);
    tablePrzebieg->setItem(row, col++, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem(numerOdbiornika);
    tablePrzebieg->setItem(row, col++, item3);

    QTableWidgetItem *item4 = new QTableWidgetItem(value_dB);
    tablePrzebieg->setItem(row, col++, item4);

    QTableWidgetItem *item5 = new QTableWidgetItem(value_perc);
    tablePrzebieg->setItem(row, col++, item5);

    QTableWidgetItem *item6 = new QTableWidgetItem(error);
    tablePrzebieg->setItem(row, col++, item6);

    if (!ok) {
        for (short e=0; e<col; ++e) {
            if (tablePrzebieg->item(row, e))
                tablePrzebieg->item(row, e)->setBackground(Qt::red);
        }
    }
}

void ListaBadan::initPowtarzalnoscTable()
{
    QTableWidget * tablePrzebieg = ui->powtarzalnoscTablePrzebieg;
    tablePrzebieg->clear();
    if (tablePrzebieg->columnCount() < 3)
        tablePrzebieg->setColumnCount(3);

    QTableWidgetItem *cn1 = new QTableWidgetItem(QString::fromUtf8("C[n]"));
    tablePrzebieg->setHorizontalHeaderItem(0, cn1);
    tablePrzebieg->setColumnWidth(0, 50);

    QTableWidgetItem *cn2 = new QTableWidgetItem(QString::fromUtf8("C[n]"));
    tablePrzebieg->setHorizontalHeaderItem(1, cn2);
    tablePrzebieg->setColumnWidth(1, 50);

    QTableWidgetItem *itemUwagi = new QTableWidgetItem("Uwagi");
    tablePrzebieg->setHorizontalHeaderItem(2, itemUwagi);
    tablePrzebieg->setColumnWidth(2, 130);

    tablePrzebieg->setMaximumWidth(245);
    tablePrzebieg->setMinimumWidth(245);
}

void ListaBadan::addPowtarzalnoscRekord(short num, const QString & value_dB, const QString & value_perc, bool ok, const QString & error)
{
    int row = num;
    QTableWidget * tablePrzebieg = ui->powtarzalnoscTablePrzebieg;
    QTableWidgetItem *itemVert = new QTableWidgetItem(QString::number(num+1));
    //ui->tablePrzebieg
    tablePrzebieg->setVerticalHeaderItem(row, itemVert);

    int col = 0;
    QTableWidgetItem *item0 = new QTableWidgetItem(value_dB);
    tablePrzebieg->setItem(row, col++, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem(value_perc);
    tablePrzebieg->setItem(row, col++, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem(error);
    tablePrzebieg->setItem(row, col++, item2);

    if (!ok) {
        for (short e=0; e<col; ++e) {
            tablePrzebieg->item(row, e)->setBackground(Qt::red);
        }
    }
}



