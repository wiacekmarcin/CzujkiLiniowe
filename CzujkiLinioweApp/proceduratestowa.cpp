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

#include "zasilacz.h"
#include <QMessageBox>
#include <QSharedPointer>

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
    dlg14(nullptr)
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

void ProceduraTestowa::zas_value(int kind, int value)
{

}

void ProceduraTestowa::ster_setValue(short silnik, const double & val)
{
    if (dlg10)
        dlg10->ster_setValue(silnik, val);

    if (dlg14)
        dlg14->ster_setValue(silnik, val);
}

void ProceduraTestowa::czujkaOn()
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
}

bool ProceduraTestowa::startBadanie(short id, const QString & nameTest, const ParametryBadania & b,
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

    default:
        QMessageBox::warning(parent, QString("Badanie"), QString("Dane badanie nie zostało zaimplementowane"));
        break;
    }

    return false;
}



bool ProceduraTestowa::Odtwarzalnosc(const ParametryBadania & daneBadania, const Ustawienia & ust)
{
    short powtorzPomiar;
    for (short nrPom = 1; nrPom <= daneBadania.getIloscCzujek(); ++nrPom)
    {
        if (!parametryTest(nrPom, daneBadania))
            return false;

        bool pomiar1 = true;
        do {
            if (!montazZerowanieZasilanie(0, 0, true, nrPom == 1, true, false, daneBadania))
                return false;

            powtorzPomiar = pomiarCzujki(pomiar1, false, true, true, daneBadania.getCzasPomZmianaTlumenia_s(), daneBadania, ust);
            pomiar1 = false;
            if (powtorzPomiar == -1)
                return false;
        } while(powtorzPomiar);
        if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
            zas->setOutput(false);
    }
    zerowanieSterownika(false, true, false, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver());
    dane.obliczOdtwarzalnosc(ust);
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);
    {
        dane.setWykonany(true);
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    }
    return true;
}

