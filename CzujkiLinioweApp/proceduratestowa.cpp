#include "proceduratestowa.h"
#include "oczekiwanienaurzadzenia.h"
#include "sterownik.h"
#include "test0zerowanieurzadzenia.h"
#include "test1parametrytestu.h"
#include "test2potwierdzenie.h"
#include "test3sprawdzenie.h"
#include "test4montaz.h"
#include "test5zasilanieczujki.h"
#include "test6stabilizacjaczujki.h"
#include "zasilacz.h"
#include <QMessageBox>
#include <QSharedPointer>

ProceduraTestowa::ProceduraTestowa(QWidget * widget):
    parent(widget),
    zas(nullptr),
    ster(nullptr),
    dlg7(nullptr)
{

}

ProceduraTestowa::~ProceduraTestowa()
{

}

void ProceduraTestowa::flt_zerowanieFiltrowDone()
{
    if (dlg7) {
        dlg7->flt_zerowanieFiltrowDone();
    }
}

void ProceduraTestowa::flt_setUkladFiltrowDone()
{
    if (dlg7)
        dlg7->flt_setUkladFiltrowDone();
}

void ProceduraTestowa::flt_bladFiltrow(short silnik, bool zerowanie)
{
    if (dlg7)
        dlg7->flt_bladFiltrow(silnik, zerowanie);
}

void ProceduraTestowa::startBadanie(short id, const QString & nameTest, const ParametryBadania & b,
                                    const Ustawienia & ust, Zasilacz * zas_, Sterownik * ster_)
{
    zas = zas_;
    ster = ster_;


    if (!oczekiwanieNaUrzadzenie())
        return;

    if(!zerowanieSterownika())
        return;


    switch(id) {
    case 0:
        Odtwarzalnosc(id, nameTest, b, ust);
        break;
    default:
        QMessageBox::warning(parent, QString("Badanie"), QString("Dane badanie nie zostało zaimplementowane"));
        break;
    }


}



void ProceduraTestowa::Odtwarzalnosc(short id, const QString & nameTest, const ParametryBadania & daneBadania, const Ustawienia & ust)
{
    DaneTestu nowyTest;
    nowyTest.setId(id);
    nowyTest.setName(nameTest);
    for (short nrPom = 1; nrPom <= daneBadania.getIloscCzujek(); ++nrPom)
    {
        if (!parametryTestu(nrPom, &nowyTest, daneBadania, ust))
            return;

        if (nrPom == 0) {
            if (!potwierdzenieDanych(nrPom, nowyTest, daneBadania, ust))
                return;
        }

        if (!montazCzujki(nrPom, nowyTest, daneBadania, ust))
            return;

        if (!zasilenieCzujki(nrPom, nowyTest, daneBadania, ust))
            return;

        stabilizacjaCzujki(nrPom, nowyTest, daneBadania, ust);

        pomiarCzujki(nrPom, nowyTest, daneBadania, ust);

        podsumowanie(nrPom, nowyTest, daneBadania, ust);
    }
}

bool ProceduraTestowa::oczekiwanieNaUrzadzenie()
{
    OczekiwanieNaUrzadzenia *dlg = new OczekiwanieNaUrzadzenia(parent);

    dlg->connect(zas, &Zasilacz::kontrolerConfigured, dlg, &OczekiwanieNaUrzadzenia::zasilacz);
    dlg->connect(ster, &Sterownik::kontrolerConfigured, dlg, &OczekiwanieNaUrzadzenia::sterownik);
    zas->connectToDevice();
    ster->connectToDevice();

    if (!dlg->exec()) {
#ifdef DEFVAL

#else
        delete dlg;
        return false;
#endif
    }
    delete dlg;
    return true;
}

bool ProceduraTestowa::zerowanieSterownika()
{
    Test0ZerowanieUrzadzenia *dlg = new Test0ZerowanieUrzadzenia(parent);

    //dlg->connect(ster, &Sterownik::kontrolerConfigured, dlg, &OczekiwanieNaUrzadzenia::sterownik);
    if (!dlg->exec()) {
#ifdef DEFVAL

#else
        delete dlg;
        return false;
#endif
    }
    delete dlg;
    return true;
}

bool ProceduraTestowa::parametryTestu(short nrPomiaru, DaneTestu *daneTestu, const ParametryBadania &daneBadania,
                                      const Ustawienia &)
{
    Test1ParametryTestu *dlg1 = new Test1ParametryTestu(nrPomiaru, daneTestu, daneBadania, parent);
    if (!dlg1->exec()) {
        delete dlg1;
        return false;
    }
    delete dlg1;
         return true;
}

bool ProceduraTestowa::potwierdzenieDanych(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania,
                                           const Ustawienia &)
{
    Test2Potwierdzenie *dlg2 = new Test2Potwierdzenie(nrPomiaru, daneTestu, daneBadania, parent);
    if (!dlg2->exec()) {
        delete dlg2;
        return false;
    }
    delete dlg2;
    return true;
}

bool ProceduraTestowa::potwierdzenieNarazenia(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania,
                                              const Ustawienia &)
{
    Test3Sprawdzenie *dlg3 = new Test3Sprawdzenie(nrPomiaru, daneTestu, daneBadania, parent);
    if (!dlg3->exec()) {
        delete dlg3;
        return false;
    }
    delete dlg3;
    return true;
}
bool ProceduraTestowa::montazCzujki(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania,
                                    const Ustawienia &)
{
    Test4Montaz *dlg4 = new Test4Montaz(nrPomiaru, daneTestu, daneBadania, parent);
    if (!dlg4->exec()) {
        delete dlg4;
        return false;
    }
    delete dlg4;
    return true;
}

bool ProceduraTestowa::zasilenieCzujki(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania,
                                       const Ustawienia &)
{
    //ustawienia zasilania czujki z zasilacza
    if (daneBadania.getZasCzujekWbudZasilacz()) {
        zas->setVoltage_mV(daneBadania.getNapiecieZasCzujki_mV());
        zas->setOutput(true);
    }
    //TODO
    if (!daneBadania.getWyzwalanieAlarmuPrzekaznik()) {
    //    zas->setCurrentLimit_mA(b.getPrzekroczeniePraduZasilania_mA());
    }

    Test5ZasilanieCzujki *dlg5 = new Test5ZasilanieCzujki(nrPomiaru, daneTestu, daneBadania, parent);
    dlg5->connect(zas, &Zasilacz::value, dlg5, &Test5ZasilanieCzujki::value);
    if (!dlg5->exec()) {
        zas->setOutput(false);
        delete dlg5;
        return false;
    }
    delete dlg5;
    return true;
}

void ProceduraTestowa::stabilizacjaCzujki(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania, const Ustawienia &)
{
    Test6StabilizacjaCzujki *dlg6 = new Test6StabilizacjaCzujki(nrPomiaru, daneTestu, daneBadania, parent);
    dlg6->exec();
    delete dlg6;
}
void ProceduraTestowa::pomiarCzujki(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    dlg7 = new Test7Badanie(nrPomiaru, daneTestu, daneBadania, ust, parent);
    dlg7->exec();
    delete dlg7;
    dlg7 = nullptr;
}

void ProceduraTestowa::podsumowanie(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania, const Ustawienia &)
{

}
