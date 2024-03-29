#include "teststanowiskadlg.h"
#include "oczekiwanienaurzadzenia.h"
#include "ui_teststanowiskadlg.h"

#include "sterownik.h"
#include "zasilacz.h"
#include "ustawienia.h"
#include "ukladfiltrow.h"

#include <QMessageBox>
#include <QLineEdit>

#define SETCONF(N) ui->step##N->setText("0"); \
                   ui->speed##N->setText(QString::number(ust->wyliczPredkosc(ust->getMotorPrzelozenieImpJedn##N(),\
                                                                               ust->getMotorCzasMiedzyImpNormal##N())));


#define SETCONF_ALL SETCONF(1) \
                    SETCONF(2) \
                    SETCONF(3) \
                    SETCONF(4) \
                    SETCONF(5) \
                    SETCONF(6) \
                    SETCONF(7) \
                    SETCONF(8) \
                    SETCONF(9)


TestStanowiskaDlg::TestStanowiskaDlg(Zasilacz * zas_, Sterownik * ster_, Ustawienia * ust_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestStanowiskaDlg)
    ,zas(zas_)
    ,ster(ster_)
    ,ust(ust_)
    ,czasUstF(this)
{
    ui->setupUi(this);

    SETCONF_ALL

    bool conn = zas->getConnected() && ster->getConnected();
    ui->pbConnect->setEnabled(!conn);
    connect(ui->pbConnect, &QPushButton::clicked, this, &TestStanowiskaDlg::connect2Device);
    connect(ui->pbFiltrUstaw, &QPushButton::clicked, this, &TestStanowiskaDlg::pbFiltrUstaw);
    connect(ui->cbFiltrFala, &QComboBox::currentTextChanged, this, &TestStanowiskaDlg::changeDlugoscFiltra);
    connect(ui->cbTlumienie, &QComboBox::currentTextChanged, this, &TestStanowiskaDlg::changeTlumienie);
    connect(ui->pbZeruj, &QPushButton::clicked, this, &TestStanowiskaDlg::zerowanieFiltrow);
    connect(&czasUstF, &QTimer::timeout, this, &TestStanowiskaDlg::ukladFiltrowTimeout);
    connect(ui->cbNaped, &QComboBox::currentIndexChanged, this, [this](int index) {
        if (index + 1 == 6 || index + 1 == 7) {
            this->ui->unitMove->setText("<html><body>[mm]</body></html>");
            this->ui->unitSpeed->setText("<html><body>[mm/min]</body></html>");
        } else {
            this->ui->unitMove->setText("<html><body>[&deg;]</body></html>");
            this->ui->unitSpeed->setText("<html><body>[&deg;/min]</body></html>");
        }
    });
    connect(ui->cbRozkaz, &QComboBox::currentIndexChanged, this, [this](int index) {
        this->ui->speed->setEnabled(index == 1);
        this->ui->przemieszczenie->setEnabled(index == 1);
        this->ui->etSpeed->setEnabled(index == 1);
        this->ui->etPrzemieszczenie->setEnabled(index == 1);
        this->ui->unitSpeed->setEnabled(index == 1);
        this->ui->unitMove->setEnabled(index == 1);
    });
    connect(ui->pbSet, &QPushButton::clicked, this, &TestStanowiskaDlg::rozkaz);

    this->ui->speed->setEnabled(false);
    this->ui->przemieszczenie->setEnabled(false);
    this->ui->etSpeed->setEnabled(false);
    this->ui->etPrzemieszczenie->setEnabled(false);
    this->ui->unitSpeed->setEnabled(false);
    this->ui->unitMove->setEnabled(false);
    //connect(ui->volt1, &QLineEdit::editingFinished, this, [this](){ th-});
    prevChooseFiltrDlugoscFali = "655";
    for (auto & t : ust->getTlumienia655() ) {
        ui->cbTlumienie->addItem(t.at(0));
    }
    ui->pbFiltrUstaw->setEnabled(false);
    ui->gb_Napedy->setEnabled(false);
    ui->gb_Zasilanie->setEnabled(false);
    ui->cb_pradMaks->setEnabled(false);
    ui->cb_pradZadzialania->setEnabled(false);
    ui->cb_przekaznikNI->setEnabled(false);

    for (short s=1; s<10; ++s) {
        ui->cbNaped->addItem(ust->getMotorNazwa(s), QVariant::fromValue(s));
    }
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
        ui->cb_pradMaks->setEnabled(true);
        ui->cb_pradZadzialania->setEnabled(true);
    } else {
        ui->zasilaczConn->setText(QString("NIE POŁĄCZONY"));
        ui->gb_Zasilanie->setEnabled(false);
        ui->cb_pradMaks->setEnabled(false);
        ui->cb_pradZadzialania->setEnabled(false);
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
        ui->cb_przekaznikNI->setEnabled(true);
    } else {
        ui->zasilaczConn->setText(QString("NIE POŁĄCZONY"));
        ui->gb_Napedy->setEnabled(false);
        ui->cb_przekaznikNI->setEnabled(false);
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

void TestStanowiskaDlg::flt_bladFiltrow(QChar silnik, bool zerowanie)
{
    QMessageBox::critical(this, zerowanie ? "Zerowanie układu filtrów" : "Ustawianie układu filtrów",
                          zerowanie ? QString("Wystąpił błąd podczas zerowania układu. Niepoprawnie zachował się filtr %1").arg(silnik) :
                                      QString("Wystąpił błąd podczas ustawiania układu filtrów dla zadanego tłumienia. Niepoprawnie zachował się filtr %1").arg(silnik));
    if (!zerowanie) {
        ui->pbFiltrUstaw->setEnabled(true);
        if (czasUstF.isActive())
            czasUstF.stop();
    } else {
        ui->pbZeruj->setEnabled(true);
    }
}

void TestStanowiskaDlg::ster_czujkaOn()
{
    if (ui->cb_przekaznikNI->isChecked())
        QMessageBox::information(this, QString("Czujki liniowe - test stanowiska"),
                                 QString("Nastąpiło wyzwolenie czujki przekaźnikiem"));
}

void TestStanowiskaDlg::ster_setValue(short nrSilnika, const double &val)
{
    switch(nrSilnika) {
    case 1: ui->step1->setText(QString::number(val)); break;
    case 2: ui->step2->setText(QString::number(val)); break;
    case 3: ui->step3->setText(QString::number(val)); break;
    case 4: ui->step4->setText(QString::number(val)); break;
    case 5: ui->step5->setText(QString::number(val)); break;
    case 6: ui->step6->setText(QString::number(val)); break;
    case 7: ui->step7->setText(QString::number(val)); break;
    case 8: ui->step8->setText(QString::number(val)); break;
    case 9: ui->step9->setText(QString::number(val)); break;
    default:
        break;

    }
}

void TestStanowiskaDlg::connect2Device()
{
    OczekiwanieNaUrzadzenia *dlg = new OczekiwanieNaUrzadzenia(true, this);

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
    ster->setFiltrPos(fA, fB, fC);
    czasUstF.setInterval(2500);
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
    bool dl655 = false;
    bool dl880 = false;
    if (dlugoscFali.toUInt() == 655) {
        tl = ust->getTlumienia655();
         dl655 = true;
    } else if (dlugoscFali.toUInt() == 880) {
        tl = ust->getTlumienia880();
        dl880 = true;
    } else
        return;

    for (auto & t : tl) {
        if (t.at(0) == text) {
            ui->fA->setText(t.at(1));
            ui->fB->setText(t.at(2));
            ui->fC->setText(t.at(3));
            double tlA = 0, tlB = 0, tlC = 0;
            if (dl655) {
                tlA = ust->getFiltr_db_655_A(t.at(1).toShort());
                tlB = ust->getFiltr_db_655_B(t.at(2).toShort());
                tlC = ust->getFiltr_db_655_B(t.at(3).toShort());
            } else if (dl880) {
                tlA = ust->getFiltr_db_880_A(t.at(1).toShort());
                tlB = ust->getFiltr_db_880_B(t.at(2).toShort());
                tlC = ust->getFiltr_db_880_B(t.at(3).toShort());
            }
            ui->tlA->setText(QString::number(tlA) + QString( "dB"));
            ui->tlB->setText(QString::number(tlB) + QString( "dB"));
            ui->tlC->setText(QString::number(tlC) + QString( "dB"));
            ui->tlAll->setText(QString::number(tlA+tlB+tlC) + QString( "dB"));
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
    ster->setFiltrReset();
    //ui->
}

void TestStanowiskaDlg::rozkaz()
{
    short nrSilnika = ui->cbNaped->currentIndex()+1;
    if (nrSilnika < 1 || nrSilnika > 9)
        return;
    short rozkaz = ui->cbRozkaz->currentIndex();
    switch(rozkaz) {
    case 0: //brak
        return;
    case 1: //przemieszczenie
    {
        if (ui->przemieszczenie->text().isEmpty() || ui->speed->text().isEmpty())
            return;
        bool ok1, ok2;
        double val1 = ui->przemieszczenie->text().toDouble(&ok1);
        double val2 = ui->speed->text().toDouble(&ok2);
        if (!ok1 || !ok2)
            return;
        unsigned long pozycja = ust->wyliczPozycje(nrSilnika, ust->getMotorIloscImpSrodek(nrSilnika),
                                                   ust->getMotorMaksIloscImp(nrSilnika),
                                                   ust->getMotorPrzelozenieImpJedn(nrSilnika),
                                                   val1);
        unsigned long speed = ust->wyliczPredkosc(ust->getMotorPrzelozenieImpJedn(nrSilnika),val2);
        double val = ust->wyliczPredkosc(ust->getMotorPrzelozenieImpJedn(nrSilnika), val2);
        switch(nrSilnika) {
        case 1: ui->speed1->setText(QString::number(val)); break;
        case 2: ui->speed2->setText(QString::number(val)); break;
        case 3: ui->speed3->setText(QString::number(val)); break;
        case 4: ui->speed4->setText(QString::number(val)); break;
        case 5: ui->speed5->setText(QString::number(val)); break;
        case 6: ui->speed6->setText(QString::number(val)); break;
        case 7: ui->speed7->setText(QString::number(val)); break;
        case 8: ui->speed8->setText(QString::number(val)); break;
        case 9: ui->speed9->setText(QString::number(val)); break;
        default:
            break;

        }

        ster->setPositionSilnik(nrSilnika, false, pozycja, speed);
        break;
    }
    case 2: //wylaczenie wybranego
    {
        ster->setEnableMotor(nrSilnika, false);
        break;
    }
    case 3: //zerowanie wybranego
    {
        ster->setPositionSilnik(nrSilnika, true, 0, ust->getMotorCzasMiedzyImpZerow(nrSilnika));
        break;
    }
    case 4: //stop wybranego
    {
        ster->setStopMotor(nrSilnika);
        break;
    }
    case 5: //wylacznie wszystkich
    {
        ster->setEnableMotorAll(false);
        break;
    }
    case 6: //zerowanie wszystkich
    {
        //ster->setPositionSilnik()
        break;
    }
    case 7: //stop wszystkich
    {
        ster->setStopMotorAll();
        break;
    }
    case 8: //wlaczenie wybranego
    {
        ster->setEnableMotor(nrSilnika, true);
        break;
    }
    case 9: //wlaczneie wszystkich
    {
        ster->setEnableMotorAll(true);
        break;
    }
    default:
        break;
    }
}