bool ProceduraTestowa::Powtarzalnosc(const ParametryBadania & daneBadania, const Ustawienia & ust)
{
    short powtorzPomiar;

    if (!parametryTest(1, daneBadania))
        return false;

    bool firstTime = true;
    do {
        if (!montazZerowanieZasilanie(0, 0, true, true, true, false, daneBadania))
            return false;

        powtorzPomiar = pomiarCzujki(firstTime, false, true, true, daneBadania.getCzasStabilizacjiCzujki_s(), daneBadania, ust);
        firstTime = false;
        if (powtorzPomiar == -1)
            return false;
    } while (powtorzPomiar != 0);

    dane.addNextPomiar();

    if(!zerowanieSterownika(false, true, false,daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
        return false;

    for (short num = 0; num < 2; ++num)
    {
        pomiarCzujki(false, true, false, false, dane.getCzasPowtarzalnosci(), daneBadania, ust);

        dane.addNextPomiar();

        if(!zerowanieSterownika(false, true, false,daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
            return false;
    }

    pomiarCzujki(false, true, false, false, ust.getCzasOczekiwaniaPowtarzalnosc4Test(), daneBadania, ust);
    zerowanieSterownika(false, true, false, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver());
    dane.obliczPowtarzalnosc(ust);
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);
    do {
        dane.setWykonany(true);
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    } while(false);
    return true;
}

bool ProceduraTestowa::Niewspolosiowosc(const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    if (!parametryTest(1, daneBadania))
        return false;

    if (!montazZerowanieZasilanie(0, 0, true, true, true, false, daneBadania))
        return false;

    if (!NiewspolosiowoscBadanie(daneBadania, ust))
        return false;
    dane.obliczZaleznoscKatowa(ust);
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);
    do {
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    } while(false);
    return true;

}


bool ProceduraTestowa::SzybkieZmianyTlumienia(const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    if (!parametryTest(1, daneBadania))
        return false;

    if (!montazZerowanieZasilanie(0, 0, false, true, true, false, daneBadania))
        return false;

    QString tlumienie = "-";
    QString error = "";
    dlg6 = new OknoStabilizacjaCzujki(true, daneBadania.getCzasStabilizacjiCzujki_s(), dane.getName(), "", parent);
    bool stabOk = dlg6->exec() == QDialog::Accepted;
    delete dlg6;
    dlg6 = nullptr;
    if (!stabOk)
        return false;

    bool testOk = true;
    dlg12 = new OknoBadanieReakcji6dB(ust.getMaksCzasZadzialaniaCzujkidlaTlumnikaA(),
                                        ust.getMaksCzasTestuCzujkidlaTlumnikaA(),
                                        daneBadania.getDlugoscFaliFiltrow(),
                                        ust.getSzybkieZmianyWartoscTlumnikaA(),
                                        dane.getName(), QString::fromUtf8("Pomiar dla tłumnika A"), ust, ster, parent);
    if (!dlg12->exec()) {
        testOk = false;
        dane.setSuccessBadaniaCzujki(false, QString::number(ust.getSzybkieZmianyWartoscTlumnikaA(), 'f', 1), dlg12->getError());
    } else {
        dane.setSuccessBadaniaCzujki(true, QString::number(ust.getSzybkieZmianyWartoscTlumnikaA(), 'f', 1), dlg12->getError());
    }
    delete dlg12;
    dlg12 = nullptr;
    dane.addNextPomiar();
    zerowanieSterownika(false, true, false, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver());

    resetCzujki(dane.getName(), QString::fromUtf8("Resetowanie czujki"), ust.getCzasWylaczeniaCzujkiDlaResetu(),
                daneBadania.getCzasStabilizacjiPoResecie_s(), daneBadania);

    dlg12 = new OknoBadanieReakcji6dB(ust.getMaksCzasZadzialaniaCzujkidlaTlumnikaB(),
                                        ust.getMaksCzasTestuCzujkidlaTlumnikaB(),
                                        daneBadania.getDlugoscFaliFiltrow(),
                                        ust.getSzybkieZmianyWartoscTlumnikaB(),
                                        dane.getName(), QString::fromUtf8("Pomiar dla tłumnika B"), ust, ster, parent);
    if (!dlg12->exec()) {
        testOk = false;
        dane.setSuccessBadaniaCzujki(false, QString::number(ust.getSzybkieZmianyWartoscTlumnikaB(), 'f', 1), dlg12->getError());
    } else {
        dane.setSuccessBadaniaCzujki(true, QString::number(ust.getSzybkieZmianyWartoscTlumnikaB(), 'f', 1), dlg12->getError());
    }
    delete dlg12;
    dlg12 = nullptr;
    zerowanieSterownika(false, true, false, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver());
    dane.setOk(testOk);
    dane.setDataZakonczenia();
    dane.setWykonany(true);
    dane.obliczSzybkieZmianyTlumienia(ust);
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);
    do {
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    } while(false);
    return true;
}

bool ProceduraTestowa::DlugoscDrogiOptycznej(const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    if (!parametryTest(1, daneBadania))
        return false;

    short powtorzPomiar = 0;
    bool ok1, ok2;
    do {
        if (!montazZerowanieZasilanie(0, 1, true, true, true, false, daneBadania))
            return false;

        powtorzPomiar = pomiarCzujki(true, false, true, true, daneBadania.getCzasStabilizacjiCzujki_s(), daneBadania, ust);
        if (powtorzPomiar == -1)
            return false;
        ok1 = powtorzPomiar == 0;
    } while (powtorzPomiar != 0);

    dane.addNextPomiar();
    do {
        if (!montazZerowanieZasilanie(0, 2, true, true, true, false, daneBadania))
            return false;

        powtorzPomiar = pomiarCzujki(true, false, true, true, daneBadania.getCzasStabilizacjiCzujki_s(), daneBadania, ust);
        if (powtorzPomiar == -1)
            return false;
        ok2 = powtorzPomiar == 0;
    } while (powtorzPomiar != 0);

    dane.setOk(ok1 & ok2);
    dane.setDataZakonczenia();
    dane.setWykonany(true);
    zerowanieSterownika(false, true, false, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver());
    dane.obliczDlugoscOptyczna(ust);
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);
    do {
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    } while(false);
    return true;
}

bool ProceduraTestowa::RozproszoneSwiatlo(const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    if (!parametryTest(1, daneBadania))
        return false;
    if (potwierdzenieNarazenia(dane, daneBadania, ust))
        return false;
    return true;
}

