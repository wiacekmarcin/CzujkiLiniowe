#include "oknowynikbadaniatlumienia.h"
#include "ui_oknowynikbadaniatlumienia.h"
#include <QDateTime>

#include <QMessageBox>

OknoWynikBadaniaTlumienia::OknoWynikBadaniaTlumienia(bool success, const QString &tlumienie,
                           const QString &nazwa, bool repeat, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoWynikBadaniaTlumienia),
    powtorzPomiar(false)

{
    ui->setupUi(this);
    ui->testName->setText(nazwa);
    ui->tlumienie->setText(tlumienie + QString(" dB"));
    if (success) {
        ui->result->setText("poprawnie");
        ui->result->setStyleSheet("color:green");
    } else {
        ui->result->setText("błędem");
        ui->result->setStyleSheet("color:red");
        ui->tlumienie->setText("- -");
    }

    ui->pbRepeat->setVisible(repeat);
    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->done(QDialog::Accepted); });
    connect(ui->pbRepeat, &QPushButton::clicked, this, [this]() { powtorzPomiar = true; this->done(QDialog::Accepted); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
}

OknoWynikBadaniaTlumienia::~OknoWynikBadaniaTlumienia()
{
    delete ui;
}

bool OknoWynikBadaniaTlumienia::getPowtorzPomiar() const
{
    return powtorzPomiar;
}

void OknoWynikBadaniaTlumienia::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        done(QDialog::Rejected);
}
