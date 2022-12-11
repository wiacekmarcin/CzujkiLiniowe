#include "teststanowiskadlg.h"
#include "ui_teststanowiskadlg.h"

#include "sterownik.h"
#include "zasilacz.h"

TestStanowiskaDlg::TestStanowiskaDlg(Zasilacz * zas_, Sterownik * ster_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestStanowiskaDlg),
    ,zas(zas_)
    ,ster(ster_)
{
    ui->setupUi(this);


}

TestStanowiskaDlg::~TestStanowiskaDlg()
{
    delete ui;
}
