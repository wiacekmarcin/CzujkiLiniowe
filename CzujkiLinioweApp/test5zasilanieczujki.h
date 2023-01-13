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
    explicit Test5ZasilanieCzujki(const DaneTestu &daneTestu, const ParametryBadania &daneBadania,
                                  QWidget *parent = 0);
    ~Test5ZasilanieCzujki();
    void setCurrent_mA(int mA);
    void setVolage_mV(int mV);
public slots:
    void value(int kind, int value);
protected:
    void pbCancel_clicked();
private:
    Ui::Test5ZasilanieCzujki *ui;
};

#endif // TEST5ZASILANIECZUJKI_H
