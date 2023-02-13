#ifndef OKNOPOTWIERDZENIEEMCNARAZENIE_H
#define OKNOPOTWIERDZENIEEMCNARAZENIE_H

#include <QDialog>
#include <QCloseEvent>
#include "danetestu.h"
namespace Ui {
class OknoPotwierdzenieEMCNarazenie;
}

class OknoPotwierdzenieEMCNarazenie : public QDialog
{
    Q_OBJECT

public:
    explicit OknoPotwierdzenieEMCNarazenie(const DaneTestu &daneTestu, QWidget *parent = nullptr);
    ~OknoPotwierdzenieEMCNarazenie();

    void changeComboBox(short nrCombo, int index);
    bool getWynik() const;
    QString getKomenatarz() const;
    void closeEvent(QCloseEvent *event);
protected:
    void pbCancel_clicked();
private:
    Ui::OknoPotwierdzenieEMCNarazenie *ui;
    unsigned idTest;
};

#endif // OKNOPOTWIERDZENIEEMCNARAZENIE_H
