        #include "teststerownikadlg.h"
#include "ui_teststerownikadlg.h"
#include "ustawienia.h"
#include "sterownik.h"
#define SETCONF(N) ui->base##N->setText(QString::number(u->getMotorIloscKrokowBaza##N())); \
                   ui->delay##N->setText(QString::number(u->getMotorOpoznienieImp##N())); \
                   ui->maxSteps##N->setText(QString::number(u->getMotorMaksIloscKrokow##N())); \
                   ui->obrot##N->setChecked(u->getMotorOdwrocObroty##N()); \
                   ui->ratio##N->setText(QString::number(u->getMotorPrzelozenie##N())); \
                   ui->srodekKroki##N->setText(QString::number(u->getMotorIloscKrokowSrodek##N()));

#define SETCONF_ALL SETCONF(1) \
                    SETCONF(2) \
                    SETCONF(3) \
                    SETCONF(4) \
                    SETCONF(5) \
                    SETCONF(6) \
                    SETCONF(7) \
                    SETCONF(8) \
                    SETCONF(9)

TestSterownikaDlg::TestSterownikaDlg(Ustawienia *ust, Sterownik *sdv, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestSterownikaDlg),
    u(ust),
    sd(sdv)
{
    ui->setupUi(this);
    SETCONF_ALL
}

TestSterownikaDlg::~TestSterownikaDlg()
{
    delete ui;
}

#define WRITECONF(N) u->setMotorIloscKrokowBaza##N(ui->base##N->text()); \
                     u->setMotorOpoznienieImp##N(ui->delay##N->text()); \
                     u->setMotorMaksIloscKrokow##N(ui->maxSteps##N->text()); \
                     u->setMotorOdwrocObroty##N(ui->obrot##N->isChecked()); \
                     u->setMotorPrzelozenie##N(ui->ratio##N->text()); \
                     u->setMotorIloscKrokowSrodek##N(ui->srodekKroki##N->text());

#define WRITECONF_ALL WRITECONF(1) \
                      WRITECONF(2) \
                      WRITECONF(3) \
                      WRITECONF(4) \
                      WRITECONF(5) \
                      WRITECONF(6) \
                      WRITECONF(7) \
                      WRITECONF(8) \
                      WRITECONF(9)

void TestSterownikaDlg::on_pbSetConfiguration_clicked()
{
    WRITECONF_ALL
}

void TestSterownikaDlg::sd_deviceName(QString name)
{
    ui->namePort->setText(name);
}

void TestSterownikaDlg::sd_kontrolerConfigured(bool success, int state)
{
    //ui->textBrowser->insertPlainText(QString("success=%1, state=%2").arg(success).arg(state));
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
    case Sterownik::PARAMS_FAILD:
        ui->pbConnect->setEnabled(true);
        ui->pbDisconnect->setEnabled(false);
        break;
    case Sterownik::PARAMS_OK:
        ui->rbConf_2->setChecked(true);
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);
        break;
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

void TestSterownikaDlg::disconnect()
{

}

void TestSterownikaDlg::sd_setPositionDone(bool home, bool success)
{

}

void TestSterownikaDlg::on_pbConnect_clicked()
{
    sd->connectToDevice();
}

#define PBHOME(N) void TestSterownikaDlg::on_pbHome_##N##_clicked() \
{ pbHome_clicked(N); }

PBHOME(1)
PBHOME(2)
PBHOME(3)
PBHOME(4)
PBHOME(5)
PBHOME(6)
PBHOME(7)
PBHOME(8)
PBHOME(9)


void TestSterownikaDlg::pbHome_clicked(int silnik)
{
    sd->setPositionSilnik(silnik, true, 0);
}

void TestSterownikaDlg::on_pbHomeAll_clicked()
{
    for (int i=1; i<10; ++i)
        pbHome_clicked(i);
}

void TestSterownikaDlg::pbUstawPos_clicked(int silnik, const float & x, const float & ratio)
{
    sd->setPositionSilnik(silnik, false, x*ratio);
}

#define PBSETPOS(N) void TestSterownikaDlg::on_pbUstawPos_##N##_clicked() \
{ pbUstawPos_clicked(N, ui->posX_##N->text().toFloat(), u->getMotorPrzelozenie##N()); }

PBSETPOS(1)
PBSETPOS(2)
PBSETPOS(3)
PBSETPOS(4)
PBSETPOS(5)
PBSETPOS(6)
PBSETPOS(7)
PBSETPOS(8)
PBSETPOS(9)
