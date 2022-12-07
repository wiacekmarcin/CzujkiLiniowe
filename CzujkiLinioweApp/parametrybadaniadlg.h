#ifndef PARAMETRYBADANIADLG_H
#define PARAMETRYBADANIADLG_H

#include "danebadania.h"
#include "qtimer.h"
#include "ustawienia.h"
#include <QDialog>

namespace Ui {
class ParametryBadaniaDlg;
}

class ParametryBadaniaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ParametryBadaniaDlg(const Ustawienia &u, DaneBadania * d, QWidget *parent = nullptr);
    ~ParametryBadaniaDlg();
private slots:
    void checkValid();
    void on_pbOK_clicked();

    void on_pbCancel_clicked();

    void on_pbCancel_pressed();

    void on_pbOK_pressed();

private:
    Ui::ParametryBadaniaDlg *ui;
    DaneBadania * dane;
    QTimer checkValidTmr;
};

#endif // PARAMETRYBADANIADLG_H
