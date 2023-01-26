#include "oczekiwanienaurzadzenia.h"
#include "ui_oczekiwanienaurzadzenia.h"
#include "zasilacz.h"
#include "sterownik.h"
#include <QTimer>
#include <QMessageBox>

OczekiwanieNaUrzadzenia::OczekiwanieNaUrzadzenia(bool zasilacz, Zasilacz * zas_, Sterownik * ster_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OczekiwanieNaUrzadzenia),
    timer(this),
    cntTmt(0),
    zasilaczOk(false),
    sterownikOk(false),
    koniecznyZasilacz(zasilacz),
    ster(ster_),
    zas(zas_)
{
    ui->setupUi(this);

    if (zasilacz) {
        connect(zas, &Zasilacz::kontrolerConfigured, this, &OczekiwanieNaUrzadzenia::zasilacz);
    }
    connect(ster, &Sterownik::kontrolerConfigured, this, &OczekiwanieNaUrzadzenia::sterownik);
    timer.setInterval(1000);

    connect(&timer, &QTimer::timeout, this, &OczekiwanieNaUrzadzenia::timeout);
#ifndef DEFVAL
    ui->pbSkip->setVisible(false);
#endif

    connect(ui->pbBreak, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    connect(ui->pbAgain, &QPushButton::clicked, this, [this]() { this->init(); });
    connect(ui->pbSkip, &QPushButton::clicked, this, [this]() { this->done(QDialog::Accepted); });
    init();
    adjustSize();
}

void OczekiwanieNaUrzadzenia::init()
{
    zasilaczOk = false;
    sterownikOk = false;
    cntTmt = 0;
    ui->errorSterownik->setVisible(false);
    ui->errorZasilacz->setVisible(false);
    ui->frameError->setVisible(false);
    ui->progressBarSterownik->setMaximum(maxCzas);
    ui->progressBarZasilacz->setMaximum(maxCzas);
    if (!koniecznyZasilacz) {
        ui->frZasilacz->setEnabled(false);
        ui->errorZasilacz->setEnabled(false);
    }
    ui->frameError->setVisible(false);
    if (koniecznyZasilacz) {
        zas->connectToDevice();
    }
    ster->connectToDevice();
    timer.start();
}

OczekiwanieNaUrzadzenia::~OczekiwanieNaUrzadzenia()
{
    timer.stop();
    delete ui;

}

#define TSTOP(ERR) ui->statusZasilacz->setText(ERR); ui->progressBarZasilacz->setValue(maxCzas)
void OczekiwanieNaUrzadzenia::zasilacz(int state)
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
        case Zasilacz::IDENT_OK:
        case Zasilacz::ALL_OK:
            TSTOP("Urządzenie gotowe");
            zasilaczOk = true; break;
        case Zasilacz::FOUND:
            ui->statusZasilacz->setText("Urzadzenie znalezione. Trwa łączenie..."); break;
        case Zasilacz::OPEN:
            ui->statusZasilacz->setText("Urzadzenie połączone. Trwa identyfikacja..."); break;
        default:
            break;
    }
}

void OczekiwanieNaUrzadzenia::timeout()
{
    ++cntTmt;
    if (!sterownikOk && ui->progressBarSterownik->value() < maxCzas)
        ui->progressBarSterownik->setValue(cntTmt);
    if (koniecznyZasilacz) {
        if (!zasilaczOk && ui->progressBarZasilacz->value() < maxCzas)
            ui->progressBarZasilacz->setValue(cntTmt);
    }

    if (sterownikOk && ((koniecznyZasilacz && zasilaczOk) || !koniecznyZasilacz)) {
        timer.stop();
        done(QDialog::Accepted);
    }
    if (cntTmt == maxCzas || (ui->progressBarSterownik->value() == maxCzas &&
                                ((koniecznyZasilacz && ui->progressBarZasilacz->value() == maxCzas) || !koniecznyZasilacz))) {
        timer.stop();
        ui->frameError->setVisible(true);
        ui->errorSterownik->setVisible(!sterownikOk);
        if (koniecznyZasilacz)
            ui->errorZasilacz->setVisible(!zasilaczOk);
    }
}

#define TSTOPSTER(ERR) ui->statusSterownik->setText(ERR); ui->progressBarSterownik->setValue(maxCzas)
void OczekiwanieNaUrzadzenia::sterownik(int state)
{
    //qDebug() << "Sterownik" << state;
    switch(state) {
        case Sterownik::NO_FOUND:
            TSTOPSTER("Nie znaleziono urządzenia");  break;
        case Sterownik::NO_OPEN:
        case Sterownik::NO_READ:
            TSTOPSTER("Błąd w komunikacji");  break;
        case Sterownik::IDENT_FAILD:
            TSTOPSTER("Nie znane urządzenie");  break;
        case Sterownik::PARAMS_FAILD:
            TSTOPSTER("Błąd konfiguracji");  break;
        case Sterownik::CLOSE:
            TSTOPSTER("Urządzenie zamknięte");  break;
        case Sterownik::PARAMS_OK:
        case Sterownik::ALL_OK:
            TSTOPSTER("Urządzenie gotowe");
            sterownikOk = true; break;
        case Sterownik::FOUND:
            ui->statusSterownik->setText("Urządzenie znalezione. Trwa łączenie...."); break;
        case Sterownik::OPEN:
            ui->statusSterownik->setText("Urządzenie połączone. Trwa identyfikacja..."); break;
        case Sterownik::IDENT_OK:
            ui->statusSterownik->setText("Urządzenie authoryzowane. Trwa konfigruacja..."); break;
        default:
            break;
    }
}


void OczekiwanieNaUrzadzenia::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Oczekiwanie na urządzenia"),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        done(QDialog::Rejected);
}
