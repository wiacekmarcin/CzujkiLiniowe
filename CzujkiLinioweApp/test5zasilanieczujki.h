#ifndef TEST5ZASILANIECZUJKI_H
#define TEST5ZASILANIECZUJKI_H

#include <QDialog>

namespace Ui {
class Test5ZasilanieCzujki;
}

class Test5ZasilanieCzujki : public QDialog
{
    Q_OBJECT

public:
    explicit Test5ZasilanieCzujki(QWidget *parent = 0);
    ~Test5ZasilanieCzujki();

private:
    Ui::Test5ZasilanieCzujki *ui;
};

#endif // TEST5ZASILANIECZUJKI_H
