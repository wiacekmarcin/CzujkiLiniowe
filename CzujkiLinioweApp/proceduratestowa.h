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
    bool startBadanie(short id, const QString &nameTest, const ParametryBadania &b,
                      const Ustawienia &ust, Zasilacz *zas, Sterownik *ster);
    void flt_zerowanieFiltrowDone();
    void flt_setUkladFiltrowDone();
    void flt_bladFiltrow(QChar silnik, bool zerowanie);
    void ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt);
    void zas_value(int kind, int value);
    void czujkaOn();
    DaneTestu &getDane();

protected:
    bool Odtwarzalnosc(const ParametryBadania &b, const Ustawienia & ust);
    bool Powtarzalnosc(const ParametryBadania &b, const Ustawienia & ust);

private:
    bool oczekiwanieNaUrzadzenie(const ParametryBadania & daneBadania);
    bool zerowanieSterownika(bool ramiona, bool filtry, bool wozek);

    bool potwierdzenieNarazenia(short nrBadania, const DaneTestu &daneTestu, const ParametryBadania & daneBadania, const Ustawienia & ust);

    bool zasilenieCzujki(const ParametryBadania &daneBadania);
    void stabilizacjaCzujki(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania, const Ustawienia &);
    short pomiarCzujki(const ParametryBadania &daneBadania, bool repeatPomiar, bool nowait, const Ustawienia &);
    void podsumowanie(DaneTestu &daneTestu, const ParametryBadania &badanie);
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
