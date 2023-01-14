#ifndef OKNOZEROWANIEURZADZENIA_H
#define OKNOZEROWANIEURZADZENIA_H

class Sterownik;
class QCheckBox;

#include <QDialog>
#include <QTimer>

namespace Ui {
class OknoZerowanieUrzadzenia;
}

class OknoZerowanieUrzadzenia : public QDialog
{
    Q_OBJECT

public:
    explicit OknoZerowanieUrzadzenia(bool ramiona, bool filtry, bool wozek, Sterownik * device, QWidget *parent = nullptr);
    ~OknoZerowanieUrzadzenia();

    void ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt);

private slots:
    void timeout();
private:
    Ui::OknoZerowanieUrzadzenia *ui;
    bool silnikZero[10];
    QCheckBox* buttons[10];
    QTimer timer;
    bool errorMsg;
};

#endif // OKNOZEROWANIEURZADZENIA_H
