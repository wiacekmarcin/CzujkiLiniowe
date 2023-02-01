#include "listabadan.h"
#include "ui_listabadan.h"
#include "zasilacz.h"
#include <math.h>

#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QScreen>

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

    testyWidget[DRY_HEAT] = testWidget{ui->pSucheCieplo, ui->sucheCieploWyniki,
                                                ui->pbSucheCieplo};

    testyWidget[COLD] = testWidget{ui->pZimno, ui->zimnoWyniki,
                                                ui->pbZimno};

    testyWidget[DAMP_HEAT_STADY_STATE_OPERATIONAL] = testWidget{ui->pWilgotneCieploUstalone, ui->wilgotneCieploUstaloneWyniki,
                                                ui->pbWilgotneCieploUstalone};

    testyWidget[DAMP_HEAT_STADY_STATE_ENDURANCE] = testWidget{ui->pWilgotneCieploStanStacjonarny,
                                                 ui->wilgotneCieploStanStacjonarnyWyniki,
                                                 ui->pbWilgotneCieploStanStacjonarny};


    testyWidget[VIBRATION] = testWidget{ui->pWibracja, ui->wibracjaWyniki,
                                                ui->pbWibracja};

    testyWidget[IMPACT] = testWidget{ui->pUderzenia, ui->uderzeniaWyniki,
                                                ui->pbUderzenia};

    testyWidget[SULPHUR_DIOXIDE_SO2_CORROSION] = testWidget{ui->pKorozjaS02, ui->korozjaS02Wyniki,
                                                    ui->pbKorozjaS02};
    
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

bool ListaBadan::startBadanie(short id, ParametryBadania & badanie, const Ustawienia & ust,
                              Zasilacz *zas, Sterownik * ster)
{

    QString nameTest = "Nieznane Badanie";
    if (id == -1)
        return false;
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
        if (id != TEST_MEASUREAMENT) {
            testyWidget[id].wyniki->setVisible(false);
            testyWidget[id].button->setVisible(true);
        }
        badanieWTrakcie = false;
        return false;
    }

    DaneTestu test = procedura.getDane();
    test.setDataZakonczenia(QDate::currentDate().toString("yyyy-MM-dd") + QString(" ") + QTime::currentTime().toString("HH:mm"));
    test.setWykonany(true);
    badanie.setDaneTestu(id, test);
    setUkonczoneBadanie(test.getId(), badanie);
    ui->stackedWidget->setCurrentWidget(testyWidget[test.getId()].page);

    return true;
}

