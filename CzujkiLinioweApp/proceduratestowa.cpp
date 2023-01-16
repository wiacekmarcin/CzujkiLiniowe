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

#include "zasilacz.h"
#include <QMessageBox>
#include <QSharedPointer>

ProceduraTestowa::ProceduraTestowa(QWidget * widget):
    parent(widget),
    zas(nullptr),
    ster(nullptr),
    dlg7(nullptr),
    dlg0(nullptr),
    dlg10(nullptr),
    dlg11(nullptr),
    dlg12(nullptr),
    dlg13(nullptr)
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
//void ProceduraTestowa::ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt)
{
    /*
    if (dlg0)
        dlg0->ster_setPositionDone(silnik, home, move, error, interrupt);
    if (dlg10)
        dlg10->ster_setPositionDone(silnik, home, move, error, interrupt);
        */
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
}

void ProceduraTestowa::czujkaOn()
{
    if (dlg7)
       dlg7->czujkaOn();

    if (dlg10)
        dlg10->czujkaOn();
    if (dlg11)
        dlg11->czujkaOn();
    if (dlg12)
        dlg12->czujkaOn();
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
        {
            QSharedPointer<OknoParametryTestu> dlg1(new OknoParametryTestu(nrPom, &dane, daneBadania, parent));
            if (!(dlg1->exec()))
                return false;
        }

        {
            QSharedPointer<OknoSprawdzenieDanych> dlg2(new OknoSprawdzenieDanych(dane, parent));
            if (!(dlg2->exec()))
                return false;
        }

        {
            QSharedPointer<OknoMontaz> dlg4(new OknoMontaz(dane, parent));
            if (!(dlg4->exec()))
                return false;
        }


        if (!ster->getConnected() || (daneBadania.getWyzwalanieAlarmuPradem() && !zas->getConnected())) {
            if (!oczekiwanieNaUrzadzenie(daneBadania))
                return false;
        }

        do {
            if(!zerowanieSterownika(nrPom == 1, true, false))
                return false;

            if (!zasilenieCzujki(daneBadania))
                return false;

            {
                QSharedPointer<OknoStabilizacjaCzujki> dlg6(
                            new OknoStabilizacjaCzujki(daneBadania.getCzasStabilizacjiCzujki_s(),
                                                        dane.getName(), true, parent));
                dlg6->exec();
            }

            powtorzPomiar = pomiarCzujki(daneBadania, true, true, ust);
            if (powtorzPomiar == -1)
                return false;

        } while(powtorzPomiar);
    }

    {
        dane.setWykonany(true);
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania));
        dlg->exec();
    }
    return true;
}

bool ProceduraTestowa::Powtarzalnosc(const ParametryBadania & daneBadania, const Ustawienia & ust)
{
    short powtorzPomiar;
    {
        QSharedPointer<OknoParametryTestu> dlg1(new OknoParametryTestu(1, &dane, daneBadania, parent));
        if (!(dlg1->exec()))
            return false;
    }

    {
        QSharedPointer<OknoSprawdzenieDanych> dlg2(new OknoSprawdzenieDanych(dane, parent));
        if (!(dlg2->exec()))
            return false;
    }

    do {
        {
            QSharedPointer<OknoMontaz> dlg4(new OknoMontaz(dane, parent));
            if (!(dlg4->exec()))
                return false;
        }

        if (!ster->getConnected() || (daneBadania.getWyzwalanieAlarmuPradem() && !zas->getConnected())) {
            if (!oczekiwanieNaUrzadzenie(daneBadania))
                return false;
        }

        if(!zerowanieSterownika(true, true, false))
            return false;

        if (!zasilenieCzujki(daneBadania))
            return false;

        {
            QSharedPointer<OknoStabilizacjaCzujki> dlg6(
                new OknoStabilizacjaCzujki(daneBadania.getCzasStabilizacjiCzujki_s(),
                                            dane.getName(), true, parent));
            dlg6->exec();
        }


        powtorzPomiar = pomiarCzujki(daneBadania, true, true, ust);
        if (powtorzPomiar == -1)
            return false;
    } while (powtorzPomiar != 0);


    for (short num = 0; num < 2; ++num)
    {
        {
            QSharedPointer<OknoStabilizacjaCzujki> dlg6(
                new OknoStabilizacjaCzujki(dane.getCzasPowtarzalnosci(),
                                        dane.getName(), false, parent));
            dlg6->exec();
        }
        pomiarCzujki(daneBadania, false, false, ust);

        dane.addNextPomiar();

        if(!zerowanieSterownika(false, true, false))
            return false;
    }

    {
        QSharedPointer<OknoStabilizacjaCzujki> dlg6(
            new OknoStabilizacjaCzujki(1L*3600*72,
                                    dane.getName(), false, parent));
        dlg6->exec();
    }

    pomiarCzujki(daneBadania, false, false, ust);

    {
        dane.setWykonany(true);
        QSharedPointer<OknoPodsumowanieTestu> dlg(new OknoPodsumowanieTestu(dane, daneBadania));
        dlg->exec();
    }
    return true;
}

