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

#include "oknopodsumowanietestu.h"
#include "danetestu.h"
#include "parametrybadania.h"


#define CONN_PB(F) connect(ui->action##F, &QAction::triggered, this, &MainWindow::action##F##_triggered)
#define CONN_PB_ALL CONN_PB(Parametry_Badania); \
                    CONN_PB(ParametryKalibracyjne); \
                    CONN_PB(TestZasilacza); \
                    CONN_PB(TestSterownikaDlg); \
                    CONN_PB(StartTestu); \
                    CONN_PB(TestStanowiska); \
                    CONN_PB(ZapiszZadanie); \
                    CONN_PB(ZapiszJako); \
                    CONN_PB(OtworzBadanie); \
                    CONN_PB(Otw_rz_okno); \
                    CONN_PB(NoweBadanie); \
                    CONN_PB(UsunBadanie); \
                    CONN_PB(Sterownik); \
                    CONN_PB(ParametryBadania); \
                    CONN_PB(ZamknijBadanie); \
                    CONN_PB(Test);



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
    //sd->setThread(&sdThreadW, &sdThreadR);
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
    connect(sd, &Sterownik::positionDone, this, &MainWindow::ster_setPositionDone);
    connect(sd, &Sterownik::progressImp, this, &MainWindow::ster_progressImp);

    connect(zas, &Zasilacz::error, this, &MainWindow::zas_error);
    connect(zas, &Zasilacz::debug, this, &MainWindow::zas_debug);
    connect(zas, &Zasilacz::kontrolerConfigured, this, &MainWindow::zas_configured);
    connect(zas, &Zasilacz::kontrolerSerialNo, this, &MainWindow::zas_serialNo);
    connect(zas, &Zasilacz::deviceName, this, &MainWindow::zas_deviceName);
    connect(zas, &Zasilacz::value, this, &MainWindow::zas_value);
    connect(zas, &Zasilacz::sendMsg, this, &MainWindow::zas_sendMsg);
    connect(zas, &Zasilacz::recvMsg, this, &MainWindow::zas_recvMsg);

    connect(sd, &Sterownik::zerowanieFiltrowDone, this, &MainWindow::flt_zerowanieFiltrowDone);
    connect(sd, &Sterownik::setUkladFiltrowDone, this, &MainWindow::flt_setUkladFiltrowDone);
    connect(sd, &Sterownik::bladFiltrow, this, &MainWindow::flt_bladFiltrow);

    CONN_PB_ALL

    //connect(ui->pbDisconnect, &QPushButton::clicked, this, &MainWindow::pbDisconnect_clicked);

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

void MainWindow::actionOtw_rz_okno_triggered()
{
    showDebug = !showDebug;
    if (showDebug)
        dbgDlg->show();
    else
        dbgDlg->hide();
}


void MainWindow::ster_error(QString s)
{
    dbgDlg->add(s);
}

