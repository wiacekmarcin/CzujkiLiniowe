#include "testzasilaczadlg.h"
#include "ui_testzasilaczadlg.h"

TestZasilaczaDlg::TestZasilaczaDlg(Ustawienia * ust, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestZasilaczaDlg)
  , u(ust)
{
    ui->setupUi(this);
}

TestZasilaczaDlg::~TestZasilaczaDlg()
{
    delete ui;
}

void TestZasilaczaDlg::on_pbCmdSend_clicked()
{
    //if (ui->multi_cmd->text().isEmpty())
    //    return;
    //ui->multi_request->append(QString(ui->multi_cmd->text())+QString("\n"));
}
