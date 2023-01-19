#include "oknobadaniatlumienia.h"
#include "ui_oknobadaniatlumienia.h"
#include <QDateTime>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "sterownik.h"
#include <QMutexLocker>
#include <QDebug>

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
    tlumienie("0.0"),
    czasPostoju(czasPostojuFiltra)
{
    ui->setupUi(this);

    if (dlugoscFali == 880) {
        tlumienia = ust.getTlumienia880();
    } else {
        tlumienia = ust.getTlumienia655();
    }
    maxTlum=tlumienia.size();

    ui->dlugoscFali->setText(QString::number(dlugoscFali) + " nm");
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
    tmSterownika.singleShot(15000, this, &OknoBadaniaTlumienia::timeoutSterownika);

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
    sterResponse = true;
    tmZmFiltra.start();
    ui->progressBar->setValue(0);
    tmZmProgressBar.start();
    ui->pozostal_czas->setText(QString::number(czasPostoju) + " s ");
}

void OknoBadaniaTlumienia::flt_bladFiltrow(QChar filtr, bool zerowanie)
{
    qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") << __FILE__ << __LINE__ <<
                "Blad filtrow" << filtr << zerowanie;
    sterResponse = true;
    tmSterownika.stop();
    tmZmFiltra.stop();
    tmZmProgressBar.stop();
    error = QString::fromUtf8("Błąd ustawienia filtra %1").arg(filtr);
    reject();
}

void OknoBadaniaTlumienia::czujkaOn()
{
    qDebug() << "Czujka ON";
    tmZmFiltra.stop();
    tmZmProgressBar.stop();
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
    error = QString::fromUtf8("Brak komunikacji ze stanowiskiem");
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

#ifdef DEFVAL
void OknoBadaniaTlumienia::testValue()
{
    TestValueDialog * dlg = new TestValueDialog(tlumienie, this);
    if (dlg->exec()) {
        tlumienie = dlg->value();
        error = "";
        delete dlg;
        accept();
    } else {
        error = QString::fromUtf8("Czujka nie zadziała");
        reject();
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
