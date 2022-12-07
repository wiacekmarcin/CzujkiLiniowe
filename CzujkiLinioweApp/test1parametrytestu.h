#ifndef TEST1PARAMETRYTESTU_H
#define TEST1PARAMETRYTESTU_H

#include <QDialog>
#include "danebadania.h"

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
    explicit Test1ParametryTestu(DaneBadania * badanie, short TestId, short CzujkaId, QWidget *parent = nullptr);
    ~Test1ParametryTestu();

private:
    Ui::Test1ParametryTestu *ui;
    DaneBadania * badanie;
};

#endif // TEST1PARAMETRYTESTU_H
