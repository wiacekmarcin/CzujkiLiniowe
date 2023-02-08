#ifndef OKNOBADANIAMAKSYMALNEGOKATA_H
#define OKNOBADANIAMAKSYMALNEGOKATA_H

#include <QDialog>
#include <QTimer>

#include <QMutex>
#include "ustawienia.h"

#include "sterownik.h"

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class OknoBadaniaMaksymalnegoKata;
}


class OknoBadaniaMaksymalnegoKata : public QDialog
{
    Q_OBJECT

public:
    explicit OknoBadaniaMaksymalnegoKata(short nrSilnika_, const QString &name,
                             const QString & podtitle, const double &startKat,
                             const double & kat,
                             const Ustawienia &ust,
                             Sterownik * ster_, QWidget *parent = 0);
    ~OknoBadaniaMaksymalnegoKata();


    void czujkaOn();

    bool getWynikBadania() const;
    double getDegrees() const;

    const QString &getError() const;

    void ster_setPositionDone(short silnik, RuchSilnikaType r);
    void ster_setValue(short silnik, const double & val);
    void testValue();

protected slots:
    void timeoutSterownika();
private:
    Ui::OknoBadaniaMaksymalnegoKata *ui;

    QTimer tmSterownika;

    Sterownik * ster;
    bool wynikBadania;
    QString error;
    float destPos;
    double wspolImpKat;
    short nrSilnika;
    bool deviceisOk;
    float prevVal;
    double speedMin;
    float startPos;
    float maxDelta;

    QTimer * moveTimer;
    float valBeg;
    float valEnd;
    float valAct;
    float valPer100ms;


};

#endif // OKNOBADANIAMAKSYMALNEGOKATA_H

