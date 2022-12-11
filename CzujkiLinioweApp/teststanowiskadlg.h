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
    void deviceNameZasilacz(const QString & serial);
    void valueZasilacz(int kind, int value);
    void configuredZasilacz(int state);
    void ster_zdarzenieSilnik(int );
    void configuredSterownik(int state);
    void deviceNameSter(const QString & serial);
private slots:
    void connect2Device();
private:
    Ui::TestStanowiskaDlg *ui;

    Zasilacz *zas;
    Sterownik *ster;
    QString zasilaczName;
    QString sterownikName;
};

#endif // TESTSTANOWISKADLG_H

