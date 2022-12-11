#ifndef TESTSTANOWISKADLG_H
#define TESTSTANOWISKADLG_H

#include <QDialog>

class Sterownik;
class Zasilacz;

namespace Ui {
class TestStanowiskaDlg;
}

class TestStanowiskaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TestStanowiskaDlg(Zasilacz * zas, Sterownik * ster, QWidget *parent = nullptr);
    ~TestStanowiskaDlg();
    void zas_serialNo(const QString & serial);
    void deviceNameZasilacz(const QString & serial);
    void valueZasilacz(int kind, int value);

private:
    Ui::TestStanowiskaDlg *ui;

    Zasilacz *zas;
    Sterownik *ster;
};

#endif // TESTSTANOWISKADLG_H