bool ProceduraTestowa::Niewspolosiowosc(const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    {
        QSharedPointer<OknoParametryTestu> dlg1(new OknoParametryTestu(1, &dane, daneBadania, parent));
        if (!(dlg1->exec()))
            return false;
    }

    {
        QSharedPointer<OknoSprawdzenieDanych> dlg2(new OknoSprawdzenieDanych(dane, parent));
        if (!(dlg2->exec()))
            return false;
    }

    {
        QSharedPointer<OknoMontaz> dlg4(new OknoMontaz(dane, parent));
        if (!(dlg4->exec()))
            return false;
    }


    if (!ster->getConnected() || (daneBadania.getWyzwalanieAlarmuPradem() && !zas->getConnected())) {
        if (!oczekiwanieNaUrzadzenie(daneBadania))
            return false;
    }

    if(!zerowanieSterownika(true, true, false))
        return false;

    if (!zasilenieCzujki(daneBadania))
        return false;


    pomiarKata(daneBadania, ust);

    return true;
}

bool ProceduraTestowa::parametryTest(short numerProby, const ParametryBadania &daneBadania, const Ustawienia &ust)
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
    QSharedPointer<OczekiwanieNaUrzadzenia> dlg(new OczekiwanieNaUrzadzenia(daneBadania.getZasilanieCzujekZasilaczZewnetrzny(), parent));
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny()) {
        dlg->connect(zas, &Zasilacz::kontrolerConfigured, dlg.get(), &OczekiwanieNaUrzadzenia::zasilacz);
        zas->connectToDevice();
    }
    dlg->connect(ster, &Sterownik::kontrolerConfigured, dlg.get(), &OczekiwanieNaUrzadzenia::sterownik);
    ster->connectToDevice();

    if (!dlg->exec()) {
#ifdef DEFVAL

#else
        return false;
#endif
    }
    return true;
}

bool ProceduraTestowa::zerowanieSterownika(bool ramiona, bool filtry, bool wozek)
{
    dlg0 = new OknoZerowanieUrzadzenia(ramiona, filtry, wozek, ster, parent);

    if (!dlg0->exec()) {
#ifdef DEFVAL

#else
        delete dlg0;
        dlg0 = nullptr;
        return false;
#endif
    }
    delete dlg0;
    dlg0 = nullptr;
    return true;
}


bool ProceduraTestowa::potwierdzenieNarazenia(const DaneTestu &daneTestu, const ParametryBadania &daneBadania,
                                              const Ustawienia &)
{
    OknoPotwierdzenieNarazenia *dlg3 = new OknoPotwierdzenieNarazenia(daneTestu, parent);
    if (!dlg3->exec()) {
        delete dlg3;
        return false;
    }
    delete dlg3;
    return true;
}


bool ProceduraTestowa::zasilenieCzujki(const ParametryBadania &daneBadania)
{
    //ustawienia zasilania czujki z zasilacza
    if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny()) {
        zas->setVoltage_mV(daneBadania.getNapiecieZasilaniaCzujki_mV());
        zas->setOutput(true);
    }
    //TODO
    if (daneBadania.getWyzwalanieAlarmuPradem()) {
        zas->setCurrentLimit_mA(daneBadania.getPrzekroczeniePraduZasilania_mA().toInt());
    }

    QSharedPointer<OknoZasilaniaCzujki> dlg5(new OknoZasilaniaCzujki(dane, daneBadania, parent));
    dlg5->connect(zas, &Zasilacz::value, dlg5.get(), &OknoZasilaniaCzujki::value);
    if (!dlg5->exec()) {
        zas->setOutput(false);
        return false;
    }
    return true;
}

