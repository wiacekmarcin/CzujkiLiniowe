#include "testzasilaczadlg.h"
#include "ui_testzasilaczadlg.h"

#include <QMessageBox>
#include <QDebug>

TestZasilaczaDlg::TestZasilaczaDlg(Ustawienia * ust, Zasilacz * zas, const QString &portName, const QString &serial, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestZasilaczaDlg)
  , u(ust)
  , z(zas)
{
    ui->setupUi(this);

    connect(ui->pbZasOpen, &QPushButton::clicked, this, &TestZasilaczaDlg::openZasilacz);
    connect(ui->pbZasClose, &QPushButton::clicked, this, &TestZasilaczaDlg::closeZasilacz);


    ui->leCurrLim->setVisible(zas->getConnected());
    ui->leCurrSet->setVisible(zas->getConnected());
    ui->leVoltLim->setVisible(zas->getConnected());
    ui->leVoltSet->setVisible(zas->getConnected());
    ui->tbSave->setEnabled(zas->getConnected());
    ui->tbCancel->setEnabled(zas->getConnected());

    ui->pbZasClose->setDisabled(!zas->getConnected());
    ui->frame->setDisabled(!zas->getConnected());

    ui->zasilaczKomendy->setDisabled(!zas->getConnected());
    if (zas->getConnected()) {
        configuredZasilacz(Zasilacz::ALL_OK);
        serialNoZasilacz(serial);
        deviceNameZasilacz(portName);
    }
    adjustSize();
}

TestZasilaczaDlg::~TestZasilaczaDlg()
{
    delete ui;
    //zas.closeDevice(false);
}

void TestZasilaczaDlg::on_pbCmdSend_clicked()
{
    if (ui->zasilacz_cmd->text().isEmpty())
        return;
    //
}


void TestZasilaczaDlg::errorZasilacz(QString s)
{
    QMessageBox::critical(this, "Zasilacz", s);
    //showStatusMessage(QString::fromUtf8("Błąd : ") + s);
}

void TestZasilaczaDlg::debugZasilacz(QString /*d*/)
{
    //qDebug() << d;
}

void TestZasilaczaDlg::configuredZasilacz(int state)
{
    switch(state) {
    case Zasilacz::NO_FOUND:
        QMessageBox::warning(this, "Zasilacz sterowany", "Nie znaleziono podłączonego zasilacza");
        break;
    case Zasilacz::FOUND:
        break;
    case Zasilacz::TO_MANY_FOUND:
        QMessageBox::warning(this, "Zasilacz sterowany", "Za dużo urządzeń podłączonych do portu USB");
        break;
    case Zasilacz::NO_OPEN:
        QMessageBox::warning(this, "Zasilacz sterowany", "Nie udało się otworzyć urządzenia. Sprawdź czy inne programy nie używają portu. Odłącz kabel USB i podłącz ponownie. Jeżeli to nie pomoże uruchom ponownie komputer zachowując wczęsniej dane.");
        break;
    case Zasilacz::OPEN:
        break;
    case Zasilacz::NO_READ:
        QMessageBox::warning(this, "Zasilacz sterowany", "Nie udało się czytać z urządzenia. Sprawdź czy inne programy nie używają portu. Odłącz kabel USB i podłącz ponownie. Jeżeli to nie pomoże uruchom ponownie komputer zachowując wczęsniej dane.");
        break;
    case Zasilacz::IDENT_FAILD:
        QMessageBox::warning(this, "Zasilacz sterowany", "Do portu zostało podłączone nieprawidłowe urządzenie");
        break;
    case Zasilacz::IDENT_OK:
        break;
    case Zasilacz::ALL_OK:
        ui->pbZasOpen->setDisabled(true);
        ui->pbZasClose->setDisabled(false);
        ui->frame->setDisabled(false);
        ui->zasilaczKomendy->setDisabled(false);
        break;
    case Zasilacz::CLOSE:
        ui->pbZasClose->setDisabled(true);
        ui->frame->setDisabled(true);
        ui->zasilaczKomendy->setDisabled(true);
        ui->pbZasOpen->setDisabled(false);
        break;
    }
}

