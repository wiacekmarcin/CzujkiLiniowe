#include "proceduratestowa.h"
#include "ui_proceduratestowa.h"

ProceduraTestowa::ProceduraTestowa(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProceduraTestowa)
{
    ui->setupUi(this);
}

ProceduraTestowa::~ProceduraTestowa()
{
    delete ui;
}
