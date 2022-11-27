#ifndef USTAWIENIANAPEDOW_H
#define USTAWIENIANAPEDOW_H

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

private:
    Ui::UstawieniaNapedow *ui;
};

#endif // USTAWIENIANAPEDOW_H
