#ifndef OKNOPODSUMOWANIETESTU_H
#define OKNOPODSUMOWANIETESTU_H

#include "parametrybadania.h"
#include <QDialog>
#include <QTimer>

#include "danetestu.h"

class QFrame;
class QGridLayout;

namespace Ui {
class OknoPodsumowanieTestu;
}


class OknoPodsumowanieTestu : public QDialog
{
    Q_OBJECT

public:
    explicit OknoPodsumowanieTestu(DaneTestu &daneTestu, const ParametryBadania &badanie,
                          QWidget *parent = 0);
    ~OknoPodsumowanieTestu();



protected:

    void addRekordPodsumowanie(QFrame * fr, QGridLayout * lay, const QString & suffix,
                                       short r, short nrProby, const QString & nadajnik, const QString & odbiornik,
                                                                         const QString &tlumienie_db, const QString &tlumienie_per,
                                                                         bool ok, const QString &inneText);
    void headTable(QFrame *frameTable, QGridLayout *layout, const QString &suffix, const QString &nadajnik, const QString &odbiornik);
    void oneHeadRecord(QFrame *frameTable, QGridLayout *layout, const QString &objectName, const QString &text, int row, int col);
    void oneTableTd(bool err, QFrame *frameTable, QGridLayout *layout, const QString &objectName, const QString &text, int row, int col);
    void oneTableFrame(bool err, QFrame *frame, QGridLayout *layout, const QString &objectName, const QString &text, int row, int col);
    void addLine(QFrame *frame, QGridLayout *layout, const QString & objectName, bool vert, int row, int col, int rowspan, int colspan);
    void obliczOdtwarzalnosc(DaneTestu &daneTestu);
private:
    Ui::OknoPodsumowanieTestu *ui;
    bool powtorzPomiar;
};

#endif // OKNOPODSUMOWANIETESTU_H


