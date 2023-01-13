#ifndef TEST0ZEROWANIEURZADZENIA_H
#define TEST0ZEROWANIEURZADZENIA_H

class Sterownik;
class QCheckBox;

#include <QDialog>
#include <QTimer>

namespace Ui {
class Test0ZerowanieUrzadzenia;
}

class Test0ZerowanieUrzadzenia : public QDialog
{
    Q_OBJECT

public:
    explicit Test0ZerowanieUrzadzenia(bool ramiona, bool filtry, bool wozek, Sterownik * device, QWidget *parent = nullptr);
    ~Test0ZerowanieUrzadzenia();

    void ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt);

private slots:
    void timeout();
private:
    Ui::Test0ZerowanieUrzadzenia *ui;
    bool silnikZero[10];
    QCheckBox* buttons[10];
    QTimer timer;
    bool errorMsg;
};

#endif // TEST0ZEROWANIEURZADZENIA_H
