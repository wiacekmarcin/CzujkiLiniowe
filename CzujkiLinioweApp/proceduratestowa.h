#ifndef PROCEDURATESTOWA_H
#define PROCEDURATESTOWA_H

#include <QObject>
#include <QWidget>
#include "danetestu.h"
#include "parametrybadania.h"
#include "test7badanie.h"
#include "zasilacz.h"
#include "sterownik.h"
#include "ustawienia.h"


class Test7Badanie;
class Test0ZerowanieUrzadzenia;

class ProceduraTestowa
{

public:
    ~ProceduraTestowa();
    ProceduraTestowa(QWidget * widget);
    void startBadanie(short id, const QString &nameTest, const ParametryBadania &b,
                      const Ustawienia &ust, Zasilacz *zas, Sterownik *ster);
    void flt_zerowanieFiltrowDone();
    void flt_setUkladFiltrowDone();
    void flt_bladFiltrow(QChar silnik, bool zerowanie);
    void ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt);
protected:
    void Odtwarzalnosc(short id, const QString &nameTest, const ParametryBadania &b, const Ustawienia & ust);

private:
    bool oczekiwanieNaUrzadzenie();
    bool zerowanieSterownika();
    bool parametryTestu(short nrBadania, DaneTestu * daneTestu, const ParametryBadania & b, const Ustawienia & ust);
    bool potwierdzenieDanych(short nrBadania, const DaneTestu &daneTestu, const ParametryBadania & daneBadania, const Ustawienia & ust);
    bool potwierdzenieNarazenia(short nrBadania, const DaneTestu &daneTestu, const ParametryBadania & daneBadania, const Ustawienia & ust);
    bool montazCzujki(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania, const Ustawienia &);
    bool zasilenieCzujki(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania, const Ustawienia &);
    void stabilizacjaCzujki(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania, const Ustawienia &);
    void pomiarCzujki(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania, const Ustawienia &);
    void podsumowanie(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania, const Ustawienia &);
private:
    QWidget* parent;
    DaneTestu dane;
    Zasilacz * zas;
    Sterownik* ster;
    Ustawienia u;
    Test7Badanie *dlg7;
    Test0ZerowanieUrzadzenia *dlg0;
};

#endif // PROCEDURATESTOWA_H
