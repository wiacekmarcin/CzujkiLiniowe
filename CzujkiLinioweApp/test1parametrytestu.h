#ifndef TEST1PARAMETRYTESTU_H
#define TEST1PARAMETRYTESTU_H

#include <QDialog>
#include "danebadania.h"

namespace Ui {
class Test1ParametryTestu;
}

class Test1ParametryTestu : public QDialog
{
    Q_OBJECT

public:
    explicit Test1ParametryTestu(DaneBadania * badanie, short TestId, short CzujkaId, QWidget *parent = nullptr);
    ~Test1ParametryTestu();

private:
    Ui::Test1ParametryTestu *ui;
    DaneBadania * badanie;
};

#endif // TEST1PARAMETRYTESTU_H
