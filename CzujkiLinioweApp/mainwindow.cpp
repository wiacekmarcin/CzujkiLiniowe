#include "mainwindow.h"
#include "teststerownikadlg.h"
#include "ui_mainwindow.h"

#include "parametrybadaniadlg.h"
#include "parametrykalibracyjnedlg.h"
#include "testzasilaczadlg.h"
#include "wybortestu.h"
#include "danetestu.h"
#include "teststanowiskadlg.h"

#include <QIcon>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sd(nullptr)
    , dlgTestZas(nullptr)
    , dlgTestSter(nullptr)
    , dlgTestStan(nullptr)
    , fileDaneBadania("")

{
    ui->setupUi(this);


    sd = new Sterownik(&u, this);
    sd->setThread(&sdThreadW, &sdThreadR);
    zas = new Zasilacz(&u, this);
    zas->setThread(&zasThr);

    dbgDlg = new DebugDialog(this);
    dbgDlg->hide();
    showDebug = false;

    ui->actionStartTestu->setEnabled(false);
    ui->actionParametryBadania->setEnabled(false);
    ui->actionUsunBadanie->setEnabled(false);
    ui->actionZapiszJako->setEnabled(false);
    ui->actionZapiszZadanie->setEnabled(false);
    ui->actionUsunBadanie->setEnabled(false);

    connect(sd, &Sterownik::error, this, &MainWindow::ster_error);
    connect(sd, &Sterownik::debug, this, &MainWindow::ster_debug);
    connect(sd, &Sterownik::zdarzenieSilnik, this, &MainWindow::ster_zdarzenieSilnik);
    connect(sd, &Sterownik::kontrolerConfigured, this, &MainWindow::ster_kontrolerConfigured);
    connect(sd, &Sterownik::deviceName, this, &MainWindow::ster_deviceName);
    connect(sd, &Sterownik::czujkaOn, this, &MainWindow::ster_czujkaOn);
    //connect(sd, &Sterownik::setPositionDone, this, &MainWindow::sd_setPositionDone);



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

    ui->centralwidget->setVisible(false);
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


void MainWindow::ster_error(QString s)
{
    if (dlgTestSter)
        dlgTestSter->sd_error(s);
    dbgDlg->add(s);
}

void MainWindow::ster_debug(QString d)
{
    if(dlgTestSter)
        dlgTestSter->sd_debug(d);
    dbgDlg->add(d);
}

void MainWindow::ster_setParamsDone(bool success)
{
    if (dlgTestSter)
        dlgTestSter->sd_setParamsDone(success);
}

void MainWindow::ster_kontrolerConfigured(int state)
{

    switch(state) {
    case Sterownik::NO_FOUND:

        break;
    case Sterownik::FOUND:

        break;
    case Sterownik::NO_OPEN:
    case Sterownik::NO_READ:
    case Sterownik::IDENT_FAILD:
        break;

    case Sterownik::OPEN:

        break;
    case Sterownik::IDENT_OK:

        break;
    case Sterownik::PARAMS_FAILD:
        break;
    case Sterownik::PARAMS_OK:
        break;
    case Sterownik::ALL_OK:
        break;
    case Sterownik::CLOSE:
        break;
    default:
        break;
    }

    if (dlgTestSter)
        dlgTestSter->sd_kontrolerConfigured(state);
    if (dlgTestStan)
        dlgTestStan->configuredSterownik(state);
}

void MainWindow::ster_deviceName(QString name)
{
    //ui->namePort->setText(name);
    if (dlgTestSter)
        dlgTestSter->sd_deviceName(name);
    if (dlgTestStan)
        dlgTestStan->deviceNameSter(name);
}

void MainWindow::ster_setPositionDone(bool home, bool success)
{
    if (dlgTestSter)
        dlgTestSter->sd_setPositionDone(home, success);
}

void MainWindow::ster_zdarzenieSilnik(short silnik, short zdarzenie)
{
    if (dlgTestSter)
        dlgTestSter->sd_setZdarzenieSilnik(silnik, zdarzenie);
}

void MainWindow::ster_czujkaOn()
{
    if (dlgTestSter)
        dlgTestSter->sd_czujkaOn(true);
}

void MainWindow::zas_error(QString s)
{
    qDebug() << s;
    if (dlgTestZas)
        dlgTestZas->errorZasilacz(s);
}

void MainWindow::zas_debug(QString d)
{
    qDebug() << d;
    if (dlgTestZas)
        dlgTestZas->debugZasilacz(d);
}

void MainWindow::zas_configured(int state)
{
    if (dlgTestZas)
        dlgTestZas->configuredZasilacz(state);
    if (dlgTestStan)
        dlgTestStan->configuredZasilacz(state);
}

void MainWindow::zas_serialNo(QString s)
{
    if (dlgTestZas)
        dlgTestZas->serialNoZasilacz(s);
}

void MainWindow::zas_deviceName(QString name)
{
    if (dlgTestZas)
        dlgTestZas->deviceNameZasilacz(name);
    if (dlgTestStan)
        dlgTestStan->deviceNameZasilacz(name);
}

void MainWindow::zas_value(int kind, int value)
{
    if (dlgTestZas)
        dlgTestZas->valueZasilacz(kind, value);
    if (dlgTestStan)
        dlgTestStan->valueZasilacz(kind, value);
}

void MainWindow::zas_sendMsg(const QString &msg)
{
    if (dlgTestZas)
        dlgTestZas->sendMsg(msg);
}

void MainWindow::zas_recvMsg(const QString &msg)
{
    if (dlgTestZas)
        dlgTestZas->recvMsg(msg);
}

void MainWindow::on_actionParametry_Badania_triggered()
{
    ParametryBadaniaDlg * dlg = new ParametryBadaniaDlg(u, &b, this);
    dlg->exec();

}


void MainWindow::on_actionParametryKalibracyjne_triggered()
{
    //qDebug() << "Parametry Kalibracyjne";
    ParametryKalibracyjneDlg * dlg = new ParametryKalibracyjneDlg(&u, this);
    dlg->exec();
    delete dlg;

}

void MainWindow::on_pbDisconnect_clicked()
{
    sd->closeDevice(true);
    if (dlgTestSter)
        dlgTestSter->sd_disconnect();
}

void MainWindow::on_actionTestZasilacza_triggered()
{
    dlgTestZas = new TestZasilaczaDlg(&u, zas, this);
    dlgTestZas->exec();
    delete dlgTestZas;
    dlgTestZas = nullptr;
}


void MainWindow::on_actionTestSterownikaDlg_triggered()
{
    dlgTestSter = new TestSterownikaDlg(&u, sd, this);
    dlgTestSter->exec();
    delete dlgTestSter;
    dlgTestSter = nullptr;
}


void MainWindow::on_actionNoweBadanie_triggered()
{
    QString fileName = QString("BadanieCzujki_%1%2.dat").arg(QDate::currentDate().toString("yyyyMMdd"),
                                                             QTime::currentTime().toString("HHmmss"));
    QFileInfo fi(QDir(QStandardPaths::displayName(QStandardPaths::DocumentsLocation)), fileName);
    fileDaneBadania = fi.absoluteFilePath();
    setWindowTitle("");
    setWindowFilePath(QString("Czujniki Liniowe [%1]").arg(fi.baseName()));
    setWindowModified(true);
    b = ParametryBadania();
    ParametryBadaniaDlg * dlg = new ParametryBadaniaDlg(u, &b, this);
    if (dlg->exec() == QDialog::Rejected) {
        fileDaneBadania = "";
        setWindowTitle("Czujniki Liniowe");
        setWindowModified(false);
        return;
    }

    ui->actionStartTestu->setEnabled(true);
    ui->actionParametryBadania->setEnabled(true);
    ui->actionUsunBadanie->setEnabled(true);
    ui->actionZapiszJako->setEnabled(true);
    ui->actionZapiszZadanie->setEnabled(true);
    ui->actionUsunBadanie->setEnabled(true);
    ui->centralwidget->setVisible(true);
}


void MainWindow::on_actionStartTestu_triggered()
{
    WyborTestu *dlg1 = new WyborTestu(lt, b.getOdtwarzalnosc(), this);
    if(!dlg1->exec()) {
        return;
    }
    short idTest = dlg1->getWyborTestu();
    delete dlg1;
    ui->centralwidget->startBadanie(idTest, b, u, zas, sd);

}


void MainWindow::on_actionTestStanowiska_triggered()
{
    dlgTestStan = new TestStanowiskaDlg(zas, sd, this);
    dlgTestStan->exec();
    delete dlgTestStan;
    dlgTestStan = nullptr;
}


void MainWindow::on_actionZapiszZadanie_triggered()
{
    if (fileDaneBadania.isEmpty()) {
        on_actionZapiszJako_triggered();
        return;
    }
    saveFile();
}


void MainWindow::on_actionZapiszJako_triggered()
{
    qDebug() << QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Zachowaj dane w pliku"),
                               QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                               tr("Badania (*.dat)"));
    if (fileName.isEmpty())
        return;

    fileDaneBadania = fileName;
    QFileInfo fi(fileName);
    setWindowTitle("");
    setWindowFilePath(QString("Czujniki Liniowe [%1]").arg(fi.baseName()));
    saveFile();
}

void MainWindow::saveFile()
{
    b.save(fileDaneBadania);
    setWindowModified(false);
}


void MainWindow::on_actionOtworzBadanie_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Otwórz badanie"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    tr("Badania (*.dat)"));
    if (fileName.isEmpty())
        return;
    fileDaneBadania = fileName;
    QFileInfo fi(fileName);
    setWindowTitle("");
    setWindowFilePath(QString("Czujniki Liniowe [%1]").arg(fi.baseName()));
    setWindowModified(false);
    b.load(fileDaneBadania);
}

