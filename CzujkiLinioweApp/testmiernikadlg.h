#ifndef TESTMIERNIKADLG_H
#define TESTMIERNIKADLG_H

#include <QDialog>

namespace Ui {
class TestMiernikaDlg;
}

class TestMiernikaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TestMiernikaDlg(QWidget *parent = nullptr);
    ~TestMiernikaDlg();

protected slots:
    void on_pbCmdSend_clicked();
private:
    Ui::TestMiernikaDlg *ui;
};

#endif // TESTMIERNIKADLG_H
