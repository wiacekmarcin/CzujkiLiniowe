#include "mainwindow.h"
#include "teststerownikadlg.h"
#include "ui_mainwindow.h"

#include "parametrybadania.h"
#include "parametrykalibracyjnedlg.h"
#include "testzasilaczadlg.h"

#include <QIcon>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sd(nullptr)
    , dlgMierSignal(nullptr)
    , dlgSterSign(nullptr)

{
    ui->setupUi(this);

    sd = new SerialDevice(&u, this);
    sd->setThread(&sdThreadW, &sdThreadR);
    zas = new Zasilacz(&u, this);
    zas->setThread(&zasThr);

    dbgDlg = new DebugDialog(this);
    dbgDlg->hide();
    showDebug = false;

    connect(sd, &SerialDevice::error, this, &MainWindow::sd_error);
    connect(sd, &SerialDevice::debug, this, &MainWindow::sd_debug);
    connect(sd, &SerialDevice::setParamsDone, this, &MainWindow::sd_setParamsDone);
    connect(sd, &SerialDevice::kontrolerConfigured, this, &MainWindow::sd_kontrolerConfigured);
    connect(sd, &SerialDevice::deviceName, this, &MainWindow::sd_deviceName);
    connect(sd, &SerialDevice::setPositionDone, this, &MainWindow::sd_setPositionDone);



    connect(zas, &Zasilacz::error, this, &MainWindow::zas_error);
    connect(zas, &Zasilacz::debug, this, &MainWindow::zas_debug);
    connect(zas, &Zasilacz::kontrolerConfigured, this, &MainWindow::zas_configured);
    connect(zas, &Zasilacz::kontrolerSerialNo, this, &MainWindow::zas_serialNo);
    connect(zas, &Zasilacz::deviceName, this, &MainWindow::zas_deviceName);
    connect(zas, &Zasilacz::value, this, &MainWindow::zas_value);
    connect(zas, &Zasilacz::sendMsg, this, &MainWindow::zas_sendMsg);
    connect(zas, &Zasilacz::recvMsg, this, &MainWindow::zas_recvMsg);

    //QIcon icon5(QString::fromUtf8(":/zasilacz/zasilacz_on"));

    //icon5.addFile(QString::fromUtf8(":/zasilacz/zasilacz_off"), QSize(), QIcon::Normal, QIcon::Off);
    //icon5.addFile(QString::fromUtf8(":/zasilacz/zasilacz_on"), QSize(), QIcon::Normal, QIcon::On);
    //icon5.addFile(QString::fromUtf8(":/zasilacz/zasilacz_notconnected"), QSize(), QIcon::Disabled, QIcon::Off);
    //icon5.addFile(QString::fromUtf8(":/zasilacz/zasilacz_notconnected"), QSize(), QIcon::Disabled, QIcon::On);
    //ui->actionZasilacz->setIcon(icon5);
    //ui->actionZasilacz->setEnabled(true);
    //ui->toolBar->setIconSize(QSize(32,32));
    //QIcon icon6(QString::fromUtf8(":/sterownik/sterownik_on"));
    //ui->actionSterownik->setIcon(icon6);
}

MainWindow::~MainWindow()
{
    if (sd)
        delete sd;
    if (zas)
        delete zas;
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
    if (dlgSterSign)
        dlgSterSign->sd_setParamsDone(success);
}

void MainWindow::sd_kontrolerConfigured(bool success, int state)
{

    switch(state) {
    case SerialDevice::NO_FOUND:

        break;
    case SerialDevice::FOUND:

        break;
    case SerialDevice::NO_OPEN:
    case SerialDevice::NO_READ:
    case SerialDevice::IDENT_FAILD:
        break;

    case SerialDevice::OPEN:

        break;
    case SerialDevice::IDENT_OK:

        break;
    case SerialDevice::PARAMS_FAILD:
        break;
    case SerialDevice::PARAMS_OK:
        break;
    case SerialDevice::ALL_OK:
        break;
    case SerialDevice::CLOSE:
        break;
    default:
        break;
    }

    if (dlgSterSign)
        dlgSterSign->sd_kontrolerConfigured(success, state);
}

void MainWindow::sd_deviceName(QString name)
{
    //ui->namePort->setText(name);
    if (dlgSterSign)
        dlgSterSign->sd_deviceName(name);
}

void MainWindow::sd_setPositionDone(bool home, bool success)
{
    if (dlgSterSign)
        dlgSterSign->sd_setPositionDone(home, success);
}

void MainWindow::zas_error(QString s)
{
    qDebug() << s;
    if (dlgMierSignal)
        dlgMierSignal->errorZasilacz(s);
}

void MainWindow::zas_debug(QString d)
{
    qDebug() << d;
    if (dlgMierSignal)
        dlgMierSignal->debugZasilacz(d);
}

void MainWindow::zas_configured(bool success, int state)
{
    if (dlgMierSignal)
        dlgMierSignal->configuredZasilacz(success, state);
}

void MainWindow::zas_serialNo(QString s)
{
    if (dlgMierSignal)
        dlgMierSignal->serialNoZasilacz(s);
}

void MainWindow::zas_deviceName(QString name)
{
    if (dlgMierSignal)
        dlgMierSignal->deviceNameZasilacz(name);
}

void MainWindow::zas_value(int kind, int value)
{
    if (dlgMierSignal)
        dlgMierSignal->valueZasilacz(kind, value);
}

void MainWindow::zas_sendMsg(const QString &msg)
{
    if (dlgMierSignal)
        dlgMierSignal->sendMsg(msg);
}

void MainWindow::zas_recvMsg(const QString &msg)
{
    if (dlgMierSignal)
        dlgMierSignal->recvMsg(msg);
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
    if (dlgSterSign)
        dlgSterSign->disconnect();
}

void MainWindow::on_actionTestZasilacza_triggered()
{
    TestZasilaczaDlg * dlg = new TestZasilaczaDlg(&u, zas, this);
    dlgMierSignal = dlg;
    dlg->exec();
    delete dlg;
    dlgMierSignal = nullptr;
}


void MainWindow::on_actionTestSterownikaDlg_triggered()
{
    TestSterownikaDlg * dlg = new TestSterownikaDlg(&u, sd, this);
    dlgSterSign = dlg;
    dlg->exec();
    delete dlg;
    dlgSterSign = nullptr;
}


void MainWindow::on_actionNoweBadanie_triggered()
{
    ParametryBadania * dlg = new ParametryBadania(this);
    dlg->exec();
}


void MainWindow::on_actionStartTestu_triggered()
{

}

