#include "proceduratestowa.h"
#include "test1parametrytestu.h"
#include "test2potwierdzenie.h"
#include "test3sprawdzenie.h"
#include "test4montaz.h"

#include <QMessageBox>

ProceduraTestowa::ProceduraTestowa(QWidget * widget):
    parent(widget)
{

}

void ProceduraTestowa::startBadanie(short id, const QString & nameTest, const ParametryBadania & b)
{

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

        Test3Sprawdzenie *dlg3 = new Test3Sprawdzenie(b, noweBadanie, parent);
        if (!dlg3->exec()) {
            delete dlg3;
            return ;
        }
        delete dlg3;

        Test4Montaz *dlg4 = new Test4Montaz(b, noweBadanie, parent);
        if (!dlg4->exec()) {
            delete dlg4;
            return ;
        }
        delete dlg4;

        if (b.getZasCzujekWbudZasilacz()) {
            //TODO
        }
    }
}
