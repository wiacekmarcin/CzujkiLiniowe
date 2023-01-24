#include "listabadan.h"
#include "ui_listabadan.h"
#include "zasilacz.h"
#include <QDate>
#include <QTime>
#include <QDateTime>

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
    testyWidget[TOLERANCE_TO_BEAM_MISALIGNMENT] = testWidget{ui->pZaleznoscKierunkowa, ui->zaleznoscKierunkowaWyniki,
                                                ui->pbZaleznoscKierunkowa};

    testyWidget[RAPID_CHANGES_IN_ATTENUATION] = testWidget{ui->pSzybkieZmianyTlumienia, ui->szybkieZmianyTlumieniaWyniki,
                                                ui->pbSzybkieZmianyTlumienia};

    testyWidget[OPTICAL_PATH_LENGTH_DEPEDENCE] = testWidget{ui->pZaleznoscDlugisciDrogiOptycznej, ui->zaleznoscDlugisciDrogiOptycznejWyniki,
                                                ui->pbZaleznoscDlugisciDrogiOptycznej};

    testyWidget[STRAY_LIGHT] = testWidget{ui->pRozproszoneSwiatlo, ui->rozproszoneSwiatloWyniki,
                                                ui->pbRozproszoneSwiatlo};

    testyWidget[TOLERANCE_TO_SUPPLY_VOLTAGE] = testWidget{ui->pTolerancjaNapieciaZasilania, ui->tolerancjaNapieciaZasilaniaWyniki,
                                                ui->pbTolerancjaNapieciaZasilania};

    ui->stackedWidget->setCurrentWidget(ui->pOdtwarzalnosc);

    for (const auto & tid : testyWidget.keys())
    {
        const auto & wid = testyWidget[tid];
        wid.button->setVisible(true);
        wid.wyniki->setVisible(false);
        connect(wid.button, &QPushButton::clicked, this, [this, tid]() { this->startBadanieRequest(tid);});
        wid.button->setEnabled(false);
    }
    prevCzujkaOn = QDateTime::currentDateTime();
}

