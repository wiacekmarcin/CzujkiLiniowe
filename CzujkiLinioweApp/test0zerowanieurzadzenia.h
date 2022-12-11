#ifndef TEST0ZEROWANIEURZADZENIA_H
#define TEST0ZEROWANIEURZADZENIA_H

#include <QDialog>

namespace Ui {
class Test0ZerowanieUrzadzenia;
}

class Test0ZerowanieUrzadzenia : public QDialog
{
    Q_OBJECT

public:
    explicit Test0ZerowanieUrzadzenia(QWidget *parent = nullptr);
    ~Test0ZerowanieUrzadzenia();

private:
    Ui::Test0ZerowanieUrzadzenia *ui;
};

#endif // TEST0ZEROWANIEURZADZENIA_H
