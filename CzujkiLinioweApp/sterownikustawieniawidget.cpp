#include "sterownikustawieniawidget.h"
#include "ui_sterownikustawieniawidget.h"
#include "ustawienia.h"

#define SETCONF(N) ui->base##N->setText(u->getBaseStepsSilnik##N()); \
                   ui->delay##N->setText(u->getDelayusSilnik##N()); \
                   ui->maxSteps##N->setText(u->getMaxStepsSilnik##N()); \
                   ui->obrot##N->setChecked(u->getLeftRotationSilnik##N()); \
                   ui->ratio##N->setText(u->getRatioSilnik##N())

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

#define WRITECONF(N) u->setBaseStepsSilnik##N(ui->base##N->text()); \
                     u->setDelayusSilnik##N(ui->delay##N->text()); \
                     u->setMaxStepsSilnik##N(ui->maxSteps##N->text()); \
                     u->setLeftRotationSilnik##N(ui->obrot##N->isChecked()); \
                     u->setRatioSilnik##N(ui->ratio##N->text())

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
