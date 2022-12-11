#ifndef PROCEDURATESTOWA_H
#define PROCEDURATESTOWA_H

#include <QObject>
#include <QWidget>
#include "danetestu.h"
#include "parametrybadania.h"
#include "zasilacz.h"
#include "sterownik.h"
#include "ustawienia.h"
class ProceduraTestowa
{

public:
    ~ProceduraTestowa();
    ProceduraTestowa(QWidget * widget);
    void startBadanie(short id, const QString &nameTest, const ParametryBadania &b,
                      const Ustawienia &ust, Zasilacz *zas, Sterownik *ster);
protected:
    void Odtwarzalnosc(short id, const QString &nameTest, const ParametryBadania &b, const Ustawienia & ust);
private:
    QWidget* parent;
    DaneTestu dane;
    Zasilacz * zas;
    Sterownik* ster;
    Ustawienia u;
};

#endif // PROCEDURATESTOWA_H
