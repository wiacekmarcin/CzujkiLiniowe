#include "danetestu.h"

ListaTestow::ListaTestow()
{
    nazwyTestow << "Odtwarzalność" << "Powtarzalność" << "Zależność kierunkowa" << "Zmiana parametrów zasilania"
    << "Szybkie zmiany tłumienia" << "Powolne zmiany tłumienia" << "Zależność od długości drogi optycznej"
    << "Czułość na pożar" << "Światło rozproszone" << "Odporność na suche gorąco"
    << "Odporność na zimno" << "Odporność na wilgotne gorąco, stan ustalony"
    << "Wytrzymałość na wilgotne gorąco, stan ustalony" << "Wytrzymałość na wibracje"
    << "Odporność na wyładowania elektrostatyczne"
    << "Odporność na wypromieniowanie pola elektromagnetycznego"
    << "Odporność na przewodzone zakłocenia indukowane przez pola elektromagnetyczne"
    << "Odporność na szybkie impulsy przejściowe"
    << "Odporność na powolne udary napęciowe wysokiej energii"
    << "Wytrzymałość na korozję powodowaną przez dwutlenek siarki"
    << "Odporność na uderzenie"
    ;
    odtwarzalnosc = false;

}



DaneTestu::DaneTestu(QObject *parent)
    : QObject{parent}
{

}

QDataStream &operator<<(QDataStream &out, const Test &dane)
{
    out << dane.id
        << dane.name
        << dane.wykonany
        << dane.osobaWykonujaca
        << dane.rozpoczeto
        << dane.temperatura
        << dane.wilgotnosc
        << dane.cisnienie
        << dane.uwagi
        << dane.numerPierwszy
        << dane.numerDrugi
           ;
    return out;
}

QDataStream &operator>>(QDataStream &in, Test &dane)
{
    in  >> dane.id
        >> dane.name
        >> dane.wykonany
        >> dane.osobaWykonujaca
        >> dane.rozpoczeto
        >> dane.temperatura
        >> dane.wilgotnosc
        >> dane.cisnienie
        >> dane.uwagi
        >> dane.numerPierwszy
        >> dane.numerDrugi
            ;
    return in;
}

short Test::getId() const
{
    return id;
}

void Test::setId(short newId)
{
    id = newId;
}

const QString &Test::getName() const
{
    return name;
}

void Test::setName(const QString &newName)
{
    name = newName;
}

bool Test::getWykonany() const
{
    return wykonany;
}

void Test::setWykonany(bool newWykonany)
{
    wykonany = newWykonany;
}

const QString &Test::getOsobaWykonujaca() const
{
    return osobaWykonujaca;
}

void Test::setOsobaWykonujaca(const QString &newOsobaWykonujaca)
{
    osobaWykonujaca = newOsobaWykonujaca;
}

const QString &Test::getRozpoczeto() const
{
    return rozpoczeto;
}

void Test::setRozpoczeto(const QString &newRozpoczeto)
{
    rozpoczeto = newRozpoczeto;
}

const QString &Test::getTemperatura() const
{
    return temperatura;
}

void Test::setTemperatura(const QString &newTemperatura)
{
    temperatura = newTemperatura;
}

const QString &Test::getWilgotnosc() const
{
    return wilgotnosc;
}

void Test::setWilgotnosc(const QString &newWilgotnosc)
{
    wilgotnosc = newWilgotnosc;
}

const QString &Test::getCisnienie() const
{
    return cisnienie;
}

void Test::setCisnienie(const QString &newCisnienie)
{
    cisnienie = newCisnienie;
}

const QString &Test::getUwagi() const
{
    return uwagi;
}

void Test::setUwagi(const QString &newUwagi)
{
    uwagi = newUwagi;
}

const QString &Test::getNumerPierwszy() const
{
    return numerPierwszy;
}

void Test::setNumerPierwszy(const QString &newNumerPierwszy)
{
    numerPierwszy = newNumerPierwszy;
}

const QString &Test::getNumerDrugi() const
{
    return numerDrugi;
}

void Test::setNumerDrugi(const QString &newNumerDrugi)
{
    numerDrugi = newNumerDrugi;
}
