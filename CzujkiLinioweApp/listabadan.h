#ifndef LISTABADAN_H
#define LISTABADAN_H

#include <QWidget>

#include "parametrybadania.h"
#include "danetestu.h"
#include "proceduratestowa.h"
#include "ustawienia.h"

class Zasilacz;
class Sterownik;
namespace Ui {
class ListaBadan;
}

class ListaBadan : public QWidget
{
    Q_OBJECT

public:
    explicit ListaBadan(QWidget *parent = nullptr);
    ~ListaBadan();
    void startBadanie(short id, const ParametryBadania & badanie, const Ustawienia &ust, Zasilacz *zas, Sterownik *ster);

    void flt_zerowanieFiltrowDone();
    void flt_setUkladFiltrowDone();
    void flt_bladFiltrow(short silnik, bool zerowanie);

private:
    Ui::ListaBadan *ui;
    DaneTestu test;
    ProceduraTestowa procedura;
    ListaTestow lt;
};

#endif // LISTABADAN_H
