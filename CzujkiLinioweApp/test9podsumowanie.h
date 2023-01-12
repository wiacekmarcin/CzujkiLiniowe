#ifndef TEST9PODSUMOWANIE_H
#define TEST9PODSUMOWANIE_H

#include "parametrybadania.h"
#include <QDialog>
#include <QTimer>

#include "danetestu.h"
namespace Ui {
class Test9Podsumowanie;
}


class Test9Podsumowanie : public QDialog
{
    Q_OBJECT

public:
    explicit Test9Podsumowanie(const DaneTestu &daneTestu, const ParametryBadania &badanie,
                          QWidget *parent = 0);
    ~Test9Podsumowanie();



protected:

private:
    Ui::Test9Podsumowanie *ui;
    bool powtorzPomiar;
};

#endif // TEST9PODSUMOWANIE_H


