#ifndef TEST3SPRAWDZENIE_H
#define TEST3SPRAWDZENIE_H

#include <QDialog>
#include "danetestu.h"
namespace Ui {
class Test3Sprawdzenie;
}

class Test3Sprawdzenie : public QDialog
{
    Q_OBJECT

public:
    explicit Test3Sprawdzenie(short nrPomiaru, const DaneTestu &daneTestu, QWidget *parent = nullptr);
    ~Test3Sprawdzenie();

protected:
    void pbCancel_clicked();
private:
    Ui::Test3Sprawdzenie *ui;
};

#endif // TEST3SPRAWDZENIE_H
