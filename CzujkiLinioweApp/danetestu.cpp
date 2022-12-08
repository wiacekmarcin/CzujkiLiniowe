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

QDataStream &operator<<(QDataStream &out, const DaneTestu &dane)
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
        << dane.numerWybranejCzujki
           ;
    return out;
}

QDataStream &operator>>(QDataStream &in, DaneTestu &dane)
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
        >> dane.numerWybranejCzujki
            ;
    return in;
}

short DaneTestu::getId() const
{
    return id;
}

void DaneTestu::setId(short newId)
{
    id = newId;
}

const QString &DaneTestu::getName() const
{
    return name;
}

void DaneTestu::setName(const QString &newName)
{
    name = newName;
}

bool DaneTestu::getWykonany() const
{
    return wykonany;
}

void DaneTestu::setWykonany(bool newWykonany)
{
    wykonany = newWykonany;
}

const QString &DaneTestu::getOsobaWykonujaca() const
{
    return osobaWykonujaca;
}

void DaneTestu::setOsobaWykonujaca(const QString &newOsobaWykonujaca)
{
    osobaWykonujaca = newOsobaWykonujaca;
}

const QString &DaneTestu::getRozpoczeto() const
{
    return rozpoczeto;
}

void DaneTestu::setRozpoczeto(const QString &newRozpoczeto)
{
    rozpoczeto = newRozpoczeto;
}

const QString &DaneTestu::getTemperatura() const
{
    return temperatura;
}

void DaneTestu::setTemperatura(const QString &newTemperatura)
{
    temperatura = newTemperatura;
}

const QString &DaneTestu::getWilgotnosc() const
{
    return wilgotnosc;
}

void DaneTestu::setWilgotnosc(const QString &newWilgotnosc)
{
    wilgotnosc = newWilgotnosc;
}

const QString &DaneTestu::getCisnienie() const
{
    return cisnienie;
}

void DaneTestu::setCisnienie(const QString &newCisnienie)
{
    cisnienie = newCisnienie;
}

const QString &DaneTestu::getUwagi() const
{
    return uwagi;
}

void DaneTestu::setUwagi(const QString &newUwagi)
{
    uwagi = newUwagi;
}

const QString &DaneTestu::getNumerPierwszy() const
{
    return numerPierwszy;
}

void DaneTestu::setNumerPierwszy(const QString &newNumerPierwszy)
{
    numerPierwszy = newNumerPierwszy;
}

const QString &DaneTestu::getNumerDrugi() const
{
    return numerDrugi;
}

void DaneTestu::setNumerDrugi(const QString &newNumerDrugi)
{
    numerDrugi = newNumerDrugi;
}

const QList<short> &DaneTestu::getNumerWybranejCzujki() const
{
    return numerWybranejCzujki;
}

void DaneTestu::setNumerWybranejCzujki(const QList<short> &newNumerWybranejCzujki)
{
    numerWybranejCzujki = newNumerWybranejCzujki;
}

void DaneTestu::addNumerWybranejCzujki(short id)
{
    numerWybranejCzujki.append(id);
}
