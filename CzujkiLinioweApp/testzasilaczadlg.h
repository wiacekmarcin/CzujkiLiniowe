#ifndef TESTZASILACZADLG_H
#define TESTZASILACZADLG_H

#include <QThread>
#include "zasilacz.h"
#include "ustawienia.h"
#include <QDialog>

namespace Ui {
class TestZasilaczaDlg;
}

class TestZasilaczaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TestZasilaczaDlg(Ustawienia * ust, QWidget *parent = nullptr);
    ~TestZasilaczaDlg();

    typedef enum _serialZasWork
        {
            SEND_NONE = 0,
            SEND_IDENT = 1,
            SEND_VOLT,
            SEND_CURR
        } SerialZasWork;

private slots:

    void errorZasilacz(QString s);
    void debugZasilacz(QString d);
    void configuredZasilacz(bool success, int state);
    void serialNoZasilacz(QString s);
    void deviceNameZasilacz(QString name);
    void valueZasilacz(int kind, int value);

    void openZasilacz();
    void closeZasilacz();

    void on_tbChange_clicked();
    void on_tbSave_clicked();
    void on_tbCancel_clicked();

protected:

    bool findSterownik();
    QList<QStringList> getComPorts();



protected slots:
    void on_pbCmdSend_clicked();
private:
    Ui::TestZasilaczaDlg *ui;
    Ustawienia * u;
    Zasilacz zas;
    QThread zasThr;
};

#endif // TESTZASILACZADLG_H
