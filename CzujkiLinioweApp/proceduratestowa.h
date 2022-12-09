#ifndef PROCEDURATESTOWA_H
#define PROCEDURATESTOWA_H

#include <QObject>
#include <QWidget>
#include "danetestu.h"
#include "parametrybadania.h"
#include "zasilacz.h"
#include "ustawienia.h"
class ProceduraTestowa
{

public:
    ~ProceduraTestowa();
    ProceduraTestowa(QWidget * widget);
    void startBadanie(short id, const QString &nameTest, const ParametryBadania &b,
                      const Ustawienia &ust, Zasilacz *zas);
protected:
    void Odtwarzalnosc(short id, const QString &nameTest, const ParametryBadania &b, const Ustawienia & ust);
private:
    QWidget* parent;
    DaneTestu dane;
    Zasilacz * zas;
    Ustawienia u;
};

#endif // PROCEDURATESTOWA_H
