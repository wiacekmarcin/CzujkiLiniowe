#ifndef USTAWIENIAFILTROW_H
#define USTAWIENIAFILTROW_H

#include <QWidget>

namespace Ui {
class UstawieniaFiltrow;
}

class UstawieniaFiltrow : public QWidget
{
    Q_OBJECT

public:
    explicit UstawieniaFiltrow(QWidget *parent = nullptr);
    ~UstawieniaFiltrow();

private:
    Ui::UstawieniaFiltrow *ui;
};

#endif // USTAWIENIAFILTROW_H
