#include "oknotestrozproszoneswiatlo.h"
#include "danetestu.h"
#include "ui_oknotestrozproszoneswiatlo.h"
#include <QTime>
#include <QDateTime>
#include <QMessageBox>
#include <QAbstractButton>

#include "danetestu.h"


#include <QMessageBox>

OknoTestRozproszoneSwiatlo::OknoTestRozproszoneSwiatlo(const DaneTestu & daneTestu,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoTestRozproszoneSwiatlo)
{
    ui->setupUi(this);
    ui->testName->setText(daneTestu.getName());
    ui->cbAlarmYesNo->setCurrentIndex(0);
    alarm = false;

    QTime dt = QTime::currentTime();
    secsFromEpocsStart = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();

    ui->time0->setText(dt.toString("HH:mm"));
    ui->time1->setText(dt.addSecs(55*60).toString("HH:mm"));
    ui->time2->setText(dt.addSecs(3600).toString("HH:mm"));
    ui->time3->setText(dt.addSecs(3600+800).toString("HH:mm"));
    ui->time3->setText(dt.addSecs(3600+800).toString("HH:mm"));
    ui->time4->setText(dt.addSecs(7200+3600+800).toString("HH:mm"));

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->pbNext_clicked(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    connect(ui->cbAlarmYesNo, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int index) { this->alarm = index == 1; });
    adjustSize();

#ifdef TESTVAL
    ui->pbCzujkaOn->setVisible(true);
    connect(ui->pbCzujkaOn, &QPushButton::clicked, this, [this](){ this->czujkaOn(); });
#else
    ui->pbCzujkaOn->setVisible(false);
#endif
}

OknoTestRozproszoneSwiatlo::~OknoTestRozproszoneSwiatlo()
{
    delete ui;
}

void OknoTestRozproszoneSwiatlo::czujkaOn()
{
    ui->cbAlarmYesNo->setCurrentIndex(1);
    alarm = true;
    unsigned long delta = QDateTime::currentDateTimeUtc().toSecsSinceEpoch() - secsFromEpocsStart;
    if ( delta < 3600) {
        ui->errorInfo->setText(QString::fromUtf8("Czujka wyzwoliła się podczas oczekiwania na narażenie"));
    } else if ( delta < 3600+800) {
        ui->errorInfo->setText(QString::fromUtf8("Czujka wyzwoliła się podczas narażenia"));
    } else
        ui->errorInfo->setText(QString::fromUtf8("Czujka wyzwoliła się podczas oczekiwania po narażeniu"));
}

static int question(QWidget * parent, const QString & title, const QString & pytanie) {
    QMessageBox messageBox(QMessageBox::Question, title, pytanie, 
                        QMessageBox::Yes | QMessageBox::No, parent);

    messageBox.setButtonText(QMessageBox::Yes, QString::fromUtf8("Tak"));
    messageBox.setButtonText(QMessageBox::No, QString::fromUtf8("Nie"));
    return messageBox.exec();
}


void OknoTestRozproszoneSwiatlo::pbCancel_clicked()
{
    int ret = question(this, QString::fromUtf8("Badanie : %1").arg(ui->testName->text()),
                                    QString::fromUtf8("Czy napewno chcesz przerwać badanie"));
    if (ret == QMessageBox::Yes)
        done(QDialog::Rejected);
}

void OknoTestRozproszoneSwiatlo::pbNext_clicked()
{
    unsigned long delta = QDateTime::currentDateTimeUtc().toSecsSinceEpoch() - secsFromEpocsStart;
    unsigned long maxCzas = 7200+3600+800;
    if ( delta < maxCzas ) {

        QTime t(0,0,0);
        t = t.addSecs(maxCzas - delta);
        QString test = QString::fromUtf8("Test według normy powinien trwać jeszcze około ");

        if (t.hour() > 0) {
            short h = t.hour();
            test += QString::fromUtf8("%1 godzin%2 i ").arg(h).arg(h == 1 ? "ę" : "y");
            t = t.addSecs(-h*3600);
        }
        short m = t.minute();
        if (m == 0 && t.hour() == 0)
            test = QString::fromUtf8("Test według normy powinien trwać jeszcze około 1 minuty.");
        else if (m == 1)
            test += QString::fromUtf8("1 minutę");
        else if (m < 5)
            test += QString::fromUtf8("%1 minuty.").arg(m);
        else
            test += QString::fromUtf8("%1 minut.").arg(m);
        test +=  QString::fromUtf8(" Czy chcesz kontynuować ?");

        QMessageBox messageBox(QMessageBox::Information,
                                QString::fromUtf8("Rozproszone światło - zakończenie testu"),
                                test, 
                                QMessageBox::Yes | QMessageBox::No, this);

        messageBox.setButtonText(QMessageBox::Yes, QString::fromUtf8("Tak"));
        messageBox.setButtonText(QMessageBox::No, QString::fromUtf8("Nie"));

        if (messageBox.exec() == QMessageBox::No)
            return;
    }
    info = ui->errorInfo->toPlainText();
    done(QDialog::Accepted);
}

QString OknoTestRozproszoneSwiatlo::getInfo() const
{
    if (info.isEmpty() && alarm) {
        return QString::fromUtf8("Czujka zgłosiła alarm podczas narażania");
    }
    return info;
}

bool OknoTestRozproszoneSwiatlo::getAlarm() const
{
    return alarm;
}
