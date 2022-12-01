#include "teststerownikadlg.h"
#include "ui_teststerownikadlg.h"

TestSterownikaDlg::TestSterownikaDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestSterownikaDlg)
{
    ui->setupUi(this);
}

TestSterownikaDlg::~TestSterownikaDlg()
{
    delete ui;
}
