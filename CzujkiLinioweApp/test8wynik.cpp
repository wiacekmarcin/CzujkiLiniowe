#include "test8wynik.h"
#include "ui_test8wynik.h"
#include <QDateTime>

#include <QMessageBox>

Test8Wynik::Test8Wynik(bool success, const QString &tlumienie,
                           const QString &nazwa, bool repeat, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test8Wynik),
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
    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
    connect(ui->pbRepeat, &QPushButton::clicked, this, [this]() { powtorzPomiar = true; this->accept(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
}

Test8Wynik::~Test8Wynik()
{
    delete ui;
}

bool Test8Wynik::getPowtorzPomiar() const
{
    return powtorzPomiar;
}

void Test8Wynik::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}
