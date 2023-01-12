#ifndef ODTWARZALNOSCWYNIKI_H
#define ODTWARZALNOSCWYNIKI_H
#include "danetestu.h"
#include "parametrybadania.h"
#include <QWidget>

namespace Ui {
class OdtwarzalnoscWyniki;
}

class OdtwarzalnoscWyniki : public QWidget
{
    Q_OBJECT

public:
    explicit OdtwarzalnoscWyniki(QWidget *parent = nullptr);
    ~OdtwarzalnoscWyniki();
    void setPodsumowanie(bool pods);
    void setDaneTest(DaneTestu &daneTestu, const ParametryBadania &badanie);
protected:
    void addRekordPodsumowanie(short r, short nrProby, const QString &nadajnik, const QString &odbiornik, const QString &tlumienie_db, const QString &tlumienie_per, 
    bool ok, const QString &inneText);

    void addRekordWyniki(short num, short nrPomiaru, const QString & numerNadajnika, const QString & numerOdbiornika,
                              const QString & value_dB, const QString & value_perc, bool ok, const QString & error)
    void headTable(const QString &nadajnik, const QString &odbiornik);
    void oneHeadRecord(const QString &objectName, const QString &text, int row, int col);
    void oneTableTd(const QString &objectName, const QString &text, int row, int col);
    void oneTableFrame(const QString &objectName, const QString &text, int row, int col);
    void addLine(const QString &objectName, bool vert, int row, int col, int rowspan, int colspan);
    void initWynikTable(const QString &nadajnik, const QString &odbiornik);
private:
    Ui::OdtwarzalnoscWyniki *ui;
};

#endif // ODTWARZALNOSCWYNIKI_H
