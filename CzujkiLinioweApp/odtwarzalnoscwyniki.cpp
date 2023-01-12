#include "odtwarzalnoscwyniki.h"
#include "ui_odtwarzalnoscwyniki.h"

#include <QMessageBox>
//#define DEBUG




OdtwarzalnoscWyniki::OdtwarzalnoscWyniki(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OdtwarzalnoscWyniki)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->empty);
}

OdtwarzalnoscWyniki::~OdtwarzalnoscWyniki()
{
    delete ui;
}

void OdtwarzalnoscWyniki::setDaneTest(DaneTestu &daneTestu, const ParametryBadania & badanie)
{
    QString pierwszy, drugi;
    pierwszy = badanie.getTypNadajnika();
    drugi = badanie.getTypOdbiornika();
    headTable(pierwszy, drugi);
    initWynikTable(pierwszy, drugi);

    bool badanieOk = true;
    int cntAvg = daneTestu.getDaneBadanCzujek().size();
    float Cavg = 0;
    
    float Cmin = 100;
    float Cmax = -100;

    for (const auto & dane : daneTestu.getDaneBadanCzujek())
    {
        if (!dane.getOk()) {
            badanieOk = false;
            daneTestu.setErrStr(dane.error);
            continue;
        }
        bool ok;
        double C = dane.val.toDouble(&ok);
        if (!ok) {
            badanieOk = false;
            daneTestu.setErrStr(QString::fromUtf8("Błędna wartość C"));
            continue;
        }
        if (val < 0.4) {
            badanieOk = false;
            daneTestu.setErrStr(QString::fromUtf8("Cn < 0.4"));
        }
        if (val > Cmax)
            Cmax = val;
        if (val < Cmin)
            Cmin = val;
        Cavg += val / cntAvg;
    }

    if ()

    daneTestu.setsetCrep(Cavg);
    daneTestu.setCmin(Cmin);
    daneTestu.setCmax(Cmax);
    daneTestu.setCmaxCrep(Cmax/Cavg);
    daneTestu.setCrepCmin(Cavg/Cmin);
    daneTestu.setWykonany(true);

    if (daneTestu.getCmaxCrep() > 1.33) {
        daneTestu.setOk(false);
        daneTestu.setErrStr("Cmax/Crep>1.33");
    } else if (daneTestu.getCrepCmin() > 1.5) {
        daneTestu.setOk(false);
        daneTestu.setErrStr("Crep/Cmin<1.5");
    } else {
        daneTestu.setOk(badanieOk);
    }

#ifdef DEBUG
        addRekordPodsumowanie(0, 1, "AXA1", "AYA1", "-", "-", false, "Błąd sprzętowy");
        addRekordPodsumowanie(1, 2, "AXA2", "AYA2", "0.2", "34", false, "Crep<0.3");
        addRekordPodsumowanie(2, 3, "AXA3", "AYA3", "0.5", "45", true, "");
        addRekordPodsumowanie(3, 4, "AXA4", "AYA4", "1.5", "45", true, "");
        addRekordPodsumowanie(4, 5, "AXA5", "AYA5", "3.5", "45", true, "");
        addRekordPodsumowanie(5, 6, "AXA6", "AYA6", "2.5", "45", true, "");
        addRekordPodsumowanie(6, 7, "AXA7", "AYA7", "0.6", "45", true, "");

#else
        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            //QString inne;
            addRekordPodsumowanie(num, dane.nrPomiaru, dane.numerNadajnika, dane.numerOdbiornika,
                              dane.value_dB, dane.value_perc, dane.ok, dane.error);
            addRekordWyniki(num, dane.nrPomiaru, dane.numerNadajnika, dane.numerOdbiornika,
                              dane.value_dB, dane.value_perc, dane.ok, dane.error);
            num++;
        }
#endif

        ui->gridLayoutResults->setVerticalSpacing(0);
        ui->gridLayoutResults->setHorizontalSpacing(0);
        ui->gridLayoutResults->setSpacing(0);
        
        ui->etcmaxcrep->setText(QString::number(daneTestu.getCmaxCrep()));
        ui->etcrep/cmin->setText(QString::number(daneTestu.getCmaxCrep()));
}

