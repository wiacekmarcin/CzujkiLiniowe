#include "haslodialog.h"
#include "ui_haslodialog.h"

HasloDialog::HasloDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HasloDialog)
{
    ui->setupUi(this);
}

HasloDialog::~HasloDialog()
{
    delete ui;
}

QString HasloDialog::getHaslo() const
{
    return ui->haslo->text();
}
