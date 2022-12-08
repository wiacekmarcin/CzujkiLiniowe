#ifndef WYBORTESTU_H
#define WYBORTESTU_H

#include <QDialog>
#include "danetestu.h"
#include <QList>
#include <QPair>

namespace Ui {
class WyborTestu;
}
class QRadioButton;
class WyborTestu : public QDialog
{
    Q_OBJECT

public:
    explicit WyborTestu(const ListaTestow & testy, bool powtarzalnosc, QWidget *parent = nullptr);
    ~WyborTestu();

    short getWyborTestu() const;

private:
    Ui::WyborTestu *ui;
    short wyborTestu;
    QList<QPair<short, QRadioButton*>> radio;
};

#endif // WYBORTESTU_H
