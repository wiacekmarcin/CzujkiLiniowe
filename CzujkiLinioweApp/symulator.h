#ifndef SYMULATOR_H
#define SYMULATOR_H
#include "sterownik.h"
#include "ustawienia.h"

#include <QMainWindow>

namespace Ui {
class Symulator;
}

class Symulator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Symulator(const Ustawienia & u, QWidget *parent = nullptr);
    ~Symulator();

signals:

    void ster_setPositionDone(short silnik, RuchSilnikaType ruch);
    void ster_zdarzenieSilnik(short silnik, short zdarzenie);
    void ster_czujkaOn();
    void ster_progressImp(short silnik, unsigned int position);

    void zas_value(int kind, int value);

    void flt_zerowanieFiltrowDone();
    void flt_setUkladFiltrowDone();
    void flt_bladFiltrow(QChar filtr, bool zerowanie);
protected:
    void ster_setPositionDoneSig(short silnik, RuchSilnikaType ruch);
private slots:
    void homeAll();
    void pbStart();
    void pbStop();
    void timeout();

private:
    Ui::Symulator *ui;
    QTimer timer;
    float valBeg;
    float valEnd;
    float valAct;
    float valPer100ms;
    short silnik;
    const Ustawienia & u;
};

#endif // SYMULATOR_H
