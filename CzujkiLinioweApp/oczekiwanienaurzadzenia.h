#ifndef OCZEKIWANIENAURZADZENIA_H
#define OCZEKIWANIENAURZADZENIA_H

#include "sterownik.h"
#include "zasilacz.h"
#include <QTimer>
#include <QDialog>

namespace Ui {
class OczekiwanieNaUrzadzenia;
}

class OczekiwanieNaUrzadzenia : public QDialog
{
    Q_OBJECT

public:
    explicit OczekiwanieNaUrzadzenia(bool zasilacz, Zasilacz * zas_, Sterownik * ster_, QWidget *parent = nullptr);
    ~OczekiwanieNaUrzadzenia();
public slots:
    void zasilacz(int state);
    void sterownik(int state);
protected:

    void init();
    void pbCancel_clicked();
private slots:
    void timeout();
private:
    Ui::OczekiwanieNaUrzadzenia *ui;
    QTimer timer;
    short cntTmt;
    bool zasilaczOk;
    bool sterownikOk;
    bool koniecznyZasilacz;
    Sterownik * ster;
    Zasilacz * zas;

    static constexpr short maxCzas = 60;
};

#endif // OCZEKIWANIENAURZADZENIA_H
