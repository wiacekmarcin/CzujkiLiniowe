#include "listabadan.h"
#include "ui_listabadan.h"
#include "test1parametrytestu.h"
ListaBadan::ListaBadan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListaBadan),
    procedura(parent)
{
    ui->setupUi(this);
}

ListaBadan::~ListaBadan()
{
    delete ui;
}

void ListaBadan::startBadanie(short id, const ParametryBadania & badanie)
{
    QString nameTest = "Nieznane Badanie";
    if (id < lt.nazwyTestow.size())
        nameTest = lt.nazwyTestow.at(id);
    procedura.startBadanie(id, nameTest, badanie);
}
