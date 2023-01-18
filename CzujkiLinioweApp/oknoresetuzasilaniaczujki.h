#ifndef OKNORESETUZASILANIACZUJKI_H
#define OKNORESETUZASILANIACZUJKI_H
#include "parametrybadania.h"
#include <QDialog>
#include <QTimer>

class Zasilacz;

namespace Ui {
class OknoResetuZasilaniaCzujki;
}

class OknoResetuZasilaniaCzujki : public QDialog
{
    Q_OBJECT

public:
    explicit OknoResetuZasilaniaCzujki(const QString & testName, const QString &podtitle,
                                       unsigned int maxTime_, const ParametryBadania &daneBadania, Zasilacz *zas,
                                  QWidget *parent = 0);
    ~OknoResetuZasilaniaCzujki();

public slots:
    void timeoutProgress();
protected:
    void pbCancel_clicked();
private:
    Ui::OknoResetuZasilaniaCzujki *ui;

    unsigned int maxTime;
    unsigned int actTime;
    QTimer timer;
    Zasilacz * zas;
};

#endif // OKNORESETUZASILANIACZUJKI_H



