#ifndef OKNOBADANIEREAKCJI6DB_H
#define OKNOBADANIEREAKCJI6DB_H

#include <QDialog>
#include <QTimer>

#include <QMutex>
#include "ustawienia.h"


namespace Ui {
class OknoBadanieReakcji6dB;
}

class Sterownik;
class OknoBadanieReakcji6dB : public QDialog
{
    Q_OBJECT

public:
    explicit OknoBadanieReakcji6dB(unsigned int time1, unsigned int time2, const double &tlumnienie, unsigned int dlugoscFali, const double & wartTlum,
                          const QString & name, const QString &subtitle, const Ustawienia &ust,
                          Sterownik * ster_, QWidget *parent = 0);
    ~OknoBadanieReakcji6dB();

    void flt_zerowanieFiltrowDone();
    void flt_setUkladFiltrowDone();
    void flt_bladFiltrow(QChar silnik, bool zerowanie);

    void czujkaOn();

    bool getWynikBadania() const;
    const QString &getError() const;

    bool getResetFiltrowOk();
    void setResetFiltrowOk(bool newResetFiltrowOk);

protected:
    void usunTlumnik();

private slots:
    void progressBarUpdate();
protected slots:
    void timeoutSterownika();
private:
    Ui::OknoBadanieReakcji6dB *ui;
    QTimer tmZmProgressBar;
    int timeCzujkaOn;
    int timeOknoClose;
    QString posA, posB, posC;
    QTimer tmSterownika;
    Sterownik * ster;
    bool wynikBadania;
    QString error;
    bool sterResponse;
    int time_sec;
    bool waitForZeroFiltr;
    bool endReject;
    QString pos0A, pos0B, pos0C;
};

#endif // OKNOBADANIEREAKCJI6DB_H