void ListaBadan::startBadanieRequest(int testId)
{
    emit startBadanieReq(testId);
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
    if (badanie.getWyzwalanieAlarmuPradem()) {
        intCurrAlarm = badanie.getPrzekroczeniePraduZasilania_mA().toUInt();
    }

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
        testyWidget[id].button->setVisible(false);
        testyWidget[id].wyniki->setVisible(true);
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

void ListaBadan::initialTestyTable(const ParametryBadania & badanie)
{
    ui->tableWidget->clear();

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
        procedura.czujkaOn(true);
}

void ListaBadan::zas_value(int kind, int value)
{
    if (!badanieWTrakcie)
        return;

    if (zasilaczZewnetrzny || wyzwalaniePradem)
        procedura.zas_value(kind, value);

    if (wyzwalaniePradem && kind == Zasilacz::CURRENT_MEAS) {
        if (value >= (int)intCurrAlarm) {
            auto t = QDateTime::currentDateTime();
            if (t.toMSecsSinceEpoch() - prevCzujkaOn.toMSecsSinceEpoch() > 100) {
                prevCzujkaOn = t;
                procedura.czujkaOn(false);
            }
        }
    }

}

void ListaBadan::setBadanie(const ParametryBadania &badanie)
{
    ui->tableWidget->clear();
    for (const auto & tid : testyWidget.keys())
    {
        const auto & wid = testyWidget[tid];
        wid.button->setEnabled(tid == REPRODUCIBILITY || badanie.getTestOdtwarzalnosci());
        wid.button->setVisible(true);
        wid.wyniki->setVisible(false);
    }

    initialTestyTable(badanie);
    const auto & keys = badanie.getTesty().keys();
    for (const auto & k : keys) {
        setUkonczoneBadanie(k, badanie);
    }
    ui->stackedWidget->setCurrentWidget(testyWidget[REPRODUCIBILITY].page);
}

void ListaBadan::closeBadanie()
{
    ui->tableWidget->clear();
    ui->frame_tab->setVisible(false);
    for (const auto & tid : testyWidget.keys())
    {
        const auto & wid = testyWidget[tid];
        wid.button->setEnabled(true);
        wid.button->setVisible(true);
        wid.wyniki->setVisible(false);
    }
}

void ListaBadan::on_tableWidget_cellClicked(int row, int column)
{
    (void)column;

    ui->stackedWidget->setCurrentWidget(testyWidget[ui->tableWidget->item(row, 0)->text().toInt()].page);
}

double ListaBadan::d2p(const double &val)
{
    double l = pow(10, val/10);
    if (l == 0)
        return 0;
    return 100.0/l;
}

QString ListaBadan::d2p(const QString &val)
{
    bool ok;
    double dval = val.toDouble(&ok);
    if (!ok)
        return "-";
    return QString::number(d2p(dval), 'f', 1);
}

const QMap<int, testWidget> &ListaBadan::getTestyWidget() const
{
    return testyWidget;
}


#define ADDITEM(T,S,R,C) do { QTableWidgetItem *item = new QTableWidgetItem(S); T->setItem(R,C,item); } while(false)
#define ADDHEADITEM(T,S,R,W) do { QTableWidgetItem *item = new QTableWidgetItem(S); T->setHorizontalHeaderItem(R, item); T->setColumnWidth(0, W); } while(false)
#define ADDVHEADITEM(T,S,R) do { QTableWidgetItem *item = new QTableWidgetItem(S); T->setVerticalHeaderItem(R, item); } while(false)

void ListaBadan::addC(QTableWidget * table, const QString & Cname, const QString & val1, const QString & val2, int row)
{
    QTableWidgetItem *headitem = new QTableWidgetItem(Cname); 
    table->setVerticalHeaderItem(row, headitem);
    
    QTableWidgetItem *item1 = new QTableWidgetItem(val1); 
    table->setItem(row, 0, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem(val2); 
    table->setItem(row, 1, item2);
}

void ListaBadan::addC(QTableWidget * table, const QString & Cname, const QString & val1, int row)
{
    QTableWidgetItem *headitem = new QTableWidgetItem(Cname); 
    table->setVerticalHeaderItem(row, headitem);
    
    QTableWidgetItem *item1 = new QTableWidgetItem(val1); 
    table->setItem(row, 0, item1);
}

void ListaBadan::setDaneTest(const DaneTestu &daneTestu, const ParametryBadania & badanie)
{
    QString transmitter, receiver;
    transmitter = daneTestu.getNazwaNumerTransmitter();
    receiver = daneTestu.getNazwaNumerReceiver();

    if (daneTestu.getId() == REPRODUCIBILITY) {
        
 

        QTableWidget * tableParams = ui->odtwarzalnoscTableParams;
        tableParams->clear();

        addC(tableParams, "Cmin", QString::number(daneTestu.getCmin(), 'f', 2) + " dB", QString::number(d2p(daneTestu.getCmin()), 'f', 2) + " %", 0);
        addC(tableParams, "Cmax", QString::number(daneTestu.getCmax(), 'f', 2) + " dB", QString::number(d2p(daneTestu.getCmax()), 'f', 2) + " %", 1);
        addC(tableParams, "Crep", QString::number(daneTestu.getCrep(), 'f', 2) + " dB", QString::number(d2p(daneTestu.getCrep()), 'f', 2) + " %", 2);
        addC(tableParams, "Cmax/Crep", QString::number(daneTestu.getCmaxCrep(), 'f', 2), 3);
        addC(tableParams, "Crep/Cmin", QString::number(daneTestu.getCrepCmin(), 'f', 2), 4);

        
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
        //initOdtwarzalnoscTable(transmitter, receiver);
        QStringList head;
        QList<int> width;
        head << "Nr czujki" << "Kol. pomiarów" << transmitter << receiver << "C[n] dB" << "C[n] %" << "Uwagi";
        width << 75 << 75 << 150 << 150 << 50 << 50 << 200;
        clearinitTable(ui->odtwarzalnoscTablePrzebieg, head, width);

        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            int col = addR(ui->odtwarzalnoscTablePrzebieg, num, 0, QString::number(dane.nrCzujki), QString::number(badanie.getSortedId(dane.nrCzujki-1)),
                                    dane.numerNadajnika, dane.numerOdbiornika,
                                    dane.value_dB, d2p(dane.value_dB), dane.error);
            if (!dane.ok) {
                for (short e=0; e<col; ++e) {
                if (ui->odtwarzalnoscTablePrzebieg->item(num, e))
                    ui->odtwarzalnoscTablePrzebieg->item(num, e)->setBackground(Qt::red);
                }
            }
        }
    } else if (daneTestu.getId() == REPEATABILITY) {

        QTableWidget * tableParams = ui->powtarzalnoscTableParams;
        tableParams->clear();
        addC(tableParams, "Cmin", QString::number(daneTestu.getCmin(), 'f', 2) + " dB", QString::number(d2p(daneTestu.getCmin()), 'f', 2) + " %", 0);
        addC(tableParams, "Cmax", QString::number(daneTestu.getCmax(), 'f', 2) + " dB", QString::number(d2p(daneTestu.getCmax()), 'f', 2) + " %", 1);
        addC(tableParams, "Cmax/Cmin", QString::number(daneTestu.getCmaxCmin(), 'f', 2), 3);

        //clearinitTable()
        
        initPowtarzalnoscTable();

        QTableWidget * tableCzujka = ui->powarzalnosctableCzujka;
        int col = 0;
        //ADDHEADITEM(tableCzujka, "Nr czujki", col++, 50);
        ADDHEADITEM(tableCzujka, badanie.getNazwaNumerTransmitter(), col++, 200);
        ADDHEADITEM(tableCzujka, badanie.getNazwaNumerReceiver(), col++, 200);
        col = 0;
        //ADDITEM(tableCzujka, daneTestu.getNumerCzujki(), 0, col++);
        ADDITEM(tableCzujka, daneTestu.getNumerTransmitter(), 0, col++);
        ADDITEM(tableCzujka, daneTestu.getNumerReceiver(), 0, col++);



        
        
        //ADDVHEADITEM(tableParams, "Cmin", 0);
        //ADDITEM(tableParams, QString::number(daneTestu.getCmin(), 'f', 2) + " dB", 0, 0);
        //ADDITEM(tableParams, QString::number(d2p(daneTestu.getCmin()), 'f', 1) + " %" , 0, 1);

        //ADDVHEADITEM(tableParams, "Cmax", 1);
        //ADDITEM(tableParams, QString::number(daneTestu.getCmax(), 'f', 2) + " dB", 1, 0);
        //ADDITEM(tableParams, QString::number(d2p(daneTestu.getCmax()), 'f', 1) + " %" , 1, 1);

        //ADDVHEADITEM(tableParams, "Cmax/Cmin", 2);
        //ADDITEM(tableParams, QString::number(daneTestu.getCmaxCmin(), 'f', 2), 2, 0);

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
            addPowtarzalnoscRekord(num, dane.value_dB, d2p(dane.value_dB), dane.ok, dane.error);
            num++;
        }
    } else if (daneTestu.getId() == RAPID_CHANGES_IN_ATTENUATION) {

        initSzybkieZmianyTlumieniaTable();

        QTableWidget * tableCzujka = ui->szybkieZmianyTlumieniatableCzujka;
        int col = 0;
        //ADDHEADITEM(tableCzujka, "Nr czujki", col++, 50);
        ADDHEADITEM(tableCzujka, badanie.getNazwaNumerTransmitter(), col++, 200);
        ADDHEADITEM(tableCzujka, badanie.getNazwaNumerReceiver(), col++, 200);
        col = 0;
        //ADDITEM(tableCzujka, daneTestu.getNumerCzujki(), 0, col++);
        ADDITEM(tableCzujka, daneTestu.getNumerTransmitter(), 0, col++);
        ADDITEM(tableCzujka, daneTestu.getNumerReceiver(), 0, col++);

        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            addSzybkieZmianyTlumieniaRekord(num, dane.value_dB, dane.ok, dane.error);
            num++;
        }

        if (daneTestu.getOk()) {
            ui->szybkieZmianyTlumieniaResult->setText("POZYTYWNY");
        } else {
            ui->szybkieZmianyTlumieniaResult->setText(QString("NEGATYWNY - %1").arg(daneTestu.getErrStr()));
        }
    } else if (daneTestu.getId() == OPTICAL_PATH_LENGTH_DEPEDENCE) {

        initZaleznoscDrogiOptycznejTable();
        QTableWidget * tableCzujka = ui->zaleznoscDlugisciDrogiOptycznejtableCzujka;
        int col = 0;
        //ADDHEADITEM(tableCzujka, "Nr czujki", col++, 50);
        ADDHEADITEM(tableCzujka, badanie.getNazwaNumerTransmitter(), col++, 200);
        ADDHEADITEM(tableCzujka, badanie.getNazwaNumerReceiver(), col++, 200);
        col = 0;
        //ADDITEM(tableCzujka, daneTestu.getNumerCzujki(), 0, col++);
        ADDITEM(tableCzujka, daneTestu.getNumerTransmitter(), 0, col++);
        ADDITEM(tableCzujka, daneTestu.getNumerReceiver(), 0, col++);

        QTableWidget * tableParams = ui->zaleznoscDlugisciDrogiOptycznejTableParams;
        ADDVHEADITEM(tableParams, "Cmin", 0);
        ADDITEM(tableParams, QString::number(daneTestu.getCmin(), 'f', 2) + " dB", 0, 0);
        ADDITEM(tableParams, QString::number(d2p(daneTestu.getCmin()), 'f', 1) + " %" , 0, 1);

        ADDVHEADITEM(tableParams, "Cmax", 1);
        ADDITEM(tableParams, QString::number(daneTestu.getCmax(), 'f', 2) + " dB", 1, 0);
        ADDITEM(tableParams, QString::number(d2p(daneTestu.getCmax()), 'f', 1) + " %" , 1, 1);

        ADDVHEADITEM(tableParams, "Cmax/Cmin", 2);
        ADDITEM(tableParams, QString::number(daneTestu.getCmaxCmin(), 'f', 2), 2, 0);

        if (daneTestu.getCmaxCmin() > badanie.getDrogaoptycznaCmaxCmin()) {
            tableParams->item(2, 0)->setBackground(Qt::red);
        }

        short num = 0;

        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            addZaleznoscDrogiOptycznejRekord(num,
                (num == 0 ? daneTestu.getMinimalneRozstawienie() : daneTestu.getMaksymalneRozstawienie()),
                dane.value_dB, dane.ok, dane.error);
            num++;
        }

        if (daneTestu.getOk()) {
            ui->zaleznoscDlugisciDrogiOptycznejResult->setText("POZYTYWNY");
        } else {
            ui->zaleznoscDlugisciDrogiOptycznejResult->setText(QString("NEGATYWNY - %1").arg(daneTestu.getErrStr()));
        }
    } else if (daneTestu.getId() == STRAY_LIGHT) {
        initRozproszoneSwiatloTable();
        QTableWidget * tableCzujka = ui->rozproszoneSwiatlotableCzujka;
        int col = 0;
        //ADDHEADITEM(tableCzujka, "Nr czujki", col++, 50);
        ADDHEADITEM(tableCzujka, badanie.getNazwaNumerTransmitter(), col++, 200);
        ADDHEADITEM(tableCzujka, badanie.getNazwaNumerReceiver(), col++, 200);
        col = 0;
        //ADDITEM(tableCzujka, daneTestu.getNumerCzujki(), 0, col++);
        ADDITEM(tableCzujka, daneTestu.getNumerTransmitter(), 0, col++);
        ADDITEM(tableCzujka, daneTestu.getNumerReceiver(), 0, col++);

        QTableWidget * tableParams = ui->rozproszoneSwiatloTableParams;
        ADDVHEADITEM(tableParams, "Cmin", 0);
        ADDITEM(tableParams, QString::number(daneTestu.getCmin(), 'f', 2) + " dB", 0, 0);
        ADDITEM(tableParams, QString::number(d2p(daneTestu.getCmin()), 'f', 1) + " %" , 0, 1);

        ADDVHEADITEM(tableParams, "Cmax", 1);
        ADDITEM(tableParams, QString::number(daneTestu.getCmax(), 'f', 2) + " dB", 1, 0);
        ADDITEM(tableParams, QString::number(d2p(daneTestu.getCmax()), 'f', 1) + " %" , 1, 1);

        ADDVHEADITEM(tableParams, "Cmax/Cmin", 2);
        ADDITEM(tableParams, QString::number(daneTestu.getCmaxCmin(), 'f', 2), 2, 0);

        if (daneTestu.getCmaxCmin() > badanie.getRozproszoneSwiatloCmaxCmin()) {
            tableParams->item(2, 0)->setBackground(Qt::red);
        }

        short num = 0;

        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            addRozproszoneSwiatloRekord(num,  dane.value_dB, dane.ok, dane.error);
            num++;
        }

        if (daneTestu.getOk()) {
            ui->rozproszoneSwiatloResult->setText("POZYTYWNY");
        } else {
            ui->rozproszoneSwiatloResult->setText(QString("NEGATYWNY - %1").arg(daneTestu.getErrStr()));
        }
    } if (daneTestu.getId() == TOLERANCE_TO_SUPPLY_VOLTAGE) {
        initTolerancjaZasilaniaTable();
        QTableWidget * tableCzujka = ui->tolerancjaNapieciaZasilaniatableCzujka;
        int col = 0;
        //ADDHEADITEM(tableCzujka, "Nr czujki", col++, 50);
        ADDHEADITEM(tableCzujka, badanie.getNazwaNumerTransmitter(), col++, 200);
        ADDHEADITEM(tableCzujka, badanie.getNazwaNumerReceiver(), col++, 200);
        col = 0;
        //ADDITEM(tableCzujka, daneTestu.getNumerCzujki(), 0, col++);
        ADDITEM(tableCzujka, daneTestu.getNumerTransmitter(), 0, col++);
        ADDITEM(tableCzujka, daneTestu.getNumerReceiver(), 0, col++);

        QTableWidget * tableParams = ui->tolerancjaNapieciaZasilaniaTableParams;
        ADDVHEADITEM(tableParams, "Cmin", 0);
        ADDITEM(tableParams, QString::number(daneTestu.getCmin(), 'f', 2) + " dB", 0, 0);
        ADDITEM(tableParams, QString::number(d2p(daneTestu.getCmin()), 'f', 1) + " %" , 0, 1);

        ADDVHEADITEM(tableParams, "Cmax", 1);
        ADDITEM(tableParams, QString::number(daneTestu.getCmax(), 'f', 2) + " dB", 1, 0);
        ADDITEM(tableParams, QString::number(d2p(daneTestu.getCmax()), 'f', 2) + " %" , 1, 1);

        ADDVHEADITEM(tableParams, "Cmax/Cmin", 2);
        ADDITEM(tableParams, QString::number(daneTestu.getCmaxCmin(), 'f', 2), 2, 0);

        if (daneTestu.getCmaxCmin() > badanie.getTolerancjaNapieciaZasilaniaCmaxCmin()) {
            tableParams->item(2, 0)->setBackground(Qt::red);
        }

        short num = 0;

        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            addTolerancjaNapieciaZasilaniaRekord(num, "Napiecie", dane.value_dB, dane.ok, dane.error);
            num++;
        }

        if (daneTestu.getOk()) {
            ui->tolerancjaNapieciaZasilaniaResult->setText("POZYTYWNY");
        } else {
            ui->tolerancjaNapieciaZasilaniaResult->setText(QString("NEGATYWNY - %1").arg(daneTestu.getErrStr()));
        }
    }

}

