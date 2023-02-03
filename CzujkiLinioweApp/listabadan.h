#ifndef LISTABADAN_H
#define LISTABADAN_H

#include <QWidget>

#include "parametrybadania.h"
#include "danetestu.h"
#include "proceduratestowa.h"
#include "ustawienia.h"
#include "sterownik.h"
#include <QDateTime>
#include <QList>
#include <QStringList>

class Zasilacz;
class Sterownik;
class QWidget;
class QTableWidget;
class QLabel;

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
    bool startBadanie(short id, ParametryBadania &badanie, const Ustawienia &ust, Zasilacz *zas, Sterownik *ster);
    void clearBadanie();

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
    const QMap<int, testWidget> &getTestyWidget() const;
    void closeBadanie();

protected:
    void initialTestyTable(const ParametryBadania &badanie);

    void startBadanieRequest(int testId);
private slots:
    void on_tableWidget_cellClicked(int row, int column);

signals:
    void startBadanieReq(int tid);

protected:
    double d2p(const double & val);
    QString d2p(const QString & val);

    void narazeniaWynik(const DaneTestu &daneTestu, const ParametryBadania &badanie,
                        const QString &transmitter, const QString &receiver, float cmaxcmin,
                        const QString &opisNarazenia, QTableWidget *tableParams, QTableWidget *tablePrzebieg,
                        QTableWidget *tableNarazenia, QTableWidget *tableCzujka, QLabel *wynik);
private:
    void addC(QTableWidget * table, const QString & Cname, const QString & val1, const QString & val2, int row);
    void addC(QTableWidget * table, const QString & Cname, const QString & val, int row);
    void clearinitTable( QTableWidget * table, const QStringList & head, const QList<int> & width);
    void initCzujkaInfo(QTableWidget *table, const QString &transmitterName, const QString &receiverName, const QString &nrCzujki, const QString &transmitter, const QString &receiver);
    void initNarazenieInfo(QTableWidget *table, bool wynik, const QString &rodzaj, const QString &uwagi);

    void addR0(QTableWidget * table, int row);
    short addR1(QTableWidget * table, int row, int col, const QString & C1);
    short addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2);
    short addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2, const QString &C3);
    short addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2, const QString &C3, const QString &C4);
    short addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2, const QString &C3, const QString &C4, const QString &C5);
    short addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2, const QString &C3, const QString &C4, const QString &C5,
                const QString & C6);

    short addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2, const QString &C3, const QString &C4, const QString &C5,
                const QString & C6, const QString & C7);

    short addR(QTableWidget * table, int row, int col, const QString & C1, const QString & C2, const QString &C3, const QString &C4, const QString &C5,
                const QString & C6, const QString & C7, const QString & C8);

    short addR(QTableWidget * table, int row, int col, const QString &C1, const QString & C2, const QString &C3, const QString &C4, const QString &C5,
                const QString & C6, const QString & C7, const QString &C8, const QString &C9);

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
    QDateTime prevCzujkaOn;
    bool odtwarzalnosc;
};

#endif // LISTABADAN_H
