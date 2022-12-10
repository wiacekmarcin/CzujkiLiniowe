#include "teststerownikadlg.h"
#include "ui_teststerownikadlg.h"
#include "ustawienia.h"
#include "sterownik.h"

#define SETCONF(N) ui->base##N->setText(QString::number(u->getMotorIloscImpBaza##N())); \
                   ui->delay##N->setText(QString::number(u->getMotorCzasMiedzyImp##N())); \
                   ui->maxSteps##N->setText(QString::number(u->getMotorMaksIloscImp##N())); \
                   ui->obrot##N->setChecked(u->getMotorOdwrocObroty##N()); \
                   ui->ratio##N->setText(QString::number(u->getMotorPrzelozenieImpJedn##N())); \
                   ui->srodekKroki##N->setText(QString::number(u->getMotorIloscImpSrodek##N())); \
                   ui->motorName##N->setText(u->getMotorNazwa##N());

#define SETCONF_ALL SETCONF(1) \
                    SETCONF(2) \
                    SETCONF(3) \
                    SETCONF(4) \
                    SETCONF(5) \
                    SETCONF(6) \
                    SETCONF(7) \
                    SETCONF(8) \
                    SETCONF(9)

#define CONN(N) connect(ui->pbHome_##N, &QPushButton::clicked, this, [this](){ this->pbHome_clicked(N, this->ui->delay##N->text()); }); \
                connect(ui->pbUstawPos_##N, &QPushButton::clicked, this, [this](){ this->pbUstawPos_clicked(N, ui->posX_##N->text(), ui->ratio##N->text()); }); \


#define CONN_ALL CONN(1) \
                 CONN(2) \
                 CONN(3) \
                 CONN(4) \
                 CONN(5) \
                 CONN(6) \
                 CONN(7) \
                 CONN(8) \
                 CONN(9)

#define CONN_STER(F) connect(sdv, &Sterownik::F , this, &TestSterownikaDlg::sd_##F)

TestSterownikaDlg::TestSterownikaDlg(Ustawienia *ust, Sterownik *sdv, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestSterownikaDlg),
    u(ust),
    sd(sdv)
{
    ui->setupUi(this);
    SETCONF_ALL

    CONN_ALL

    connect(ui->pbHomeAll, &QPushButton::clicked, this, &TestSterownikaDlg::pbHomeAll_clicked);
    connect(ui->pbSetConfiguration, &QPushButton::clicked, this, &TestSterownikaDlg::pbSetConfiguration_clicked);
    connect(ui->pbConnect, &QPushButton::clicked, this, &TestSterownikaDlg::pbConnect_clicked);

    //if (sd->c)
}

TestSterownikaDlg::~TestSterownikaDlg()
{
    delete ui;
}

#define WRITECONF(N) u->setMotorIloscImpBaza##N(ui->base##N->text()); \
                     u->setMotorCzasMiedzyImp##N(ui->delay##N->text()); \
                     u->setMotorMaksIloscImp##N(ui->maxSteps##N->text()); \
                     u->setMotorOdwrocObroty##N(ui->obrot##N->isChecked()); \
                     u->setMotorPrzelozenieImpJedn##N(ui->ratio##N->text()); \
                     u->setMotorIloscImpSrodek##N(ui->srodekKroki##N->text()); \
                     u->setMotorNazwa##N(ui->motorName##N->text());

#define WRITECONF_ALL WRITECONF(1) \
                      WRITECONF(2) \
                      WRITECONF(3) \
                      WRITECONF(4) \
                      WRITECONF(5) \
                      WRITECONF(6) \
                      WRITECONF(7) \
                      WRITECONF(8) \
                      WRITECONF(9)

void TestSterownikaDlg::pbSetConfiguration_clicked()
{
    WRITECONF_ALL
}

void TestSterownikaDlg::sd_deviceName(QString name)
{
    ui->dbg3->append(QString("deviceName name=%1").arg(name));
    ui->namePort->setText(name);
}

void TestSterownikaDlg::sd_kontrolerConfigured(bool success, int state)
{
    ui->dbg3->append(QString("kontrolerConfigured success=%1 state=%2").arg(success).arg(state));
    switch(state) {
    case Sterownik::NO_FOUND:
        ui->rbFound->setEnabled(false);
        ui->rbFound->setChecked(false);
        ui->rbAuth->setEnabled(false);
        ui->rbConf_2->setEnabled(false);
        ui->frame_3->setEnabled(false);
        ui->pbConnect->setEnabled(true);
        ui->pbDisconnect->setEnabled(false);
        break;
    case Sterownik::FOUND:
        ui->rbFound->setChecked(true);
        ui->rbAuth->setEnabled(true);
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(false);
        break;
    case Sterownik::NO_OPEN:
    case Sterownik::NO_READ:
    case Sterownik::IDENT_FAILD:
        ui->pbConnect->setEnabled(true);
        ui->pbDisconnect->setEnabled(false);
        break;
    case Sterownik::OPEN:
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);
        break;
    case Sterownik::IDENT_OK:
        ui->rbAuth->setChecked(true);
        ui->rbConf_2->setEnabled(true);
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);
        break;
    //case Sterownik::PARAMS_FAILD:
    //    ui->pbConnect->setEnabled(true);
    //    ui->pbDisconnect->setEnabled(false);
        break;
    //case Sterownik::PARAMS_OK:
    //    ui->rbConf_2->setChecked(true);
    //    ui->pbConnect->setEnabled(false);
    //    ui->pbDisconnect->setEnabled(true);
    //    break;
    case Sterownik::ALL_OK:
        ui->frame_3->setEnabled(true);
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);
        break;
    case Sterownik::CLOSE:
        ui->rbFound->setChecked(false);
        ui->rbAuth->setEnabled(false);
        ui->rbAuth->setChecked(false);
        ui->rbConf_2->setEnabled(false);
        ui->rbConf_2->setChecked(false);
        ui->frame_3->setEnabled(false);
        ui->pbConnect->setEnabled(true);
        ui->pbDisconnect->setEnabled(false);
        break;
    default:
        break;
    }
}

