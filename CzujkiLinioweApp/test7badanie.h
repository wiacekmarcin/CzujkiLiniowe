#ifndef TEST7BADANIE_H
#define TEST7BADANIE_H

#include <QDialog>
#include <QTimer>

#include "danetestu.h"
#include "parametrybadania.h"
#include "ustawienia.h"
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

private slots:
    void uplynalCzasPostojuFiltra();
    void progressBarUpdate();
private:
    Ui::Test7Badanie *ui;
    QTimer zmFiltraTmr;
    QTimer zmProgressBar;
    float actTlumienie;
    QList<QStringList> tlumienia;
    unsigned int actTlumPos;
    unsigned int maxTlum;
    Sterownik * ster;
    int czasPostoju;
};

#endif // TEST7BADANIE_H
