#ifndef USTAWIENIA_H
#define USTAWIENIA_H

#include "ustawienia_gen.h"
#include <QString>
#include <QSettings>

class Ustawienia : public UstawieniaGen
{
public:
    Ustawienia();

private:

    static char appnazwa[];
    static char firmnazwa[];

};

#endif // USTAWIENIA_H
