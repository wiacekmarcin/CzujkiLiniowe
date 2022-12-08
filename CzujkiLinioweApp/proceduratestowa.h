#ifndef PROCEDURATESTOWA_H
#define PROCEDURATESTOWA_H

#include <QWidget>
#include "danetestu.h"
#include "parametrybadania.h"
class ProceduraTestowa
{
public:
    ProceduraTestowa(QWidget * widget);
    void startBadanie(short id, const QString &nameTest, const ParametryBadania &b);
protected:
    void Odtwarzalnosc(short id, const QString &nameTest, const ParametryBadania &b);
private:
    QWidget* parent;
    DaneTestu dane;
};

#endif // PROCEDURATESTOWA_H
