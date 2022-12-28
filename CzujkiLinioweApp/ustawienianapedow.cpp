#include "ustawienianapedow.h"
#include "ui_ustawienianapedow.h"
#include "ustawienia.h"

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
                    ui->speed##N->setText(QString::number(ust->wyliczPredkosc(ust->getMotorPrzelozenieImpJedn##N(),ust->getMotorCzasMiedzyImp##N()), 'f')); \

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


#define WRITE_M(N)  ust->setMotorNazwa##N(ui->nazwa_##N->text());


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
    WRITE_MOTORS
    ust->sync();
}
