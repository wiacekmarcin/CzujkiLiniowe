#include "ustawienianapedow.h"
#include "ui_ustawienianapedow.h"

UstawieniaNapedow::UstawieniaNapedow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UstawieniaNapedow)
{
    ui->setupUi(this);
}

UstawieniaNapedow::~UstawieniaNapedow()
{
    delete ui;
}
