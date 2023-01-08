#include "teststanowiskadlg.h"
#include "oczekiwanienaurzadzenia.h"
#include "ui_teststanowiskadlg.h"

#include "sterownik.h"
#include "zasilacz.h"
#include "ustawienia.h"
#include "ukladfiltrow.h"

#include <QMessageBox>
#include <QLineEdit>

TestStanowiskaDlg::TestStanowiskaDlg(Zasilacz * zas_, Sterownik * ster_, SterownikFiltrow * sterF_, Ustawienia * ust_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestStanowiskaDlg)
    ,zas(zas_)
    ,ster(ster_)
    ,sterF(sterF_)
    ,ust(ust_)
    ,czasUstF(this)
{
    ui->setupUi(this);

    bool conn = zas->getConnected() && ster->getConnected();
    ui->pbConnect->setEnabled(!conn);
    connect(ui->pbConnect, &QPushButton::clicked, this, &TestStanowiskaDlg::connect2Device);
    connect(ui->pbFiltrUstaw, &QPushButton::clicked, this, &TestStanowiskaDlg::pbFiltrUstaw);
    connect(ui->cbFiltrFala, &QComboBox::currentTextChanged, this, &TestStanowiskaDlg::changeDlugoscFiltra);
    connect(ui->cbTlumienie, &QComboBox::currentTextChanged, this, &TestStanowiskaDlg::changeTlumienie);
    connect(ui->pbZeruj, &QPushButton::clicked, this, &TestStanowiskaDlg::zerowanieFiltrow);
    connect(&czasUstF, &QTimer::timeout, this, &TestStanowiskaDlg::ukladFiltrowTimeout);

    //connect(ui->volt1, &QLineEdit::editingFinished, this, [this](){ th-});
    prevChooseFiltrDlugoscFali = "655";
    for (auto & t : ust->getTlumienia655() ) {
        ui->cbTlumienie->addItem(t.at(0));
    }
    ui->pbFiltrUstaw->setEnabled(false);
    ui->gb_Napedy->setEnabled(false);
    ui->gb_Zasilanie->setEnabled(false);
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
    if (state == Zasilacz::IDENT_OK || state == Zasilacz::ALL_OK) {
        ui->zasilaczConn->setText(QString("POŁĄCZONY [%1]").arg(zasilaczName));
        ui->gb_Zasilanie->setEnabled(true);
    } else {
        ui->zasilaczConn->setText(QString("NIE POŁĄCZONY"));
        ui->gb_Zasilanie->setEnabled(false);
    }
}

void TestStanowiskaDlg::ster_zdarzenieSilnik(int)
{

}

void TestStanowiskaDlg::configuredSterownik(int state)
{
    if (state == Sterownik::PARAMS_OK || state == Sterownik::ALL_OK ) {
        ui->zasilaczConn->setText(QString("POŁĄCZONY [%1]").arg(sterownikName));
        ui->gb_Napedy->setEnabled(true);
    } else {
        ui->zasilaczConn->setText(QString("NIE POŁĄCZONY"));
        ui->gb_Napedy->setEnabled(false);
    }
}

void TestStanowiskaDlg::deviceNameSter(const QString &serial)
{
    sterownikName = serial;
}

void TestStanowiskaDlg::flt_zerowanieFiltrowDone()
{
    ui->pbFiltrUstaw->setEnabled(true);
}

void TestStanowiskaDlg::flt_setUkladFiltrowDone()
{
    ui->pbFiltrUstaw->setEnabled(true);
    if (czasUstF.isActive())
        czasUstF.stop();
    else {
        QMessageBox::warning(this, "Ustawienie tłumienia", "Udało się ustawić układ filtrów, jednak po czasie 2 sekund");
    }
    ui->pbFiltrUstaw->setEnabled(true);
}

void TestStanowiskaDlg::flt_bladFiltrow(short silnik, bool zerowanie)
{
    QMessageBox::critical(this, zerowanie ? "Zerowanie układu filtrów" : "Ustawianie układu filtrów",
                          zerowanie ? QString("Wystąpił błąd podczas zerowania układu. Niepoprawnie zachował się silnik %1").arg(silnik) :
                                      QString("Wystąpił błąd podczas ustawiania układu filtrów dla zadanego tłumienia. Niepoprawnie zachował się silnik %1").arg(silnik));
    if (!zerowanie) {
        ui->pbFiltrUstaw->setEnabled(true);
        if (czasUstF.isActive())
            czasUstF.stop();
    } else {
        ui->pbZeruj->setEnabled(true);
    }
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

void TestStanowiskaDlg::pbFiltrUstaw()
{
    short fA = ui->fA->text().toShort();
    short fB = ui->fB->text().toShort();
    short fC = ui->fC->text().toShort();
    sterF->setPos(fA, fB, fC);
    czasUstF.setInterval(2000);
    czasUstF.setSingleShot(true);
    czasUstF.start();
}

void TestStanowiskaDlg::changeDlugoscFiltra(const QString &text)
{
    if (prevChooseFiltrDlugoscFali == text)
        return;
    ui->cbTlumienie->clear();
    QList<QStringList> tl;
    if (text.toUInt() == 655) {
        tl = ust->getTlumienia655();
    } else if (text.toUInt() == 880) {
        tl = ust->getTlumienia880();
    } else
        return;

    for (auto & t : tl) {
        ui->cbTlumienie->addItem(t.at(0));
    }
}

void TestStanowiskaDlg::changeTlumienie(const QString &text)
{
    QString dlugoscFali = ui->cbFiltrFala->currentText();
    QList<QStringList> tl;
    if (dlugoscFali.toUInt() == 655) {
        tl = ust->getTlumienia655();
    } else if (dlugoscFali.toUInt() == 880) {
        tl = ust->getTlumienia880();
    } else
        return;

    for (auto & t : tl) {
        if (t.at(0) == text) {
            ui->fA->setText(t.at(1));
            ui->fB->setText(t.at(2));
            ui->fC->setText(t.at(3));
            break;
        }
    }
}

void TestStanowiskaDlg::ukladFiltrowTimeout()
{
    QMessageBox::warning(this, "Ustawienia tłumiania", "Nie udało się ustawić odpowiedniego układu filtrów");
    ui->pbFiltrUstaw->setEnabled(true);
}

void TestStanowiskaDlg::zerowanieFiltrow()
{
    qDebug() << __FILE__ << __LINE__;
    sterF->setZero();
    //ui->
}
