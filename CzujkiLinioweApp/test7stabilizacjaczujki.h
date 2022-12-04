#ifndef TEST7STABILIZACJACZUJKI_H
#define TEST7STABILIZACJACZUJKI_H

#include <QDialog>

namespace Ui {
class Test7StabilizacjaCzujki;
}

class Test7StabilizacjaCzujki : public QDialog
{
    Q_OBJECT

public:
    explicit Test7StabilizacjaCzujki(QWidget *parent = 0);
    ~Test7StabilizacjaCzujki();

private:
    Ui::Test7StabilizacjaCzujki *ui;
};

#endif // TEST7STABILIZACJACZUJKI_H
