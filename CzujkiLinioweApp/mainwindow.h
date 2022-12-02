#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "debugdialog.h"
#include "ustawienia.h"
#include "sterownik.h"
#include "zasilacz.h"
#include <QThread>

class TestZasilaczaDlg;
class TestSterownikaDlg;

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

    void sd_error(QString s);
    void sd_debug(QString d);
    void sd_setParamsDone(bool success);
    void sd_kontrolerConfigured(bool success, int state);
    void sd_deviceName(QString name);
    void sd_setPositionDone(bool home, bool success);

    void zas_error(QString s);
    void zas_debug(QString d);
    void zas_configured(bool success, int state);
    void zas_serialNo(QString s);
    void zas_deviceName(QString name);
    void zas_value(int kind, int value);
    void zas_sendMsg(const QString & msg);
    void zas_recvMsg(const QString & msg);

    void on_actionParametry_Badania_triggered();
    void on_actionParametryKalibracyjne_triggered();
    void on_actionTestZasilacza_triggered();
    void on_pbDisconnect_clicked();

    void on_actionTestSterownikaDlg_triggered();

private:
    Ui::MainWindow *ui;
    Ustawienia u;
    Sterownik * sd;
    QThread sdThreadW;
    QThread sdThreadR;
    DebugDialog * dbgDlg;
    bool showDebug;

    TestZasilaczaDlg * dlgMierSignal;
    TestSterownikaDlg * dlgSterSign;

    Zasilacz * zas;
    QThread zasThr;
};
#endif // MAINWINDOW_H
