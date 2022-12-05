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

protected:
    void loadTable();
    void countFiltr665(short A, short B, short C, bool wylicz=true);
    void countFiltr880(short A, short B, short C, bool wylicz=true);
private slots:
    void on_cb_880_A_currentIndexChanged(int index);
    void on_cb_880_B_currentIndexChanged(int index);
    void on_cb_880_C_currentIndexChanged(int index);
    void on_cb_655_A_currentIndexChanged(int index);
    void on_cb_655_B_currentIndexChanged(int index);
    void on_cb_655_C_currentIndexChanged(int index);

    void on_tableWidget_cellEntered(int row, int column);

    void on_tableWidget_cellClicked(int row, int column);

    void on_tableWidget_cellActivated(int row, int column);

    void on_tableWidget_cellPressed(int row, int column);

private:
    Ui::UstawieniaFiltrow *ui;
    Ustawienia *ust;
    QList<QPair<double, QString>> tlumienia;
    QList<QStringList> tlumienia655;
    QList<QStringList> tlumienia880;
};

#endif // USTAWIENIAFILTROW_H
