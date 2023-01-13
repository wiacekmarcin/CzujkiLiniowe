#include "test0zerowanieurzadzenia.h"
#include "ui_test0zerowanieurzadzenia.h"

#include "sterownik.h"

#include <QMessageBox>
#include <QRadioButton>

Test0ZerowanieUrzadzenia::Test0ZerowanieUrzadzenia(bool ramiona, bool filtry, bool wozek, Sterownik *device, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test0ZerowanieUrzadzenia),
    timer(this),
    errorMsg(false)
{
    ui->setupUi(this);
    for (short id = 0; id < 10; ++id) {
        silnikZero[id] = id == 0;
        if (!ramiona && (id == 1 || id == 2 || id == 8 || id == 9))
            silnikZero[id] = true;
        else if (!filtry && (id == 3 || id == 4 || id == 5))
            silnikZero[id] = true;
        else if (!wozek && (id == 6 || id == 7))
            silnikZero[id] = true;
        else
            silnikZero[id] = false;

    }
    buttons[0] = nullptr;
    buttons[1] = ui->rb_pierwszy_poziom;
    buttons[2] = ui->rb_pierwwszy_pion;
    buttons[3] = ui->rb_a;
    buttons[4] = ui->rb_b;
    buttons[5] = ui->rb_c;
    buttons[6] = ui->wozek_poziom;
    buttons[7] = ui->wozek_pion;
    buttons[8] = ui->rb_drugi_poziom;
    buttons[9] = ui->rb_drugi_pion;

    ui->frame_filtry->setVisible(filtry);
    ui->frame_nadajnik->setVisible(ramiona);
    ui->frame_odbiornik->setVisible(ramiona);
    ui->frame_wozek->setVisible(wozek);
    adjustSize();

    device->setZerowanieUrzadzen(ramiona, filtry, wozek);
    timer.singleShot(5000, this, &Test0ZerowanieUrzadzenia::timeout);
}

Test0ZerowanieUrzadzenia::~Test0ZerowanieUrzadzenia()
{
    if (timer.isActive())
        timer.stop();
    delete ui;
}

void Test0ZerowanieUrzadzenia::ster_setPositionDone(short silnik, bool home, bool move, bool error, bool interrupt)
{
    if (!home || move)
        return;

    if (error) {
        if (!errorMsg) {
            errorMsg = true;
            QMessageBox::critical(this, QString::fromUtf8("Czujki Liniowe - zerowanie urządzenia"),
                    QString("Podczas zerowania silnika numer %1 wystąpił błąd").arg(1));
            reject();
        }
    }
    if (interrupt) {
        if (!errorMsg) {
            errorMsg = true;
            QMessageBox::warning(this, QString::fromUtf8("Czujki Liniowe - zerowanie urządzenia"),
                    QString("Podczas zerowania silnika numer %1 wystąpił żadanie zatrzymania silnika").arg(1));
            reject();
        }
    }
    silnikZero[silnik] = true;
    if (buttons[silnik])
        buttons[silnik]->setChecked(true);
    for (short id = 0; id < 10; ++id) {
        if (!silnikZero[id])
            return;
    }
    if (timer.isActive())
        timer.stop();
    accept();
}

void Test0ZerowanieUrzadzenia::timeout()
{
    if (errorMsg)
        return;
    errorMsg = true;
    QMessageBox::warning(this, QString::fromUtf8("Czujki Liniowe - zerowanie urządzenia"),
                QString("Upłynął czas potrzebny do wyzerowania urządzenia"));
    reject();
}