void OdtwarzalnoscWyniki::setPodsumowanie(bool pods)
{
    if (pods)
        ui->stackedWidget->setCurrentWidget(ui->podsumowanie);
    else
        ui->stackedWidget->setCurrentWidget(ui->wyniki);
}

void OdtwarzalnoscWyniki::addRekordPodsumowanie(short r, short nrProby, const QString & nadajnik, const QString & odbiornik,
                                          const QString &tlumienie_db, const QString &tlumienie_per,
                                          bool ok, const QString &inneText)
{

    short col = 0;
    short row = 2*r+3;

    addLine(QString("line_%1_%2").arg(row).arg(col), true, row, col, 1, 1);++col;
    oneTableTd(QString("label_%1_%2").arg(row).arg(col), QString::number(nrProby), row, col);++col;
    addLine(QString("line_%1_%2").arg(row).arg(col), true, row, col, 1, 1);++col;
    oneTableFrame(QString("frame_%1_%2").arg(row, col), nadajnik, row, col);++col;
    addLine(QString("line_%1_%2").arg(row).arg(col), true, row, col, 1, 1);++col;
    oneTableFrame(QString("frame_%1_%2").arg(row, col), odbiornik, row, col);++col;
    addLine(QString("line_%1_%2").arg(row).arg(col), true, row, col, 1, 1);++col;
    oneTableFrame(QString("frame_%1_%2").arg(row, col), tlumienie_db, row, col);++col;
    addLine(QString("line_%1_%2").arg(row).arg(col), true, row, col, 1, 1);++col;
    oneTableFrame(QString("frame_%1_%2").arg(row, col), tlumienie_per, row, col);++col;
    addLine(QString("line_%1_%2").arg(row).arg(col), true, row, col, 1, 1);++col;
    oneTableFrame(QString("label_%1_%2").arg(row).arg(col), ok ? "POZYTYWNY" : "NEGATYWNY", row, col);++col;
    addLine(QString("line_%1_%2").arg(row).arg(col), true, row, col, 1, 1);++col;
    if (inneText.isEmpty())
        oneTableTd(QString("label_%1_%2").arg(row).arg(col), inneText, row, col);
    else
        oneTableFrame(QString("frame_%1_%2").arg(row).arg(col), inneText, row, col);
    ++col;
    addLine(QString("line_%1_%2").arg(row).arg(col), true, row, col, 1, 1);++col;

    addLine(QString("vertline_%1").arg(row), false, row+1, 0, 1, col);

}

void OdtwarzalnoscWyniki::headTable(const QString & nadajnik, const QString & odbiornik)
{
    short col = 0;

    addLine("lhead0", true, 1, col++, 1, 1);
    oneHeadRecord("etProba", QString::fromUtf8("Próba"), 1, col++);
    addLine("lhead1", true, 1, col++, 1, 1);
    oneHeadRecord("etNadajnik", nadajnik, 1, col++);
    addLine("lhead2", true, 1, col++, 1, 1);
    oneHeadRecord("etOdbiornik", odbiornik, 1, col++);
    addLine("lhead3", true, 1, col++, 1, 1);
    oneHeadRecord("etCndB", "<html><body><b>C<sub>[n]</sub></b> <i>[dB]</i></body></html>", 1, col++);
    addLine("lhead4", true, 1, col++, 1, 1);
    oneHeadRecord("etCnPer", "<html><body><b>C<sub>[n]</sub></b> <i>[%]</i></body></html>", 1, col++);
    addLine("lhead5", true, 1, col++, 1, 1);
    oneHeadRecord("etResult", "Wynik", 1, col++);
    addLine("lhead6", true, 1, col++, 1, 1);
    oneHeadRecord("etUwagi", "Uwagi", 1, col++);
    addLine("lhead7", true, 1, col++, 1, 1);

    addLine("lheadUp",   false, 0, 0, 1, col);
    addLine("lheadDown", false, 2, 0, 1, col);
}

void OdtwarzalnoscWyniki::oneHeadRecord(const QString & objectName, const QString & text, int row, int col)
{
    QLabel * lh = new QLabel(ui->frameTable);
    lh->setObjectName(objectName);
    lh->setText(text);
    lh->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    QFont f(lh->font());
    f.setBold(true);
    lh->setFont(f);
    ui->gridLayoutResults->addWidget(lh, row, col, 1, 1);
}

