#ifndef PARAMETRYBADANIAOGOLNEDLG_H
#define PARAMETRYBADANIAOGOLNEDLG_H

#include <QDialog>
#include <QLabel>

#include "ustawienia.h"
#include "parametrybadania.h"
namespace Ui {
class ParametryBadaniaOgolneDlg;
}

class ParametryBadaniaDlg;

class ParametryBadaniaOgolneDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ParametryBadaniaOgolneDlg(QWidget *parent = nullptr);
    ~ParametryBadaniaOgolneDlg();

    void init(bool edit, const Ustawienia & u, ParametryBadania * badanie, QLabel * errLab);
    void save(ParametryBadania * badanie);
    bool check();
protected:
    void setWewnetrznyZasilacz(bool inside);
    void setWyzwolenieAlarmu(bool przekaznik);
private slots:

    void on_dlugoscFali_currentIndexChanged(int index);

    void on_rbInsideSupply_toggled(bool checked);
    void on_rbCentralSupply_toggled(bool checked);
    void on_rbAlarmPrzekaznik_toggled(bool checked);
    void on_rbPrad_toggled(bool checked);
    void changeHaslo();
private:
    Ui::ParametryBadaniaOgolneDlg *ui;
    ParametryBadania * badanie;
    QLabel * errorLabel;
    unsigned minVolt;
    unsigned maxVolt;
    unsigned minCzasStab;
    unsigned maxCzasStab;
    unsigned minPrad;
    unsigned maxPrad;
    unsigned minCzasZmianyFiltra;
    unsigned maxCzasZmianyFiltra;
};

#endif // PARAMETRYBADANIAOGOLNEDLG_H