void ListaBadan::clearBadanie()
{
    ui->tableWidget->clear();
    for (auto & it : testyWidget) {
        it.button->setVisible(true);
        it.wyniki->setVisible(false);
    }
    badanieWTrakcie = true;

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
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
    ui->tableWidget->setColumnWidth(0, 700);

    QTableWidgetItem *itemPerson = new QTableWidgetItem(QString::fromUtf8("Osoba wykonująca test"));
    ui->tableWidget->setHorizontalHeaderItem(2, itemPerson);
    ui->tableWidget->setColumnWidth(0, 250);

    QTableWidgetItem *itemStatus = new QTableWidgetItem(QString::fromUtf8("Wynik"));
    ui->tableWidget->setHorizontalHeaderItem(3, itemStatus);
    ui->tableWidget->setColumnWidth(0, 100);

    QTableWidgetItem *itemBegin = new QTableWidgetItem(QString::fromUtf8("Rozpoczęto"));
    ui->tableWidget->setHorizontalHeaderItem(4, itemBegin);
    ui->tableWidget->setColumnWidth(0, 100);

    QTableWidgetItem *itemEnd = new QTableWidgetItem(QString::fromUtf8("Zakończono"));
    ui->tableWidget->setHorizontalHeaderItem(5, itemEnd);
    ui->tableWidget->setColumnWidth(0, 100);

    QTableWidgetItem *itemTemp = new QTableWidgetItem(QString::fromUtf8("Temp."));
    ui->tableWidget->setHorizontalHeaderItem(6, itemTemp);
    ui->tableWidget->setColumnWidth(0, 75);

    QTableWidgetItem *itemWilg = new QTableWidgetItem(QString::fromUtf8("Wilgotn."));
    ui->tableWidget->setHorizontalHeaderItem(7, itemWilg);
    ui->tableWidget->setColumnWidth(0, 75);

    QTableWidgetItem *itemCisn = new QTableWidgetItem(QString::fromUtf8("Ciśnienie"));
    ui->tableWidget->setHorizontalHeaderItem(8, itemCisn);
    ui->tableWidget->setColumnWidth(0, 75);

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
{
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
    double l = std::pow(10, val/10);
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
        QTableWidget * tablePrzebieg = ui->odtwarzalnoscTablePrzebieg;
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

        QStringList head;
        QList<int> width;
        head << "Kolej. pomiar." << "Nr czujki"  << "Nr uporząd." << transmitter << receiver << "C[n] dB" << "C[n] %" << "Uwagi" ;
        width << 90 << 90 << 90 << 150 << 150 << 50 << 50 << 200;
        clearinitTable(tablePrzebieg, head, width);

        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            int col = addR(tablePrzebieg, num, 0,
                           QString::number(num+1),
                           QString::number(dane.nrCzujki),
                           (dane.nrSortCzujki == 0 ? QString("-") : QString::number(dane.nrSortCzujki)),
                           dane.numerNadajnika,
                           dane.numerOdbiornika,
                           dane.value_dB,
                           d2p(dane.value_dB),
                           dane.error
                           );

            if (!dane.ok) {
                for (short e=0; e<col; ++e) {
                    if (tablePrzebieg->item(num, e))
                        tablePrzebieg->item(num, e)->setBackground(Qt::red);
                }
            }
            ++num;
        }
    } else if (daneTestu.getId() == REPEATABILITY) {
        QTableWidget * tableParams = ui->powtarzalnoscTableParams;
        QTableWidget * tablePrzebieg = ui->powtarzalnoscTablePrzebieg;
        tableParams->clear();
        addC(tableParams, "Cmin", QString::number(daneTestu.getCmin(), 'f', 2) + " dB", QString::number(d2p(daneTestu.getCmin()), 'f', 2) + " %", 0);
        addC(tableParams, "Cmax", QString::number(daneTestu.getCmax(), 'f', 2) + " dB", QString::number(d2p(daneTestu.getCmax()), 'f', 2) + " %", 1);
        addC(tableParams, "Cmax/Cmin", QString::number(daneTestu.getCmaxCmin(), 'f', 2), 2);

        if (daneTestu.getCmaxCmin() > badanie.getPowtarzalnoscCmaxCmin()) {
            tableParams->item(2, 0)->setBackground(Qt::red);
        }

        initCzujkaInfo(ui->powarzalnosctableCzujka,                        transmitter, receiver,
                       badanie.getNumerSortedCzujki(daneTestu.getNumerTransmitter(),
                                               daneTestu.getNumerReceiver()),

                       daneTestu.getNumerTransmitter(),
                       daneTestu.getNumerReceiver());

        QStringList head;
        QList<int> width;
        head << "Nr Próby" << "C[n] dB" << "C[n] %" << "Uwagi";
        width << 75 << 50 << 50 << 200;
        clearinitTable(tablePrzebieg, head, width);

        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            int col = addR(tablePrzebieg, num, 0,
                           QString::number(num+1),
                           dane.value_dB, d2p(dane.value_dB), dane.error);
            if (!dane.ok) {
                for (short e=0; e<col; ++e) {
                    if (tablePrzebieg->item(num, e))
                        tablePrzebieg->item(num, e)->setBackground(Qt::red);
                }
            }
            num++;
        }

        if (daneTestu.getOk()) {
            ui->powtarzalnoscResult->setText("POZYTYWNY");
        } else {
            ui->powtarzalnoscResult->setText(QString("NEGATYWNY - %1").arg(daneTestu.getErrStr()));
        }
    } else if (daneTestu.getId() == TOLERANCE_TO_BEAM_MISALIGNMENT) {
        QTableWidget * tablePrzebieg = ui->zaleznoscKierunkowaTablePrzebieg;


        initCzujkaInfo(ui->zaleznoscKierunkowatableCzujka,
                       transmitter, receiver,
                       badanie.getNumerSortedCzujki(daneTestu.getNumerTransmitter(),
                                               daneTestu.getNumerReceiver()),
                       daneTestu.getNumerTransmitter(),
                       daneTestu.getNumerReceiver());


       QString err;
       qDebug() << __FILE__ << __LINE__ << daneTestu.getPomiaryKatow().size();
       QStringList head;
       QList<int> width;
       head << "Nazwa Badania" << "Kąt prod." << "Kąt zmierz." << "Wynik" << "Błąd" << "Uwaga";
       width << 200 << 100 << 100 << 100 << 300 << 500;
       clearinitTable(tablePrzebieg, head, width);

       int num=0;
       for (const auto & wynik : daneTestu.getPomiaryKatow()) {
           int col = addR(tablePrzebieg, num, 0, wynik.nazwaBadania,
                          QString::number(wynik.katProducenta, 'f', 2),
                          QString::number(wynik.katZmierzony, 'f', 2),
                          (wynik.ok ? "POZYTYWNY" : "NEGATYWNY"),
                          wynik.errorStr,
                          wynik.errorDetail);
           if (!wynik.ok) {
               for (short e=0; e<col; ++e) {
                   if (tablePrzebieg->item(num, e))
                       tablePrzebieg->item(num, e)->setBackground(Qt::red);
               }
           }
           num++;
       }

       if (daneTestu.getOk()) {
           ui->zaleznoscKierunkowaResult->setText("POZYTYWNY");
       } else {
           ui->zaleznoscKierunkowaResult->setText(QString("NEGATYWNY - %1").arg(daneTestu.getErrStr()));
       }
    } else if (daneTestu.getId() == RAPID_CHANGES_IN_ATTENUATION) {
        QTableWidget * tablePrzebieg = ui->szybkieZmianyTlumieniaTablePrzebieg;


        initCzujkaInfo(ui->szybkieZmianyTlumieniatableCzujka,
                       transmitter, receiver,
                       badanie.getNumerSortedCzujki(daneTestu.getNumerTransmitter(),
                                               daneTestu.getNumerReceiver()),
                       daneTestu.getNumerTransmitter(),
                       daneTestu.getNumerReceiver());

        QStringList head;
        QList<int> width;
        head << "Nr próby" << "Wartość tłumnienia [dB]" << "Wartość tłumienia [%]" << "Wynik" << "Uwagi";
        width << 75 << 150 << 150 << 100 << 300;
        clearinitTable(tablePrzebieg, head, width);

        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            int col = addR(tablePrzebieg, num, 0, QString::number(num+1),
                           dane.value_dB, d2p(dane.value_dB), (dane.ok ? "POZYTYWNY" : "NEGATYWNY"),  dane.error);
            if (!dane.ok) {
                for (short e=0; e<col; ++e) {
                    if (tablePrzebieg->item(num, e))
                        tablePrzebieg->item(num, e)->setBackground(Qt::red);
                }
            }
            num++;
        }

        if (daneTestu.getOk()) {
            ui->szybkieZmianyTlumieniaResult->setText("POZYTYWNY");
        } else {
            ui->szybkieZmianyTlumieniaResult->setText(QString("NEGATYWNY - %1").arg(daneTestu.getErrStr()));
        }
    } else if (daneTestu.getId() == OPTICAL_PATH_LENGTH_DEPEDENCE) {
        QTableWidget * tableParams = ui->zaleznoscDlugisciDrogiOptycznejTableParams;
        QTableWidget * tablePrzebieg = ui->zaleznoscDlugisciDrogiOptycznejTablePrzebieg;
        tableParams->clear();
        addC(tableParams, "Cmin", QString::number(daneTestu.getCmin(), 'f', 2) + " dB", QString::number(d2p(daneTestu.getCmin()), 'f', 2) + " %", 0);
        addC(tableParams, "Cmax", QString::number(daneTestu.getCmax(), 'f', 2) + " dB", QString::number(d2p(daneTestu.getCmax()), 'f', 2) + " %", 1);
        addC(tableParams, "Cmax/Cmin", QString::number(daneTestu.getCmaxCmin(), 'f', 2), 2);

        if (daneTestu.getCmaxCmin() > badanie.getDrogaoptycznaCmaxCmin()) {
            tableParams->item(2, 0)->setBackground(Qt::red);
        }

        initCzujkaInfo(ui->zaleznoscDlugisciDrogiOptycznejtableCzujka,
                       transmitter, receiver,
                       badanie.getNumerSortedCzujki(daneTestu.getNumerTransmitter(),
                                               daneTestu.getNumerReceiver()),
                       daneTestu.getNumerTransmitter(),
                       daneTestu.getNumerReceiver());

        QStringList head;
        QList<int> width;
        head << "Nr Próby" << "Rozstawienie [m]" << "C[n] dB" << "C[n] %" << "Wynik" << "Uwagi";
        width << 75 << 100 << 50 << 50 << 100 << 200;
        clearinitTable(tablePrzebieg, head, width);

        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            int col = addR(tablePrzebieg, num, 0, QString::number(num+1),
                           (num == 0 ? daneTestu.getMinimalneRozstawienie() : daneTestu.getMaksymalneRozstawienie()),
                           dane.value_dB, d2p(dane.value_dB), (dane.ok ? "POZYTYWNY" : "NEGATYWNY"), dane.error);
            if (!dane.ok) {
                for (short e=0; e<col; ++e) {
                    if (tablePrzebieg->item(num, e))
                        tablePrzebieg->item(num, e)->setBackground(Qt::red);
                }
            }
            num++;
        }
        if (daneTestu.getOk()) {
            ui->zaleznoscDlugisciDrogiOptycznejResult->setText("POZYTYWNY");
        } else {
            ui->zaleznoscDlugisciDrogiOptycznejResult->setText(QString("NEGATYWNY - %1").arg(daneTestu.getErrStr()));
        }
    } else if (daneTestu.getId() == STRAY_LIGHT) {
        QTableWidget * tableParams = ui->rozproszoneSwiatloTableParams;
        QTableWidget * tablePrzebieg = ui->rozproszoneSwiatloTablePrzebieg;
        QTableWidget * tableNarazenia = ui->rozproszoneSwiatloTableNarazenie;
        tableParams->clear();
        addC(tableParams, "Cmin", QString::number(daneTestu.getCmin(), 'f', 2) + " dB", QString::number(d2p(daneTestu.getCmin()), 'f', 2) + " %", 0);
        addC(tableParams, "Cmax", QString::number(daneTestu.getCmax(), 'f', 2) + " dB", QString::number(d2p(daneTestu.getCmax()), 'f', 2) + " %", 1);
        addC(tableParams, "Cmax/Cmin", QString::number(daneTestu.getCmaxCmin(), 'f', 2), 2);

        if (daneTestu.getCmaxCmin() > badanie.getRozproszoneSwiatloCmaxCmin()) {
            tableParams->item(2, 0)->setBackground(Qt::red);
        }
        initCzujkaInfo(ui->rozproszoneSwiatlotableCzujka,
                       transmitter, receiver,
                       badanie.getNumerSortedCzujki(daneTestu.getNumerTransmitter(),
                                               daneTestu.getNumerReceiver()),
                       daneTestu.getNumerTransmitter(),
                       daneTestu.getNumerReceiver());

        short num = 0;
        QStringList head;
        QList<int> width;
        head << "Stan czujki" << "C[n] dB" << "C[n] %" << "Wynik" <<  "Uwagi";
        width << 150 << 75 << 75 << 120 << 300;
        clearinitTable(tablePrzebieg, head, width);
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            int col = addR(tablePrzebieg, num, 0,
                           (num == 0 ? "Przed narażeniem" : "Po narażeniu"),
                           dane.value_dB, d2p(dane.value_dB), (dane.ok ? "POZYT." : "NEGAT."), dane.error);
            if (!dane.ok) {
                for (short e=0; e<col; ++e) {
                    if (tablePrzebieg->item(num, e))
                        tablePrzebieg->item(num, e)->setBackground(Qt::red);
                }
            }
            num++;
        }
        QStringList headNar;
        QList<int> widthNar;
        headNar << "Wynik narażenia" << "Opis narażenia" << "Uwagi";
        widthNar << 150 << 250 << 350;
        clearinitTable(tableNarazenia, headNar, widthNar);
        tableNarazenia->setRowCount(2);
        num = 0;
        int col = addR(tableNarazenia, 0, 0,
                       daneTestu.getWynikNarazenia() ? "POZYTYWNY" : "NEGATYWNY",
                       QString::fromUtf8("Swiatło rozproszone"), daneTestu.getInfoNarazenia());

        if (!daneTestu.getWynikNarazenia()) {
            for (short e=0; e<col; ++e) {
                if (tableNarazenia->item(num, e))
                    tableNarazenia->item(num, e)->setBackground(Qt::red);
            }
        }

        if (daneTestu.getOk()) {
            ui->rozproszoneSwiatloResult->setText("POZYTYWNY");
        } else {
            ui->rozproszoneSwiatloResult->setText(QString("NEGATYWNY - %1").arg(daneTestu.getErrStr()));
        }


    } if (daneTestu.getId() == TOLERANCE_TO_SUPPLY_VOLTAGE) {
        QTableWidget * tableParams = ui->tolerancjaNapieciaZasilaniaTableParams;
        QTableWidget * tablePrzebieg = ui->tolerancjaNapieciaZasilaniaTablePrzebieg;
        tableParams->clear();
        addC(tableParams, "Cmin", QString::number(daneTestu.getCmin(), 'f', 2) + " dB", QString::number(d2p(daneTestu.getCmin()), 'f', 2) + " %", 0);
        addC(tableParams, "Cmax", QString::number(daneTestu.getCmax(), 'f', 2) + " dB", QString::number(d2p(daneTestu.getCmax()), 'f', 2) + " %", 1);
        addC(tableParams, "Cmax/Cmin", QString::number(daneTestu.getCmaxCmin(), 'f', 2), 2);

        initCzujkaInfo(ui->tolerancjaNapieciaZasilaniatableCzujka,
                       transmitter, receiver,
                       badanie.getNumerSortedCzujki(daneTestu.getNumerTransmitter(),
                                               daneTestu.getNumerReceiver()),
                       daneTestu.getNumerTransmitter(),
                       daneTestu.getNumerReceiver());

        if (daneTestu.getCmaxCmin() > badanie.getTolerancjaNapieciaZasilaniaCmaxCmin()) {
            tableParams->item(2, 0)->setBackground(Qt::red);
        }

        short num = 0;
        QStringList head;
        QList<int> width;
        head << "Napięcie zasilania" << "C[n] dB" << "C[n] %" << "Wynik" <<  "Uwagi";
        width << 150 << 50 << 50 << 100 << 200;
        clearinitTable(tablePrzebieg, head, width);
        tablePrzebieg->setRowCount(daneTestu.getDaneBadanCzujek().size());
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            int col = addR(tablePrzebieg, num, 0,
                           (num == 0 ? daneTestu.getMinimalneNapiecie() : daneTestu.getMaksymalneNapiecie()),
                           dane.value_dB, d2p(dane.value_dB), (dane.ok ? "POZYT." : "NEGAT."), dane.error);
            if (!dane.ok) {
                for (short e=0; e<col; ++e) {
                    if (tablePrzebieg->item(num, e))
                        tablePrzebieg->item(num, e)->setBackground(Qt::red);
                }
            }
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
    unsigned int wAll = 0;
    for (const auto & h : head) {
        QTableWidgetItem *item = new QTableWidgetItem(h);
        table->setHorizontalHeaderItem(col, item);
        if (col < width.size()) {
            table->setColumnWidth(col, width.at(col));
            wAll += width.at(col);
        }
        ++col;
    }
    table->setMinimumWidth(wAll);
    table->setMaximumWidth(wAll + 20);
}

