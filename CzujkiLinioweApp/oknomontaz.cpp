#include "oknomontaz.h"
#include "ui_oknomontaz.h"

#include <QMessageBox>

/*
 * opticalLen = 0 brak
 * 1 = minimalne
 * 2 = maksymalna
 */
OknoMontaz::OknoMontaz(short opticalLen, const DaneTestu &daneTestu, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoMontaz)
{
    ui->setupUi(this);

    ui->numerNadajnika->setText(daneTestu.getNumerTransmitter());
    ui->numerReceiver->setText(daneTestu.getNumerReceiver());
    ui->testName->setText(daneTestu.getName());

    ui->eTransmitter->setText(QString("%1:").arg(daneTestu.getNazwaTransmitter_a()));
    ui->eReceiver->setText(QString("%1:").arg(daneTestu.getNazwaReceiver_a()));

    if (opticalLen > 0) {
        ui->frame_dlugoscDrogiOptycznej->setVisible(true);
        ui->eminimalnerozstawienie->setVisible(opticalLen == 1);
        ui->emaksymalnerozstawienie->setVisible(opticalLen == 2);
        ui->minimalnerozstawienie->setVisible(opticalLen == 1);
        ui->maksymalnerozstawienie->setVisible(opticalLen == 2);
        ui->minimalnerozstawienie->setText(QString("%1 m").arg(daneTestu.getMinimalneRozstawienie()));
        ui->maksymalnerozstawienie->setText(QString("%1 m").arg(daneTestu.getMaksymalneRozstawienie()));
    } else
        ui->frame_dlugoscDrogiOptycznej->setVisible(false);

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });

}

OknoMontaz::~OknoMontaz()
{
    delete ui;
}

void OknoMontaz::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}
