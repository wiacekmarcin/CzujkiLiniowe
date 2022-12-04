#ifndef USTAWIENIA_H
#define USTAWIENIA_H

#include "ustawienia_gen.h"
#include <QString>
#include <QSettings>

class Ustawienia : public UstawieniaGen
{
public:
    Ustawienia();
    ~Ustawienia();
    static double wyliczPredkosc(const double & ratioImpJedn, const double & impTime);
private:

    static char appnazwa[];
    static char firmnazwa[];

};

#endif // USTAWIENIA_H
