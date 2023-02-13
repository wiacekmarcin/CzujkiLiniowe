#ifndef OKNOWYNIKBADANIATLUMIENIA_H
#define OKNOWYNIKBADANIATLUMIENIA_H

#include <QDialog>
#include <QTimer>
#include <QCloseEvent>
#include "danetestu.h"
#include "parametrybadania.h"
#include "ustawienia.h"
namespace Ui {
class OknoWynikBadaniaTlumienia;
}


class OknoWynikBadaniaTlumienia : public QDialog
{
    Q_OBJECT

public:
    explicit OknoWynikBadaniaTlumienia(bool success, const QString & tlumienie, const QString &nazwa,
                          bool repeatPomiar, QWidget *parent = 0);
    ~OknoWynikBadaniaTlumienia();

    bool getPowtorzPomiar() const;
    void closeEvent(QCloseEvent *event);
protected:
    void pbCancel_clicked();
private:
    Ui::OknoWynikBadaniaTlumienia *ui;
    bool powtorzPomiar;
};

#endif // OKNOWYNIKBADANIATLUMIENIA_H
