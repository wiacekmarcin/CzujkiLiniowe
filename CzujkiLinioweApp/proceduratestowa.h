#ifndef PROCEDURATESTOWA_H
#define PROCEDURATESTOWA_H

#include <QObject>
#include <QWidget>
#include "danetestu.h"
#include "parametrybadania.h"
#include "oknobadaniatlumienia.h"
#include "zasilacz.h"
#include "sterownik.h"
#include "ustawienia.h"


class OknoBadaniaTlumienia;
class OknoZerowanieUrzadzenia;

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
    bool Niewspolosiowosc(const ParametryBadania &b, const Ustawienia & ust);

private:
    bool parametryTest(short numerProby, const ParametryBadania &b, const Ustawienia & ust);

    bool oczekiwanieNaUrzadzenie(const ParametryBadania & daneBadania);
    bool zerowanieSterownika(bool ramiona, bool filtry, bool wozek);

    bool potwierdzenieNarazenia(const DaneTestu &daneTestu, const ParametryBadania & daneBadania, const Ustawienia & ust);

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
    OknoBadaniaTlumienia *dlg7;
    OknoZerowanieUrzadzenia *dlg0;
};

#endif // PROCEDURATESTOWA_H
