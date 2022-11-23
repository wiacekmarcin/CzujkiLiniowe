#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "parametrybadania.h"

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
    switch(state) {
    case SerialDevice::NO_FOUND:
        ui->rbFound->setChecked(false);
        ui->rbAuth->setEnabled(false);
        ui->rbConf_2->setEnabled(false);
        ui->rbMultimetr->setEnabled(false);
        ui->frame_3->setEnabled(false);
        break;
    case SerialDevice::FOUND:
        ui->rbFound->setChecked(true);
        ui->rbAuth->setEnabled(true);
        break;
    case SerialDevice::NO_OPEN:
    case SerialDevice::OPEN:
    case SerialDevice::NO_READ:
    case SerialDevice::IDENT_FAILD:
        break;
    case SerialDevice::IDENT_OK:
        ui->rbAuth->setChecked(true);
        ui->rbConf_2->setEnabled(true);
        break;
    case SerialDevice::PARAMS_FAILD:
        break;
    case SerialDevice::PARAMS_OK:
        ui->rbConf_2->setChecked(true);
        break;
    case SerialDevice::ALL_OK:
        ui->frame_3->setEnabled(true);
        ui->pbConnect->setEnabled(false);
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
        break;
    default:
        break;
    }
    ui->pbConnect->setEnabled(!success);
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

