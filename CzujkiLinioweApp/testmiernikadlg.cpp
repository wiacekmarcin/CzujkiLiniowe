#include "testmiernikadlg.h"
#include "ui_testmiernikadlg.h"

TestMiernikaDlg::TestMiernikaDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestMiernikaDlg)
{
    ui->setupUi(this);
}

TestMiernikaDlg::~TestMiernikaDlg()
{
    delete ui;
}