void ListaBadan::initCzujkaInfo(QTableWidget * table, const QString & transmitterName, const QString & receiverName,
                                const QString & nrCzujki, const QString & transmitter, const QString & receiver)
{
    table->clear();
    if (table->columnCount() != 3)
        table->setColumnCount(3);

    QTableWidgetItem *itemh0 = new QTableWidgetItem(QString::fromUtf8("Nr czujki"));
    table->setHorizontalHeaderItem(0, itemh0);
    table->setColumnWidth(0, 75);

    QTableWidgetItem *itemh1 = new QTableWidgetItem(transmitterName);
    table->setHorizontalHeaderItem(1, itemh1);
    table->setColumnWidth(1, 150);

    QTableWidgetItem *itemh2 = new QTableWidgetItem(receiverName);
    table->setHorizontalHeaderItem(2, itemh2);
    table->setColumnWidth(2, 150);

    table->setMinimumWidth(375);
    table->setMaximumWidth(400);


    addR(table, 0, 0, nrCzujki, transmitter, receiver);
}

void ListaBadan::initNarazenieInfo(QTableWidget * table, bool wynik, const QString & rodzaj,
                                const QString & uwagi)
{
    table->clear();
    if (table->columnCount() != 3)
        table->setColumnCount(3);

    QTableWidgetItem *itemh0 = new QTableWidgetItem(QString::fromUtf8("Wynik Narażenia"));
    table->setHorizontalHeaderItem(0, itemh0);
    table->setColumnWidth(0, 125);

    QTableWidgetItem *itemh1 = new QTableWidgetItem("Rodzaj narażenia");
    table->setHorizontalHeaderItem(1, itemh1);
    table->setColumnWidth(1, 250);

    QTableWidgetItem *itemh2 = new QTableWidgetItem("Uwagi");
    table->setHorizontalHeaderItem(2, itemh2);
    table->setColumnWidth(2, 325);

    table->setMinimumWidth(700);
    table->setMaximumWidth(710);


    int col = addR(table, 0, 0, wynik ? "POZYTYWNY" : "NEGATYWNY", rodzaj, uwagi);
    if (!wynik) {
        for (short e=0; e<col; ++e) {
            if (table->item(0, e))
                table->item(0, e)->setBackground(Qt::red);
        }
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