void MainWindow::ster_debug(QString d)
{
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

//void MainWindow::ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt)
void MainWindow::ster_setPositionDone(short silnik, RuchSilnikaType ruch)
{
    //ui->centralwidget->ster_setPositionDone(silnik, home, move, error, interrupt);
    ui->centralwidget->ster_setPositionDone(silnik, ruch);
    if (dlgTestSter) {
        //dlgTestSter->sd_setPositionDone(silnik, home, move, error, interrupt);
        dlgTestSter->sd_setPositionDone(silnik, ruch);
    }
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
    if (dlgTestStan)
        dlgTestStan->ster_czujkaOn();
    ui->centralwidget->ster_czujkaOn();
}

void MainWindow::ster_progressImp(short silnik, unsigned int position)
{
    double valReal = u.convertImp2Value(silnik, position);
    ui->centralwidget->ster_setValue(silnik, valReal);
    if (dlgTestSter)
        dlgTestSter->sd_setValue(silnik, valReal);
    if (dlgTestStan)
        dlgTestStan->ster_setValue(silnik, valReal);
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
    ui->centralwidget->zas_value(kind, value);
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

void MainWindow::flt_zerowanieFiltrowDone()
{
    ui->centralwidget->flt_zerowanieFiltrowDone();
    if (dlgTestStan)
        dlgTestStan->flt_zerowanieFiltrowDone();
}

void MainWindow::flt_setUkladFiltrowDone()
{
    ui->centralwidget->flt_setUkladFiltrowDone();
    if (dlgTestStan)
        dlgTestStan->flt_setUkladFiltrowDone();
}

void MainWindow::flt_bladFiltrow(QChar filtr, bool zerowanie)
{
    ui->centralwidget->flt_bladFiltrow(filtr, zerowanie);
    if (dlgTestStan)
        dlgTestStan->flt_bladFiltrow(filtr, zerowanie);
}

void MainWindow::actionParametry_Badania_triggered()
{
    ParametryBadaniaDlg * dlg = new ParametryBadaniaDlg(true, u, &b, this);
    dlg->exec();

}


void MainWindow::actionParametryKalibracyjne_triggered()
{
    //qDebug() << "Parametry Kalibracyjne";
    ParametryKalibracyjneDlg * dlg = new ParametryKalibracyjneDlg(&u, this);
    dlg->exec();
    delete dlg;

}

void MainWindow::pbDisconnect_clicked()
{
    sd->closeDevice(true);
    if (dlgTestSter)
        dlgTestSter->sd_disconnect();
}

void MainWindow::actionTestZasilacza_triggered()
{
    dlgTestZas = new TestZasilaczaDlg(&u, zas, this);
    dlgTestZas->exec();
    delete dlgTestZas;
    dlgTestZas = nullptr;
}


void MainWindow::actionTestSterownikaDlg_triggered()
{
    dlgTestSter = new TestSterownikaDlg(&u, sd, this);
    dlgTestSter->exec();
    delete dlgTestSter;
    dlgTestSter = nullptr;
}


void MainWindow::actionNoweBadanie_triggered()
{
    QString fileName = QString("BadanieCzujki_%1%2.dat").arg(QDate::currentDate().toString("yyyyMMdd"),
                                                             QTime::currentTime().toString("HHmmss"));
    QFileInfo fi(QDir(QStandardPaths::displayName(QStandardPaths::DocumentsLocation)), fileName);
    fileDaneBadania = fi.absoluteFilePath();
    setWindowTitle("");
    setWindowFilePath(QString("Czujniki Liniowe [%1]").arg(fi.baseName()));
    setWindowModified(true);
    b = ParametryBadania();
    ParametryBadaniaDlg * dlg = new ParametryBadaniaDlg(false, u, &b, this);
    if (dlg->exec() == QDialog::Rejected) {
        fileDaneBadania = "";
        setWindowTitle("Czujniki Liniowe");
        setWindowModified(false);
        delete dlg;
        return;
    }

    ui->centralwidget->setBadanie(b);
    ui->actionStartTestu->setEnabled(true);
    ui->actionParametryBadania->setEnabled(true);
    ui->actionUsunBadanie->setEnabled(true);
    ui->actionZapiszJako->setEnabled(true);
    ui->actionZapiszZadanie->setEnabled(true);
    ui->actionUsunBadanie->setEnabled(true);
    ui->centralwidget->setVisible(true);

}


void MainWindow::actionStartTestu_triggered()
{
    WyborTestu *dlg1 = new WyborTestu(lt, b.getTestOdtwarzalnosci(), this);
    if(!dlg1->exec()) {
        return;
    }
    short idTest = dlg1->getWyborTestu();
    delete dlg1;
    ui->centralwidget->startBadanie(idTest, b, u, zas, sd);

}


void MainWindow::actionTestStanowiska_triggered()
{
    dlgTestStan = new TestStanowiskaDlg(zas, sd, &u, this);
    dlgTestStan->exec();
    delete dlgTestStan;
    dlgTestStan = nullptr;
}


void MainWindow::actionZapiszZadanie_triggered()
{
    if (fileDaneBadania.isEmpty()) {
        actionZapiszJako_triggered();
        return;
    }
    saveFile();
}


void MainWindow::actionZapiszJako_triggered()
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


void MainWindow::actionOtworzBadanie_triggered()
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
    ui->centralwidget->setBadanie(b);
    ui->actionStartTestu->setEnabled(true);
    ui->actionParametryBadania->setEnabled(true);
    ui->actionUsunBadanie->setEnabled(true);
    ui->actionZapiszJako->setEnabled(true);
    ui->actionZapiszZadanie->setEnabled(true);
    ui->actionUsunBadanie->setEnabled(true);
    ui->centralwidget->setVisible(true);
}

void MainWindow::actionUsunBadanie_triggered()
{

}

void MainWindow::actionSterownik_triggered()
{

}

void MainWindow::actionParametryBadania_triggered()
{
    ParametryBadaniaDlg * dlg = new ParametryBadaniaDlg(true, u, &b, this);
    if (dlg->exec() == QDialog::Rejected) {
        setWindowTitle("Czujniki Liniowe");
        setWindowModified(false);
        delete dlg;
        return;
    }
    if (!b.getTestOdtwarzalnosci()) {
        ui->centralwidget->setBadanie(b);
        setWindowTitle("Czujniki Liniowe");
        setWindowModified(false);
    }
    delete dlg;
}

void MainWindow::actionZamknijBadanie_triggered()
{

}

void MainWindow::actionTest_triggered()
{
    DaneTestu dt;
    ParametryBadania pb;
    dt.setId(0);
    dt.addWybranaCzujka(1, "n1", "o1");
    dt.addWybranaCzujka(2, "n2", "o2");
    dt.setSuccessBadaniaCzujki(true, "6.2", "");
    dt.setSuccessBadaniaCzujki(false, "-", "blad sprzetowy");
    OknoPodsumowanieTestu * dlg = new OknoPodsumowanieTestu(dt, pb, u,  this);
    dlg->exec();
}

