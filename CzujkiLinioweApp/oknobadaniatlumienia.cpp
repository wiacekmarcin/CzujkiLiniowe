#include "oknobadaniatlumienia.h"
#include "ui_oknobadaniatlumienia.h"
#include <QDateTime>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "sterownik.h"
#include <QMutexLocker>

OknoBadaniaTlumienia::OknoBadaniaTlumienia(unsigned int czasPostojuFiltra, unsigned int dlugoscFali,
                           const QString & name, const Ustawienia &ust,
                           Sterownik * ster_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoBadaniaTlumienia),
    tmZmFiltra(this),
    tmZmProgressBar(this),
    tmSterownika(this),
    actTlumPos(0),
    maxTlum(0),
    ster(ster_),
    czasPostoju(czasPostojuFiltra),
    wynikBadania(false)

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
    ui->czas->setText(QString::number(czasPostoju) + QString(" s"));
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(czasPostoju);

    tmZmFiltra.setInterval(czasPostoju*1000);
    tmZmProgressBar.setInterval(1000);

    connect(&tmZmProgressBar, &QTimer::timeout, this, &OknoBadaniaTlumienia::progressBarUpdate);
    connect(&tmZmFiltra, &QTimer::timeout, this, &OknoBadaniaTlumienia::uplynalCzasPostojuFiltra);


    sterResponse = false;
    ster->setFiltrReset();
    tmSterownika.singleShot(5000, this, &OknoBadaniaTlumienia::timeoutSterownika);

#ifndef DEFVAL
    ui->pbTest->setVisible(false);
#else
    connect(ui->pbTest, &QPushButton::clicked, this, [this]() { this->testValue(); });
#endif
}

OknoBadaniaTlumienia::~OknoBadaniaTlumienia()
{
    tmZmFiltra.stop();
    tmZmProgressBar.stop();
    tmSterownika.stop();
    delete ui;
}

void OknoBadaniaTlumienia::flt_zerowanieFiltrowDone()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "Rozpoczynam zmiane filtra";
    sterResponse = true;
    tmSterownika.stop();
    actTlumPos = 0;

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
    qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << __FILE__ << __LINE__ <<
                "Filtr zmieniony";
    tmZmFiltra.start();
    ui->progressBar->setValue(0);
    tmZmProgressBar.start();
    ui->pozostal_czas->setText(QString::number(czasPostoju) + " s ");
}

void OknoBadaniaTlumienia::flt_bladFiltrow(QChar filtr, bool zerowanie)
{
    qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << __FILE__ << __LINE__ <<
                "Blad filtrow" << filtr << zerowanie;
    //TODO
    return;
    wynikBadania = false;
    tmSterownika.stop();
    tmZmFiltra.stop();
    tmZmProgressBar.stop();
    error = QString::fromUtf8("Błąd filtra %1").arg(filtr);
    reject();
}

void OknoBadaniaTlumienia::czujkaOn()
{
    qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << __FILE__ << __LINE__ <<
             "czujka on";
    tmZmFiltra.stop();
    tmZmProgressBar.stop();
    tmSterownika.stop();
    wynikBadania = true;
    error = "";
    accept();
}

void OknoBadaniaTlumienia::uplynalCzasPostojuFiltra()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << " timeout postoju filtra " << actTlumPos;

    tmZmProgressBar.stop();
    tmZmFiltra.stop();

    ui->progressBar->setValue(czasPostoju);
    ui->pozostal_czas->setText("0 s");
    ++actTlumPos;
    if (actTlumPos == maxTlum) {
        error = QString::fromUtf8("Czujka nie zadziała");
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
    short posFiltrA = pos.at(1).toShort();
    short posFiltrB = pos.at(2).toShort();
    short posFiltrC = pos.at(3).toShort();

    qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << __FILE__ << __LINE__ <<
             "Rozpoczynam zmiane filtra " << posFiltrA << posFiltrA << posFiltrB << posFiltrC;
    ster->setFiltrPos(posFiltrA, posFiltrB, posFiltrC);
}


void OknoBadaniaTlumienia::progressBarUpdate()
{
    ui->progressBar->setValue(ui->progressBar->value() + 1);
    ui->pozostal_czas->setText(QString::number(czasPostoju - ui->progressBar->value())+ " s");
}

void OknoBadaniaTlumienia::timeoutSterownika()
{
    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz")
             << "hardware timeout ";
    if (sterResponse)
        return;
    error = QString::fromUtf8("Błąd sprzętowy");
    wynikBadania = false;
    tmZmProgressBar.stop();
    tmZmFiltra.stop();
    reject();
}

const QString &OknoBadaniaTlumienia::getError() const
{
    return error;
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
    bool t1 = tmZmFiltra.isActive();
    if (t1)
        tmZmFiltra.stop();
    bool t2 = tmZmProgressBar.isActive();
    if (t2)
        tmZmProgressBar.stop();
    bool t3 = tmSterownika.isActive() || tmSterownika.isSingleShot();
    if (t3)
        tmSterownika.stop();

    TestValueDialog * dlg = new TestValueDialog(tlumienie, this);
    if (dlg->exec()) {
        tlumienie = dlg->value();
        wynikBadania = true;
        delete dlg;
        accept();
    } else {
        delete dlg;
        if (t1)
            tmZmFiltra.start();
        if (t2)
            tmZmProgressBar.start();
        if (t3)
            tmSterownika.start();
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
