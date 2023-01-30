#ifndef OKNOTESTROZPROSZONESWIATLO_H
#define OKNOTESTROZPROSZONESWIATLO_H

#include <QDialog>
#include "danetestu.h"
#include <QTime>
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

    QString getInfo() const;

protected:
    void pbCancel_clicked();
    void pbNext_clicked();
private:
    Ui::OknoTestRozproszoneSwiatlo *ui;
    bool alarm;
    unsigned long secsFromEpocsStart;
    QString info;
};

#endif // OKNOTESTROZPROSZONESWIATLO_H
