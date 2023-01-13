#ifndef TEST8WYNIK_H
#define TEST8WYNIK_H

#include <QDialog>
#include <QTimer>

#include "danetestu.h"
#include "parametrybadania.h"
#include "ustawienia.h"
namespace Ui {
class Test8Wynik;
}


class Test8Wynik : public QDialog
{
    Q_OBJECT

public:
    explicit Test8Wynik(bool success, const QString & tlumienie, const QString &nazwa,
                          bool repeatPomiar, QWidget *parent = 0);
    ~Test8Wynik();

    bool getPowtorzPomiar() const;

protected:
    void pbCancel_clicked();
private:
    Ui::Test8Wynik *ui;
    bool powtorzPomiar;
};

#endif // TEST8WYNIK_H
