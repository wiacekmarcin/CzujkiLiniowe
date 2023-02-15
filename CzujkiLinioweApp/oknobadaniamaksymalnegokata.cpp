#include "oknobadaniamaksymalnegokata.h"
#include "ui_oknobadaniamaksymalnegokata.h"
#include <QDateTime>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "sterownik.h"
#include <QMutexLocker>
#include <QDebug>
#include <QMessageBox>

static int questionQuit(const QString & title, const QString & pytanie, QWidget * parent) {
    QMessageBox messageBox(QMessageBox::Question, title, pytanie,
                        QMessageBox::Close | QMessageBox::Cancel, parent);


    messageBox.setButtonText(QMessageBox::Close, QString::fromUtf8("Zamknij"));
    messageBox.setButtonText(QMessageBox::Cancel, QString::fromUtf8("Anuluj"));

    return messageBox.exec();
}

OknoBadaniaMaksymalnegoKata::OknoBadaniaMaksymalnegoKata(short nrSilnika_, const QString &name,
                                 const QString & podtitle,
                                 const double & startKat,
                                 const double & kat,
                                 const Ustawienia &ust,
                           Sterownik * ster_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoBadaniaMaksymalnegoKata),
    tmSterownika(this),
    ster(ster_),
    wynikBadania(false),
    destPos(kat),
    nrSilnika(nrSilnika_),
    prevVal(startKat),
    startPos(startKat),
    breakBadanie(false)

{
    ui->setupUi(this);
    ui->testName->setText(name);
    if (podtitle.isEmpty()) {
        ui->nazwaPodTestu->setVisible(false);
    } else {
        ui->nazwaPodTestu->setText(podtitle);
        ui->nazwaPodTestu->setVisible(true);
    }
    ui->maxkat->setText(QString("<html><body>%1 &deg;</body></html>").arg(kat));
    ui->kat->setText(QString("<html><body>%1 &deg;</body></html>").arg(startKat, 4, 'f', 2));
    maxDelta = ust.getMaksRoznicaKatNieWspolOsiowosci();

    unsigned int speed = ust.predkoscRoboczaImp(nrSilnika);
    unsigned int impulsy = ust.wyliczPozycje(nrSilnika, kat);
    destPos = kat;
    speedMin = ust.wyliczPredkosc(ust.getMotorPrzelozenieImpJedn(nrSilnika), speed);
    ui->speed->setText(QString("<html><body>%1 &deg; / <sub>min<sub></body></html>").arg(speedMin, 3, 'f', 2));

    qDebug() << __FILE__ << __LINE__ << speed << impulsy << "Start ruch";

    ster->setPositionSilnik(nrSilnika, false, impulsy, speed);
    //tmSterownika.singleShot(15000, this, &OknoBadaniaMaksymalnegoKata::timeoutSterownika);
    deviceisOk = false;

    if (!ust.testMode)
        ui->frameDebug->setVisible(false);
    else {
        moveTimer = new QTimer(this);
        valBeg = startPos;
        valEnd = destPos;
        valAct = valBeg;
        ui->frameDebug->setVisible(true);
        connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->wynikBadania = true; this->prevVal = this->destPos; done(QDialog::Accepted); });
        connect(ui->pbCzujkaOn, &QPushButton::clicked, this, [this]() {this->czujkaOn(); });
        connect(ui->pbStartRuchu, &QPushButton::clicked, this, [this]() {this->testValue(); });
        connect(ui->pbStop, &QPushButton::clicked, this, [this]() {this->ster_setPositionDone(this->nrSilnika, RuchSilnikaType{false, false, true, false}); });
    }
    adjustSize();
}

OknoBadaniaMaksymalnegoKata::~OknoBadaniaMaksymalnegoKata()
{
    if (Ustawienia::testMode) {
        moveTimer->stop();
        delete moveTimer;
    }
    tmSterownika.stop();
    delete ui;
}

void OknoBadaniaMaksymalnegoKata::czujkaOn()
{
    //qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << "czujka on";
    tmSterownika.stop();
    wynikBadania = true;
    //error = QString::fromUtf8("Czujka zadziała dla kąta : %1").arg(prevVal);
    ster->setStopMotor(nrSilnika);
    done(QDialog::Accepted);
}


void OknoBadaniaMaksymalnegoKata::timeoutSterownika()
{
    //qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
    //         << "hardware timeout ";
    if (deviceisOk)
        return;
    error = QString::fromUtf8("Błąd sprzętowy");
    wynikBadania = false;
    done(QDialog::Rejected);
}

