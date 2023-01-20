#include "oknoresetuzasilaniaczujki.h"
#include "ui_oknoresetuzasilaniaczujki.h"
#include "zasilacz.h"
#include <QMessageBox>

OknoResetuZasilaniaCzujki::OknoResetuZasilaniaCzujki(const QString & testName, const QString &podtitle,
                                                     unsigned int maxTime_,
                                                     const ParametryBadania &daneBadania,
                                                     Zasilacz * zas_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoResetuZasilaniaCzujki),
    maxTime(maxTime_),
    actTime(0),
    zas(zas_)
{
    ui->setupUi(this);
    ui->testName->setText(testName);
    if (podtitle.isEmpty()) {
        ui->podTestName->setVisible(false);
    } else {
        ui->podTestName->setText(podtitle);
        ui->podTestName->setVisible(true);
    }

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });

    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny()) {

        ui->rodzajZasilania->setText("Zasilacz");
        ui->lTypCentrali->setEnabled(false);
        ui->typCentrali->setEnabled(false);
        ui->pbCzasResetu->setTextVisible(true);
        ui->widget->setVisible(false);
        ui->frczujkaonoff->setVisible(false);
        ui->frame_next->setVisible(false);
        timer.start();
        zas->setOutput(false);
        ui->pbCzasResetu->setMaximum(maxTime);
        ui->pbCzasResetu->setValue(actTime);
        connect(&timer, &QTimer::timeout, this, &OknoResetuZasilaniaCzujki::timeoutProgress);
        timer.setInterval(1000);
        timer.start();

    } else if (daneBadania.getZasilanieCzujekCentrala()) {
        ui->rodzajZasilania->setText("Centrala sygnalizacji pożarowej");
        ui->typCentrali->setText(daneBadania.getZasilanieCzujekTypCentrali());
        ui->lTypCentrali->setEnabled(true);
        ui->typCentrali->setEnabled(true);
        ui->pbCzasResetu->setTextVisible(false);
        ui->widget->setVisible(true);
        ui->automatyczmyReset->setVisible(true);
    }
}

OknoResetuZasilaniaCzujki::~OknoResetuZasilaniaCzujki()
{
    //zas->setOutput(true);
    timer.stop();
    delete ui;
}

void OknoResetuZasilaniaCzujki::timeoutProgress()
{
    ++actTime;
    if (actTime == maxTime) {
        timer.stop();
        zas->setOutput(true);
        accept();
    }
    ui->pbCzasResetu->setValue(actTime);
}

void OknoResetuZasilaniaCzujki::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}
