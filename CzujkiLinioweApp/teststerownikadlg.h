#ifndef TESTSTEROWNIKADLG_H
#define TESTSTEROWNIKADLG_H

#include "serialdevice.h"
#include "ustawienia.h"
#include <QDialog>

namespace Ui {
class TestSterownikaDlg;
}

class TestSterownikaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TestSterownikaDlg(Ustawienia *ust, SerialDevice *sdv, QWidget *parent = nullptr);
    ~TestSterownikaDlg();

    void sd_kontrolerConfigured(bool success, int state);
    void sd_deviceName(QString name);
    void sd_setPositionDone(bool home, bool success);
    void sd_setParamsDone(bool success);
    void disconnect();
protected slots:
    void on_pbSetConfiguration_clicked();
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
    void on_pbConnect_clicked();

private:
    Ui::TestSterownikaDlg *ui;
    Ustawienia * u;
    SerialDevice *sd;
};

#endif // TESTSTEROWNIKADLG_H
