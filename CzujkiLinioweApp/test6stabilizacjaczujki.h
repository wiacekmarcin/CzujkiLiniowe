#ifndef TEST6STABILIZACJACZUJKI_H
#define TEST6STABILIZACJACZUJKI_H

#include <QDialog>
#include <QTimer>
namespace Ui {
class Test6StabilizacjaCzujki;
}

class Test6StabilizacjaCzujki : public QDialog
{
    Q_OBJECT

public:
    explicit Test6StabilizacjaCzujki(unsigned long timeWait, const QString &name, bool stabikizacja, QWidget *parent = 0);
    ~Test6StabilizacjaCzujki();
protected:
    QString getMM_SS(unsigned long secs);
private slots:
    void timeout();
private:
    Ui::Test6StabilizacjaCzujki *ui;
    QTimer timer;
    unsigned long elapsedTime;
};

#endif // TEST6STABILIZACJACZUJKI_H
