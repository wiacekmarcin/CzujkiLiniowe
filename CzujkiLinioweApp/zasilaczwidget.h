#ifndef ZASILACZWIDGET_H
#define ZASILACZWIDGET_H

#include <QWidget>
#include "zasilacz.h"

namespace Ui {
class ZasilaczWidget;
}

class ZasilaczWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZasilaczWidget(QWidget *parent = nullptr);
    ~ZasilaczWidget();


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
    void configuredZasilacz(int state);
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


private:
    Ui::ZasilaczWidget *ui;
    Zasilacz zas;
    QThread zasThr;
};

#endif // ZASILACZWIDGET_H

