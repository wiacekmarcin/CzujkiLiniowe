#include "teststerownikadlg.h"
#include "ui_teststerownikadlg.h"
#include "ustawienia.h"
#include "sterownik.h"

#include <QMessageBox>

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
                connect(ui->pbUstawPos_##N, &QPushButton::clicked, this, [this](){ \
    this->pbUstawPos_clicked(N, ui->posX_##N->text(), ui->ratio##N->text(), this->ui->delay##N->text()); }); \


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


#define ADDICONS(N) QHBoxLayout * horizontalLayoutfr##N = new QHBoxLayout(ui->fr##N); \
                    horizontalLayoutfr##N->setSpacing(2);\
                    horizontalLayoutfr##N->setObjectName(QString("horizontalLayoutfr_%1").arg(N));\
                    horizontalLayoutfr##N->setContentsMargins(2, 2, 2, 2);\
                    QLabel * iconOK##N = new QLabel(ui->fr##N);\
                    iconOK##N->setObjectName(QString("iconOK%1").arg(N));\
                    iconOK##N->setMinimumSize(QSize(15, 15)); \
                    iconOK##N->setMaximumSize(QSize(15, 15)); \
                    iconOK##N->setStyleSheet("background-color:red");\
                    horizontalLayoutfr##N->addWidget(iconOK##N);\
                    QLabel * iconErr##N = new QLabel(ui->fr##N);\
                    iconErr##N->setObjectName(QString("iconErr%1").arg(N));\
                    iconErr##N->setMinimumSize(QSize(15, 15));\
                    iconErr##N->setMaximumSize(QSize(15, 15));\
                    iconErr##N->setStyleSheet("background-color:grey");\
                    horizontalLayoutfr##N->addWidget(iconErr##N);\
                    ikonyStatusu[N] = qMakePair(iconOK##N, iconErr##N);

#define ADDICONS_ALL    ADDICONS(1) \
                        ADDICONS(2) \
                        ADDICONS(3) \
                        ADDICONS(4) \
                        ADDICONS(5) \
                        ADDICONS(6) \
                        ADDICONS(7) \
                        ADDICONS(8) \
                        ADDICONS(9)

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
    connect(ui->pbDisconnect,&QPushButton::clicked, this, &TestSterownikaDlg::pbDisconnect_clicked);
    connect(ui->pbReset, &QPushButton::clicked, this, &TestSterownikaDlg::pbResett_clicked);

    ADDICONS_ALL

    ui->rbFound->setDisabled(false);
    ui->rbFound->setChecked(false);
    ui->rbFound->setCheckable(false);

    ui->rbAuth->setDisabled(false);
    ui->rbAuth->setChecked(false);
    ui->rbAuth->setCheckable(false);

    ui->rbConf_2->setDisabled(false);
    ui->rbConf_2->setChecked(false);
    ui->rbConf_2->setCheckable(false);
    ui->rbConf_2->setEnabled(false);

    ui->rbOpen->setDisabled(false);
    ui->rbOpen->setChecked(false);
    ui->rbOpen->setCheckable(false);
    ui->rbOpen->setEnabled(false);
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
    ui->namePort->setText(name);
}

void TestSterownikaDlg::sd_kontrolerConfigured(int state)
{
    ui->dbg3->append(QString("kontrolerConfigured state=%1").arg(state));
    bool conn = false;
    switch(state) {
    case Sterownik::NO_FOUND:
        ui->rbFound->setDisabled(false);
        ui->rbFound->setChecked(false);
        ui->rbAuth->setDisabled(false);
        ui->rbAuth->setChecked(false);
        ui->rbConf_2->setDisabled(false);
        ui->rbConf_2->setChecked(false);
        break;
    case Sterownik::FOUND:
        ui->rbFound->setDisabled(false);
        ui->rbFound->setChecked(true);
        ui->rbAuth->setDisabled(false);
        break;
    case Sterownik::NO_OPEN:
    case Sterownik::NO_READ:
    case Sterownik::IDENT_FAILD:
    case Sterownik::OPEN:
        break;
    case Sterownik::IDENT_OK:
        break;
    case Sterownik::PARAMS_OK:
    case Sterownik::ALL_OK:
        ui->rbAuth->setChecked(true);
        ui->rbConf_2->setDisabled(false);
        conn = true;
        break;
    case Sterownik::PARAMS_FAILD:
        break;


        break;
    case Sterownik::CLOSE:
        ui->rbFound->setChecked(false);
        ui->rbAuth->setEnabled(false);
        ui->rbAuth->setChecked(false);
        ui->rbConf_2->setEnabled(false);
        ui->rbConf_2->setChecked(false);
        break;
    default:
        break;
    }

    ui->frame_3->setEnabled(conn);
    ui->pbConnect->setEnabled(!conn);
    ui->pbDisconnect->setEnabled(conn);
}

void TestSterownikaDlg::sd_setParamsDone(bool success)
{

}

void TestSterownikaDlg::sd_disconnect()
{

}

void TestSterownikaDlg::sd_setZdarzenieSilnik(short silnik, short zdarzenie)
{
    ui->dbg3->append(QString("Zdarzenie M[%1]=%2").arg(silnik).arg(zdarzenie));
    if (silnik == 0)
        return;
    switch(zdarzenie) {
    case Sterownik::M_ACTIVE:
        ikonyStatusu[silnik].first->setStyleSheet("background-color:green");
        break;
    case Sterownik::M_NOACTIVE:
    default: {
        ikonyStatusu[silnik].first->setStyleSheet("background-color:red");
        ikonyStatusu[silnik].second->setStyleSheet("background-color:gray");
        break;
    }
    }
}

void TestSterownikaDlg::sd_czujkaOn(bool hardware)
{
    static bool showDialog = false;
    if (showDialog)
        return;
    showDialog = true;
    QMessageBox::information(this, "Czujka", "Czujka została wyzwolona");
    showDialog = false;
}

void TestSterownikaDlg::sd_setPositionDone(bool home, bool success)
{

}

void TestSterownikaDlg::pbConnect_clicked()
{
    ui->dbg3->append(QString("Connect vendor=%1 product=%2").arg(sd->getVendor(), sd->getProduct()));
    sd->connectToDevice();
}

void TestSterownikaDlg::pbDisconnect_clicked()
{
    ui->dbg3->append(QString("Disconnect on demand"));
    sd->disconnectDevice();
}

void TestSterownikaDlg::pbResett_clicked()
{
    ui->dbg3->append(QString("Reset na życzenie"));
    sd->setReset();
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

void TestSterownikaDlg::pbUstawPos_clicked(int silnik, const QString &x, const QString &ratio, const QString & impTime)
{
    bool ok;
    unsigned int val = impTime.toUInt(&ok);
    if (!ok)
        val = 10000;
    float valx = x.toFloat(&ok);
    if (!ok)
        valx = 0;
    double valr = x.toDouble(&ok);
    if (!ok)
        valr = 0;
    unsigned int impCnt = round(valx*valr);

    sd->setPositionSilnik(silnik, false, impCnt, val);
}