void ListaBadan::clearinitTable( QTableWidget * table, const QStringList & head, const QList<int> & width)
{
    table->clear();
    if (table->columnCount() != head.size())
        table->setColumnCount(head.size());
    
    int col = 0;
    for (const auto & h : head) {
        QTableWidgetItem *item = new QTableWidgetItem(h);
        table->setHorizontalHeaderItem(col, item);
        if (col < width.size())
            table->setColumnWidth(col, width.at(col));
        ++col;
    }
}

void ListaBadan::addR0(QTableWidget * table, int row)
{
    QTableWidgetItem *itemVert = new QTableWidgetItem(QString::number(row+1));
    table->setVerticalHeaderItem(row, itemVert);
}

short ListaBadan::addR1(QTableWidget * table, int row, int col, const QString & C1)
{
    QTableWidgetItem *item = new QTableWidgetItem(C1);
    table->setItem(row, col, item);
    return 1;
}

short ListaBadan::addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2) {
    addR0(table, row);
    addR1(table, row, col, C1);
    addR1(table, row, col+1, C2);
    return 2;
}

short ListaBadan::addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2, const QString &C3) {
    addR0(table, row);
    addR1(table, row, col, C1);
    addR1(table, row, col+1, C2);
    addR1(table, row, col+2, C3);
    return 3;
}

