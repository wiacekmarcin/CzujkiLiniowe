#ifndef TESTSTEROWNIKADLG_H
#define TESTSTEROWNIKADLG_H

#include <QDialog>

namespace Ui {
class TestSterownikaDlg;
}

class TestSterownikaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TestSterownikaDlg(QWidget *parent = nullptr);
    ~TestSterownikaDlg();

private:
    Ui::TestSterownikaDlg *ui;
};

#endif // TESTSTEROWNIKADLG_H
