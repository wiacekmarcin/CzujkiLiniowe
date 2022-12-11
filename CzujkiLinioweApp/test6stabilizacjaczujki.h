#ifndef TEST6STABILIZACJACZUJKI_H
#define TEST6STABILIZACJACZUJKI_H

#include <QDialog>
#include <QTimer>
#include "parametrybadania.h"
#include "danetestu.h"
namespace Ui {
class Test6StabilizacjaCzujki;
}

class Test6StabilizacjaCzujki : public QDialog
{
    Q_OBJECT

public:
    explicit Test6StabilizacjaCzujki(short nrPomiaru, const DaneTestu &daneTestu, const ParametryBadania &daneBadania, QWidget *parent = 0);
    ~Test6StabilizacjaCzujki();

protected:
    QString getMM_SS(unsigned int secs);
private slots:
    void timeout();
private:
    Ui::Test6StabilizacjaCzujki *ui;
    QTimer timer;
    unsigned int elapsedTime;
};

#endif // TEST6STABILIZACJACZUJKI_H
