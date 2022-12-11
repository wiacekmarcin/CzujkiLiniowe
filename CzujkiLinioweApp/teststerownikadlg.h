#ifndef TESTSTEROWNIKADLG_H
#define TESTSTEROWNIKADLG_H

#include "sterownik.h"
#include "ustawienia.h"
#include <QDialog>
#include <QMap>
#include <QPair>
#include <QLabel>
namespace Ui {
class TestSterownikaDlg;
}

class TestSterownikaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TestSterownikaDlg(Ustawienia *ust, Sterownik *sdv, QWidget *parent = nullptr);
    ~TestSterownikaDlg();

    void sd_kontrolerConfigured(int state);
    void sd_deviceName(QString name);
    void sd_setPositionDone(bool home, bool success);
    void sd_setParamsDone(bool success);
    void sd_debug(const QString & d);
    void sd_error(const QString & e);
    void sd_disconnect();
    void sd_setZdarzenieSilnik(short silnik, short zdarzenie);

protected slots:
    void pbSetConfiguration_clicked();
    void pbHomeAll_clicked();
    void pbConnect_clicked();
    void pbDisconnect_clicked();

protected:
    void pbHome_clicked(int silnik, const QString &impTime);
    void pbUstawPos_clicked(int silnik, const QString &x, const QString &ratio);

private:
    Ui::TestSterownikaDlg *ui;
    Ustawienia * u;
    Sterownik *sd;
    QMap<int, QPair<QLabel*, QLabel*>> ikonyStatusu;
};

#endif // TESTSTEROWNIKADLG_H
