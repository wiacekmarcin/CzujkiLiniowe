#include "parametrykalibracyjnedlg.h"
#include "ui_parametrykalibracyjnedlg.h"
#include <QDebug>

ParametryKalibracyjneDlg::ParametryKalibracyjneDlg(Ustawienia * u, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametryKalibracyjneDlg)
{
    ui->setupUi(this);
    ui->tabFiltry->setUstawienia(u);
    ui->tabNapedy->setUstawienia(u);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ParametryKalibracyjneDlg::acceptButton);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    adjustSize();
}

ParametryKalibracyjneDlg::~ParametryKalibracyjneDlg()
{
    delete ui;
}

void ParametryKalibracyjneDlg::on_buttonBox_clicked(QAbstractButton *button)
{
    if((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::Apply) ){
        ui->tabFiltry->saveFiltry();
        ui->tabNapedy->saveNapedy();
    }
}

void ParametryKalibracyjneDlg::acceptButton()
{
    ui->tabFiltry->saveFiltry();
    ui->tabNapedy->saveNapedy();
}
