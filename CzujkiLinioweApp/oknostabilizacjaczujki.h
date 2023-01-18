#ifndef OKNOSTABILIZACJACZUJKI_H
#define OKNOSTABILIZACJACZUJKI_H

#include <QDialog>
#include <QTimer>
namespace Ui {
class OknoStabilizacjaCzujki;
}

class OknoStabilizacjaCzujki : public QDialog
{
    Q_OBJECT

public:
    explicit OknoStabilizacjaCzujki(unsigned long timeWait, const QString &name, bool stabikizacja, QWidget *parent = 0);
    ~OknoStabilizacjaCzujki();
    void czujkaOn();
protected:
    QString getMM_SS(unsigned long secs);
private slots:
    void timeout();
private:
    Ui::OknoStabilizacjaCzujki *ui;
    QTimer timer;
    unsigned long elapsedTime;
};

#endif // OKNOSTABILIZACJACZUJKI_H
