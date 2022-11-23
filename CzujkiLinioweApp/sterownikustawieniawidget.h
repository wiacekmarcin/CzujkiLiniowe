#ifndef STEROWNIKUSTAWIENIAWIDGET_H
#define STEROWNIKUSTAWIENIAWIDGET_H

#include <QWidget>
#include "ustawienia.h"

namespace Ui {
class SterownikUstawieniaWidget;
}

class SterownikUstawieniaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SterownikUstawieniaWidget(QWidget *parent = nullptr);
    ~SterownikUstawieniaWidget();
    void init(Ustawienia * u);

private slots:
    void on_pbSetConfiguration_clicked();

private:
    Ui::SterownikUstawieniaWidget *ui;
    Ustawienia* u;
};

#endif // STEROWNIKUSTAWIENIAWIDGET_H
