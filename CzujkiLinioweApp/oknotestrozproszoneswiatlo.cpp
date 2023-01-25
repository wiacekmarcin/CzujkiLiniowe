#include "oknotestrozproszoneswiatlo.h"
#include "danetestu.h"
#include "ui_oknotestrozproszoneswiatlo.h"
#include <QTime>

//Nazwa testu [Ten ekran nie pojawi się dla badań odwarzalności, powatarzalności, katowych , zasilania
#include "danetestu.h"


#include <QMessageBox>

OknoTestRozproszoneSwiatlo::OknoTestRozproszoneSwiatlo(const DaneTestu & daneTestu,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoTestRozproszoneSwiatlo)
{
    ui->setupUi(this);
    ui->testName->setText(daneTestu.getName());
    QTime dt = QTime::currentTime();
    ui->time1->setText(dt.addSecs(55*60).toString("HH:mm"));
    ui->time2->setText(dt.addSecs(3600).toString("HH:mm"));
    ui->time3->setText(dt.addSecs(3600+800).toString("HH:mm"));
    ui->time3->setText(dt.addSecs(3600+800).toString("HH:mm"));
    ui->time4->setText(dt.addSecs(7200+3600+800).toString("HH:mm"));

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->done(QDialog::Accepted); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
}

OknoTestRozproszoneSwiatlo::~OknoTestRozproszoneSwiatlo()
{
    delete ui;
}

void OknoTestRozproszoneSwiatlo::czujkaOn()
{
    ui->cbAlarmYesNo->setCurrentIndex(1);
    alarm = true;
}

void OknoTestRozproszoneSwiatlo::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        done(QDialog::Rejected);
}

bool OknoTestRozproszoneSwiatlo::getAlarm() const
{
    return alarm;
}