bool ProceduraTestowa::ZmienneParametryZasilania(const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    if (!parametryTest(1, daneBadania))
        return false;

    short powtorzPomiar = 0;
    bool ok1, ok2;
    do {
        if (!montazZerowanieZasilanie(1, 0, true, true, true, false, daneBadania))
            return false;

        powtorzPomiar = pomiarCzujki(true, false, true, true, daneBadania.getCzasStabilizacjiCzujki_s(), daneBadania, ust);
        if (powtorzPomiar == -1)
            return false;
        ok1 = powtorzPomiar == 0;
    } while (powtorzPomiar != 0);
    dane.addNextPomiar();
    do {
        if (!montazZerowanieZasilanie(2, 0, true, true, true, false, daneBadania))
            return false;

        powtorzPomiar = pomiarCzujki(true, false, true, true, daneBadania.getCzasStabilizacjiCzujki_s(), daneBadania, ust);
        if (powtorzPomiar == -1)
            return false;
        ok2 = powtorzPomiar == 0;
    } while (powtorzPomiar != 0);

    dane.setOk(ok1 & ok2);
    dane.setDataZakonczenia();
    dane.setWykonany(true);
    zerowanieSterownika(false, true, false, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver());
    dane.obliczZaleznoscNapieciaZasilania(ust);
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny())
        zas->setOutput(false);
    do {
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania, ust));
        dlg->exec();
    } while(false);
    return true;
}


bool ProceduraTestowa::parametryTest(short numerProby, const ParametryBadania &daneBadania)
{

    QSharedPointer<OknoParametryTestu> dlg1(new OknoParametryTestu(numerProby, &dane, daneBadania, parent));
    if (!(dlg1->exec()))
        return false;

    QSharedPointer<OknoSprawdzenieDanych> dlg2(new OknoSprawdzenieDanych(dane, parent));
    if (!(dlg2->exec()))
        return false;
    return true;
}



bool ProceduraTestowa::oczekiwanieNaUrzadzenie(const ParametryBadania & daneBadania)
{
    QSharedPointer<OczekiwanieNaUrzadzenia> dlg(new OczekiwanieNaUrzadzenia(
                                                    daneBadania.getZasilanieCzujekZasilaczZewnetrzny(),
                                                    zas, ster, parent));
    if (!dlg->exec()) {
        return false;
    }
    return true;
}

bool ProceduraTestowa::zerowanieSterownika(bool ramiona, bool filtry, bool wozek ,const QString & trans, const QString & receiv )
{
    dlg0 = new OknoZerowanieUrzadzenia(ramiona, filtry, wozek, trans, receiv, ster, parent);

    int ret = dlg0->exec();
    delete dlg0;
    dlg0 = nullptr;
    return ret;
}


bool ProceduraTestowa::potwierdzenieNarazenia(const DaneTestu &daneTestu, const ParametryBadania &daneBadania,
                                              const Ustawienia &)
{
    OknoPotwierdzenieNarazenia *dlg3 = new OknoPotwierdzenieNarazenia(daneTestu, parent);
    int ret = dlg3->exec();
    delete dlg3;
    return ret;
}


bool ProceduraTestowa::zasilenieCzujki(short napiecie, bool maksCzulosc, const ParametryBadania &daneBadania)
{
    //ustawienia zasilania czujki z zasilacza
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny()) {
        if (napiecie == 0) {
            zas->setVoltage_mV(daneBadania.getNapiecieZasilaniaCzujki_mV());
            zas->setOutput(true);
        } else if (napiecie == 1) {
            double val = dane.getMinimalneNapiecie().toDouble()*1000;
            zas->setVoltage_mV(val);
            zas->setOutput(true);
        } else if (napiecie == 2) {
            double val = dane.getMaksymalneNapiecie().toDouble()*1000;
            zas->setVoltage_mV(val);
            zas->setOutput(true);
        }
    }


    QSharedPointer<OknoZasilaniaCzujki> dlg5(new OknoZasilaniaCzujki(napiecie, maksCzulosc, dane, daneBadania, parent));
    dlg5->connect(zas, &Zasilacz::value, dlg5.get(), &OknoZasilaniaCzujki::value);
    if (!dlg5->exec()) {
        zas->setOutput(false);
        return false;
    }
    return true;
}

