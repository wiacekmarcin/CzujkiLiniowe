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
