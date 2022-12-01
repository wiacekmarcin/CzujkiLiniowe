#ifndef TESTZASILACZADLG_H
#define TESTZASILACZADLG_H

#include "ustawienia.h"
#include <QDialog>

namespace Ui {
class TestZasilaczaDlg;
}

class TestZasilaczaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TestZasilaczaDlg(Ustawienia * ust, QWidget *parent = nullptr);
    ~TestZasilaczaDlg();

protected slots:
    void on_pbCmdSend_clicked();
private:
    Ui::TestZasilaczaDlg *ui;
    Ustawienia * u;
};

#endif // TESTZASILACZADLG_H
