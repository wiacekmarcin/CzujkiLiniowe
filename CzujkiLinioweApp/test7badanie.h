#ifndef TEST7BADANIE_H
#define TEST7BADANIE_H

#include <QDialog>
#include <QTimer>

#include "danetestu.h"
#include "parametrybadania.h"
namespace Ui {
class Test7Badanie;
}

class Test7Badanie : public QDialog
{
    Q_OBJECT

public:
    explicit Test7Badanie(const ParametryBadania &badanie, const DaneTestu &test, QWidget *parent = 0);
    ~Test7Badanie();
private slots:
    void uplynalCzasPostojuFiltra();
private:
    Ui::Test7Badanie *ui;
    QTimer zmFiltraTmr;
    float actTlumienie;
};

#endif // TEST7BADANIE_H
