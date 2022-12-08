#include "test5zasilanieczujki.h"
#include "ui_test5zasilanieczujki.h"

#include <QMessageBox>

Test5ZasilanieCzujki::Test5ZasilanieCzujki(const ParametryBadania &badanie, const DaneTestu &test, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test5ZasilanieCzujki)
{
    ui->setupUi(this);
    ui->testName->setText(test.getName());
    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
    connect(ui->pbDalej, &QPushButton::pressed, this, [this]() { this->accept(); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    connect(ui->pbPrzerwij, &QPushButton::pressed, this, [this]() { this->pbCancel_clicked(); });
}

Test5ZasilanieCzujki::~Test5ZasilanieCzujki()
{
    delete ui;
}

void Test5ZasilanieCzujki::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Badanie : %1").arg(ui->testName->text()),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}
