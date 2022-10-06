#include "debugdialog.h"
#include "ui_debugdialog.h"

DebugDialog::DebugDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugDialog)
{
    ui->setupUi(this);
}

DebugDialog::~DebugDialog()
{
    delete ui;
}

void DebugDialog::add(const QString &text)
{
    ui->plainTextEdit->appendPlainText(text);
}
