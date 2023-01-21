#ifndef OKNOPARAMETRYTESTU_H
#define OKNOPARAMETRYTESTU_H

#include <QDialog>
#include "parametrybadania.h"
#include "danetestu.h"


namespace Ui {
class OknoParametryTestu;
}

class OknoParametryTestu : public QDialog
{
    Q_OBJECT

public:
    explicit OknoParametryTestu(short nrPomiaru, DaneTestu * test, const ParametryBadania & badanie,
                                 QWidget *parent = nullptr);
    ~OknoParametryTestu();
protected:
    void check();
    void pbOK_clicked();
    void pbCancel_clicked();
private slots:
    void changeCzujka(int index);

private:

    void addError(const QString & err, bool prio);

    Ui::OknoParametryTestu *ui;
    DaneTestu * test;
    short nrPomiar;
    bool error;
    QStringList errorsMsg;
    short nrCzujkiDoWybrania;
    const ParametryBadania & badanie;

    void check1Pomiar();
};

#endif // OKNOPARAMETRYTESTU_H
