#include "ustawienia.h"

#include <QApplication>

char Ustawienia::appnazwa[]="CzujkiLiniowe";
char Ustawienia::firmnazwa[]="Kolodziejczyk";

Ustawienia::Ustawienia() :
    UstawieniaGen()
{
    if (!check()) {
        load();
        save();
        sync();
    }
    save();
    sync();
}

Ustawienia::~Ustawienia()
{
    save();
    sync();
}

