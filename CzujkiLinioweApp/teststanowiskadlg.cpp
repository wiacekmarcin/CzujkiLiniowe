#include "teststanowiskadlg.h"
#include "ui_teststanowiskadlg.h"

TestStanowiskaDlg::TestStanowiskaDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestStanowiskaDlg)
{
    ui->setupUi(this);
}

TestStanowiskaDlg::~TestStanowiskaDlg()
{
    delete ui;
}
