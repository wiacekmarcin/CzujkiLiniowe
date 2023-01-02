#ifndef USTAWIENIA_H
#define USTAWIENIA_H

#include "ustawienia_gen.h"
#include <QString>
#include <QSettings>

struct UkladFiltrow {
    short A,B,C;
};

class Ustawienia : public UstawieniaGen
{
public:
    Ustawienia();
    ~Ustawienia();
    static double wyliczPredkosc(const double & ratioImpJedn, const double & impTime);
    static unsigned long wyliczImp(const double & ratioImpJedn, const double & speed);
    unsigned long wyliczPozycje(short silnik, unsigned long middle, const double & ratioImpJedn, const double & x);

    QList<QStringList> getTlumienia655() const;
    QList<QStringList> getTlumienia880() const;
protected:
    void loadListUstawienFiltra();
    void saveListUstawienFiltra();
private:

    static char appnazwa[];
    static char firmnazwa[];

    QMap<QString,UkladFiltrow> listaUstawienFiltra655;
    QMap<QString,UkladFiltrow> listaUstawienFiltra880;
};

#endif // USTAWIENIA_H
