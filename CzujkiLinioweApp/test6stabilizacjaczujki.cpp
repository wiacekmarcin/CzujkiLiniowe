#include "test6stabilizacjaczujki.h"
#include "ui_test6stabilizacjaczujki.h"
#include <QTimer>
#include <QDebug>

Test6StabilizacjaCzujki::Test6StabilizacjaCzujki(unsigned long timeWait, const QString & name, bool stabilizacja, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test6StabilizacjaCzujki),
    timer(this)
{
    qDebug() << "Wait window" << timeWait;
    elapsedTime = timeWait;
    ui->setupUi(this);
    ui->testName->setText(name);
    ui->progressBar->setMaximum(elapsedTime);
    ui->progressBar->setValue(elapsedTime);
    ui->czas->setText(getMM_SS(elapsedTime));
    timer.setInterval(1000);

#ifdef DEFVAL
        ui->pbBreak->setVisible(true);
        connect(ui->pbBreak, &QPushButton::clicked, this, [this](){ accept(); });
#else
        ui->pbBreak->setVisible(false);
#endif

    if (stabilizacja) {
        setWindowTitle(QString::fromUtf8("Parametry Testu - oczekiwanie na stabilizację czujki"));
        ui->head->setText(QString::fromUtf8("Parametry Testu - oczekiwanie na stabilizację czujki"));
        ui->lczas->setText("Pozostały czas stabilizacji czujki");
    } else {
        setWindowTitle(QString::fromUtf8("Parametry Testu - oczekiwanie na kolejną próbę"));
        ui->head->setText(QString::fromUtf8("Parametry Testu - oczekiwanie na kolejną próbę"));
        ui->lczas->setText("Pozostały czas bezczynności czujki");
    }
    connect(&timer, &QTimer::timeout, this, &Test6StabilizacjaCzujki::timeout);
    timer.start();
}

Test6StabilizacjaCzujki::~Test6StabilizacjaCzujki()
{
    timer.stop();
    delete ui;
}

QString Test6StabilizacjaCzujki::getMM_SS(unsigned long secs)
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
    if (min >= 60)
    {
        unsigned int hour = min / 60;
        ret = QString::number(hour);
        ret += ":";
        min = min - hour*60;
        secs = secs - hour*3600 - 60*min;
    } else {
        secs = secs - 60*min;
    }

    if (min < 10) {
        ret += "0";
    }
    ret += QString::number(min);
    ret += ":";

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
