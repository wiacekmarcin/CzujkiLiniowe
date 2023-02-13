#ifndef OKNORESETUZASILANIACZUJKI_H
#define OKNORESETUZASILANIACZUJKI_H
#include "parametrybadania.h"
#include <QDialog>
#include <QTimer>
#include <QCloseEvent>
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

signals:
    void setZasilanie(bool on);
public slots:
    void timeoutProgress();
    void closeEvent(QCloseEvent *event);
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



