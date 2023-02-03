#include "proceduratestowa.h"
#include "oczekiwanienaurzadzenia.h"
#include "sterownik.h"
#include "oknozerowanieurzadzenia.h"
#include "oknoparametrytestu.h"
#include "oknosprawdzeniedanych.h"
#include "oknopotwierdzenienarazenia.h"
#include "oknomontaz.h"
#include "oknozasilaniaczujki.h"
#include "oknostabilizacjaczujki.h"
#include "oknobadaniatlumienia.h"
#include "oknowynikbadaniatlumienia.h"
#include "oknopodsumowanietestu.h"
#include "oknobadaniakata.h"
#include "oknoczekaniabadaniekatowe.h"
#include "oknobadaniereakcji6db.h"
#include "oknobadaniamaksymalnegokata.h"
#include "oknoresetuzasilaniaczujki.h"
#include "oknotestrozproszoneswiatlo.h"

#include "zasilacz.h"
#include <QMessageBox>
#include <QSharedPointer>
#include <QDebug>

#define ZER_FILTRY          0x1 << 1
#define ZER_NADAJNIK        0x1 << 2
#define ZER_ODBIORNIK       0x1 << 3
#define ZER_WOZEK           0x1 << 4
#define MIN_NAPIECIE        0x1 << 5
#define MAX_NAPIECIE        0x1 << 6
#define MIN_ROZSTAW         0x1 << 7
#define MAX_ROZSTAW         0x1 << 8
#define MIN_CZULOSC         0x1 << 9
#define RESET_CZUJKI        0x1 << 10
#define NO_STAB_CZUJKI      0x1 << 11
#define USUN_ZABEZP         0x1 << 12
#define SW_ROZPR            0x1 << 13
#define POWT_POMIAR         0x1 << 14
#define NIE_POK_WYNIK_POM   0x1 << 15
#define TEST_POMIAR         0x1 << 16

ProceduraTestowa::ProceduraTestowa(QWidget * widget):
    parent(widget),
    zas(nullptr),
    ster(nullptr),
    dlg6(nullptr),
    dlg7(nullptr),
    dlg0(nullptr),
    dlg10(nullptr),
    dlg11(nullptr),
    dlg12(nullptr),
    dlg14(nullptr),
    dlg15(nullptr)
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
    if (dlg12) {
        dlg12->flt_zerowanieFiltrowDone();
    }
}

void ProceduraTestowa::flt_setUkladFiltrowDone()
{
    if (dlg7)
        dlg7->flt_setUkladFiltrowDone();
    if (dlg12) {
        dlg12->flt_setUkladFiltrowDone();
    }
}

void ProceduraTestowa::flt_bladFiltrow(QChar silnik, bool zerowanie)
{
    if (dlg7)
        dlg7->flt_bladFiltrow(silnik, zerowanie);
    if (dlg12)
        dlg12->flt_bladFiltrow(silnik, zerowanie);
}

void ProceduraTestowa::ster_setPositionDone(short silnik, RuchSilnikaType ruch)
{
    if (dlg0)
        dlg0->ster_setPositionDone(silnik, ruch);
    if (dlg10)
        dlg10->ster_setPositionDone(silnik, ruch);
}

void ProceduraTestowa::zas_value(int , int )
{

}

void ProceduraTestowa::ster_setValue(short silnik, const double & val)
{
    qDebug() << silnik << val;
    if (dlg10)
        dlg10->ster_setValue(silnik, val);

    if (dlg14)
        dlg14->ster_setValue(silnik, val);
}

void ProceduraTestowa::czujkaOn(bool hardware)
{
    if (dlg6)
        dlg6->czujkaOn();
    else if (dlg7)
       dlg7->czujkaOn();
    else if (dlg10)
        dlg10->czujkaOn();
    else if (dlg11)
        dlg11->czujkaOn();
    else if (dlg12)
        dlg12->czujkaOn();
    else if (dlg14)
        dlg14->czujkaOn();
    else if (dlg15)
        dlg15->czujkaOn();
    if (!hardware)
        ster->setStopMotorAll();
}

