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
        bool ok;
        double C = dane.value_dB.toDouble(&ok);

        if (!ok) {
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
    }

    daneTestu.setCrep(Cavg);
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
}

