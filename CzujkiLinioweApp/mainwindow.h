#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "danetestu.h"
#include "parametrybadania.h"
#include "debugdialog.h"
#include "ustawienia.h"
#include "sterownik.h"
#include "zasilacz.h"
#include "symulator.h"
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
    MainWindow(Symulator *s, QWidget *parent = nullptr);
    ~MainWindow();

protected slots:


private slots:


    void ster_error(QString s);
    void ster_debug(QString d);
    void ster_setParamsDone(bool success);
    void ster_kontrolerConfigured(int state);
    void ster_deviceName(QString name);
    //void ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt);
    void ster_setPositionDone(short silnik, RuchSilnikaType ruch);
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
    void flt_bladFiltrow(QChar filtr, bool zerowanie);

    void actionOtw_rz_okno_triggered();
    void actionParametry_Badania_triggered();
    void actionParametryKalibracyjne_triggered();
    void actionTestZasilacza_triggered();
    void pbDisconnect_clicked();
    void actionTestSterownikaDlg_triggered();
    void actionNoweBadanie_triggered();
    void actionStartTestu_triggered();
    void actionTestStanowiska_triggered();
    void actionZapiszZadanie_triggered();
    void actionZapiszJako_triggered();
    void actionOtworzBadanie_triggered();
    void actionUsunBadanie_triggered();
    void actionSterownik_triggered();
    void actionParametryBadania_triggered();
    void actionZamknijBadanie_triggered();

    void actionTest_triggered();
protected:
    void saveFile();

private:
    Ui::MainWindow *ui;
    Ustawienia u;
    ParametryBadania b;

    Sterownik * sd;

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
