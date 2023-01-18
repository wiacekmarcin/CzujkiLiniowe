#ifndef OKNOZEROWANIEURZADZENIA_H
#define OKNOZEROWANIEURZADZENIA_H

#include "sterownik.h"
#include "debugdialog.h"
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


    void ster_setPositionDone(short silnik, RuchSilnikaType ruch);
signals:
    void debug(DebugType d, QString & html);
protected:
    void init();
    void pbCancel_clicked();
private slots:
    void timeout();
private:
    Ui::OknoZerowanieUrzadzenia *ui;
    bool silnikZero[10];
    QCheckBox* buttons[10];
    QTimer timer;
    bool errorMsg;

    bool ramiona;
    bool filtry;
    bool wozek;
    Sterownik *device;
};

#endif // OKNOZEROWANIEURZADZENIA_H
