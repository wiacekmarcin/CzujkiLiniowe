#include "proceduratestowa.h"
#include "test1parametrytestu.h"
#include "test2potwierdzenie.h"
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

        Test2Potwierdzenie *dlg2 = new Test2Potwierdzenie(noweBadanie, parent);
        dlg2->exec();
    }
}
