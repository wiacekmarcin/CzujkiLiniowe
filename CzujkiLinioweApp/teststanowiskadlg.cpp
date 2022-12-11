#include "teststanowiskadlg.h"
#include "oczekiwanienaurzadzenia.h"
#include "ui_teststanowiskadlg.h"

#include "sterownik.h"
#include "zasilacz.h"

#include <QLineEdit>

TestStanowiskaDlg::TestStanowiskaDlg(Zasilacz * zas_, Sterownik * ster_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestStanowiskaDlg)
    ,zas(zas_)
    ,ster(ster_)
{
    ui->setupUi(this);

    bool conn = zas->getConnected() && ster->getConnected();
    ui->pbConnect->setDisabled(!conn);
    connect(ui->pbConnect, &QPushButton::clicked, this, &TestStanowiskaDlg::connect2Device);

    //connect(ui->volt1, &QLineEdit::editingFinished, this, [this](){ th-});

}

TestStanowiskaDlg::~TestStanowiskaDlg()
{
    delete ui;
}

void TestStanowiskaDlg::deviceNameZasilacz(const QString &serial)
{
    zasilaczName = serial;
}

void TestStanowiskaDlg::valueZasilacz(int kind, int value)
{
    switch(kind) {
    case Zasilacz::VOLTAGE_SET:
        if (ui->volt1->text() == ui->volt2->text())
            ui->volt1->setText(QString::number(value/1000.0, 'f', 3));
        ui->volt2->setText(ui->volt1->text());
        break;

        break;
    case Zasilacz::CURRENT_SET: break;
    case Zasilacz::VOLTAGE_LIMIT: break;
    case Zasilacz::CURRENT_LIMIT: break;
    case Zasilacz::VOLTAGE_MEAS: break;
    case Zasilacz::CURRENT_MEAS: break;
    case Zasilacz::OUTPUT: break;
    default: break;
    }
}

void TestStanowiskaDlg::configuredZasilacz(int state)
{
    if (state == Zasilacz::IDENT_OK || state == Zasilacz::ALL_OK)
        ui->zasilaczConn->setText(QString("POŁĄCZONY [%1]").arg(zasilaczName));
    else
        ui->zasilaczConn->setText(QString("NIE POŁĄCZONY"));
}

void TestStanowiskaDlg::ster_zdarzenieSilnik(int)
{

}

void TestStanowiskaDlg::configuredSterownik(int state)
{
    if (state == Sterownik::PARAMS_OK || state == Sterownik::ALL_OK )
        ui->zasilaczConn->setText(QString("POŁĄCZONY [%1]").arg(zasilaczName));
    else
        ui->zasilaczConn->setText(QString("NIE POŁĄCZONY"));
}

void TestStanowiskaDlg::deviceNameSter(const QString &serial)
{
    sterownikName = serial;
}

void TestStanowiskaDlg::connect2Device()
{
    OczekiwanieNaUrzadzenia *dlg = new OczekiwanieNaUrzadzenia(this);

    dlg->connect(zas, &Zasilacz::kontrolerConfigured, dlg, &OczekiwanieNaUrzadzenia::zasilacz);
    dlg->connect(ster, &Sterownik::kontrolerConfigured, dlg, &OczekiwanieNaUrzadzenia::sterownik);
    zas->connectToDevice();
    ster->connectToDevice();
    dlg->exec();
    delete dlg;
}
