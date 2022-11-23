#ifndef PARAMETRYBADANIA1_H
#define PARAMETRYBADANIA1_H

#include <QDialog>
#include "ustawienia.h"
#include "badaniedane.h"
namespace Ui {
class ParametryBadania1;
}

class ParametryBadania1 : public QDialog
{
    Q_OBJECT

public:
    explicit ParametryBadania1(QWidget *parent = nullptr);
    ~ParametryBadania1();

    void init(const Ustawienia & u, BadanieDane * badanie);
protected:
    void setWewnetrznyZasilacz(bool inside);
    void setWyzwolenieAlarmu(bool przekaznik);
private:
    Ui::ParametryBadania1 *ui;
    BadanieDane * badanie;
};

#endif // PARAMETRYBADANIA1_H
