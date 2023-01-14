#ifndef OKNOBADANIATLUMIENIA_H
#define OKNOBADANIATLUMIENIA_H

#include <QDialog>
#include <QTimer>

#include <QMutex>
#include "ustawienia.h"

#ifdef DEFVAL

#include <QDialog>
#include <QLineEdit>

class TestValueDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TestValueDialog(const QString & val, QWidget *parent = 0);

    QString value() const;

private:
    QLineEdit *m_lineEdit;
};
#endif


namespace Ui {
class OknoBadaniaTlumienia;
}

class Sterownik;
class OknoBadaniaTlumienia : public QDialog
{
    Q_OBJECT

public:
    explicit OknoBadaniaTlumienia(unsigned int czasPostojuFiltra, unsigned int dlugoscFali_,
                          const QString & name, const Ustawienia &ust,
                          Sterownik * ster_, QWidget *parent = 0);
    ~OknoBadaniaTlumienia();

    void flt_zerowanieFiltrowDone();
    void flt_setUkladFiltrowDone();
    void flt_bladFiltrow(QChar silnik, bool zerowanie);

    void czujkaOn();

    bool getWynikBadania() const;

    const QString &getTlumienie() const;

    bool getPowtorzBadanie() const;
#ifdef DEFVAL
    void testValue();
#endif

    const QString &getError() const;


    short getPosFiltrA() const;

    short getPosFiltrB() const;

    short getPosFiltrC() const;

    bool getResetFiltrowOk();
    void setResetFiltrowOk(bool newResetFiltrowOk);

private slots:
    void uplynalCzasPostojuFiltra();
    void progressBarUpdate();
private:
    Ui::OknoBadaniaTlumienia *ui;
    QTimer zmFiltraTmr;
    QTimer zmProgressBar;
    QTimer resetFiltrow;
    float actTlumienie;
    QList<QStringList> tlumienia;
    float tlumieniaPercent;
    int dlugoscFali;
    unsigned int actTlumPos;
    unsigned int maxTlum;
    Sterownik * ster;
    int czasPostoju;
    bool resetFiltrowOk;
    bool wynikBadania;
    QString tlumienie;
    bool powtorzBadanie;
    bool wykryto;
    QString error;
    short posFiltrA;
    short posFiltrB;
    short posFiltrC;
    QMutex mutex;
};

#endif // OKNOBADANIATLUMIENIA_H
