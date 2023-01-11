#ifndef TEST9PODSUMOWANIE_H
#define TEST9PODSUMOWANIE_H

#include "parametrybadania.h"
#include <QDialog>
#include <QTimer>

#include "danetestu.h"
namespace Ui {
class Test9Podsumowanie;
}


class Test9Podsumowanie : public QDialog
{
    Q_OBJECT

public:
    explicit Test9Podsumowanie(const DaneTestu &daneTestu, const ParametryBadania &badanie,
                          QWidget *parent = 0);
    ~Test9Podsumowanie();

    bool getPowtorzPomiar() const;

protected:
    void pbCancel_clicked();
    void addOneRekordTable(short nrPozycji, short nrProby, const QString &pierwszy, const QString &drugi,
                           const QString & tlumienie, const QString &tlumienieText2, bool ok,  const QString &inne);
    void headTable(const QString &pierwszy, const QString &drugi);
    void feetTable(short nrPomiarow);
private:
    Ui::Test9Podsumowanie *ui;
    bool powtorzPomiar;
};

#endif // TEST9PODSUMOWANIE_H


