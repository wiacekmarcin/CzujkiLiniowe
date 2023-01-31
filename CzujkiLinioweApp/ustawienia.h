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
    unsigned long wyliczPozycje(short silnik, unsigned long middle, unsigned long max,
                                const double & ratioImpJedn, const double & x) const;
    inline unsigned long wyliczPozycje(short nrSilnika, const double & x) const
    { return wyliczPozycje(nrSilnika, getMotorIloscImpSrodek(nrSilnika), getMotorMaksIloscImp(nrSilnika),
                           getMotorPrzelozenieImpJedn(nrSilnika), x); }

    inline unsigned long predkoscRoboczaImp(short nrSilnika) const
    { return getMotorCzasMiedzyImpNormal(nrSilnika); }


    double convertImp2Value(short silnik, unsigned long impPos);

    double getFiltr_prc(unsigned short dlugoscfali, const char & nrTarczy, const short & nrPos) const;


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
