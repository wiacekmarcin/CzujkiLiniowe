#include "proceduratestowa.h"
#include "oczekiwanienaurzadzenia.h"
#include "test1parametrytestu.h"
#include "test2potwierdzenie.h"
#include "test3sprawdzenie.h"
#include "test4montaz.h"
#include "test5zasilanieczujki.h"
#include "test6stabilizacjaczujki.h"
#include "test7badanie.h"
#include "zasilacz.h"
#include <QMessageBox>
#include <QSharedPointer>

ProceduraTestowa::ProceduraTestowa(QWidget * widget):
    parent(widget),
    zas(nullptr)
{

}

ProceduraTestowa::~ProceduraTestowa()
{

}

void ProceduraTestowa::startBadanie(short id, const QString & nameTest, const ParametryBadania & b, Zasilacz * zas_)
{
    zas = zas_;

    OczekiwanieNaUrzadzenia *dlg = new OczekiwanieNaUrzadzenia(parent);

    dlg->connect(zas, &Zasilacz::kontrolerConfigured, dlg, &OczekiwanieNaUrzadzenia::zasilacz);
    zas->connectToDevice();

    if (!dlg->exec()) {
#ifdef DEFVAL

#else
        delete dlg;
        return;
#endif
    }
    delete dlg;
    //dodac okno o sprawdzaniu polaczenie ze sterownikiem
    switch(id) {
    case 0:
        Odtwarzalnosc(id, nameTest, b);
        break;
    default:
        QMessageBox::warning(parent, QString("Badanie"), QString("Dane badanie nie zostało zaimplementowane"));
        break;
    }


}

void ProceduraTestowa::Odtwarzalnosc(short id, const QString & nameTest, const ParametryBadania & b)
{
    DaneTestu noweBadanie;
    noweBadanie.setId(id);
    noweBadanie.setName(nameTest);
    for (short nCz = 0; b.getIloscCzujek(); ++nCz)
    {
        Test1ParametryTestu *dlg1 = new Test1ParametryTestu(b, &noweBadanie, nCz+1, parent);
        if (!dlg1->exec()) {
            delete dlg1;
            return ;
        }
        delete dlg1;

        Test2Potwierdzenie *dlg2 = new Test2Potwierdzenie(b, noweBadanie, parent);
        if (!dlg2->exec()) {
            delete dlg2;
            return ;
        }
        delete dlg2;
#ifdef DEFVAL
        Test3Sprawdzenie *dlg3 = new Test3Sprawdzenie(b, noweBadanie, parent);
        if (!dlg3->exec()) {
            delete dlg3;
            return ;
        }
        delete dlg3;
#endif
        Test4Montaz *dlg4 = new Test4Montaz(b, noweBadanie, parent);
        if (!dlg4->exec()) {
            delete dlg4;
            return ;
        }
        delete dlg4;

        //ustawienia zasilania czujki z zasilacza
        if (b.getZasCzujekWbudZasilacz()) {
            zas->setVoltage_mV(b.getNapiecieZasCzujki_mV());
            zas->setOutput(true);
        }
        //TODO
        if (!b.getWyzwalanieAlarmuPrzekaznik()) {
        //    zas->setCurrentLimit_mA(b.getPrzekroczeniePraduZasilania_mA());
        }

        Test5ZasilanieCzujki *dlg5 = new Test5ZasilanieCzujki(b, noweBadanie, parent);
        dlg5->connect(zas, &Zasilacz::value, dlg5, &Test5ZasilanieCzujki::value);
        if (!dlg5->exec()) {
            zas->setOutput(false);
            delete dlg5;
            return;
        }
        delete dlg5;

        Test6StabilizacjaCzujki *dlg6 = new Test6StabilizacjaCzujki(b, noweBadanie, parent);
        dlg6->exec();
        delete dlg6;

        Test7Badanie *dlg7 = new Test7Badanie(b, noweBadanie, parent);
        dlg7->exec();
        delete dlg7;

    }
}