void TestSterownikaDlg::sd_setParamsDone(bool success)
{

}

void TestSterownikaDlg::sd_disconnect()
{

}

void TestSterownikaDlg::sd_setPositionDone(bool home, bool success)
{

}

void TestSterownikaDlg::pbConnect_clicked()
{
    ui->dbg3->append(QString("Connect vendor=%1 product=%2").arg(sd->getVendor(), sd->getProduct()));
    sd->connectToDevice();
}

void TestSterownikaDlg::sd_debug(const QString &d)
{
    ui->dbg1->append(d.sliced(20));
}

void TestSterownikaDlg::sd_error(const QString & e)
{
    ui->dbg2->append(e);
}

void TestSterownikaDlg::pbHome_clicked(int silnik, const QString & impTime)
{
    qDebug() << silnik << " home " << impTime;
    bool ok;
    unsigned int val = impTime.toUInt(&ok);
    if (!ok)
        val = 10000;
    sd->setPositionSilnik(silnik, true, 0, val);
}

void TestSterownikaDlg::pbHomeAll_clicked()
{
    for (int i=1; i<10; ++i) {
        QLineEdit * e = nullptr;
        switch(i) {
            case 1: e = ui->delay1; break;
            case 2: e = ui->delay2; break;
            case 3: e = ui->delay3; break;
            case 4: e = ui->delay4; break;
            case 5: e = ui->delay5; break;
            case 6: e = ui->delay6; break;
            case 7: e = ui->delay7; break;
            case 8: e = ui->delay8; break;
            case 9: e = ui->delay9; break;
        }
        if (e)
            pbHome_clicked(i, e->text());
    }
}

void TestSterownikaDlg::pbUstawPos_clicked(int silnik, const QString &x, const QString &ratio)
{
    qDebug() << silnik << x << ratio;
}