short ListaBadan::addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2, const QString &C3, const QString &C4) {
    addR0(table, row);
    addR1(table, row, col, C1);
    addR1(table, row, col+1, C2);
    addR1(table, row, col+2, C3);
    addR1(table, row, col+3, C4);
    return 4;
}

short ListaBadan::addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2, const QString &C3, const QString &C4, const QString &C5) {
    addR0(table, row);
    addR1(table, row, col, C1);
    addR1(table, row, col+1, C2);
    addR1(table, row, col+2, C3);
    addR1(table, row, col+3, C4);
    addR1(table, row, col+4, C5);
    return 5;
}

short ListaBadan::addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2, const QString &C3, const QString &C4, const QString &C5,
            const QString & C6) {
    addR0(table, row);
    addR1(table, row, col, C1);
    addR1(table, row, col+1, C2);
    addR1(table, row, col+2, C3);
    addR1(table, row, col+3, C4);
    addR1(table, row, col+4, C5);
    addR1(table, row, col+5, C6);
    return 6;
}

short ListaBadan::addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2, const QString &C3, const QString &C4, const QString &C5,
            const QString & C6, const QString & C7) {
    addR0(table, row);
    addR1(table, row, col, C1);
    addR1(table, row, col+1, C2);
    addR1(table, row, col+2, C3);
    addR1(table, row, col+3, C4);
    addR1(table, row, col+4, C5);
    addR1(table, row, col+5, C6);
    addR1(table, row, col+6, C7);
    return 7;
}

