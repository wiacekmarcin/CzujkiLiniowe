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
    ui->info->setVisible(repeat);
    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->done(QDialog::Accepted); });
    connect(ui->pbRepeat, &QPushButton::clicked, this, [this]() { powtorzPomiar = true; this->done(QDialog::Accepted); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    adjustSize();
}

OknoWynikBadaniaTlumienia::~OknoWynikBadaniaTlumienia()
{
    delete ui;
}

bool OknoWynikBadaniaTlumienia::getPowtorzPomiar() const
{
    return powtorzPomiar;
}

static int question(QWidget * parent, const QString & title, const QString & pytanie) {
    QMessageBox messageBox(QMessageBox::Question, title, pytanie, 
                        QMessageBox::Yes | QMessageBox::No, parent);

    messageBox.setButtonText(QMessageBox::Yes, QString::fromUtf8("Tak"));
    messageBox.setButtonText(QMessageBox::No, QString::fromUtf8("Nie"));
    return messageBox.exec();
}


void OknoWynikBadaniaTlumienia::pbCancel_clicked()
{
    int ret = question(this, QString::fromUtf8("Badanie : %1").arg(ui->testName->text()),
                                    QString::fromUtf8("Czy napewno chcesz przerwać badanie"));
    if (ret == QMessageBox::Yes)
        done(QDialog::Rejected);
}
