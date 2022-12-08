#ifndef LISTABADAN_H
#define LISTABADAN_H

#include <QWidget>

#include "parametrybadania.h"
#include "danetestu.h"
#include "proceduratestowa.h"
namespace Ui {
class ListaBadan;
}

class ListaBadan : public QWidget
{
    Q_OBJECT

public:
    explicit ListaBadan(QWidget *parent = nullptr);
    ~ListaBadan();
    void startBadanie(short id, const ParametryBadania & badanie);

private:
    Ui::ListaBadan *ui;
    DaneTestu test;
    ProceduraTestowa procedura;
    ListaTestow lt;
};

#endif // LISTABADAN_H