bool ProceduraTestowa::startBadanie(short id, const QString & nameTest, ParametryBadania & b,
                                    const Ustawienia & ust, Zasilacz * zas_, Sterownik * ster_)
{
    zas = zas_;
    ster = ster_;

    dane = DaneTestu();
    dane.setId(id);
    dane.setName(nameTest);
    dane.setDlugoscFali(b.getDlugoscFaliFiltrow());
    switch(id) {
    case REPRODUCIBILITY:
        return Odtwarzalnosc(b, ust);

    case REPEATABILITY:
        return Powtarzalnosc(b, ust);

    case TOLERANCE_TO_BEAM_MISALIGNMENT:
        return Niewspolosiowosc(b, ust);

    case RAPID_CHANGES_IN_ATTENUATION:
        return SzybkieZmianyTlumienia(b, ust);

    case OPTICAL_PATH_LENGTH_DEPEDENCE:
        return DlugoscDrogiOptycznej(b, ust);

    case STRAY_LIGHT:
        return RozproszoneSwiatlo(b, ust);

    case TOLERANCE_TO_SUPPLY_VOLTAGE:
        return ZmienneParametryZasilania(b, ust);

    case DRY_HEAT:
    case COLD:
    case DAMP_HEAT_STADY_STATE_OPERATIONAL:
    case DAMP_HEAT_STADY_STATE_ENDURANCE:
    case VIBRATION:
    case IMPACT:
    case SULPHUR_DIOXIDE_SO2_CORROSION:
        return KlimatyczneMechaniczneNarazenia(b, ust);
        return false;

    case TEST_MEASUREAMENT:
        return ProbnyPomiar(b, ust);
    default:
        QMessageBox::warning(parent, QString::fromUtf8("Badanie"), QString("Dane badanie nie zostało zaimplementowane"));
        break;
    }

    return false;
}

bool ProceduraTestowa::ProbnyPomiar(ParametryBadania & daneBadania, const Ustawienia & ust)
{
    short powtorzPomiar;
    do {

        if (!ster->getConnected() || (daneBadania.getWyzwalanieAlarmuPradem() && !zas->getConnected())) {
            if (!oczekiwanieNaUrzadzenie(daneBadania))
                return false;
        }

        if(!zerowanieSterownika(ZER_FILTRY, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
            return false;

        if (!zasilenieCzujki(0, daneBadania.getCzasStabilizacjiCzujki_s(), daneBadania))
            return false;

        powtorzPomiar = pomiarCzujki(POWT_POMIAR | TEST_POMIAR, daneBadania, ust);
        if (powtorzPomiar == -1)
            return false;
    } while(powtorzPomiar);
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);
    return false;
}


bool ProceduraTestowa::Odtwarzalnosc(ParametryBadania & daneBadania, const Ustawienia & ust)
{
    short powtorzPomiar;
    for (short nrPom = 1; nrPom <= daneBadania.getIloscWszystkichCzujek(); ++nrPom)
    {
        if (!parametryTest(nrPom, daneBadania, ust))
            return false;

        do {

            if (!montazZerowanieZasilanie(ZER_FILTRY , daneBadania))
                return false;

            powtorzPomiar = pomiarCzujki(POWT_POMIAR, daneBadania, ust);
            if (powtorzPomiar == -1)
                return false;
        } while(powtorzPomiar);
        if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
            zas->setOutput(false);
    }

    zerowanieSterownika(ZER_FILTRY, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver());
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);

    dane.obliczOdtwarzalnosc(&daneBadania, ust);
    dane.setDataZakonczenia();
    dane.setWykonany(true);
    {
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    }
    return true;
}

