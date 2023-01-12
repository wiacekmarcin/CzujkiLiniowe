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
    void flt_bladFiltrow(QChar filtr, bool zerowanie);
    void ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt);
    void ster_czujkaOn();
    void zas_value(int kind, int value);

    void setBadanie(const ParametryBadania & badanie);

protected:
    void initialTable(const ParametryBadania &badanie);
private slots:
    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::ListaBadan *ui;

    //DaneTestu test;
    ProceduraTestowa procedura;
    ListaTestow lt;
    bool wyzwalaniePradem;
    bool wyzwalaniePrzekaznikiem;
    bool zasilaczZewnetrzny;
    bool badanieWTrakcie;
    unsigned intCurrAlarm;
};

#endif // LISTABADAN_H
