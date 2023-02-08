#include "oknoczekaniabadaniekatowe.h"
#include "ui_oknoczekaniabadaniekatowe.h"
#include <QTimer>
#include <QDebug>
#include "ustawienia.h"

OknoCzekaniaBadanieKatowe::OknoCzekaniaBadanieKatowe(unsigned long timeWait, const QString & name,
                                                     const QString & ptitle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoCzekaniaBadanieKatowe),
    timer(this)
{
    //qDebug() << "Wait window" << timeWait;
    elapsedTime = timeWait;
    ui->setupUi(this);
    ui->testName->setText(name);
    if (ptitle.isEmpty()) {
        ui->podTestName->setVisible(false);
    } else {
        ui->podTestName->setText(ptitle);
        ui->podTestName->setVisible(true);
    }
    ui->progressBar->setMaximum(elapsedTime);
    ui->progressBar->setValue(elapsedTime);
    ui->czas->setText(getMM_SS(elapsedTime));
    timer.setInterval(1000);

    if (Ustawienia::testMode) {
        ui->pbBreak->setVisible(true);
        connect(ui->pbBreak, &QPushButton::clicked, this, [this](){ done(QDialog::Accepted); });
    } else {
        ui->pbBreak->setVisible(false);
    }

    setWindowTitle(QString::fromUtf8("Badanie kątowe - postój czujki w skrajnym połozeniu"));
    ui->head->setText(QString::fromUtf8("Badanie kątowe - postój czujki położeniu podanym przez producenta"));
    ui->lczas->setText("Pozostały czas oczekiwania");

    connect(&timer, &QTimer::timeout, this, &OknoCzekaniaBadanieKatowe::timeout);
    timer.start();
    adjustSize();
}

OknoCzekaniaBadanieKatowe::~OknoCzekaniaBadanieKatowe()
{
    timer.stop();
    delete ui;
}

void OknoCzekaniaBadanieKatowe::czujkaOn()
{
    done(QDialog::Rejected);
}

QString OknoCzekaniaBadanieKatowe::getMM_SS(unsigned long secs)
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

void OknoCzekaniaBadanieKatowe::timeout()
{
    --elapsedTime;
    if (elapsedTime == 0)
        done(QDialog::Accepted);
    ui->progressBar->setValue(elapsedTime);
    ui->czas->setText(getMM_SS(elapsedTime));
}
