#include "parametrybadania2.h"
#include "ui_parametrybadania2.h"

ParametryBadania2::ParametryBadania2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametryBadania2)
{
    ui->setupUi(this);
    //ui->lcz1->setDisabled(true);
    //ui->lcz2->setEnabled(false);
    ui->nadajnik1->setDisabled(true);
    ui->odbiornik1->setDisabled(true);

    switchOdbiornikReflektor(false);
}

ParametryBadania2::~ParametryBadania2()
{
    delete ui;
}

void ParametryBadania2::switchOdbiornikReflektor(bool odbiornik)
{
    ui->hreflektor->setVisible(!odbiornik);
    ui->reflektor1->setVisible(!odbiornik);
    ui->reflektor2->setVisible(!odbiornik);
    ui->reflektor3->setVisible(!odbiornik);
    ui->reflektor4->setVisible(!odbiornik);
    ui->reflektor5->setVisible(!odbiornik);
    ui->reflektor6->setVisible(!odbiornik);
    ui->reflektor7->setVisible(!odbiornik);

    ui->hodbiornik->setVisible(odbiornik);
    ui->odbiornik1->setVisible(odbiornik);
    ui->odbiornik2->setVisible(odbiornik);
    ui->odbiornik3->setVisible(odbiornik);
    ui->odbiornik4->setVisible(odbiornik);
    ui->odbiornik5->setVisible(odbiornik);
    ui->odbiornik6->setVisible(odbiornik);
    ui->odbiornik7->setVisible(odbiornik);


}

void ParametryBadania2::on_comboBox_currentIndexChanged(int index)
{
    if (index == 0) {
        switchOdbiornikReflektor(true);
    }
    if (index == 1) {
        switchOdbiornikReflektor(false);
    }
}

