#ifndef PARAMETRYBADANIA_H
#define PARAMETRYBADANIA_H

#include <QDialog>

namespace Ui {
class ParametryBadania;
}

class ParametryBadania : public QDialog
{
    Q_OBJECT

public:
    explicit ParametryBadania(QWidget *parent = nullptr);
    ~ParametryBadania();

private:
    Ui::ParametryBadania *ui;
};

#endif // PARAMETRYBADANIA_H
