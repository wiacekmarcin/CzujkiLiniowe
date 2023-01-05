#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "danetestu.h"
#include "parametrybadania.h"
#include "debugdialog.h"
#include "ustawienia.h"
#include "sterownik.h"
#include "zasilacz.h"
#include "ukladfiltrow.h"
#include <QThread>

class TestZasilaczaDlg;
class TestSterownikaDlg;
class TestStanowiskaDlg;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:


private slots:
    void on_actionOtw_rz_okno_triggered();

    void ster_error(QString s);
    void ster_debug(QString d);
    void ster_setParamsDone(bool success);
    void ster_kontrolerConfigured(int state);
    void ster_deviceName(QString name);
    void ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt);
    void ster_zdarzenieSilnik(short silnik, short zdarzenie);
    void ster_czujkaOn();
    void ster_progressImp(short silnik, unsigned int position);

    void zas_error(QString s);
    void zas_debug(QString d);
    void zas_configured(int state);
    void zas_serialNo(QString s);
    void zas_deviceName(QString name);
    void zas_value(int kind, int value);
    void zas_sendMsg(const QString & msg);
    void zas_recvMsg(const QString & msg);

    void flt_zerowanieFiltrowDone();
    void flt_setUkladFiltrowDone();
    void flt_bladFiltrow(short silnik, bool zerowanie);

    void on_actionParametry_Badania_triggered();
    void on_actionParametryKalibracyjne_triggered();
    void on_actionTestZasilacza_triggered();
    void on_pbDisconnect_clicked();
    void on_actionTestSterownikaDlg_triggered();
    void on_actionNoweBadanie_triggered();
    void on_actionStartTestu_triggered();
    void on_actionTestStanowiska_triggered();
    void on_actionZapiszZadanie_triggered();
    void on_actionZapiszJako_triggered();

    void on_actionOtworzBadanie_triggered();

protected:
    void saveFile();

private:
    Ui::MainWindow *ui;
    Ustawienia u;
    ParametryBadania b;

    Sterownik * sd;
    SterownikFiltrow sterF;
    QThread sdThreadW;
    QThread sdThreadR;
    DebugDialog * dbgDlg;
    bool showDebug;

    TestZasilaczaDlg * dlgTestZas;
    TestSterownikaDlg * dlgTestSter;
    TestStanowiskaDlg * dlgTestStan;

    Zasilacz * zas;
    QThread zasThr;
    QString fileDaneBadania;
    ListaTestow lt;


};
#endif // MAINWINDOW_H
