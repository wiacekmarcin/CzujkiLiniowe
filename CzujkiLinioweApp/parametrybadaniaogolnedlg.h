#ifndef PARAMETRYBADANIAOGOLNEDLG_H
#define PARAMETRYBADANIAOGOLNEDLG_H

#include <QDialog>
#include <QLabel>

#include "ustawienia.h"
#include "danebadania.h"
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

    void init(const Ustawienia & u, DaneBadania * badanie, QLabel * errLab);
    void save(DaneBadania * badanie);
    bool check();
protected:
    void setWewnetrznyZasilacz(bool inside);
    void setWyzwolenieAlarmu(bool przekaznik);
private slots:
    void on_numerZlecenia_editingFinished();
    void on_numerTestu_editingFinished();
    void on_osobaOdpowiedzialna_editingFinished();
    void on_uwagi_textChanged();
    void on_napiecieZasilaniain_editingFinished();
    void on_typCentraliSygnalizacji_editingFinished();
    void on_czasStabilizacjiCzujki_editingFinished();
    void on_pradAlarmu_editingFinished();
    void on_dlugoscFali_currentIndexChanged(int index);
    void on_czasPomiedzyZmianamifiltra_editingFinished();

    void on_rbInsideSupply_toggled(bool checked);

    void on_rbCentralSupply_toggled(bool checked);

    void on_rbAlarmPrzekaznik_toggled(bool checked);

    void on_rbPrad_toggled(bool checked);

private:
    Ui::ParametryBadaniaOgolneDlg *ui;
    DaneBadania * badanie;
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
