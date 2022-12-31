#include "ustawienianapedow.h"
#include "ui_ustawienianapedow.h"
#include "ustawienia.h"

#include <QMessageBox>
UstawieniaNapedow::UstawieniaNapedow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UstawieniaNapedow)
{
    ui->setupUi(this);

}

UstawieniaNapedow::~UstawieniaNapedow()
{
    delete ui;
}

#define READ_M(N)   ui->nazwa_##N->setText(ust->getMotorNazwa##N());\
                    ui->speedMax1_##N->setText(QString::number(ust->wyliczPredkosc(ust->getMotorPrzelozenieImpJedn##N(),ust->getMotorMinOdstepImp##N()), 'f')); \
                    ui->speed##N->setText(QString::number(ust->wyliczPredkosc(ust->getMotorPrzelozenieImpJedn##N(),ust->getMotorCzasMiedzyImpNormal##N()), 'f')); \
                    ui->speedMax1_##N->setReadOnly(true); /*ui->speedMax1_##N->setEnabled(false);*/

#define READ_MOTORS     READ_M(1) \
                        READ_M(2) \
                        READ_M(3) \
                        READ_M(4) \
                        READ_M(5) \
                        READ_M(6) \
                        READ_M(7) \
                        READ_M(8) \
                        READ_M(9)

void UstawieniaNapedow::setUstawienia(Ustawienia *u)
{
    ust = u;

    READ_MOTORS
}


#define WRITE_M(N)  ust->setMotorNazwa##N(ui->nazwa_##N->text()); \
                    ust->setMotorCzasMiedzyImpNormal##N(ust->wyliczImp(ust->getMotorPrzelozenieImpJedn##N(), ui->speed##N->text().toDouble()));


#define WRITE_MOTORS    WRITE_M(1) \
                        WRITE_M(2) \
                        WRITE_M(3) \
                        WRITE_M(4) \
                        WRITE_M(5) \
                        WRITE_M(6) \
                        WRITE_M(7) \
                        WRITE_M(8) \
                        WRITE_M(9)

void UstawieniaNapedow::saveNapedy()
{
    if (!check(ui->speed1))
        return;
    if (!check(ui->speed2))
        return;
    if (!check(ui->speed3))
        return;
    if (!check(ui->speed4))
        return;
    if (!check(ui->speed5))
        return;
    if (!check(ui->speed6))
        return;
    if (!check(ui->speed7))
        return;
    if (!check(ui->speed8))
        return;
    if (!check(ui->speed9))
        return;
    WRITE_MOTORS
    ust->sync();
}

bool UstawieniaNapedow::check(QLineEdit * line)
{
    QString text = line->text();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Ustawienia napedów", "Pole z prędkością nie może być puste");
        return false;
    }
    bool ok;
    double val = text.toDouble(&ok);
    if (!ok || val < 0) {
        QMessageBox::warning(this, "Ustawienia napedów", "Pole z prędkością zawiera nie poprawną wartość");
        return false;
    }
    return true;
}
