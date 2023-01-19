#ifndef OKNOMONTAZ_H
#define OKNOMONTAZ_H

#include <QDialog>
#include "danetestu.h"
namespace Ui {
class OknoMontaz;
}

class OknoMontaz : public QDialog
{
    Q_OBJECT

public:
    explicit OknoMontaz(short opticalLen, const DaneTestu &daneTestu,
                         QWidget *parent = nullptr);
    ~OknoMontaz();

protected:
    void pbCancel_clicked();
private:
    Ui::OknoMontaz *ui;
};

#endif // OKNOMONTAZ_H
