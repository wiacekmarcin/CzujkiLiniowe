#ifndef OKNOSTABILIZACJACZUJKI_H
#define OKNOSTABILIZACJACZUJKI_H

#include <QDialog>
#include <QTimer>
#include <QCloseEvent>
namespace Ui {
class OknoStabilizacjaCzujki;
}

class OknoStabilizacjaCzujki : public QDialog
{
    Q_OBJECT

public:
    explicit OknoStabilizacjaCzujki(bool powerON, bool resetPower, bool ignoreAlarms, unsigned long timeWait,
                                    const QString & name, const QString & podTitle,
                                    QWidget *parent = 0);
    ~OknoStabilizacjaCzujki();
    void czujkaOn();
    void closeEvent(QCloseEvent *event);
    bool getBreakBadanie() const;

protected:
    QString getMM_SS(unsigned long secs);
private slots:
    void timeout();
private:
    Ui::OknoStabilizacjaCzujki *ui;
    QTimer timer;
    unsigned long elapsedTime;
    bool ignoreAlarms;
    bool breakBadanie;
};

#endif // OKNOSTABILIZACJACZUJKI_H
