#ifndef TEST2POTWIERDZENIE_H
#define TEST2POTWIERDZENIE_H

#include <QDialog>

#include "danetestu.h"

namespace Ui {
    class Test2Potwierdzenie;
}

class Test2Potwierdzenie : public QDialog
{
    Q_OBJECT

public:
    explicit Test2Potwierdzenie(const DaneTestu & test, QWidget *parent = nullptr);
    ~Test2Potwierdzenie();

private:
    Ui::Test2Potwierdzenie *ui;
};

#endif // TEST2POTWIERDZENIE_H
