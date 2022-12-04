#ifndef TEST4MONTAZ_H
#define TEST4MONTAZ_H

#include <QDialog>

namespace Ui {
class Test4Montaz;
}

class Test4Montaz : public QDialog
{
    Q_OBJECT

public:
    explicit Test4Montaz(QWidget *parent = nullptr);
    ~Test4Montaz();

private:
    Ui::Test4Montaz *ui;
};

#endif // TEST4MONTAZ_H