void TestZasilaczaDlg::serialNoZasilacz(QString s)
{
    ui->lZasIdent->setText(s);
}

void TestZasilaczaDlg::deviceNameZasilacz(QString name)
{
    ui->lZasSterPort->setText(name);
}

void TestZasilaczaDlg::valueZasilacz(int kind, int value)
{
    switch(kind) {
        case Zasilacz::VOLTAGE_SET:
            ui->mVoltage->setText(QString::number(0.001*value, 'f', 3)); break;
        case Zasilacz::CURRENT_SET:
            ui->mCurrent->setText(QString::number(0.001*value, 'f', 3)); break;
        case Zasilacz::VOLTAGE_LIMIT:
            ui->mVoltageLimit->setText(QString::number(0.001*value, 'f', 3)); break;
        case Zasilacz::CURRENT_LIMIT:
            ui->mCurrentLimit->setText(QString::number(0.001*value, 'f', 3)); break;
        case Zasilacz::VOLTAGE_MEAS:
            ui->mVoltageMeas->setText(QString::number(0.001*value, 'f', 3)); break;
        case Zasilacz::CURRENT_MEAS:
            ui->mCurrentMeas->setText(QString::number(0.001*value, 'f', 3)); break;
        case Zasilacz::OUTPUT:
            break;
    }
}

void TestZasilaczaDlg::openZasilacz()
{
    z->connectToDevice();
}

void TestZasilaczaDlg::closeZasilacz()
{
    z->closeDevice(true);
}

void TestZasilaczaDlg::sendMsg(const QString &msg)
{
    QString oldText = ui->zasilacz_request->toPlainText(); // or toHtml()
    ui->zasilacz_request->setPlainText(msg + oldText);
}

void TestZasilaczaDlg::recvMsg(const QString &msg)
{
    QString oldText = ui->zasilacz_reply->toPlainText(); // or toHtml()
    ui->zasilacz_reply->setPlainText(msg + oldText);
}

void TestZasilaczaDlg::on_tbChange_clicked()
{
    ui->tbChange->setEnabled(true);
    ui->tbCancel->setEnabled(true);
    ui->tbSave->setEnabled(true);

    ui->leCurrLim->setVisible(true);
    ui->leCurrSet->setVisible(true);
    ui->leVoltLim->setVisible(true);
    ui->leVoltSet->setVisible(true);

    ui->leVoltSet->setText(ui->mVoltage->text());
    ui->leVoltLim->setText(ui->mVoltageLimit->text());
    ui->leCurrSet->setText(ui->mCurrent->text());
    ui->leCurrLim->setText(ui->mCurrentLimit->text());
}


void TestZasilaczaDlg::on_tbSave_clicked()
{
    ui->tbSave->setEnabled(false);
    ui->tbCancel->setEnabled(false);
    ui->tbSave->setEnabled(true);

    ui->leCurrLim->setVisible(false);
    ui->leCurrSet->setVisible(false);
    ui->leVoltLim->setVisible(false);
    ui->leVoltSet->setVisible(false);


    z->setCurrent_mA(ui->leCurrSet->text().toFloat()*1000);
    z->setVoltage_mV(ui->leVoltSet->text().toFloat()*1000);
    z->setCurrentLimit_mA(ui->leCurrLim->text().toFloat()*1000);
    z->setVoltageLimit_mV(ui->leVoltLim->text().toFloat()*1000);
}


void TestZasilaczaDlg::on_tbCancel_clicked()
{
    ui->tbSave->setEnabled(false);
    ui->tbCancel->setEnabled(false);
    ui->tbSave->setEnabled(true);


    ui->leCurrLim->setVisible(false);
    ui->leCurrSet->setVisible(false);
    ui->leVoltLim->setVisible(false);
    ui->leVoltSet->setVisible(false);
}
