#ifndef USTAWIENIANAPEDOW_H
#define USTAWIENIANAPEDOW_H

#include "ustawienia.h"
#include <QWidget>

namespace Ui {
class UstawieniaNapedow;
}

class UstawieniaNapedow : public QWidget
{
    Q_OBJECT

public:
    explicit UstawieniaNapedow(QWidget *parent = nullptr);
    ~UstawieniaNapedow();

    void setUstawienia(Ustawienia *u);
    void saveNapedy();
private:
    Ui::UstawieniaNapedow *ui;
    Ustawienia * ust;
};

#endif // USTAWIENIANAPEDOW_H
