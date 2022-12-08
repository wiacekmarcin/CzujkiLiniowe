#ifndef TEST1PARAMETRYTESTU_H
#define TEST1PARAMETRYTESTU_H

#include <QDialog>
#include "parametrybadania.h"
#include "danetestu.h"


//TODO
//Osoba odpowiedzialna za test to bedzie ta samo co w parametrrach testu, bez mozliwosci edycji

//1, w momencie uruchomienia badanie - sprawdzic czy jest polaczone stanowisko
//1a. Jezeli nie - odpowiednie okno
//2. jezeli tak to reset urzadzenia
//3. sprawdzic bledy bazowania (resetu) dla wszystkich silnik
//a. - reset dla katowych to pozycja srodkowa pomiedzy krancowkami
//b. - reset dla filtrow to pozycja zero - (base)
//c . - reset dla wozek filtrow to pozycja przy jednej krancow (dolna  i przy nadajniku)
//sprawdzic pozycje skrajne w przypadku bledu wyswietlic odpowiednie okno



namespace Ui {
class Test1ParametryTestu;
}

class Test1ParametryTestu : public QDialog
{
    Q_OBJECT

public:
    explicit Test1ParametryTestu(const ParametryBadania & badanie,
                                 DaneTestu * test, short nrTestu,
                                 QWidget *parent = nullptr);
    ~Test1ParametryTestu();
protected:
    void check();
    void pbOK_clicked();
    void pbCancel_clicked();
private slots:
    void changeCzujka(int index);
private:
    Ui::Test1ParametryTestu *ui;
    DaneTestu * test;

};

#endif // TEST1PARAMETRYTESTU_H