short ProceduraTestowa::pomiarCzujki(bool stabilizacja, bool oczekiwanie, bool repeatPomiar, bool waitEkran,
                                     unsigned long timeWait, const ParametryBadania &daneBadania,
                                      const Ustawienia &ust)
{
    bool stabOk = true;
    bool pomOk = true;
    QString tlumienie = "-";
    QString error = "";
    if (stabilizacja || oczekiwanie) {
        dlg6 = new OknoStabilizacjaCzujki(stabilizacja, timeWait, dane.getName(), "", parent);
        stabOk = dlg6->exec() == QDialog::Accepted;
        delete dlg6;
        dlg6 = nullptr;
        if (!stabOk) {
            error = stabilizacja ?
                        QString::fromUtf8("Czujka zgłosiła alarm podczas stabilizacji")
                      : QString::fromUtf8("Czujka zgłosiła alarm podczas oczekiwania na kolejny pomiar");
            tlumienie = "-";
        }
    }
    if (stabOk) {
        dlg7 = new OknoBadaniaTlumienia(daneBadania.getCzasPomZmianaTlumenia_s(), dane.getDlugoscFali(),
                            dane.getName(), ust, ster, parent);
        pomOk = dlg7->exec() == QDialog::Accepted;
        tlumienie = dlg7->getTlumienie();
        error = dlg7->getError();
        delete dlg7;
        dlg7 = nullptr;

        if (!waitEkran) {
            dane.setSuccessBadaniaCzujki(pomOk, tlumienie, error);
            return 0;
        }
    }

    QSharedPointer<OknoWynikBadaniaTlumienia> dlg8(new OknoWynikBadaniaTlumienia(pomOk && stabOk,
                                                                                 tlumienie, dane.getName(),
                                                                                 repeatPomiar, parent));
    if(dlg8->exec() == QDialog::Rejected) {
        return -1;
    } else {
        if (dlg8->getPowtorzPomiar()) {
            return 1;
        } else {
            dane.setSuccessBadaniaCzujki(stabOk && pomOk, tlumienie, error);
        }
    }
    return 0;
}

bool ProceduraTestowa::montazZerowanieZasilanie(short napiecie, short rozstawienie, bool maxCzulosc, bool filtry, bool ramiona, bool wozek,
                                                const ParametryBadania &daneBadania)
{
    do {
        QSharedPointer<OknoMontaz> dlg4(new OknoMontaz(rozstawienie, dane, parent));
        if (!(dlg4->exec()))
            return false;
    } while(false);

    if (!ster->getConnected() || (daneBadania.getWyzwalanieAlarmuPradem() && !zas->getConnected())) {
        if (!oczekiwanieNaUrzadzenie(daneBadania))
            return false;
    }

    if(!zerowanieSterownika(filtry, ramiona, wozek, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver()))
        return false;

    if (!zasilenieCzujki(napiecie, maxCzulosc, daneBadania))
        return false;

    return true;
}

short ProceduraTestowa::pomiarKata(short nrSilnika, const QString & ptitle, const double & kat,
                          const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    PomiarKata p;
    p.ok = true;
    p.nazwaBadania = ptitle;
    p.katProducenta = kat;
    p.katZmierzony = 0;
    p.errorDetail = "";
    p.errorStr = "";

    short ret = pomiarKataProcedura(p, nrSilnika, ptitle, daneBadania, ust);
    if (ret == -1)
        return -1;
    dane.dodajPomiarKata(p);
    return ret;
}



short ProceduraTestowa::resetCzujki(const QString & testName, const QString & subTestName,
                                   unsigned int czasOffOn, unsigned int czasStabilizacji,
                                   const ParametryBadania &daneBadania)
{
    OknoResetuZasilaniaCzujki * dlg12 = new OknoResetuZasilaniaCzujki(testName, subTestName,
                                             czasOffOn, daneBadania, zas, parent);
    if (!dlg12->exec()) {
        delete dlg12;
        return -1;
    }
    delete dlg12;

    dlg6 = new OknoStabilizacjaCzujki(true, czasStabilizacji, testName, subTestName, parent);
    bool stabOk = dlg6->exec() == QDialog::Accepted;
    delete dlg6;
    dlg6 = nullptr;
    if (!stabOk) {
        return 1;
    } else {
        return 0;
    }
}