short ProceduraTestowa::pomiarCzujki(const ParametryBadania &daneBadania, bool repeatPomiar, bool waitEkran, const Ustawienia &ust)
{
    dlg7 = new OknoBadaniaTlumienia(daneBadania.getCzasPomZmianaTlumenia_s(), dane.getDlugoscFali(),
                            dane.getName(), ust, ster, parent);
    bool ret = dlg7->exec();
    QString tlumienie = dlg7->getTlumienie();


    bool wynikBadania = dlg7->getWynikBadania();
    QString error = dlg7->getError();
    delete dlg7;
    dlg7 = nullptr;
    if (!waitEkran) {
        dane.setSuccessBadaniaCzujki(wynikBadania, tlumienie, error);
        return 0;
    }

    QSharedPointer<OknoWynikBadaniaTlumienia> dlg8(new OknoWynikBadaniaTlumienia(wynikBadania, tlumienie, dane.getName(), repeatPomiar, parent));
    if(!(dlg8->exec())) {
        return -1;
    } else {
        if (dlg8->getPowtorzPomiar()) {
            return 1;
        }
        if (wynikBadania && tlumienie.toDouble() < 0.4) {
            dane.setSuccessBadaniaCzujki(false, tlumienie, "Crep<0.4");
        } else {
            dane.setSuccessBadaniaCzujki(wynikBadania, tlumienie, error);
        }
    }
    return 0;
}

short ProceduraTestowa::pomiarKata(const ParametryBadania &daneBadania, const Ustawienia &ust)
{
    short nrSilnika = 1;
    QString ptitle = QString("Badanie niewspółosiowości dla %1 dla osi poziomej").arg(dane.getNazwaNumerPierwszego());
    dlg10 = new OknoBadaniaKata(nrSilnika, dane.getName(), ptitle,
                                dane.getKatyProducenta().nadajnik.poziomo,
                                ust, ster, parent);
    bool ret = dlg10->exec();
    if (!ret) {
        qDebug() << "ERROR" << __FILE__ << __LINE__ << "czujka sie wyzwolila podczas jazdy do kata nominalnego";
    }

    QString error = dlg10->getError();
    qDebug() << "erorr" << error.toStdString().c_str();
    delete dlg10;
    dlg10 = nullptr;

    dlg11 = new OknoCzekaniaBadanieKatowe(120, dane.getName(), ptitle, parent);
    if (!dlg11->exec()) {
        //TODO
        qDebug() << "ERROR" << __FILE__ << __LINE__ << "czujka sie wyzwolila podczas czekania 2 min";
    }

    delete dlg11;
    dlg11 = nullptr;

    dlg12 = new OknoBadanieReakcji6dB(daneBadania.getDlugoscFaliFiltrow(), dane.getName(), ptitle, ust, ster, parent);
    if (!dlg12->exec()) {
        //TODO
        qDebug() << "ERROR" << __FILE__ << __LINE__ << "Error:" << dlg12->getError();
    }
    delete dlg12;
    dlg12 = nullptr;

    //reset czujki
    if (daneBadania.getZasilanieCzujekCentrala()) {
        QMessageBox::information(parent, "Badanie Zależność kierunkowa", "Należy zresetować czujkę (wyłączyć i włączyć). Po wykonaniu tej czynności należy kliknąć OK");
    } else if (daneBadania.getZasilanieCzujekZasilaczZewnetrzny()) {
        //zas->setOutput(false);
        //QThread::currentThread()->msleep(100); //na 5 sekund wylaczenie wlaczenie
        //zas->setOutput(true);
    }

    //maks kat
    dlg13 = new OknoBadaniaMaksymalnegoKata(nrSilnika, dane.getName(), ptitle, "15", ust, ster, parent);
    dlg13->exec();
    delete dlg13;
    dlg13 = nullptr;

    return 0;
}




DaneTestu &ProceduraTestowa::getDane()
{
    return dane;
}
