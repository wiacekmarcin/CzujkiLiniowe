#ifndef TESTSTANOWISKADLG_H
#define TESTSTANOWISKADLG_H

#include <QDialog>
#include <QTimer>

class Ustawienia;
class Sterownik;
class Zasilacz;

namespace Ui {
class TestStanowiskaDlg;
}

class TestStanowiskaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TestStanowiskaDlg(Zasilacz * zas, Sterownik * ster, Ustawienia * ust, QWidget *parent = nullptr);
    ~TestStanowiskaDlg();
    void deviceNameZasilacz(const QString & serial);
    void valueZasilacz(int kind, int value);
    void configuredZasilacz(int state);
    void ster_zdarzenieSilnik(int );
    void configuredSterownik(int state);
    void deviceNameSter(const QString & serial);

    void flt_zerowanieFiltrowDone();
    void flt_setUkladFiltrowDone();
    void flt_bladFiltrow(QChar silnik, bool zerowanie);

private slots:
    void connect2Device();
    void pbFiltrUstaw();
    void changeDlugoscFiltra(const QString & text);
    void changeTlumienie(const QString & text);
    void ukladFiltrowTimeout();
    void zerowanieFiltrow();
private:
    Ui::TestStanowiskaDlg *ui;

    Zasilacz *zas;
    Sterownik *ster;
    Ustawienia * ust;
    QString zasilaczName;
    QString sterownikName;
    QString prevChooseFiltrDlugoscFali;
    QTimer czasUstF;

};

#endif // TESTSTANOWISKADLG_H