void OknoBadaniaMaksymalnegoKata::closeEvent(QCloseEvent *event)
{
    auto btn = questionQuit(QString::fromUtf8("CzujkiLiniowe"),
                            QString::fromUtf8("Czy chcesz wyjść z badania bez zapisywania danych"),
                            this);

    if (btn == QMessageBox::Cancel) {
        event->ignore();
        return;
    }

    if (btn == QMessageBox::Close) {
        breakBadanie = true;
        event->accept();
        return;
    }
}

bool OknoBadaniaMaksymalnegoKata::getBreakBadanie() const
{
    return breakBadanie;
}

const QString &OknoBadaniaMaksymalnegoKata::getError() const
{
    return error;
}

void OknoBadaniaMaksymalnegoKata::ster_setPositionDone(short silnik, RuchSilnikaType r)
{
    //qDebug() << __FILE__ << __LINE__ <<"home" << r.home << "move" << r.move << "err" << r.err << "interrupt" << r.inter;
    deviceisOk = true;
    if (r.home || silnik != nrSilnika)
        return;


    if (r.err) {
        wynikBadania = false;
        error = QString::fromUtf8("Błąd ustawienia silnika nr %1").arg(silnik);
        tmSterownika.stop();
        done(QDialog::Rejected);
    }
    /*
    if (r.inter) {
        if (abs(destPos - prevVal) > 0.3) {
            error = QString::fromUtf8("Ruch został przerwany dla silnika nr %1").arg(silnik);
            wynikBadania = false;
            tmSterownika.stop();
            done(QDialog::Rejected);
        } else {
            wynikBadania = true;
            tmSterownika.stop();
            done(QDialog::Accepted);
        }
    }
    if (!r.move) {
        if (abs(destPos - prevVal) > 0.3) {
            error = QString::fromUtf8("Osiągnięta pozycja kątowa (%1) jest zbyt daleka od zadanej (%2)").arg(destPos, 3, 'f', 2).arg(prevVal, 3, 'f', 2);
            wynikBadania = false;
            tmSterownika.stop();
            done(QDialog::Rejected);
        } else {
            wynikBadania = true;
            tmSterownika.stop();
            done(QDialog::Accepted);
        }
    }*/
}

void OknoBadaniaMaksymalnegoKata::ster_setValue(short silnik, const double &val)
{
    //qDebug() << __FILE__ << __LINE__ << "silnik" << silnik << "val" << val << "destPos" << destPos << "prevVal" << prevVal;
    if (silnik != nrSilnika)
        return;

    if (!deviceisOk)
        deviceisOk = true;

    if (prevVal == val)
        return;
    prevVal = val;

     if (val == destPos) {
        error = QString::fromUtf8("Osiągnięto maksymalny kąt bez wyzwolenia czujki");
        wynikBadania = false;
        tmSterownika.stop();
        done(QDialog::Rejected);
    }

    if (abs(destPos - val) > maxDelta) {
        error = QString::fromUtf8("Osiągnięto maksymalny różnicę kątową, gdzie czujka powinna zadziałać");
        wynikBadania = false;
        tmSterownika.stop();
        done(QDialog::Rejected);
    }

    ui->kat->setText(QString("<html><body>%1 &deg;</body></html>").arg(val, 4, 'f', 2));
    int dt;
    if (speedMin)
        dt = 60.0*(abs(destPos-val))/speedMin;
    else
        dt = 0;
    //qDebug() << __FILE__ << __LINE__ << destPos << val << (destPos-val) << (60.0*(destPos-val)/speedMin);
    ui->szacowanyczas->setText(QString("%1 s").arg(dt));
}

bool OknoBadaniaMaksymalnegoKata::getWynikBadania() const
{
    return wynikBadania;
}

double OknoBadaniaMaksymalnegoKata::getDegrees() const
{
    return prevVal;
}

void OknoBadaniaMaksymalnegoKata::testValue()
{
    valPer100ms = 0.3/60/5;
    moveTimer->setInterval(200);
    moveTimer->start();
    this->ster_setPositionDone(this->nrSilnika, RuchSilnikaType{false, true, false, false});
    connect(moveTimer, &QTimer::timeout, this, [this]() {this->valAct+=this->valPer100ms;
                    this->ster_setValue(this->nrSilnika, this->valAct);
                    if (abs(this->valAct) > abs(this->destPos)) {
                        this->ster_setPositionDone(this->nrSilnika, RuchSilnikaType{false, false, false, false});
                        moveTimer->stop();
                    }
                    });

}
