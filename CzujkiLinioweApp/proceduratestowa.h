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
class OknoBadaniaKata;
class OknoCzekaniaBadanieKatowe;
class OknoBadanieReakcji6dB;
class OknoBadaniaMaksymalnegoKata;
class OknoStabilizacjaCzujki;
class OknoTestRozproszoneSwiatlo;
class ProceduraTestowa
{

public:
    ~ProceduraTestowa();
    ProceduraTestowa(QWidget * widget);
    bool startBadanie(short id, const QString &nameTest, ParametryBadania &b,
                      const Ustawienia &ust, Zasilacz *zas, Sterownik *ster);
    void flt_zerowanieFiltrowDone();
    void flt_setUkladFiltrowDone();
    void flt_bladFiltrow(QChar silnik, bool zerowanie);
    //void ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt);
    void ster_setPositionDone(short silnik, RuchSilnikaType ruch);
    void ster_setValue(short silnik, const double & val);
    void zas_value(int kind, int value);
    void czujkaOn(bool hardware);
    DaneTestu &getDane();

protected:
    bool Odtwarzalnosc(ParametryBadania &b, const Ustawienia & ust);
    bool Powtarzalnosc(const ParametryBadania &b, const Ustawienia & ust);
    bool Niewspolosiowosc(const ParametryBadania &b, const Ustawienia & ust);
    bool NiewspolosiowoscBadanie(const ParametryBadania &daneBadania, const Ustawienia &ust);
    bool SzybkieZmianyTlumienia(const ParametryBadania &daneBadania, const Ustawienia &ust);
    bool DlugoscDrogiOptycznej(const ParametryBadania &daneBadania, const Ustawienia &ust);
    bool RozproszoneSwiatlo(const ParametryBadania &daneBadania, const Ustawienia &ust);
    bool ZmienneParametryZasilania(const ParametryBadania &daneBadania, const Ustawienia &ust);
    bool KlimatyczneMechaniczneNarazenia(const ParametryBadania &daneBadania, const Ustawienia &ust);
    bool EMCNarazenia(const ParametryBadania &daneBadania, const Ustawienia &ust);

    bool ProbnyPomiar(ParametryBadania &daneBadania, const Ustawienia &ust);

private:
    bool parametryTest(short numerProby, const ParametryBadania &b, const Ustawienia & ust);

    bool montazZerowanieZasilanie(uint32_t flags, const ParametryBadania &daneBadania);


    bool oczekiwanieNaUrzadzenie(const ParametryBadania & daneBadania);
    bool zerowanieSterownika(uint32_t flags, const QString &trans, const QString &receiv);
    bool potwierdzenieNarazenia(DaneTestu &daneTestu, const ParametryBadania & daneBadania, const Ustawienia & ust);
    bool potwierdzenieNarazeniaEMC(DaneTestu &daneTestu, const ParametryBadania &, const Ustawienia &);
    bool zasilenieCzujki(uint32_t flags, unsigned long timeWait, const ParametryBadania &daneBadania);
    
    void stabilizacjaCzujki(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania, const Ustawienia &);
    short pomiarCzujki(uint32_t flags, const ParametryBadania &daneBadania, const Ustawienia &);
    void podsumowanie(DaneTestu &daneTestu, const ParametryBadania &badanie);
    short pomiarKataProcedura(PomiarKata & pomiar, short nrSilnika, const QString & ptitle, const ParametryBadania &daneBadania, const Ustawienia &ust);
    short pomiarKata(short nrSilnika, const QString & ptitle, const double & kat, const ParametryBadania &daneBadania, const Ustawienia &ust);
    short resetCzujki(const QString &testName, const QString &subTestName, unsigned int czasOffOn, unsigned int czasStabilizacji, const ParametryBadania &daneBadania);
private:
    QWidget* parent;
    DaneTestu dane;
    Zasilacz * zas;
    Sterownik* ster;
    Ustawienia u;
    OknoStabilizacjaCzujki *dlg6;
    OknoBadaniaTlumienia *dlg7;
    OknoZerowanieUrzadzenia *dlg0;
    OknoBadaniaKata *dlg10;
    OknoCzekaniaBadanieKatowe *dlg11;
    OknoBadanieReakcji6dB * dlg12;
    OknoBadaniaMaksymalnegoKata * dlg14;
    OknoTestRozproszoneSwiatlo * dlg15;

};

#endif // PROCEDURATESTOWA_H
