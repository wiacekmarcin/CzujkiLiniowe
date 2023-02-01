#ifndef OKNOPOTWIERDZENIENARAZENIA_H
#define OKNOPOTWIERDZENIENARAZENIA_H

#include <QDialog>
#include "danetestu.h"
namespace Ui {
class OknoPotwierdzenieNarazenia;
}

class OknoPotwierdzenieNarazenia : public QDialog
{
    Q_OBJECT

public:
    explicit OknoPotwierdzenieNarazenia(const DaneTestu &daneTestu, QWidget *parent = nullptr);
    ~OknoPotwierdzenieNarazenia();

    void changeComboBox(short nrCombo, int index);
    bool getWynik() const;
    QString getKomenatarz() const;
protected:
    void pbCancel_clicked();
private:
    Ui::OknoPotwierdzenieNarazenia *ui;
    unsigned idTest;
};

#endif // OKNOPOTWIERDZENIENARAZENIA_H
