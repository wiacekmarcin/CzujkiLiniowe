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

protected:
    void pbCancel_clicked();
private:
    Ui::OknoPotwierdzenieNarazenia *ui;
};

#endif // OKNOPOTWIERDZENIENARAZENIA_H