bool ProceduraTestowa::NiewspolosiowoscBadanie(const ParametryBadania &daneBadania, const Ustawienia &ust)
{

    NiewspolosiowoscOsUrzadzenie k = dane.getKatyProducenta();
    QString ptitle = QString("Badanie niewspółosiowości dla %1 dla osi poziomej").arg(dane.getNazwaTransmitter());
    short ret = pomiarKata(2, ptitle, k.nadajnik.poziomo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;
    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    ret = pomiarKata(2, ptitle, -k.nadajnik.poziomo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;
    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    ptitle = QString("Badanie niewspółosiowości dla %1 dla osi pionowej").arg(dane.getNazwaTransmitter());
    ret = pomiarKata(1, ptitle, k.nadajnik.pionowo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;

    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    ptitle = QString("Badanie niewspółosiowości dla %1 dla osi pionowej").arg(dane.getNazwaTransmitter());
    ret = pomiarKata(1, ptitle, -k.nadajnik.pionowo.toDouble(), daneBadania, ust);
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

    ptitle = QString("Badanie niewspółosiowości dla %1 dla osi poziomej").arg(dane.getNazwaTransmitter());
    ret = pomiarKata(9, ptitle, k.odbiornik.poziomo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;
    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    ret = pomiarKata(9, ptitle, -k.odbiornik.poziomo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;
    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    ptitle = QString("Badanie niewspółosiowości dla %1 dla osi pionowej").arg(dane.getNazwaReceiver());
    ret = pomiarKata(8, ptitle, k.odbiornik.pionowo.toDouble(), daneBadania, ust);
    if (ret == -1)
        return false;

    if (ret == 1) {
        dane.setOk(false);
        dane.setDataZakonczenia();
        dane.setWykonany(true);
        return true;
    }

    ret = pomiarKata(8, ptitle, -k.odbiornik.pionowo.toDouble(), daneBadania, ust);
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

short ProceduraTestowa::pomiarKataProcedura(PomiarKata & pomiar, short nrSilnika, const QString & ptitle,
                                   const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    dlg6 = new OknoStabilizacjaCzujki(true, daneBadania.getCzasStabilizacjiCzujki_s(), dane.getName(), "", parent);
    bool stabOk = dlg6->exec() == QDialog::Accepted;
    delete dlg6;
    dlg6 = nullptr;
    if (!stabOk)
        return 2;

    qDebug() << "Kat " << pomiar.katProducenta;
    dlg10 = new OknoBadaniaKata(nrSilnika, dane.getName(), ptitle,
                                QString::number(pomiar.katProducenta),
                                ust, ster, parent);
    bool ret = dlg10->exec();
    if (!ret) {
        qDebug() << __FILE__ << __LINE__ << "ERROR czujka sie wyzwolila podczas jazdy do kata nominalnego" << dlg10->getError().toStdString().c_str();
        pomiar.errorStr = "Kąt wyzw.<Kąt prod.";
        pomiar.errorDetail = dlg10->getError();
        pomiar.ok = false;
        return 1;
    }
    delete dlg10;
    dlg10 = nullptr;

    qDebug() << __FILE__ << __LINE__ << "OK czekam 2 minuty";
    dlg11 = new OknoCzekaniaBadanieKatowe(ust.getCzasStabilizacjiDlaKataNieWspolosiowosci(), dane.getName(), ptitle, parent);
    if (!dlg11->exec()) {
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
    if (!dlg12->exec()) {
        pomiar.errorStr = QString::fromUtf8("Tłumnik %1 dB nie wykryty").arg(ust.getWartoscTlumienieDlaKataNieWspolosiowosci(), 3, 'f',1);
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
    dlg14 = new OknoBadaniaMaksymalnegoKata(nrSilnika, dane.getName(), ptitle, maxkat, ust, ster, parent);
    if (!dlg14->exec()) {
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
    delete dlg14;
    dlg14 = nullptr;

    //zerowanie
    if(!zerowanieSterownika(true, true, false, daneBadania.getNazwaTransmitter(), daneBadania.getNazwaReceiver())) {
        pomiar.errorStr = QString::fromUtf8("Błąd stanowiska");
        pomiar.errorDetail = QString::fromUtf8("Błąd zerowania czujki");
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

    pomiar.ok = true;
    return 0;
}


DaneTestu &ProceduraTestowa::getDane()
{
    return dane;
}
