#ifndef OKNOBADANIATLUMIENIA_H
#define OKNOBADANIATLUMIENIA_H

#include <QDialog>
#include <QTimer>
#include <QCloseEvent>
#include <QMutex>
#include "ustawienia.h"


#include <QDialog>
#include <QLineEdit>

class TestValueDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TestValueDialog(const QString & val, QWidget *parent = 0);

    QString value() const;

private:
    QLineEdit *m_lineEdit;
};


namespace Ui {
class OknoBadaniaTlumienia;
}

class Sterownik;
class OknoBadaniaTlumienia : public QDialog
{
    Q_OBJECT

public:
    explicit OknoBadaniaTlumienia(unsigned int czasPostojuFiltra, unsigned int dlugoscFali,
                          const QString & name, const Ustawienia &ust,
                          Sterownik * ster_, QWidget *parent = 0);
    ~OknoBadaniaTlumienia();

    void flt_zerowanieFiltrowDone();
    void flt_setUkladFiltrowDone();
    void flt_bladFiltrow(QChar silnik, bool zerowanie);

    void czujkaOn();

    bool getWynikBadania() const;

    const QString &getTlumienie() const;

    void testValue();

    const QString &getError() const;

    bool getResetFiltrowOk();
    void setResetFiltrowOk(bool newResetFiltrowOk);

    bool getBreakBadanie() const;

private slots:
    void uplynalCzasPostojuFiltra();
    void progressBarUpdate();
protected slots:
    void timeoutSterownika();
    void closeEvent(QCloseEvent *event);
private:
    Ui::OknoBadaniaTlumienia *ui;
    QTimer tmZmFiltra;
    QTimer tmZmProgressBar;
    QTimer tmSterownika;

    QList<QStringList> tlumienia;

    unsigned int actTlumPos;
    unsigned int maxTlum;
    Sterownik * ster;
    int czasPostoju;

    QString tlumienie;

    QString error;
    bool sterResponse;
    bool breakBadanie;

};

#endif // OKNOBADANIATLUMIENIA_H
