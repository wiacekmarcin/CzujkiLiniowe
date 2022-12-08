#ifndef PARAMETRYBADANIADLG_H
#define PARAMETRYBADANIADLG_H

#include "parametrybadania.h"
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
    explicit ParametryBadaniaDlg(const Ustawienia &u, ParametryBadania * d, QWidget *parent = nullptr);
    ~ParametryBadaniaDlg();
private slots:
    void checkValid();
    void pbOK_clicked();
    void pbCancel_clicked();

private:
    Ui::ParametryBadaniaDlg *ui;
    ParametryBadania * dane;
    QTimer checkValidTmr;
};

#endif // PARAMETRYBADANIADLG_H
