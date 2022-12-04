#ifndef WYBORTESTU_H
#define WYBORTESTU_H

#include <QDialog>

namespace Ui {
class WyborTestu;
}

class WyborTestu : public QDialog
{
    Q_OBJECT

public:
    explicit WyborTestu(QWidget *parent = nullptr);
    ~WyborTestu();

private:
    Ui::WyborTestu *ui;
    QStringList nazwyTestow;
};

#endif // WYBORTESTU_H
