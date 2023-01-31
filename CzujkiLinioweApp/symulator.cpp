#include "symulator.h"
#include "ui_symulator.h"

#include <QDebug>

bool home;
bool move;
bool err;
bool inter;

#define HOME(N) connect(ui->pbHome##N, &QPushButton::clicked, this, [this]() { this->ster_setPositionDoneSig(N, RuchSilnikaType{true, false, false, false}); });

Symulator::Symulator(const Ustawienia & u_, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Symulator),
    timer(this),
    u(u_)
{
    QWidget *w = new QWidget(this);
    setCentralWidget(w);
    ui->setupUi(w);
    HOME(1);
    HOME(2);
    HOME(3);
    HOME(4);
    HOME(5);
    HOME(6);
    HOME(7);
    HOME(8);
    HOME(9);

    connect(ui->pbCzujkaOn, &QPushButton::clicked, this, &Symulator::ster_czujkaOn);
    connect(ui->pbHomeAll, &QPushButton::clicked, this, &Symulator::homeAll);
    connect(ui->pbStart, &QPushButton::clicked, this, &Symulator::pbStart);
    connect(ui->pbStop, &QPushButton::clicked, this, &Symulator::pbStop);
    timer.setInterval(200);
    ui->impBegin->setText("0");
    ui->impEnd->setText("0.51");
    ui->impSpeed->setText("0.3");
}

Symulator::~Symulator()
{
    delete ui;
}

void Symulator::ster_setPositionDoneSig(short silnik, RuchSilnikaType ruch)
{
    emit ster_setPositionDone(silnik, ruch);
}

void Symulator::homeAll()
{
    emit ster_setPositionDone(1, RuchSilnikaType{true, false, false, false});
    emit ster_setPositionDone(2, RuchSilnikaType{true, false, false, false});
    emit ster_setPositionDone(3, RuchSilnikaType{true, false, false, false});
    emit ster_setPositionDone(4, RuchSilnikaType{true, false, false, false});
    emit ster_setPositionDone(5, RuchSilnikaType{true, false, false, false});
    emit ster_setPositionDone(6, RuchSilnikaType{true, false, false, false});
    emit ster_setPositionDone(7, RuchSilnikaType{true, false, false, false});
    emit ster_setPositionDone(8, RuchSilnikaType{true, false, false, false});
    emit ster_setPositionDone(9, RuchSilnikaType{true, false, false, false});
}

void Symulator::pbStart()
{
    silnik = ui->comboBox->currentText().toInt();
    valBeg = u.wyliczPozycje(silnik, ui->impBegin->text().toDouble());
    valEnd = u.wyliczPozycje(silnik, ui->impEnd->text().toDouble());
    qDebug() << u.wyliczImp(u.getMotorPrzelozenieImpJedn(silnik), ui->impSpeed->text().toDouble());
    valPer100ms = 100000.0 / u.wyliczImp(u.getMotorPrzelozenieImpJedn(silnik), ui->impSpeed->text().toDouble());
    valAct = valBeg;
    if (valEnd < valBeg)
        valPer100ms *= -1;
    timer.start();
    qDebug() << valAct << valEnd << valPer100ms;
    emit ster_setPositionDone(silnik, RuchSilnikaType{false, true, false, false});
}

void Symulator::pbStop()
{
    timer.stop();
    emit ster_setPositionDone(silnik, RuchSilnikaType{false, false, false, false});
}

void Symulator::timeout()
{
    qDebug() << ".";
    valAct += valPer100ms;
    if (abs(valAct) > abs(valEnd))
        pbStop();
    emit ster_progressImp(silnik, (unsigned int)valAct);
}



