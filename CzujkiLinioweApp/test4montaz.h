#ifndef TEST4MONTAZ_H
#define TEST4MONTAZ_H

#include <QDialog>
#include "danetestu.h"
namespace Ui {
class Test4Montaz;
}

class Test4Montaz : public QDialog
{
    Q_OBJECT

public:
    explicit Test4Montaz(short nrPomiaru, const DaneTestu &daneTestu,
                         QWidget *parent = nullptr);
    ~Test4Montaz();

protected:
    void pbCancel_clicked();
private:
    Ui::Test4Montaz *ui;
};

#endif // TEST4MONTAZ_H
