#include "test6stabilizacjaczujki.h"
#include "parametrybadania.h"
#include "ui_test6stabilizacjaczujki.h"
#include <QTimer>
Test6StabilizacjaCzujki::Test6StabilizacjaCzujki(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test6StabilizacjaCzujki),
    timer(this)
{
    elapsedTime = daneBadania.getCzasStabilizacjiCzujki_s();
    ui->setupUi(this);
    ui->testName->setText(daneTestu.getName());
    ui->progressBar->setMaximum(elapsedTime);
    ui->progressBar->setValue(elapsedTime);
    ui->czas->setText(getMM_SS(elapsedTime));
    timer.setInterval(1000);

    connect(&timer, &QTimer::timeout, this, &Test6StabilizacjaCzujki::timeout);
    timer.start();
}

Test6StabilizacjaCzujki::~Test6StabilizacjaCzujki()
{
    timer.stop();
    delete ui;
}

QString Test6StabilizacjaCzujki::getMM_SS(unsigned int secs)
{
    QString ret;
    if (secs < 60) {
        ret = "00:";
        if (secs < 10)
            ret += "0";
        ret += QString::number(secs);
        return ret;
    }

    unsigned int min = secs / 60;

    if (min < 10) {
        ret += "0";
    }
    ret += QString::number(min);
    secs = secs - 60*min;
    if (secs < 10)
        ret += "0";
    ret += QString::number(secs);
    return ret;
}

void Test6StabilizacjaCzujki::timeout()
{
    --elapsedTime;
    if (elapsedTime == 0)
        accept();
    ui->progressBar->setValue(elapsedTime);
    ui->czas->setText(getMM_SS(elapsedTime));
}
