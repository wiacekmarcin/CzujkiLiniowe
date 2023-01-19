#ifndef OKNOPODSUMOWANIETESTU_H
#define OKNOPODSUMOWANIETESTU_H

#include "parametrybadania.h"
#include <QDialog>
#include <QTimer>

#include "danetestu.h"
#include "ustawienia.h"

class QFrame;
class QGridLayout;
class QLabel;

namespace Ui {
class OknoPodsumowanieTestu;
}

struct ZaleznoscKatowaLabelsPtr {
    QLabel * katproducenta;
    QLabel * katmierzony;
    QLabel * wynik;
};


class OknoPodsumowanieTestu : public QDialog
{
    Q_OBJECT

public:
    explicit OknoPodsumowanieTestu(DaneTestu &daneTestu, const ParametryBadania &badanie,
                                   const Ustawienia & ust,
                          QWidget *parent = 0);
    ~OknoPodsumowanieTestu();



protected:

    void odtwarzalnoscAddRekord(QFrame * fr, QGridLayout * lay, const QString & suffix,
                                       short r, short nrProby, const QString & nadajnik, const QString & odbiornik,
                                                                         const QString &tlumienie_db, const QString &tlumienie_per,
                                                                         bool ok, const QString &inneText);
    void odtwarzalnoscHeadTable(QFrame *frameTable, QGridLayout *layout, const QString &suffix, const QString &nadajnik, const QString &odbiornik);
    void powtarzalnoscHeadTable(QFrame *frameTable, QGridLayout *layout, const QString &suffix);


    void powtarzalnoscAddRekord(QFrame * fr, QGridLayout * lay, const QString & suffix,
                                       short r,
                                                                         const QString &tlumienie_db, const QString &tlumienie_per,
                                                                         bool ok, const QString &inneText);

    void oneHeadRecord(QFrame *frameTable, QGridLayout *layout, const QString &text, int row, int col, const QString &objectName);
    void oneTableTd(bool err, QFrame *frameTable, QGridLayout *layout, const QString &text, int row, int col, const QString &objectName);
    void oneTableFrame(bool err, QFrame *frame, QGridLayout *layout, const QString &text, int row, int col, const QString &objectName);
    void addLine(QFrame * frameTable, QGridLayout * layout, bool vert, int row, int col, int rowspan, int colspan, const QString & objectName);


    void szybkiezmianytlumieniaAddRekord(QFrame *fr, QGridLayout *lay, const QString &suffix, short r, const QString &tlumienie_db, const QString &tlumienie_per, bool ok, const QString &inneText);
    void szybkieZmianyTlumieniaHeadTable(QFrame *fr, QGridLayout *lay, const QString &suffix);
private:
    Ui::OknoPodsumowanieTestu *ui;
    const Ustawienia & ust;

    QVector<ZaleznoscKatowaLabelsPtr> zaleznoscKatowaVector;
};

#endif // OKNOPODSUMOWANIETESTU_H


