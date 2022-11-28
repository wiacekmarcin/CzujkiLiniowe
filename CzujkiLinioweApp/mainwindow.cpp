#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "parametrybadania.h"
#include "parametrykalibracyjnedlg.h"

#include <QDebug>
#define SETCONF(N) ui->base##N->setText(u.getBaseStepsSilnik##N()); \
                   ui->delay##N->setText(u.getDelayusSilnik##N()); \
                   ui->maxSteps##N->setText(u.getMaxStepsSilnik##N()); \
                   ui->obrot##N->setChecked(u.getLeftRotationSilnik##N()); \
                   ui->ratio##N->setText(u.getRatioSilnik##N()); \
                   ui->enable##N->setChecked(u.getEnableSilnik##N())


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sd(nullptr)
{
    ui->setupUi(this);

    sd = new SerialDevice(&u, this);
    sd->setThread(&sdThreadW, &sdThreadR);

    dbgDlg = new DebugDialog(this);
    dbgDlg->hide();
    showDebug = false;

    connect(sd, &SerialDevice::error, this, &MainWindow::sd_error);
    connect(sd, &SerialDevice::debug, this, &MainWindow::sd_debug);
    connect(sd, &SerialDevice::setParamsDone, this, &MainWindow::sd_setParamsDone);
    connect(sd, &SerialDevice::kontrolerConfigured, this, &MainWindow::sd_kontrolerConfigured);
    connect(sd, &SerialDevice::deviceName, this, &MainWindow::sd_deviceName);
    connect(sd, &SerialDevice::setPositionDone, this, &MainWindow::sd_setPositionDone);
}

MainWindow::~MainWindow()
{
    if (sd)
        delete sd;
    delete ui;
}

void MainWindow::on_actionOtw_rz_okno_triggered()
{
    showDebug = !showDebug;
    if (showDebug)
        dbgDlg->show();
    else
        dbgDlg->hide();
}

#define WRITECONF(N) u.setBaseStepsSilnik##N(ui->base##N->text()); \
                     u.setDelayusSilnik##N(ui->delay##N->text()); \
                     u.setMaxStepsSilnik##N(ui->maxSteps##N->text()); \
                     u.setLeftRotationSilnik##N(ui->obrot##N->isChecked()); \
                     u.setRatioSilnik##N(ui->ratio##N->text()); \
                     u.setEnableSilnik##N(ui->enable##N->isChecked())

void MainWindow::on_pbSetConfiguration_clicked()
{
    WRITECONF(1);
    WRITECONF(2);
    WRITECONF(3);
    WRITECONF(4);
    WRITECONF(5);
    WRITECONF(6);
    WRITECONF(7);
    WRITECONF(8);
    WRITECONF(9);
}

void MainWindow::on_pbConnect_clicked()
{
    sd->connectToDevice();
}

void MainWindow::sd_error(QString s)
{
    dbgDlg->add(s);
}

void MainWindow::sd_debug(QString d)
{
    dbgDlg->add(d);
}

void MainWindow::sd_setParamsDone(bool success)
{

}

void MainWindow::sd_kontrolerConfigured(bool success, int state)
{
    ui->textBrowser->insertPlainText(QString("success=%1, state=%2").arg(success).arg(state));
    switch(state) {
    case SerialDevice::NO_FOUND:
        ui->rbFound->setEnabled(false);
        ui->rbFound->setChecked(false);
        ui->rbAuth->setEnabled(false);
        ui->rbConf_2->setEnabled(false);
        ui->rbMultimetr->setEnabled(false);
        ui->frame_3->setEnabled(false);
        ui->pbConnect->setEnabled(true);
        ui->pbDisconnect->setEnabled(false);
        break;
    case SerialDevice::FOUND:
        ui->rbFound->setChecked(true);
        ui->rbAuth->setEnabled(true);
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(false);
        break;
    case SerialDevice::NO_OPEN:
    case SerialDevice::NO_READ:
    case SerialDevice::IDENT_FAILD:
        ui->pbConnect->setEnabled(true);
        ui->pbDisconnect->setEnabled(false);
    case SerialDevice::OPEN:
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);
        break;
    case SerialDevice::IDENT_OK:
        ui->rbAuth->setChecked(true);
        ui->rbConf_2->setEnabled(true);
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);
        break;
    case SerialDevice::PARAMS_FAILD:
        ui->pbConnect->setEnabled(true);
        ui->pbDisconnect->setEnabled(false);
        break;
    case SerialDevice::PARAMS_OK:
        ui->rbConf_2->setChecked(true);
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);
        break;
    case SerialDevice::ALL_OK:
        ui->frame_3->setEnabled(true);
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);
        break;
    case SerialDevice::CLOSE:
        ui->rbFound->setChecked(false);
        ui->rbAuth->setEnabled(false);
        ui->rbAuth->setChecked(false);
        ui->rbConf_2->setEnabled(false);
        ui->rbConf_2->setChecked(false);
        ui->rbMultimetr->setEnabled(false);
        ui->rbMultimetr->setChecked(false);
        ui->frame_3->setEnabled(false);
        ui->pbConnect->setEnabled(true);
        ui->pbDisconnect->setEnabled(false);
        break;
    default:
        break;
    }
}

void MainWindow::sd_deviceName(QString name)
{
    ui->namePort->setText(name);
}

void MainWindow::sd_setPositionDone(bool home, bool success)
{

}

void MainWindow::on_actionParametry_Badania_triggered()
{
    ParametryBadania * dlg = new ParametryBadania(this);
    dlg->exec();

}


void MainWindow::on_actionParametryKalibracyjne_triggered()
{
    qDebug() << "Parametry Kalibracyjne";
    ParametryKalibracyjneDlg * dlg = new ParametryKalibracyjneDlg(&u, this);
    dlg->exec();
    delete dlg;

}

void MainWindow::on_pbDisconnect_clicked()
{
    sd->closeDevice(true);
}

#define PBHOME(N) void MainWindow::on_pbHome_##N##_clicked() \
{ pbHome_clicked(N); }

PBHOME(1)
PBHOME(2)
PBHOME(3)
PBHOME(4)
PBHOME(5)
PBHOME(6)
PBHOME(7)
PBHOME(8)
PBHOME(9)


void MainWindow::pbHome_clicked(int silnik)
{
    sd->setPositionSilnik(silnik, true, 0);
}

void MainWindow::on_pbHomeAll_clicked()
{
    for (int i=1; i<10; ++i)
        pbHome_clicked(i);
}

void MainWindow::pbUstawPos_clicked(int silnik, const float & x, const float & ratio)
{
    sd->setPositionSilnik(silnik, false, x*ratio);
}

#define PBSETPOS(N) void MainWindow::on_pbUstawPos_##N##_clicked() \
{ pbUstawPos_clicked(N, ui->posX_##N->text().toFloat(), u.getRatioSilnik##N().toFloat()); }

PBSETPOS(1)
PBSETPOS(2)
PBSETPOS(3)
PBSETPOS(4)
PBSETPOS(5)
PBSETPOS(6)
PBSETPOS(7)
PBSETPOS(8)
PBSETPOS(9)

void MainWindow::on_pbCmdSend_clicked()
{
    if (ui->multi_cmd->text().isEmpty())
        return;
    ui->multi_request->append(QString(ui->multi_cmd->text())+QString("\n"));
}
