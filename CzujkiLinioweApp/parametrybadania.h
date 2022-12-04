#ifndef PARAMETRYBADANIA_H
#define PARAMETRYBADANIA_H

#include "danebadania.h"
#include "qtimer.h"
#include "ustawienia.h"
#include <QDialog>

namespace Ui {
class ParametryBadania;
}

class ParametryBadania : public QDialog
{
    Q_OBJECT

public:
    explicit ParametryBadania(const Ustawienia &u, DaneBadania * d, QWidget *parent = nullptr);
    ~ParametryBadania();
private slots:
    void checkValid();
    void on_pbOK_clicked();

private:
    Ui::ParametryBadania *ui;
    DaneBadania * dane;
    QTimer checkValidTmr;
};

#endif // PARAMETRYBADANIA_H