bool ProceduraTestowa::Powtarzalnosc(const ParametryBadania & daneBadania, const Ustawienia & ust)
{
    short powtorzPomiar;

    //1 pomiar
    if (!parametryTest(1, daneBadania, ust))
        return false;

    do {
        if (!montazZerowanieZasilanie(ZER_FILTRY , daneBadania))
            return false;

        powtorzPomiar = pomiarCzujki(POWT_POMIAR, daneBadania, ust);


        if (powtorzPomiar == -1)
            return false;
    } while (powtorzPomiar != 0);

    dane.addNextPomiar();
    if(!zerowanieSterownika(ZER_FILTRY, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
        return false;

    resetCzujki(dane.getName(), "Resetowanie czujki dla testu powtarzalności",  ust.getCzasWylaczeniaCzujkiDlaResetu(),
                     daneBadania.getCzasStabilizacjiPoResecie_s(), daneBadania );

    // 2 pomiar i 3 pomiar i 4 pomiar
    for (short nr = 2; nr < 5; ++nr) {
        bool alarmCzujki = false;
        do {
            dlg6 = new OknoStabilizacjaCzujki(false, false, false,
                     (nr == 4 ? ust.getCzasOczekiwaniaPowtarzalnosc4Test() : dane.getCzasPowtarzalnosci()),
                                              dane.getName(),
                                              QString::fromUtf8("Oczekiwanie na %1 pomiar powtarzalności").arg(nr), parent);
            alarmCzujki = dlg6->exec() != QDialog::Accepted;
            delete dlg6;
            dlg6 = nullptr;
        } while (false);
        if (alarmCzujki) {
            dane.setSuccessBadaniaCzujki(false, "-", QString::fromUtf8("Czujka zgłosiła alarm podczas czas bezczynności"));
        } else {
            pomiarCzujki(NIE_POK_WYNIK_POM, daneBadania, ust);
        }

        if (nr < 4) //kolejny pomiar
            dane.addNextPomiar();

        if(!zerowanieSterownika(ZER_FILTRY, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
            return false;

        if (nr < 4)
            resetCzujki(dane.getName(), QString::fromUtf8("Resetowanie czujki dla testu powtarzalności"),
                        ust.getCzasWylaczeniaCzujkiDlaResetu(),
                         daneBadania.getCzasStabilizacjiPoResecie_s(), daneBadania );
        else {
            if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
                zas->setOutput(false);
        }
    }
    dane.obliczPowtarzalnosc(ust);
    dane.setDataZakonczenia();
    dane.setWykonany(true);
    do {

        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    } while(false);
    return true;
}

bool ProceduraTestowa::Niewspolosiowosc(const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    if (!parametryTest(1, daneBadania, ust))
        return false;

    if (!montazZerowanieZasilanie(NO_STAB_CZUJKI | ZER_FILTRY | ZER_NADAJNIK | ZER_ODBIORNIK | USUN_ZABEZP, daneBadania))
        return false;

    if (!NiewspolosiowoscBadanie(daneBadania, ust))
        return false;

    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);

    dane.obliczZaleznoscKatowa(ust);
    dane.setDataZakonczenia();
    dane.setWykonany(true);
    do {
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    } while(false);
    return true;

}

bool ProceduraTestowa::SzybkieZmianyTlumienia(const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    if (!parametryTest(1, daneBadania, ust))
        return false;

    if (!montazZerowanieZasilanie(ZER_FILTRY, daneBadania))
        return false;

    bool testOk1, testOk2;
    dlg12 = new OknoBadanieReakcji6dB(ust.getMaksCzasZadzialaniaCzujkidlaTlumnikaA(),
                                        ust.getMaksCzasTestuCzujkidlaTlumnikaA(),
                                        daneBadania.getDlugoscFaliFiltrow(),
                                        ust.getSzybkieZmianyWartoscTlumnikaA(),
                                        dane.getName(), QString::fromUtf8("Pomiar dla tłumnika A"), ust, ster, parent);
    testOk1 = dlg12->exec() == QDialog::Accepted;
    dane.setSuccessBadaniaCzujki(testOk1, QString::number(ust.getSzybkieZmianyWartoscTlumnikaA(), 'f', 1), dlg12->getError());
    delete dlg12;
    dlg12 = nullptr;

    dane.addNextPomiar();

    if(!zerowanieSterownika(ZER_FILTRY, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
        return false;

    resetCzujki(dane.getName(), QString::fromUtf8("Resetowanie czujki dla testu szybkie zmiany tłumienia"),
                ust.getCzasWylaczeniaCzujkiDlaResetu(),
                     daneBadania.getCzasStabilizacjiPoResecie_s(), daneBadania );


    dlg12 = new OknoBadanieReakcji6dB(ust.getMaksCzasZadzialaniaCzujkidlaTlumnikaB(),
                                        ust.getMaksCzasTestuCzujkidlaTlumnikaB(),
                                        daneBadania.getDlugoscFaliFiltrow(),
                                        ust.getSzybkieZmianyWartoscTlumnikaB(),
                                        dane.getName(), QString::fromUtf8("Pomiar dla tłumnika B"), ust, ster, parent);
    testOk2 = dlg12->exec() == QDialog::Accepted;
    dane.setSuccessBadaniaCzujki(testOk2, QString::number(ust.getSzybkieZmianyWartoscTlumnikaB(), 'f', 1), dlg12->getError());
    delete dlg12;
    dlg12 = nullptr;


    zerowanieSterownika(ZER_FILTRY, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver());
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);

    dane.setOk(testOk1 && testOk2);
    dane.setDataZakonczenia();
    dane.setWykonany(true);
    dane.obliczSzybkieZmianyTlumienia(ust);

    do {
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    } while(false);
    return true;
}

bool ProceduraTestowa::DlugoscDrogiOptycznej(const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    if (!parametryTest(1, daneBadania, ust))
        return false;

    short powtorzPomiar;
    bool ok1, ok2;
    do {
        if (!montazZerowanieZasilanie(ZER_FILTRY | MIN_ROZSTAW, daneBadania))
            return false;

        powtorzPomiar = pomiarCzujki(POWT_POMIAR, daneBadania, ust);

        if (powtorzPomiar == -1)
            return false;
        ok1 = powtorzPomiar == 0;
    } while (powtorzPomiar != 0);

    dane.addNextPomiar();

    if(!zerowanieSterownika(ZER_FILTRY, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
        return false;

    resetCzujki(dane.getName(), QString::fromUtf8("Resetowanie czujki dla testu szubkie zmiany tłumienia"),
                    ust.getCzasWylaczeniaCzujkiDlaResetu(),
                     daneBadania.getCzasStabilizacjiPoResecie_s(), daneBadania );

    do {
        if (!montazZerowanieZasilanie(ZER_FILTRY | MAX_ROZSTAW, daneBadania))
            return false;

        powtorzPomiar = pomiarCzujki(POWT_POMIAR, daneBadania, ust);

        if (powtorzPomiar == -1)
            return false;
        ok2 = powtorzPomiar == 0;
    } while (powtorzPomiar != 0);

    if(!zerowanieSterownika(ZER_FILTRY, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
        return false;

    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);

    dane.setOk(ok1 & ok2);
    dane.setDataZakonczenia();
    dane.setWykonany(true);
    dane.obliczDlugoscOptyczna(ust);

    do {
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    } while(false);
    return true;
}

bool ProceduraTestowa::RozproszoneSwiatlo(const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    if (!parametryTest(1, daneBadania, ust))
        return false;

    if (!montazZerowanieZasilanie(ZER_FILTRY | SW_ROZPR, daneBadania))
        return false;

    OknoTestRozproszoneSwiatlo * dlg15 = new OknoTestRozproszoneSwiatlo(dane, parent);
    bool ret = dlg15->exec() == QDialog::Accepted;

    if (!ret) {
        delete dlg15;
        dlg15 = nullptr;
        return false;
    }
    dane.setWynikNarazenia(!dlg15->getAlarm());
    dane.setInfoNarazenia(dlg15->getInfo());
    bool ok1 = !dlg15->getAlarm();
    if (dlg15->getAlarm()) {
        dane.setOk(false);
        dane.setErrStr("Czujka nie przeszła testu narażenia");
    }
    delete dlg15;
    dlg15 = nullptr;
    bool ok2 = true;
    ok2 = pomiarCzujki(0, daneBadania, ust) == QDialog::Accepted;

    if(!zerowanieSterownika(ZER_FILTRY, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
        return false;

    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);


    dane.setOk(ok1 & ok2);
    dane.setDataZakonczenia();
    dane.setWykonany(true);
    dane.obliczTestNarazenia(dane.getId(), ust);
    do {
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    } while(false);
    return true;
}

bool ProceduraTestowa::ZmienneParametryZasilania(const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    if (!parametryTest(1, daneBadania, ust))
        return false;

    short powtorzPomiar;
    bool ok1, ok2;
    do {
        if (!montazZerowanieZasilanie(ZER_FILTRY | MIN_NAPIECIE, daneBadania))
            return false;

        powtorzPomiar = pomiarCzujki(POWT_POMIAR, daneBadania, ust);

        if (powtorzPomiar == -1)
            return false;
        ok1 = powtorzPomiar == 0;
    } while (powtorzPomiar != 0);

    dane.addNextPomiar();

    if(!zerowanieSterownika(ZER_FILTRY, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
        return false;

    resetCzujki(dane.getName(), QString::fromUtf8("Resetowanie czujki dla testu szubkie zmiany tłumienia"),
                    ust.getCzasWylaczeniaCzujkiDlaResetu(),
                     daneBadania.getCzasStabilizacjiPoResecie_s(), daneBadania );

    do {
        if (!montazZerowanieZasilanie(ZER_FILTRY | MAX_NAPIECIE, daneBadania))
            return false;

        powtorzPomiar = pomiarCzujki(POWT_POMIAR, daneBadania, ust);

        if (powtorzPomiar == -1)
            return false;
        ok2 = powtorzPomiar == 0;
    } while (powtorzPomiar != 0);

    if(!zerowanieSterownika(ZER_FILTRY, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
        return false;

    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);

    dane.setOk(ok1 & ok2);
    dane.setDataZakonczenia();
    dane.setWykonany(true);
    dane.obliczZaleznoscNapieciaZasilania(ust);

    do {
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    } while(false);
    return true;
}

bool ProceduraTestowa::KlimatyczneMechaniczneNarazenia(const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    if (!parametryTest(1, daneBadania, ust))
        return false;
    
    bool ret = potwierdzenieNarazenia(dane, daneBadania, ust);
    if (!ret)
        return false;

    short powtorzPomiar;
    bool ok1;
    do {
        if (!montazZerowanieZasilanie(ZER_FILTRY, daneBadania))
            return false;

        powtorzPomiar = pomiarCzujki(POWT_POMIAR, daneBadania, ust);

        if (powtorzPomiar == -1)
            return false;
        ok1 = powtorzPomiar == 0;
    } while (powtorzPomiar != 0);

    if(!zerowanieSterownika(ZER_FILTRY, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
        return false;

    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);

    dane.setOk(dane.getWynikNarazenia() && ok1);
    dane.setDataZakonczenia();
    dane.setWykonany(true);

    dane.obliczTestNarazenia(dane.getId(), ust);
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);

    do {
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    } while(false);

    return true;
}




bool ProceduraTestowa::parametryTest(short numerProby, const ParametryBadania &daneBadania, const Ustawienia & ust)
{

    QSharedPointer<OknoParametryTestu> dlg1(new OknoParametryTestu(numerProby, &dane, daneBadania, ust, parent));
    if (dlg1->exec() == QDialog::Rejected)
        return false;

    QSharedPointer<OknoSprawdzenieDanych> dlg2(new OknoSprawdzenieDanych(dane, parent));
    if (dlg2->exec() == QDialog::Rejected)
        return false;
    return true;
}



bool ProceduraTestowa::oczekiwanieNaUrzadzenie(const ParametryBadania & daneBadania)
{
    QSharedPointer<OczekiwanieNaUrzadzenia> dlg(new OczekiwanieNaUrzadzenia(
                                                    daneBadania.getZasilanieCzujekZasilaczZewnetrzny(),
                                                    zas, ster, parent));
    if (dlg->exec() == QDialog::Rejected) {
        return false;
    }
    return true;
}

bool ProceduraTestowa::zerowanieSterownika(uint32_t flags, const QString & trans, const QString & receiv )
{
    dlg0 = new OknoZerowanieUrzadzenia(flags & ZER_NADAJNIK, flags & ZER_ODBIORNIK, flags & ZER_FILTRY, flags & ZER_WOZEK, trans, receiv, ster, parent);

    int ret = dlg0->exec() == QDialog::Accepted;
    delete dlg0;
    dlg0 = nullptr;
    return ret;
}


bool ProceduraTestowa::potwierdzenieNarazenia(DaneTestu &daneTestu, const ParametryBadania &,
                                              const Ustawienia &)
{
    OknoPotwierdzenieNarazenia *dlg3 = new OknoPotwierdzenieNarazenia(daneTestu, parent);
    bool ret = dlg3->exec() == QDialog::Accepted;
    daneTestu.setWynikNarazenia(dlg3->getWynik());
    daneTestu.setInfoNarazenia(dlg3->getKomenatarz());
    delete dlg3;
    return ret;
}


bool ProceduraTestowa::zasilenieCzujki(uint32_t flags, unsigned long timeWait,
                                       const ParametryBadania &daneBadania)
{

    bool minVoltage = flags & MIN_NAPIECIE;
    bool maxVoltage = flags & MAX_NAPIECIE;
    bool minCzulosc = flags & MIN_CZULOSC;
    bool noStab = flags & NO_STAB_CZUJKI;
    
    //ustawienia zasilania czujki z zasilacza
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny()) {
        if (minVoltage) {
            double val = dane.getMinimalneNapiecie().toDouble()*1000;
            zas->setVoltage_mV(val);
            zas->setOutput(true);
        } else if (maxVoltage) {
            double val = dane.getMaksymalneNapiecie().toDouble()*1000;
            zas->setVoltage_mV(val);
            zas->setOutput(true);
        } else {
            zas->setVoltage_mV(daneBadania.getNapiecieZasilaniaCzujki_mV());
            zas->setOutput(true);
        }
    }

    do {
        QSharedPointer<OknoZasilaniaCzujki> dlg5(new OknoZasilaniaCzujki(minVoltage, maxVoltage, minCzulosc,
                                                                         dane, daneBadania, parent));
        dlg5->connect(zas, &Zasilacz::value, dlg5.get(), &OknoZasilaniaCzujki::value);
        if (dlg5->exec() ==  QDialog::Rejected) {
            zas->setOutput(false);
            return false;
        }
    } while(false);
    if (timeWait == 0 || noStab)
        return true;
                //(bool powerON, bool resetPower, bool ignoreAlarms,
    dlg6 = new OknoStabilizacjaCzujki(true, false, true, timeWait, dane.getName(), "", parent);
    bool stabOk = dlg6->exec() == QDialog::Accepted;
    delete dlg6;
    dlg6 = nullptr;
    //if (!stabOk) {
    //    error = stabilizacja ?
    //                QString::fromUtf8("Czujka zgłosiła alarm podczas stabilizacji")
    //              : QString::fromUtf8("Czujka zgłosiła alarm podczas oczekiwania na kolejny pomiar");
    //    tlumienie = "-";
    //}
    (void)stabOk;

    return true;
}

short ProceduraTestowa::pomiarCzujki(uint32_t flags,
                                     const ParametryBadania &daneBadania,
                                      const Ustawienia &ust)
{
    bool repeatPomiar = flags & POWT_POMIAR;
    bool nowait = flags & NIE_POK_WYNIK_POM;
    bool testPom = flags & TEST_POMIAR;


    dlg7 = new OknoBadaniaTlumienia(daneBadania.getCzasPomZmianaTlumenia_s(), dane.getDlugoscFali(),
                        dane.getName(), ust, ster, parent);
    bool pomOk = dlg7->exec() == QDialog::Accepted;
    QString tlumienie = dlg7->getTlumienie();
    QString error = dlg7->getError();
    delete dlg7;
    dlg7 = nullptr;

    if (!testPom && nowait) {
        dane.setSuccessBadaniaCzujki(pomOk, tlumienie, error);
        return 0;
    }

    QSharedPointer<OknoWynikBadaniaTlumienia> dlg8(new OknoWynikBadaniaTlumienia(pomOk,
                                                                                 tlumienie, dane.getName(),
                                                                                 repeatPomiar, parent));
    if(dlg8->exec() == QDialog::Rejected) {
        return -1;
    } else {
        if (dlg8->getPowtorzPomiar()) {
            return 1;
        } else {
            if (! testPom)
                dane.setSuccessBadaniaCzujki(pomOk, tlumienie, error);
        }
    }
    return 0;
}

bool ProceduraTestowa::montazZerowanieZasilanie(uint32_t flags, const ParametryBadania &daneBadania)
{
    bool usuniecieZabezp = flags & USUN_ZABEZP;
    bool minRozstawienie = flags & MIN_ROZSTAW;
    bool maxRozstawienie = flags & MAX_ROZSTAW;
    bool rozproszone = flags & SW_ROZPR;
    bool systemNadajnikObiornik = daneBadania.getSystemOdbiornikNadajnik();

    do {
        QSharedPointer<OknoMontaz> dlg4(new OknoMontaz(usuniecieZabezp, minRozstawienie, maxRozstawienie,
                                                       rozproszone, systemNadajnikObiornik, dane, parent));
        if ((dlg4->exec() == QDialog::Rejected))
            return false;
    } while(false);

    if (!ster->getConnected() || (daneBadania.getWyzwalanieAlarmuPradem() && !zas->getConnected())) {
        if (!oczekiwanieNaUrzadzenie(daneBadania))
            return false;
    }

    if(!zerowanieSterownika(flags, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
        return false;

    if (!zasilenieCzujki(flags, daneBadania.getCzasStabilizacjiCzujki_s(), daneBadania))
        return false;

    return true;
}





short ProceduraTestowa::resetCzujki(const QString & testName, const QString & subTestName,
                                   unsigned int czasOffOn, unsigned int czasStabilizacji,
                                   const ParametryBadania &daneBadania)
{
    OknoResetuZasilaniaCzujki * dlg12 = new OknoResetuZasilaniaCzujki(testName, subTestName,
                                             czasOffOn, daneBadania, zas, parent);
    if (dlg12->exec() == QDialog::Rejected) {
        delete dlg12;
        return -1;
    }
    delete dlg12;
    bool stabOk = true;
    if (czasStabilizacji > 0) {
        dlg6 = new OknoStabilizacjaCzujki(false, true, true, daneBadania.getCzasStabilizacjiPoResecie_s(), dane.getName(), "", parent);
        stabOk = dlg6->exec() == QDialog::Accepted;
        delete dlg6;
        dlg6 = nullptr;
    }

    if (!stabOk) {
        return 1;
    } else {
        return 0;
    }
}

bool ProceduraTestowa::NiewspolosiowoscBadanie(const ParametryBadania &daneBadania, const Ustawienia &ust)
{

    NiewspolosiowoscOsUrzadzenie k = dane.getKatyProducenta();
    QString ptitle = QString::fromUtf8("Badanie niewspółosiowości dla %1 dla osi poziomej (dodatni kąt)").arg(dane.getNazwaTransmitter_a());
    short ret = pomiarKata(9, ptitle, k.nadajnik.poziomo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;
    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    ptitle = QString::fromUtf8("Badanie niewspółosiowości dla %1 dla osi poziomej (ujemny kąt)").arg(dane.getNazwaTransmitter_a());
    ret = pomiarKata(9, ptitle, -k.nadajnik.poziomo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;
    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    ptitle = QString::fromUtf8("Badanie niewspółosiowości dla %1 dla osi pionowej (dodatni kąt)").arg(dane.getNazwaTransmitter_a());
    ret = pomiarKata(8, ptitle, k.nadajnik.pionowo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;

    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    ptitle = QString::fromUtf8("Badanie niewspółosiowości dla %1 dla osi pionowej (ujemny kąt)").arg(dane.getNazwaTransmitter_a());
    ret = pomiarKata(8, ptitle, -k.nadajnik.pionowo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;

    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }
    if (!daneBadania.getSystemOdbiornikNadajnik()) {
        dane.setOk(true);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    ptitle = QString::fromUtf8("Badanie niewspółosiowości dla %1 dla osi poziomej (kąty dodatnie)").arg(dane.getNazwaReceiver_a());
    ret = pomiarKata(2, ptitle, k.odbiornik.poziomo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;
    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    ptitle = QString::fromUtf8("Badanie niewspółosiowości dla %1 dla osi poziomej (kąty ujemne)").arg(dane.getNazwaReceiver_a());
    ret = pomiarKata(2, ptitle, -k.odbiornik.poziomo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;
    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    ptitle = QString::fromUtf8("Badanie niewspółosiowości dla %1 dla osi pionowej (kąty dodatnie)").arg(dane.getNazwaReceiver_a());
    ret = pomiarKata(1, ptitle, k.odbiornik.pionowo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;

    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    ptitle = QString::fromUtf8("Badanie niewspółosiowości dla %1 dla osi pionowej (kąty ujemne)").arg(dane.getNazwaReceiver_a());
    ret = pomiarKata(1, ptitle, -k.odbiornik.pionowo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;

    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    dane.setOk(true);
    dane.setDataZakonczenia();
    dane.setWykonany(true);
    return true;
}

short ProceduraTestowa::pomiarKata(short nrSilnika, const QString & ptitle, const double & kat,
                          const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    PomiarKata pomiar;
    pomiar.ok = true;
    pomiar.nazwaBadania = ptitle;
    pomiar.katProducenta = kat;
    pomiar.katZmierzony = 0;
    pomiar.errorDetail = "";
    pomiar.errorStr = "";

    short ret = pomiarKataProcedura(pomiar, nrSilnika, ptitle, daneBadania, ust);
    if (ret == -1)
        return -1;

    //zerowanie
    if(!zerowanieSterownika(ZER_FILTRY | ZER_NADAJNIK | ZER_ODBIORNIK, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver())) {
        pomiar.errorStr = QString::fromUtf8("Błąd stanowiska");
        pomiar.errorDetail = QString::fromUtf8("Błąd zerowania czujki po wykoaniu pomiarów dla kąta niewspółosowiości.");
        pomiar.ok = false;
        return 1;
    }

    short ret3 = resetCzujki(dane.getName(), ptitle, ust.getCzasWylaczeniaCzujkiDlaResetu(),
                             daneBadania.getCzasStabilizacjiPoResecie_s(),
                           daneBadania);
    if (ret3 == -1)
        return -1;
    if (ret3 == 1) {
        pomiar.errorStr = QString::fromUtf8("Alarm w pozycji 0");
        pomiar.errorDetail = QString::fromUtf8("Czujka po wyłączeniu i włączeniu zasilania zgłosiła alarm podczas stabilizacji");
        pomiar.ok = false;
        return 1;
    }

    //pomiar.ok = true;
    dane.dodajPomiarKata(pomiar);
    return ret;
}

short ProceduraTestowa::pomiarKataProcedura(PomiarKata & pomiar, short nrSilnika, const QString & ptitle,
                                   const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    dlg6 = new OknoStabilizacjaCzujki(true, false, true, daneBadania.getCzasStabilizacjiCzujki_s(), dane.getName(), ptitle, parent);
    bool stabOk = dlg6->exec() == QDialog::Accepted;
    delete dlg6;
    dlg6 = nullptr;
    if (!stabOk)
        return 2;

    qDebug() << "Kat " << pomiar.katProducenta;
    dlg10 = new OknoBadaniaKata(nrSilnika, dane.getName(), ptitle,
                                QString::number(pomiar.katProducenta),
                                ust, ster, parent);
    bool ret = dlg10->exec() == QDialog::Accepted;

    if (!ret) {
        //qDebug() << __FILE__ << __LINE__ << "ERROR czujka sie wyzwolila podczas jazdy do kata nominalnego" << dlg10->getError().toStdString().c_str();
        pomiar.errorStr = dlg10->getError();
        pomiar.errorDetail = dlg10->getErrDetails();
        pomiar.ok = false;
        pomiar.katZmierzony = dlg10->getDegrees();
        delete dlg10;
        dlg10 = nullptr;
        return 1;
    }
    pomiar.katZmierzony = dlg10->getDegrees();
    delete dlg10;
    dlg10 = nullptr;


    //qDebug() << __FILE__ << __LINE__ << "OK czekam 2 minuty";
    dlg11 = new OknoCzekaniaBadanieKatowe(ust.getCzasStabilizacjiDlaKataNieWspolosiowosci(), dane.getName(), ptitle, parent);
    if (dlg11->exec() == QDialog::Rejected) {
        pomiar.errorStr = "Kąt wyzw.=Kąt prod.";
        pomiar.errorDetail = "Czujka wyzwoliła się w czasie stabilizacji w położeniu dla nominalnego kąta podanego przez producenta";
        pomiar.ok = false;
        qDebug() << __FILE__ << __LINE__ << "ERROR czujka sie wyzwolila podczas czekania 2 min";
        return 1;
    }
    delete dlg11;
    dlg11 = nullptr;

    qDebug() << __FILE__ << __LINE__ << "Badanie reakcji 6dB" ;
    dlg12 = new OknoBadanieReakcji6dB(ust.getMaksCzasZadzialaniaCzujkiDlaKataNieWspolosiowosci(),
                                        ust.getMaksCzasTestuZadzialaniaCzujkiDlaKataNieWspolosiowosci(),
                                        daneBadania.getDlugoscFaliFiltrow(), ust.getWartoscTlumienieDlaKataNieWspolosiowosci(),
                                        dane.getName(), ptitle, ust, ster, parent);
    if (dlg12->exec() == QDialog::Rejected) {
        pomiar.errorStr = QString::fromUtf8("Brak reakcji na %1 dB").arg(ust.getWartoscTlumienieDlaKataNieWspolosiowosci(), 3, 'f',1);
        pomiar.errorDetail = dlg12->getError();
        pomiar.ok = false;
        qDebug() << "ERROR" << __FILE__ << __LINE__ << "Error:" << dlg12->getError();
        delete dlg12;
        dlg12 = nullptr;
        return 1;
    }
    delete dlg12;
    dlg12 = nullptr;


    //reset czujki
    qDebug() << __FILE__ << __LINE__ << "Reset czujki";
    short ret2 = resetCzujki(dane.getName(), ptitle, ust.getCzasWylaczeniaCzujkiDlaResetu(),
                             daneBadania.getCzasStabilizacjiPoResecie_s(),
                           daneBadania);
    if (ret2 == -1)
        return -1;
    if (ret2 == 1) {
        pomiar.errorStr = QString::fromUtf8("Alarm po resecie");
        pomiar.errorDetail = QString::fromUtf8("Czujka po wyłączeniu i włączeniu zasilania zgłosiła alarm podczas stabilizacji");
        pomiar.ok = false;
        return 1;
    }

    qDebug() << __FILE__ << __LINE__ << "Dojazd do maksa " << ust.getMaksKatNieWspolOsiowosci();

    //maks kat
    double maxkat = ust.getMaksKatNieWspolOsiowosci();
    if (pomiar.katProducenta < 0)
        maxkat *= -1;
    qDebug() << "Max kat " << maxkat;
    dlg14 = new OknoBadaniaMaksymalnegoKata(nrSilnika, dane.getName(), ptitle, pomiar.katProducenta,
                                            maxkat, ust, ster, parent);
    if (dlg14->exec() == QDialog::Rejected) {
        qDebug() << __FILE__ << __LINE__ << "NOT OK";
        pomiar.errorDetail = dlg14->getError();
        pomiar.errorStr = QString::fromUtf8("Maksymalny kąt");
        pomiar.ok = false;
        pomiar.katZmierzony = dlg14->getDegrees();
        delete dlg14;
        dlg14 = nullptr;
        return 1;
    }
    qDebug() << __FILE__ << __LINE__ << "OK";
    pomiar.katZmierzony = dlg14->getDegrees();
    pomiar.ok = true;
    delete dlg14;
    dlg14 = nullptr;

    return 0;

}


DaneTestu &ProceduraTestowa::getDane()
{
    return dane;
}
