#ifndef TEST3SPRAWDZENIE_H
#define TEST3SPRAWDZENIE_H

#include <QDialog>

namespace Ui {
class Test3Sprawdzenie;
}

class Test3Sprawdzenie : public QDialog
{
    Q_OBJECT

public:
    explicit Test3Sprawdzenie(QWidget *parent = nullptr);
    ~Test3Sprawdzenie();

private:
    Ui::Test3Sprawdzenie *ui;
};

#endif // TEST3SPRAWDZENIE_H
