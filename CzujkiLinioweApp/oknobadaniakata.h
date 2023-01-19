#ifndef OKNOBADANIAKATA_H
#define OKNOBADANIAKATA_H

#include <QDialog>
#include <QTimer>

#include <QMutex>
#include "ustawienia.h"

#include "sterownik.h"

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class OknoBadaniaKata;
}


class OknoBadaniaKata : public QDialog
{
    Q_OBJECT

public:
    explicit OknoBadaniaKata(short nrSilnika_, const QString &name,
                             const QString & podtitle,
                             const QString & kat,
                             const Ustawienia &ust,
                             Sterownik * ster_, QWidget *parent = 0);
    ~OknoBadaniaKata();


    void czujkaOn();


#ifdef DEFVAL
    void testValue();
#endif

    const QString &getError() const;

    //void ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt);
    void ster_setPositionDone(short silnik, RuchSilnikaType r);
    void ster_setValue(short silnik, const double & val);


protected slots:
    void timeoutSterownika();
private:
    Ui::OknoBadaniaKata *ui;

    QTimer tmSterownika;

    Sterownik * ster;
    bool wynikBadania;
    QString error;
    float destPos;
    double wspolImpKat;
    short nrSilnika;
    bool deviceisOk;
    double prevVal;
    double speedMin;

};

#endif // OKNOBADANIAKATA_H


