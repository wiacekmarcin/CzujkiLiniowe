#include "zasilaczwidget.h"
#include "ui_zasilaczwidget.h"

#include <QMessageBox>

ZasilaczWidget::ZasilaczWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZasilaczWidget)
    , zas(this)
    , zasThr(this)
{
    ui->setupUi(this);

    zas.setThread(&zasThr);

    connect(&zas, &Zasilacz::error, this, &ZasilaczWidget::errorZasilacz);
    connect(&zas, &Zasilacz::debug, this, &ZasilaczWidget::debugZasilacz);
    connect(&zas, &Zasilacz::kontrolerConfigured, this, &ZasilaczWidget::configuredZasilacz);
    connect(&zas, &Zasilacz::kontrolerSerialNo, this, &ZasilaczWidget::serialNoZasilacz);
    connect(&zas, &Zasilacz::deviceName, this, &ZasilaczWidget::deviceNameZasilacz);
    connect(&zas, &Zasilacz::value, this, &ZasilaczWidget::valueZasilacz);


    connect(ui->pbZasOpen, &QPushButton::clicked, this, &ZasilaczWidget::openZasilacz);
    connect(ui->pbZasClose, &QPushButton::clicked, this, &ZasilaczWidget::closeZasilacz);


    ui->leCurrLim->setVisible(false);
    ui->leCurrSet->setVisible(false);
    ui->leVoltLim->setVisible(false);
    ui->leVoltSet->setVisible(false);
    ui->tbSave->setEnabled(false);
    ui->tbCancel->setEnabled(false);
}

ZasilaczWidget::~ZasilaczWidget()
{
    delete ui;
    zas.closeDevice(false);
}

void ZasilaczWidget::errorZasilacz(QString s)
{
    QMessageBox::critical(this, "Zasilacz", s);
    //showStatusMessage(QString("Błąd : ") + s);
}

void ZasilaczWidget::debugZasilacz(QString d)
{
    qDebug() << d;
}

void ZasilaczWidget::configuredZasilacz(bool success, int state)
{

}

void ZasilaczWidget::serialNoZasilacz(QString s)
{
    ui->lZasIdent->setText(s);
}

void ZasilaczWidget::deviceNameZasilacz(QString name)
{
    ui->lZasSterPort->setText(name);
}

void ZasilaczWidget::valueZasilacz(int kind, int value)
{
    switch(kind) {
        case Zasilacz::VOLTAGE_SET:
            ui->mVoltage->setText(QString::number(0.001*value, 'f', 3)); break;
        case Zasilacz::CURRENT_SET:
            ui->mCurrent->setText(QString::number(0.001*value, 'f', 3)); break;
        case Zasilacz::VOLTAGE_LIMIT:
            ui->mVoltageLimit->setText(QString::number(0.001*value, 'f', 3)); break;
        case Zasilacz::CURRENT_LIMIT:
            ui->mCurrentLimit->setText(QString::number(0.001*value, 'f', 3)); break;
        case Zasilacz::VOLTAGE_MEAS:
            ui->mVoltageMeas->setText(QString::number(0.001*value, 'f', 3)); break;
        case Zasilacz::CURRENT_MEAS:
            ui->mCurrentMeas->setText(QString::number(0.001*value, 'f', 3)); break;
        case Zasilacz::OUTPUT:
            break;
    }
}

void ZasilaczWidget::openZasilacz()
{
    zas.connectToDevice();
}

void ZasilaczWidget::closeZasilacz()
{
    zas.closeDevice(true);
}

void ZasilaczWidget::on_tbChange_clicked()
{
    ui->tbChange->setEnabled(true);
    ui->tbCancel->setEnabled(true);
    ui->tbSave->setEnabled(true);

    ui->leCurrLim->setVisible(true);
    ui->leCurrSet->setVisible(true);
    ui->leVoltLim->setVisible(true);
    ui->leVoltSet->setVisible(true);

    ui->leVoltSet->setText(ui->mVoltage->text());
    ui->leVoltLim->setText(ui->mVoltageLimit->text());
    ui->leCurrSet->setText(ui->mCurrent->text());
    ui->leCurrLim->setText(ui->mCurrentLimit->text());
}


void ZasilaczWidget::on_tbSave_clicked()
{
    ui->tbSave->setEnabled(false);
    ui->tbCancel->setEnabled(false);
    ui->tbSave->setEnabled(true);

    ui->leCurrLim->setVisible(false);
    ui->leCurrSet->setVisible(false);
    ui->leVoltLim->setVisible(false);
    ui->leVoltSet->setVisible(false);


    zas.setCurrent(ui->leCurrSet->text().toFloat()*1000);
    zas.setVoltage(ui->leVoltSet->text().toFloat()*1000);
    zas.setCurrentLimit(ui->leCurrLim->text().toFloat()*1000);
    zas.setVoltageLimit(ui->leVoltLim->text().toFloat()*1000);
}


void ZasilaczWidget::on_tbCancel_clicked()
{
    ui->tbSave->setEnabled(false);
    ui->tbCancel->setEnabled(false);
    ui->tbSave->setEnabled(true);


    ui->leCurrLim->setVisible(false);
    ui->leCurrSet->setVisible(false);
    ui->leVoltLim->setVisible(false);
    ui->leVoltSet->setVisible(false);
}

