#include "ustawieniafiltrow.h"
#include "ui_ustawieniafiltrow.h"

UstawieniaFiltrow::UstawieniaFiltrow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UstawieniaFiltrow)
{
    ui->setupUi(this);
}

UstawieniaFiltrow::~UstawieniaFiltrow()
{
    delete ui;
}
