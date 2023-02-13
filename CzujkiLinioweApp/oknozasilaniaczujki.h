#ifndef OKNOZASILANIACZUJKI_H
#define OKNOZASILANIACZUJKI_H
#include "parametrybadania.h"
#include "danetestu.h"
#include <QDialog>
#include <QCloseEvent>
namespace Ui {
class OknoZasilaniaCzujki;
}

class OknoZasilaniaCzujki : public QDialog
{
    Q_OBJECT

public:
    explicit OknoZasilaniaCzujki(bool minVoltage, bool maxVoltage, bool minCzulosc, const DaneTestu &daneTestu, const ParametryBadania &daneBadania,
                                  QWidget *parent = 0);
    ~OknoZasilaniaCzujki();
    void setCurrent_mA(int mA);
    void setVolage_mV(int mV);
public slots:
    void value(int kind, int value);
    void closeEvent(QCloseEvent *event);
protected:
    void pbCancel_clicked();
private:
    Ui::OknoZasilaniaCzujki *ui;
};

#endif // OKNOZASILANIACZUJKI_H
