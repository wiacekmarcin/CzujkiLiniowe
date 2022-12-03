#ifndef TESTSTANOWISKADLG_H
#define TESTSTANOWISKADLG_H

#include <QDialog>

namespace Ui {
class TestStanowiskaDlg;
}

class TestStanowiskaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TestStanowiskaDlg(QWidget *parent = nullptr);
    ~TestStanowiskaDlg();

private:
    Ui::TestStanowiskaDlg *ui;
};

#endif // TESTSTANOWISKADLG_H
