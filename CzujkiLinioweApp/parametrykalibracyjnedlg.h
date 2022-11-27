#ifndef PARAMETRYKALIBRACYJNEDLG_H
#define PARAMETRYKALIBRACYJNEDLG_H

#include "ustawienia.h"
#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class ParametryKalibracyjneDlg;
}

class ParametryKalibracyjneDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ParametryKalibracyjneDlg(Ustawienia * u, QWidget *parent = nullptr);
    ~ParametryKalibracyjneDlg();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void acceptButton();

private:
    Ui::ParametryKalibracyjneDlg *ui;
};

#endif // PARAMETRYKALIBRACYJNEDLG_H