short ListaBadan::addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2, const QString &C3, const QString &C4, const QString &C5,
            const QString & C6, const QString & C7, const QString & C8) {
    addR0(table, row);
    addR1(table, row, col, C1);
    addR1(table, row, col+1, C2);
    addR1(table, row, col+2, C3);
    addR1(table, row, col+3, C4);
    addR1(table, row, col+4, C5);
    addR1(table, row, col+5, C6);
    addR1(table, row, col+6, C7);
    addR1(table, row, col+7, C8);
    return 8;
}

short ListaBadan::addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2, const QString &C3, const QString &C4, const QString &C5,
            const QString & C6, const QString & C7, const QString & C8, const QString & C9) {
    addR0(table, row);
    addR1(table, row, col, C1);
    addR1(table, row, col+1, C2);
    addR1(table, row, col+2, C3);
    addR1(table, row, col+3, C4);
    addR1(table, row, col+4, C5);
    addR1(table, row, col+5, C6);
    addR1(table, row, col+6, C7);
    addR1(table, row, col+7, C8);
    addR1(table, row, col+8, C9);
    return 9;
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

    //tablePrzebieg->setMaximumWidth(245);
    //tablePrzebieg->setMinimumWidth(245);
    tablePrzebieg->adjustSize();
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

void ListaBadan::initSzybkieZmianyTlumieniaTable()
{
    QTableWidget * tablePrzebieg = ui->szybkieZmianyTlumieniaTablePrzebieg;
    tablePrzebieg->clear();
    if (tablePrzebieg->columnCount() < 4)
        tablePrzebieg->setColumnCount(4);

    QTableWidgetItem *cn1 = new QTableWidgetItem(QString::fromUtf8("Wielkość tłumnika [dB]"));
    tablePrzebieg->setHorizontalHeaderItem(0, cn1);
    tablePrzebieg->setColumnWidth(0, 50);

    QTableWidgetItem *cn2 = new QTableWidgetItem(QString::fromUtf8("Wielkość tłumnika [%]"));
    tablePrzebieg->setHorizontalHeaderItem(1, cn2);
    tablePrzebieg->setColumnWidth(1, 50);

    QTableWidgetItem *wynik = new QTableWidgetItem(QString::fromUtf8("Wynik"));
    tablePrzebieg->setHorizontalHeaderItem(2, wynik);
    tablePrzebieg->setColumnWidth(2, 50);

    QTableWidgetItem *itemUwagi = new QTableWidgetItem("Uwagi");
    tablePrzebieg->setHorizontalHeaderItem(3, itemUwagi);
    tablePrzebieg->setColumnWidth(3, 130);

    //tablePrzebieg->setMaximumWidth(245);
    //tablePrzebieg->setMinimumWidth(245);
    tablePrzebieg->adjustSize();
}

void ListaBadan::addSzybkieZmianyTlumieniaRekord(short num, const QString &value_dB, bool ok, const QString &error)
{
    int row = num;
    QTableWidget * tablePrzebieg = ui->szybkieZmianyTlumieniaTablePrzebieg;
    QTableWidgetItem *itemVert = new QTableWidgetItem(QString::number(num+1));
    //ui->tablePrzebieg
    tablePrzebieg->setVerticalHeaderItem(row, itemVert);

    int col = 0;
    QTableWidgetItem *item0 = new QTableWidgetItem(value_dB);
    tablePrzebieg->setItem(row, col++, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem(d2p(value_dB));
    tablePrzebieg->setItem(row, col++, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem(ok ? "POZYTYWNY" : "NEGATYWNY");
    tablePrzebieg->setItem(row, col++, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem(error);
    tablePrzebieg->setItem(row, col++, item3);

    if (!ok) {
        for (short e=0; e<col; ++e) {
            tablePrzebieg->item(row, e)->setBackground(Qt::red);
        }
    }
}

void ListaBadan::initZaleznoscDrogiOptycznejTable()
{
    QTableWidget * tablePrzebieg = ui->zaleznoscDlugisciDrogiOptycznejTablePrzebieg;
    tablePrzebieg->clear();
    if (tablePrzebieg->columnCount() < 5)
        tablePrzebieg->setColumnCount(5);

    QTableWidgetItem *rozs = new QTableWidgetItem(QString::fromUtf8("Rozstawienie [m]"));
    tablePrzebieg->setHorizontalHeaderItem(0, rozs);
    tablePrzebieg->setColumnWidth(0, 150);

    QTableWidgetItem *cn1 = new QTableWidgetItem(QString::fromUtf8("Cn [dB]"));
    tablePrzebieg->setHorizontalHeaderItem(1, cn1);
    tablePrzebieg->setColumnWidth(1, 50);

    QTableWidgetItem *cn2 = new QTableWidgetItem(QString::fromUtf8("Cn [%]"));
    tablePrzebieg->setHorizontalHeaderItem(2, cn2);
    tablePrzebieg->setColumnWidth(2, 50);

    QTableWidgetItem *itemWynik = new QTableWidgetItem("Wynik");
    tablePrzebieg->setHorizontalHeaderItem(3, itemWynik);
    tablePrzebieg->setColumnWidth(3, 130);

    QTableWidgetItem *itemUwagi = new QTableWidgetItem("Uwagi");
    tablePrzebieg->setHorizontalHeaderItem(4, itemUwagi);
    tablePrzebieg->setColumnWidth(4, 130);

    //tablePrzebieg->setMaximumWidth(245);
    //tablePrzebieg->setMinimumWidth(245);
    tablePrzebieg->adjustSize();
}

void ListaBadan::addZaleznoscDrogiOptycznejRekord(short row, const QString &rozstawienie, const QString &value_dB, bool ok, const QString &error)
{
    QTableWidget * tablePrzebieg = ui->zaleznoscDlugisciDrogiOptycznejTablePrzebieg;
    QTableWidgetItem *itemVert = new QTableWidgetItem(QString::number(row+1));
    //ui->tablePrzebieg
    tablePrzebieg->setVerticalHeaderItem(row, itemVert);

    int col = 0;
    QTableWidgetItem *item0 = new QTableWidgetItem(QString("%1 m").arg(rozstawienie));
    tablePrzebieg->setItem(row, col++, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem(value_dB);
    tablePrzebieg->setItem(row, col++, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem(d2p(value_dB));
    tablePrzebieg->setItem(row, col++, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem(ok ? "POZYTYWNY" : "NEGATYWNY");
    tablePrzebieg->setItem(row, col++, item3);

    QTableWidgetItem *item4 = new QTableWidgetItem(error);
    tablePrzebieg->setItem(row, col++, item4);

    if (!ok) {
        for (short e=0; e<col; ++e) {
            tablePrzebieg->item(row, e)->setBackground(Qt::red);
        }
    }
}

void ListaBadan::initRozproszoneSwiatloTable()
{

}

void ListaBadan::addRozproszoneSwiatloRekord(short /*num*/, const QString &/*value_dB*/, bool /*ok*/, const QString &/*error*/)
{

}

void ListaBadan::initTolerancjaZasilaniaTable()
{

}

void ListaBadan::addTolerancjaNapieciaZasilaniaRekord(short /*num*/, const QString &/*napiecie1*/, const QString &/*value_dB*/, bool /*ok*/, const QString &/*error*/)
{

}




