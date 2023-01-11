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
#include "test7badanie.h"
#include "test8wynik.h"
#include "test9podsumowanie.h"
#include "zasilacz.h"
#include <QMessageBox>
#include <QSharedPointer>

ProceduraTestowa::ProceduraTestowa(QWidget * widget):
    parent(widget),
    zas(nullptr),
    ster(nullptr),
    dlg7(nullptr),
    dlg0(nullptr)
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

void ProceduraTestowa::flt_bladFiltrow(QChar silnik, bool zerowanie)
{
    if (dlg7)
        dlg7->flt_bladFiltrow(silnik, zerowanie);
}

void ProceduraTestowa::ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt)
{
    if (dlg0)
        dlg0->ster_setPositionDone(silnik, home, move, error, interrupt);
}

void ProceduraTestowa::startBadanie(short id, const QString & nameTest, const ParametryBadania & b,
                                    const Ustawienia & ust, Zasilacz * zas_, Sterownik * ster_)
{
    zas = zas_;
    ster = ster_;


    //if (!oczekiwanieNaUrzadzenie())
    //    return;

    //if(!zerowanieSterownika())
    //    return;


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
    short powtorzPomiar;
    for (short nrPom = 1; nrPom <= daneBadania.getIloscCzujek(); ++nrPom)
    {
        if (!parametryTestu(nrPom, &nowyTest, daneBadania, ust))
            return;

        if (nrPom == 1) {
            if (!potwierdzenieDanych(nrPom, nowyTest, daneBadania, ust))
                return;
        }

        if (!montazCzujki(nrPom, nowyTest, daneBadania, ust))
            return;

        if (!ster->getConnected() || !zas->getConnected()) {
            if (!oczekiwanieNaUrzadzenie(daneBadania))
                return;
        }

        do {
            if(!zerowanieSterownika())
                return;

            if (!zasilenieCzujki(nrPom, nowyTest, daneBadania, ust))
                return;

            stabilizacjaCzujki(nrPom, nowyTest, daneBadania, ust);

            powtorzPomiar = pomiarCzujki(nrPom, nowyTest, daneBadania, ust);
            if (powtorzPomiar == -1)
                return;

        } while(powtorzPomiar);
    }
    podsumowanie(nowyTest, daneBadania);
}

bool ProceduraTestowa::oczekiwanieNaUrzadzenie(const ParametryBadania & daneBadania)
{
    OczekiwanieNaUrzadzenia *dlg = new OczekiwanieNaUrzadzenia(daneBadania.getZasCzujekWbudZasilacz(), parent);

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
    dlg0 = new Test0ZerowanieUrzadzenia(ster, parent);

    if (!dlg0->exec()) {
#ifdef DEFVAL

#else
        delete dlg0;
        return false;
#endif
    }
    delete dlg0;
    dlg0 = nullptr;
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

short ProceduraTestowa::pomiarCzujki(short nrPomiaru, DaneTestu &daneTestu, const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    bool ret = true;
    dlg7 = new Test7Badanie(nrPomiaru, daneTestu, daneBadania, ust, ster, parent);
    dlg7->exec();

    Test8Wynik * dlg8 = new Test8Wynik(dlg7->getWynikBadania(), dlg7->getTlumienie(), daneTestu, parent);
    if(!dlg8->exec()) {
        return -1;
    } else {
        if (dlg8->getPowtorzPomiar()) {
            return 1;
        }
        if (dlg7->getTlumienie().toDouble() < 0.4) {
            daneTestu.setSuccessBadaniaCzujki(false, dlg7->getTlumienie(), "Crep<0.4");
        } else {
            daneTestu.setSuccessBadaniaCzujki(dlg7->getWynikBadania(), dlg7->getTlumienie(), dlg7->getError());
        }
    }

    delete dlg7;
    dlg7 = nullptr;

    delete dlg8;

    return 0;
}

void ProceduraTestowa::podsumowanie(const DaneTestu &daneTestu, const ParametryBadania & badanie)
{
    Test9Podsumowanie * dlg = new Test9Podsumowanie(daneTestu, badanie);
    dlg->exec();
}
