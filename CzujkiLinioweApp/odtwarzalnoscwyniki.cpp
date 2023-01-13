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

void OdtwarzalnoscWyniki::setDaneTest(bool podsumowanie, DaneTestu &daneTestu, const ParametryBadania & badanie)
{
    QString pierwszy, drugi;
    pierwszy = daneTestu.getNazwaPierwszego();
    drugi = daneTestu.getNazwaDrugiego();

    if (podsumowanie) {
        headTable(pierwszy, drugi);
        ui->stackedWidget->setCurrentWidget(ui->podsumowanie);
        ui->result->setText(daneTestu.getOk() ? "POZYTYWNY" : "NEGATYWNY");
        ui->result->setText(daneTestu.getOk() ? "POZYTYWNY" : "NEGATYWNY");

        ui->Crep->setText(QString::number(daneTestu.getCrep(), 'g', 2) + " dB");
        ui->Crep2->setText(QString::number(daneTestu.getCrep2(), 'g', 2) + " %");
        ui->cmin->setText(QString::number(daneTestu.getCmin(), 'g', 2) + " dB");
        ui->cmin2->setText(QString::number(daneTestu.getCmin2(), 'g', 2) + " %");
        ui->cmax->setText(QString::number(daneTestu.getCmax(), 'g', 2) + " dB");
        ui->cmax2->setText(QString::number(daneTestu.getCmax(), 'g', 2) + " %");
        ui->CmaxCrep->setText(QString::number(daneTestu.getCmaxCrep(), 'g', 2));
        ui->CrepCmin->setText(QString::number(daneTestu.getCrepCmin(), 'g', 2));

        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            addRekordPodsumowanie(num, dane.nrPomiaru, dane.numerNadajnika, dane.numerOdbiornika,
                              dane.value_dB, dane.value_perc, dane.ok, dane.error);
            num++;
        }
        ui->gridLayoutResults->setVerticalSpacing(0);
        ui->gridLayoutResults->setHorizontalSpacing(0);
        ui->gridLayoutResults->setSpacing(0);

    } else {
        initWynikTable(pierwszy, drugi);
        ui->stackedWidget->setCurrentWidget(ui->podsumowanie);
        ui->tableParams->item(0,0)->setText(QString::number(daneTestu.getCmin(), 'g', 2) + " dB");
        ui->tableParams->item(0,1)->setText(QString::number(daneTestu.getCmin2(), 'g', 2) + " %");
        ui->tableParams->item(1,0)->setText(QString::number(daneTestu.getCmax(), 'g', 2) + " dB");
        ui->tableParams->item(1,1)->setText(QString::number(daneTestu.getCmax2(), 'g', 2) + " %");
        ui->tableParams->item(2,0)->setText(QString::number(daneTestu.getCrep(), 'g', 2) + " dB");
        ui->tableParams->item(2,1)->setText(QString::number(daneTestu.getCrep2(), 'g', 2) + " %");
        ui->tableParams->item(3,0)->setText(QString::number(daneTestu.getCmaxCrep(), 'g', 2) + " dB");
        ui->tableParams->item(4,0)->setText(QString::number(daneTestu.getCrepCmin(), 'g', 2) + " dB");

        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            addRekordWyniki(num, dane.nrPomiaru, dane.numerNadajnika, dane.numerOdbiornika,
                              dane.value_dB, dane.value_perc, dane.ok, dane.error);
            num++;
        }
    }
}

void OdtwarzalnoscWyniki::breakTest()
{
    ui->stackedWidget->setCurrentWidget(ui->empty);
}

void OdtwarzalnoscWyniki::valueTest(DaneTestu &daneTestu)
{
    bool badanieOk = true;
    int cntAvg = daneTestu.getDaneBadanCzujek().size();
    float Cavg = 0;
    float C2avg = 0;

    float Cmin = 100;
    float Cmax = -100;
    float C2min = 100;
    float C2max = -100;

    for (const auto & dane : daneTestu.getDaneBadanCzujek())
    {
        if (!dane.ok) {
            badanieOk = false;
            daneTestu.setErrStr(dane.error);
            continue;
        }
        bool ok, ok2;
        double C = dane.value_dB.toDouble(&ok);
        double C2 = dane.value_perc.toDouble(&ok2);
        if (!ok || !ok2) {
            badanieOk = false;
            daneTestu.setErrStr(QString::fromUtf8("Błędna wartość C"));
            continue;
        }
        if (C < 0.4) {
            badanieOk = false;
            daneTestu.setErrStr(QString::fromUtf8("Cn < 0.4"));
        }
        if (C > Cmax)
            Cmax = C;
        if (C < Cmin)
            Cmin = C;
        Cavg += C / cntAvg;

        if (C2 > C2max)
            C2max = C2;
        if (C2 < C2min)
            C2min = C2;
        C2avg += C2 / cntAvg;
    }

    daneTestu.setCrep(Cavg);
    daneTestu.setCmin(Cmin);
    daneTestu.setCmax(Cmax);
    daneTestu.setCrep2(C2avg);
    daneTestu.setCmin2(C2min);
    daneTestu.setCmax2(C2max);
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
    ui->tablePrzebieg->setHorizontalHeaderItem(0, itemNadajnik);
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


}

void OdtwarzalnoscWyniki::addRekordWyniki(short num, short nrPomiaru, const QString & numerNadajnika, const QString & numerOdbiornika,
                              const QString & value_dB, const QString & value_perc, bool ok, const QString & error)
{
    int row = num;

    QTableWidgetItem *itemVert = new QTableWidgetItem(QString::number(num+1));
    //ui->tablePrzebieg
    ui->tablePrzebieg->setVerticalHeaderItem(row, itemVert);

    //QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(num));
    //ui->tableWidget->setItem(row, 0, item0);
    (void)ok;
    (void)error;

    QTableWidgetItem *item1 = new QTableWidgetItem(numerNadajnika);
    ui->tablePrzebieg->setItem(row, 0, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem(numerOdbiornika);
    ui->tablePrzebieg->setItem(row, 1, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem(value_dB);
    ui->tablePrzebieg->setItem(row, 2, item3);

    QTableWidgetItem *item4 = new QTableWidgetItem(value_perc);
    ui->tablePrzebieg->setItem(row, 3, item4);

    //QTableWidgetItem *item5 = new QTableWidgetItem(ok ? "POZYTYWNY" : "NEGATYWNY");
    //ui->tablePrzebieg->setItem(row, 4, item5);

    //QTableWidgetItem *item6 = new QTableWidgetItem(error);
    //ui->tablePrzebieg->setItem(row, 5, item6);


}
