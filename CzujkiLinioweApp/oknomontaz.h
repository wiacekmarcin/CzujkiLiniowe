#ifndef OKNOMONTAZ_H
#define OKNOMONTAZ_H

#include <QDialog>
#include <QCloseEvent>
#include "danetestu.h"
namespace Ui {
class OknoMontaz;
}

class OknoMontaz : public QDialog
{
    Q_OBJECT

public:
    explicit OknoMontaz(bool usuniecieZabezp, bool minRozstawienie, bool maxRozstawienie,
                        bool rozproszone, bool systemNadajnikObiornik, const DaneTestu &daneTestu,
                         QWidget *parent = nullptr);
    ~OknoMontaz();

protected:
    void pbCancel_clicked();
    void closeEvent(QCloseEvent *event);
private:
    Ui::OknoMontaz *ui;
};

#endif // OKNOMONTAZ_H