void OdtwarzalnoscWyniki::oneTableTd(const QString & objectName, const QString & text, int row, int col)
{
    QLabel * l = new QLabel(ui->frameTable);
    l->setObjectName(objectName);
    l->setText(text);
    ui->gridLayoutResults->addWidget(l, row, col, 1, 1);
}

void OdtwarzalnoscWyniki::oneTableFrame(const QString & objectName, const QString & text, int row, int col)
{
    QFrame * frame = new QFrame(ui->frameTable);
    frame->setObjectName(objectName);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    QHBoxLayout * horizontalLayout = new QHBoxLayout(frame);
    horizontalLayout->setSpacing(3);
    horizontalLayout->setObjectName(QString("horizontalLayout_%1").arg(objectName));
    horizontalLayout->setContentsMargins(5, 5, 5, 5);
    QLabel * label = new QLabel(frame);
    label->setObjectName(QString("label_%1").arg(objectName));
    horizontalLayout->addWidget(label);
    label->setText(text);
    ui->gridLayoutResults->addWidget(frame, row, col, 1, 1);
}

void OdtwarzalnoscWyniki::addLine(const QString & objectName, bool vert, int row, int col, int rowspan, int colspan)
{
    QFrame * line = new QFrame(ui->frameTable);
    line->setObjectName(objectName);
    line->setFrameShape(vert ? QFrame::VLine : QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(2);
    ui->gridLayoutResults->addWidget(line, row, col, rowspan, colspan);
}

void OdtwarzalnoscWyniki::initWynikTable(const QString &nadajnik, const QString &odbiornik)
{
    if (ui->tablePrzebieg->columnCount() < 4)
        ui->tablePrzebieg->setColumnCount(4);

    QTableWidgetItem *itemNadajnik = new QTableWidgetItem(nadajnik);
    ui->tablePrzebieg->setHorizontalHeaderItem(0, itemId);
    ui->tablePrzebieg->setColumnWidth(0, 130);

    QTableWidgetItem *itemOdbiornik = new QTableWidgetItem(odbiornik);
    ui->tablePrzebieg->setHorizontalHeaderItem(1, itemOdbiornik);
    ui->tablePrzebieg->setColumnWidth(1, 130);

    QTableWidgetItem *cn1 = new QTableWidgetItem(QString::fromUtf8("C[n]"));
    ui->tablePrzebieg->setHorizontalHeaderItem(2, cn1);
    ui->tablePrzebieg->setColumnWidth(2, 50);

    QTableWidgetItem *cn2 = new QTableWidgetItem(QString::fromUtf8("C[n]"));
    ui->tablePrzebieg->setHorizontalHeaderItem(3, cn2);
    ui->tablePrzebieg->setColumnWidth(3, 50);

    /*
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
*/

}

void OdtwarzalnoscWyniki::addRekordWyniki(short num, short nrPomiaru, const QString & numerNadajnika, const QString & numerOdbiornika,
                              const QString & value_dB, const QString & value_perc, bool ok, const QString & error)
{
    int row = num;

    QTableWidgetItem *itemVert = new QTableWidgetItem(QString::number(num+1));
    ui->tableWidget->setVerticalHeaderItem(row, itemVert);
    
    //QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(num));
    //ui->tableWidget->setItem(row, 0, item0);
    (void)ok;
    (void)err;

    QTableWidgetItem *item1 = new QTableWidgetItem(numerNadajnika);
    ui->tableWidget->setItem(row, 0, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem(numerOdbiornika);
    ui->tableWidget->setItem(row, 1, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem(value_dB);
    ui->tableWidget->setItem(row, 2, item3);

    QTableWidgetItem *item4 = new QTableWidgetItem(value_perc);
    ui->tableWidget->setItem(row, 3, item4);

    //QTableWidgetItem *item5 = new QTableWidgetItem(ok ? "POZYTYWNY" : "NEGATYWNY");
    //ui->tableWidget->setItem(row, 4, item5);

    //QTableWidgetItem *item6 = new QTableWidgetItem(error);
    //ui->tableWidget->setItem(row, 5, item6);


}