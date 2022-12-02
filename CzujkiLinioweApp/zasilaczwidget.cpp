#include "zasilaczwidget.h"
#include "ui_zasilaczwidget.h"

#include <QMessageBox>

ZasilaczWidget::ZasilaczWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZasilaczWidget)
    , zas(this)
    , zasThr(this)
{
    ui->setupUi(this);


}

ZasilaczWidget::~ZasilaczWidget()
{
    delete ui;

}


