#ifndef LISTABADAN_H
#define LISTABADAN_H

#include <QWidget>

#include "parametrybadania.h"
#include "danetestu.h"
#include "proceduratestowa.h"
#include "ustawienia.h"
#include "sterownik.h"

class Zasilacz;
class Sterownik;
class QWidget;

namespace Ui {
class ListaBadan;
}

struct testWidget {
    QWidget * page;
    QWidget * wyniki;
    QPushButton * button;
};

class ListaBadan : public QWidget
{
    Q_OBJECT

public:
    explicit ListaBadan(QWidget *parent = nullptr);
    ~ListaBadan();
    void startBadanie(short id, ParametryBadania &badanie, const Ustawienia &ust, Zasilacz *zas, Sterownik *ster);

    void flt_zerowanieFiltrowDone();
    void flt_setUkladFiltrowDone();
    void flt_bladFiltrow(QChar filtr, bool zerowanie);
    //void ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt);
    void ster_setPositionDone(short silnik, RuchSilnikaType ruch);
    void ster_setValue(short silnik, const double & val);
    void ster_czujkaOn();
    void zas_value(int kind, int value);

    void setBadanie(const ParametryBadania & badanie);

    void setDaneTest(const DaneTestu &daneTestu, const ParametryBadania &badanie);
    void setUkonczoneBadanie(short id, const ParametryBadania &badanie);
protected:
    void initialTable(const ParametryBadania &badanie);
    void initOdtwarzalnoscTable(const QString &nadajnik, const QString &odbiornik);
    void addOdtwarzalnoscRekord(short num, short nrPomiaru, short sortPomiar, const QString &numerNadajnika,
                                const QString &numerOdbiornika, const QString &value_dB, const QString &value_perc,
                                bool ok, const QString &error);

    void addPowtarzalnoscRekord(short num, const QString & value_dB, const QString & value_perc, bool ok, const QString & error);
    void initPowtarzalnoscTable();

    void initSzybkieZmianyTlumieniaTable();
    void addSzybkieZmianyTlumieniaRekord(short num, const QString & value_dB, bool ok, const QString & error);

private slots:
    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::ListaBadan *ui;


    ProceduraTestowa procedura;
    ListaTestow lt;
    bool wyzwalaniePradem;
    bool wyzwalaniePrzekaznikiem;
    bool zasilaczZewnetrzny;
    bool badanieWTrakcie;
    unsigned intCurrAlarm;
    QMap<int, testWidget> testyWidget;
};

#endif // LISTABADAN_H
