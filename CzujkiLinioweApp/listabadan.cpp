#include "listabadan.h"
#include "ui_listabadan.h"
#include "test1parametrytestu.h"

ListaBadan::ListaBadan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListaBadan),
    procedura(parent)
{
    ui->setupUi(this);

}

ListaBadan::~ListaBadan()
{
    delete ui;
}

void ListaBadan::startBadanie(short id, const ParametryBadania & badanie, const Ustawienia & ust,
                              Zasilacz *zas, Sterownik * ster)
{


    QString nameTest = "Nieznane Badanie";
    if (id < lt.nazwyTestow.size())
        nameTest = lt.nazwyTestow.at(id);
    procedura.startBadanie(id, nameTest, badanie, ust, zas, ster);
}

void ListaBadan::flt_zerowanieFiltrowDone()
{
    procedura.flt_zerowanieFiltrowDone();
}

void ListaBadan::flt_setUkladFiltrowDone()
{
    procedura.flt_setUkladFiltrowDone();
}

void ListaBadan::flt_bladFiltrow(short silnik, bool zerowanie)
{
    procedura.flt_bladFiltrow(silnik, zerowanie);
}
