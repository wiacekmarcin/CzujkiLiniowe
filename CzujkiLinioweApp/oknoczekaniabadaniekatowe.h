#ifndef OKNOCZEKANIABADANIEKATOWE_H
#define OKNOCZEKANIABADANIEKATOWE_H

#include <QDialog>
#include <QTimer>
#include <QCloseEvent>
namespace Ui {
class OknoCzekaniaBadanieKatowe;
}

class OknoCzekaniaBadanieKatowe : public QDialog
{
    Q_OBJECT

public:
    explicit OknoCzekaniaBadanieKatowe(unsigned long timeWait, const QString &name, const QString &ptitle, QWidget *parent = 0);
    ~OknoCzekaniaBadanieKatowe();
    void czujkaOn();
protected:
    QString getMM_SS(unsigned long secs);
private slots:
    void timeout();
    void closeEvent(QCloseEvent *event);
private:
    Ui::OknoCzekaniaBadanieKatowe *ui;
    QTimer timer;
    unsigned long elapsedTime;
};

#endif // OKNOCZEKANIABADANIEKATOWE_H


