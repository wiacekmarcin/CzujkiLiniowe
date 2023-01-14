#ifndef OKNOPODSUMOWANIETESTU_H
#define OKNOPODSUMOWANIETESTU_H

#include "parametrybadania.h"
#include <QDialog>
#include <QTimer>

#include "danetestu.h"
namespace Ui {
class OknoPodsumowanieTestu;
}


class OknoPodsumowanieTestu : public QDialog
{
    Q_OBJECT

public:
    explicit OknoPodsumowanieTestu(DaneTestu &daneTestu, const ParametryBadania &badanie,
                          QWidget *parent = 0);
    ~OknoPodsumowanieTestu();



protected:

private:
    Ui::OknoPodsumowanieTestu *ui;
    bool powtorzPomiar;
};

#endif // OKNOPODSUMOWANIETESTU_H


