#include "sterownikustawieniawidget.h"
#include "ui_sterownikustawieniawidget.h"
#include "ustawienia.h"

#define SETCONF(N) ui->base##N->setText(0); \
                   ui->delay##N->setText(QString::number(u->getMotorCzasMiedzyImp##N())); \
                   ui->maxSteps##N->setText(QString::number(u->getMotorMaksIloscImp##N())); \
                   ui->obrot##N->setChecked(u->getMotorOdwrocObroty##N()); \
                   ui->ratio##N->setText(QString::number(u->getMotorPrzelozenieImpJedn##N()))

SterownikUstawieniaWidget::SterownikUstawieniaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SterownikUstawieniaWidget)
{
    ui->setupUi(this);
}

SterownikUstawieniaWidget::~SterownikUstawieniaWidget()
{
    delete ui;
}

void SterownikUstawieniaWidget::init(Ustawienia *u)
{
    this->u = u;
    SETCONF(1);
    SETCONF(2);
    SETCONF(3);
    SETCONF(4);
    SETCONF(5);
    SETCONF(6);
    SETCONF(7);
    SETCONF(8);
    SETCONF(9);
}

#define WRITECONF(N) /*u->setBaseStepsSilnik##N(ui->base##N->text());*/ \
                     u->setMotorCzasMiedzyImp##N(ui->delay##N->text()); \
                     u->setMotorMaksIloscImp##N(ui->maxSteps##N->text()); \
                     u->setMotorOdwrocObroty##N(ui->obrot##N->isChecked()); \
                     u->setMotorPrzelozenieImpJedn##N(ui->ratio##N->text())

void SterownikUstawieniaWidget::on_pbSetConfiguration_clicked()
{
    WRITECONF(1);
    WRITECONF(2);
    WRITECONF(3);
    WRITECONF(4);
    WRITECONF(5);
    WRITECONF(6);
    WRITECONF(7);
    WRITECONF(8);
    WRITECONF(9);
}
