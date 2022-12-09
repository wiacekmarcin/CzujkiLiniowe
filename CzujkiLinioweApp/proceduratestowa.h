#ifndef PROCEDURATESTOWA_H
#define PROCEDURATESTOWA_H

#include <QObject>
#include <QWidget>
#include "danetestu.h"
#include "parametrybadania.h"
#include "zasilacz.h"

class ProceduraTestowa
{

public:
    ~ProceduraTestowa();
    ProceduraTestowa(QWidget * widget);
    void startBadanie(short id, const QString &nameTest, const ParametryBadania &b, Zasilacz *zas);
protected:
    void Odtwarzalnosc(short id, const QString &nameTest, const ParametryBadania &b);
private:
    QWidget* parent;
    DaneTestu dane;
    Zasilacz * zas;
};

#endif // PROCEDURATESTOWA_H
