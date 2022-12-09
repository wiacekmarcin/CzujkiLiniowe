#include "oczekiwanienaurzadzenia.h"
#include "ui_oczekiwanienaurzadzenia.h"
#include "zasilacz.h"
#include <QTimer>

OczekiwanieNaUrzadzenia::OczekiwanieNaUrzadzenia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OczekiwanieNaUrzadzenia),
    timer(this),
    cntTmt(0),
    zasilaczOk(false),
    sterownikOk(false)
{
    ui->setupUi(this);
    ui->progressBarSterownik->setMaximum(maxCzas);
    ui->progressBarZasilacz->setMaximum(maxCzas);
    timer.setInterval(1000);

    connect(&timer, &QTimer::timeout, this, &OczekiwanieNaUrzadzenia::timeout);
    connect(ui->pushButton, &QPushButton::clicked, this, [this]() { this->reject(); });
    connect(ui->pushButton, &QPushButton::pressed, this, [this]() { this->reject(); });
    ui->frameError->setVisible(false);
    timer.start();
}

OczekiwanieNaUrzadzenia::~OczekiwanieNaUrzadzenia()
{
    timer.stop();
    delete ui;

}

#define TSTOP(ERR) ui->statusZasilacz->setText(ERR); ui->progressBarZasilacz->setValue(maxCzas)
void OczekiwanieNaUrzadzenia::zasilacz(bool success, int state)
{
    switch(state) {
        case Zasilacz::NO_FOUND:
        case Zasilacz::TO_MANY_FOUND:
            TSTOP("Nie znaleziono urządzenia");  break;
        case Zasilacz::NO_OPEN:
        case Zasilacz::NO_READ:
            TSTOP("Błąd w komunikacji");  break;
        case Zasilacz::IDENT_FAILD:
            TSTOP("Nie znane urządzenie");  break;
        case Zasilacz::CLOSE:
            TSTOP("Urządzenie zamknięte");  break;
        case Zasilacz::ALL_OK:
            zasilaczOk = true; break;
        default:
            break;
    }
}

void OczekiwanieNaUrzadzenia::timeout()
{
    ++cntTmt;
    if (!sterownikOk && ui->progressBarSterownik->value() < maxCzas)
        ui->progressBarSterownik->setValue(cntTmt);
    if (!zasilaczOk && ui->progressBarZasilacz->value() < maxCzas)
        ui->progressBarZasilacz->setValue(cntTmt);

    if (sterownikOk && zasilaczOk) {
        timer.stop();
        QTimer::singleShot(3000, this, [this]() {this->accept();});
    }
    if (cntTmt == maxCzas) {
        timer.stop();
        ui->frameError->setVisible(true);
        ui->errorSterownik->setVisible(!sterownikOk);
        ui->errorZasilacz->setVisible(!zasilaczOk);
    }
}
