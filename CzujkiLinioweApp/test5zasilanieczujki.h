#ifndef TEST5ZASILANIECZUJKI_H
#define TEST5ZASILANIECZUJKI_H
#include "parametrybadania.h"
#include "danetestu.h"
#include <QDialog>

namespace Ui {
class Test5ZasilanieCzujki;
}

class Test5ZasilanieCzujki : public QDialog
{
    Q_OBJECT

public:
    explicit Test5ZasilanieCzujki(const ParametryBadania &badanie, const DaneTestu &test, QWidget *parent = 0);
    ~Test5ZasilanieCzujki();

protected:
    void pbCancel_clicked();
private:
    Ui::Test5ZasilanieCzujki *ui;
};

#endif // TEST5ZASILANIECZUJKI_H
