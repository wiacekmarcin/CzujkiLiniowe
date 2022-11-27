#ifndef USTAWIENIAFILTROW_H
#define USTAWIENIAFILTROW_H

#include <QWidget>
#include "ustawienia.h"
namespace Ui {
class UstawieniaFiltrow;
}

class UstawieniaFiltrow : public QWidget
{
    Q_OBJECT

public:
    explicit UstawieniaFiltrow(QWidget *parent = nullptr);
    ~UstawieniaFiltrow();
    void setUstawienia(Ustawienia *u);
    void saveFiltry();
private:
    Ui::UstawieniaFiltrow *ui;
    Ustawienia *ust;
};

#endif // USTAWIENIAFILTROW_H
