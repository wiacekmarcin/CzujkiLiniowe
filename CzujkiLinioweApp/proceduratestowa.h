#ifndef PROCEDURATESTOWA_H
#define PROCEDURATESTOWA_H

#include <QWidget>

namespace Ui {
class ProceduraTestowa;
}

class ProceduraTestowa : public QWidget
{
    Q_OBJECT

public:
    explicit ProceduraTestowa(QWidget *parent = nullptr);
    ~ProceduraTestowa();

private:
    Ui::ProceduraTestowa *ui;
};

#endif // PROCEDURATESTOWA_H
