#ifndef TEST7BADANIE_H
#define TEST7BADANIE_H

#include <QDialog>
#include <QTimer>

#include "danetestu.h"
#include "parametrybadania.h"
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
class Test7Badanie;
}

class Sterownik;
class Test7Badanie : public QDialog
{
    Q_OBJECT

public:
    explicit Test7Badanie(short nrPomiaru, const DaneTestu &daneTestu,
                          const ParametryBadania &daneBadania, const Ustawienia &,
                          Sterownik * ster, QWidget *parent = 0);
    ~Test7Badanie();

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

private slots:
    void uplynalCzasPostojuFiltra();
    void progressBarUpdate();
private:
    Ui::Test7Badanie *ui;
    QTimer zmFiltraTmr;
    QTimer zmProgressBar;
    QTimer resetFiltrow;
    float actTlumienie;
    QList<QStringList> tlumienia;
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
};

#endif // TEST7BADANIE_H
