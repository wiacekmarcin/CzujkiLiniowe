#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "debugdialog.h"
#include "ustawienia.h"
#include "serialdevice.h"

#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOtw_rz_okno_triggered();
    void on_pbConnect_clicked();

    void sd_error(QString s);

    void sd_debug(QString d);

    void sd_setParamsDone(bool success);

    void sd_kontrolerConfigured(bool success, int state);

    void sd_deviceName(QString name);

    void sd_setPositionDone(bool home, bool success);

    void on_actionParametry_Badania_triggered();

    void on_actionParametryKalibracyjne_triggered();
    void on_pbDisconnect_clicked();

    void on_pbHome_1_clicked();
    void on_pbHome_2_clicked();
    void on_pbHome_3_clicked();
    void on_pbHome_4_clicked();
    void on_pbHome_5_clicked();
    void on_pbHome_6_clicked();
    void on_pbHome_7_clicked();
    void on_pbHome_8_clicked();
    void on_pbHome_9_clicked();

    void pbHome_clicked(int silnik);

    void on_pbHomeAll_clicked();

    void on_pbUstawPos_1_clicked();
    void on_pbUstawPos_2_clicked();
    void on_pbUstawPos_3_clicked();
    void on_pbUstawPos_4_clicked();
    void on_pbUstawPos_5_clicked();
    void on_pbUstawPos_6_clicked();
    void on_pbUstawPos_7_clicked();
    void on_pbUstawPos_8_clicked();
    void on_pbUstawPos_9_clicked();
    void pbUstawPos_clicked(int silnik, const float &x, const float &ratio);

    void on_pbCmdSend_clicked();

private:
    Ui::MainWindow *ui;
    Ustawienia u;
    SerialDevice * sd;
    QThread sdThreadW;
    QThread sdThreadR;
    DebugDialog * dbgDlg;
    bool showDebug;
};
#endif // MAINWINDOW_H
