#include "oknobadaniatlumienia.h"
#include "ui_oknobadaniatlumienia.h"
#include <QDateTime>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "sterownik.h"
#include <QMutexLocker>

OknoBadaniaTlumienia::OknoBadaniaTlumienia(unsigned int czasPostojuFiltra, unsigned int dlugoscFali_,
                           const QString & name, const Ustawienia &ust,
                           Sterownik * ster_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoBadaniaTlumienia),
    zmFiltraTmr(this),
    zmProgressBar(this),
    resetFiltrow(this),
    actTlumienie(0.0),
    dlugoscFali(dlugoscFali_),
    actTlumPos(0),
    maxTlum(0),
    ster(ster_),
    resetFiltrowOk(false),
    wynikBadania(false),
    posFiltrA(0),
    posFiltrB(0),
    posFiltrC(0)

{
    ui->setupUi(this);

    if (dlugoscFali == 880) {
        tlumienia = ust.getTlumienia880();
    } else {
        tlumienia = ust.getTlumienia655();
    }
    maxTlum=tlumienia.size();
    ui->dlugoscFali->setText(QString::number(dlugoscFali));
    ui->testName->setText(name);
    ui->czas->setText(QString::number(czasPostojuFiltra) + QString(" s"));
    connect(&zmFiltraTmr, &QTimer::timeout, this, &OknoBadaniaTlumienia::uplynalCzasPostojuFiltra);
    czasPostoju = czasPostojuFiltra;
    zmFiltraTmr.setInterval(czasPostojuFiltra*1000);
    //zmFiltraTmr.start();
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(czasPostojuFiltra);
    connect(&zmProgressBar, &QTimer::timeout, this, &OknoBadaniaTlumienia::progressBarUpdate);
    zmProgressBar.setInterval(1000);
    qDebug() << "__FILE__" << __LINE__;
    ster->setFiltrReset();
    resetFiltrow.singleShot(5000, [this]() {
        if (!this->getResetFiltrowOk())
            reject();
    });
#ifndef DEFVAL
    ui->pbTest->setVisible(false);
#else
    connect(ui->pbTest, &QPushButton::clicked, this, [this]() { this->testValue(); });
#endif
}

OknoBadaniaTlumienia::~OknoBadaniaTlumienia()
{
    zmFiltraTmr.stop();
    zmProgressBar.stop();
    resetFiltrow.stop();
    delete ui;
}

void OknoBadaniaTlumienia::flt_zerowanieFiltrowDone()
{
    actTlumPos = 0;

    setResetFiltrowOk(true);
    auto pos = tlumienia.at(actTlumPos);
    ui->tlumienie->setText(pos.at(0) + QString(" dB"));
    ui->a->setText(pos.at(1));
    ui->b->setText(pos.at(2));
    ui->c->setText(pos.at(3));
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "Rozpoczynam zmiane filtra";
    ster->setFiltrPos(pos.at(1).toShort(), pos.at(2).toShort(), pos.at(3).toShort());
}

void OknoBadaniaTlumienia::flt_setUkladFiltrowDone()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << "Zmiana filtra";
    zmFiltraTmr.start();
    ui->progressBar->setValue(0);
    zmProgressBar.start();
    ui->pozostal_czas->setText(0);
}

void OknoBadaniaTlumienia::flt_bladFiltrow(QChar filtr, bool zerowanie)
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << "Blad filtrow"
             << filtr << zerowanie;

    zmFiltraTmr.stop();
    ui->progressBar->setValue(0);
    zmProgressBar.stop();
}

void OknoBadaniaTlumienia::czujkaOn()
{
    zmFiltraTmr.stop();
    zmProgressBar.stop();
    resetFiltrow.stop();
    wynikBadania = true;
    wykryto = true;
    error = "";
    accept();
}

void OknoBadaniaTlumienia::uplynalCzasPostojuFiltra()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << ui->progressBar->value() << " timeout 15s " << actTlumPos;
                ;
    zmProgressBar.stop();
    ui->progressBar->setValue(0);
    ui->pozostal_czas->setText(0);
    ++actTlumPos;
    if (actTlumPos == maxTlum) {
        zmFiltraTmr.stop();
        resetFiltrow.stop();
        zmFiltraTmr.stop();
        wykryto = false;
        error = "Czujka nie zadziała";
        wynikBadania = false;
        reject();
        return;
    }
    auto pos = tlumienia.at(actTlumPos);
    tlumienie = pos.at(0);
    ui->tlumienie->setText(pos.at(0) + QString(" dB"));
    ui->a->setText(pos.at(1));
    ui->b->setText(pos.at(2));
    ui->c->setText(pos.at(3));
    posFiltrA = pos.at(1).toShort();
    posFiltrB = pos.at(2).toShort();
    posFiltrC = pos.at(3).toShort();

    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "Rozpoczynam zmiane filtra";
    ster->setFiltrPos(posFiltrA, posFiltrB, posFiltrC);
}


void OknoBadaniaTlumienia::progressBarUpdate()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "Progress update " << ui->progressBar->value();
    ui->progressBar->setValue(ui->progressBar->value() + 1);
    ui->pozostal_czas->setText(QString::number(czasPostoju - ui->progressBar->value()));
}

bool OknoBadaniaTlumienia::getResetFiltrowOk()
{
    QMutexLocker lock(&mutex);
    return resetFiltrowOk;
}

void OknoBadaniaTlumienia::setResetFiltrowOk(bool newResetFiltrowOk)
{
    QMutexLocker lock(&mutex);
    resetFiltrowOk = newResetFiltrowOk;
}

short OknoBadaniaTlumienia::getPosFiltrC() const
{
    return posFiltrC;
}

short OknoBadaniaTlumienia::getPosFiltrB() const
{
    return posFiltrB;
}

short OknoBadaniaTlumienia::getPosFiltrA() const
{
    return posFiltrA;
}

const QString &OknoBadaniaTlumienia::getError() const
{
    return error;
}

bool OknoBadaniaTlumienia::getPowtorzBadanie() const
{
    return powtorzBadanie;
}

const QString &OknoBadaniaTlumienia::getTlumienie() const
{
    return tlumienie;
}

bool OknoBadaniaTlumienia::getWynikBadania() const
{
    return wynikBadania;
}

#ifdef DEFVAL
void OknoBadaniaTlumienia::testValue()
{
    bool t1 = zmFiltraTmr.isActive();
    if (t1)
        zmFiltraTmr.stop();
    bool t2 = zmProgressBar.isActive();
    if (t2)
        zmProgressBar.stop();
    bool t3 = resetFiltrow.isActive();
    if (t3)
        resetFiltrow.stop();

    TestValueDialog * dlg = new TestValueDialog(tlumienie, this);
    if (dlg->exec()) {
        tlumienie = dlg->value();
        wynikBadania = true;
        delete dlg;
        accept();
    } else {
        delete dlg;
        if (t1)
            zmFiltraTmr.start();
        if (t2)
            zmProgressBar.start();
        if (t3)
            resetFiltrow.start();
    }
}



TestValueDialog::TestValueDialog(const QString &val, QWidget *parent) :
    QDialog(parent)
{
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setText(val);

    QPushButton *searchButton = new QPushButton(tr("Ustaw"));
    searchButton->setDefault(true);

    QPushButton *cancelButton = new QPushButton(tr("Cancel"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(searchButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *lt = new QVBoxLayout;
    lt->addWidget(m_lineEdit);
    lt->addWidget(buttonBox);

    setLayout(lt);
}

QString TestValueDialog::value() const
{
    return m_lineEdit->text();
}

#endif
