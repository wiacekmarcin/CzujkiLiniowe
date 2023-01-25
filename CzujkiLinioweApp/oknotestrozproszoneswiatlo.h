#ifndef OKNOTESTROZPROSZONESWIATLO_H
#define OKNOTESTROZPROSZONESWIATLO_H

#include <QDialog>
#include "danetestu.h"
namespace Ui {
class OknoTestRozproszoneSwiatlo;
}

class OknoTestRozproszoneSwiatlo : public QDialog
{
    Q_OBJECT

public:
    explicit OknoTestRozproszoneSwiatlo(const DaneTestu &daneTestu, QWidget *parent = nullptr);
    ~OknoTestRozproszoneSwiatlo();
    void czujkaOn();

    bool getAlarm() const;

protected:
    void pbCancel_clicked();
private:
    Ui::OknoTestRozproszoneSwiatlo *ui;
    bool alarm;
};

#endif // OKNOTESTROZPROSZONESWIATLO_H
