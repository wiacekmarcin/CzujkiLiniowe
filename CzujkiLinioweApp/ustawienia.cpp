#include "ustawienia.h"

#include <QApplication>

char Ustawienia::appnazwa[]="CzujkiLiniowe";
char Ustawienia::firmnazwa[]="Kolodziejczyk";

Ustawienia::Ustawienia() :
    UstawieniaGen()
{
    //if (!check()) {
    //    load();
    //    save();
    //    sync();
    //}
    save();
    sync();
}

Ustawienia::~Ustawienia()
{
    save();
    sync();
}

double Ustawienia::wyliczPredkosc(const double &ratioImpJedn, const double &impTime)
{
    //impTime w us
    //wartosc zwracana w jedn/min;

    double cntImp1Min = 60 * 1000000 / impTime;
    return cntImp1Min*ratioImpJedn;
}

