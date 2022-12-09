#ifndef TESTSTEROWNIKADLG_H
#define TESTSTEROWNIKADLG_H

#include "sterownik.h"
#include "ustawienia.h"
#include <QDialog>

namespace Ui {
class TestSterownikaDlg;
}

class TestSterownikaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TestSterownikaDlg(Ustawienia *ust, Sterownik *sdv, QWidget *parent = nullptr);
    ~TestSterownikaDlg();

    void sd_kontrolerConfigured(bool success, int state);
    void sd_deviceName(QString name);
    void sd_setPositionDone(bool home, bool success);
    void sd_setParamsDone(bool success);
    void sd_debug(const QString & d);
    void sd_error(const QString & e);
    void sd_disconnect();

protected slots:
    void pbSetConfiguration_clicked();
    void pbHomeAll_clicked();
    void pbConnect_clicked();


protected:
    void pbHome_clicked(int silnik);
    void pbUstawPos_clicked(int silnik, const QString &x, const QString &ratio);

private:
    Ui::TestSterownikaDlg *ui;
    Ustawienia * u;
    Sterownik *sd;
};

#endif // TESTSTEROWNIKADLG_H
