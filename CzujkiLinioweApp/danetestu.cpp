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

QDataStream &operator<<(QDataStream &out, const DanePomiaru &dane)
{
    out << dane.nrPomiaru
        << dane.numerPierwszy
        << dane.numerDrugi
        << dane.value
           ;
    return out;
}

QDataStream &operator>>(QDataStream &in, DanePomiaru &dane)
{
    in >> dane.nrPomiaru
       >> dane.numerPierwszy
       >> dane.numerDrugi
       >> dane.value
           ;
    return in;
}

QDataStream &operator<<(QDataStream &out, const QList<DanePomiaru> &dane)
{
    out << dane.size();
    for (const auto & d : dane) {
        out << d;
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, QList<DanePomiaru> &dane)
{
    dane.clear();
    unsigned int ss;
    in >> ss;
    DanePomiaru p;
    for (unsigned int s = 0; s < ss; ++s)
    {
        in >> p;
        dane.append(p);
    }
    return in;
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
        << dane.daneWybranejCzujki
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
        >> dane.daneWybranejCzujki
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

QString DaneTestu::getNumerPierwszy(short nrPomiaru) const
{
    if (nrPomiaru - 1 > daneWybranejCzujki.size())
        return QString();
    return daneWybranejCzujki[nrPomiaru-1].numerPierwszy;
}

QString DaneTestu::getNumerDrugi(short nrPomiaru) const
{
    if (nrPomiaru - 1 > daneWybranejCzujki.size())
        return QString();
    return daneWybranejCzujki[nrPomiaru-1].numerDrugi;
}

void DaneTestu::addWybranaCzujka(const QString &pierwszy, const QString &drugi)
{
    DanePomiaru nowyPomiar;
    nowyPomiar.nrPomiaru = daneWybranejCzujki.size() + 1;
    nowyPomiar.numerPierwszy = pierwszy;
    nowyPomiar.numerDrugi = drugi;
    nowyPomiar.value = 0.0;
    daneWybranejCzujki.append(nowyPomiar);
}

bool DaneTestu::sprawdzCzyBadanaCzujka(const QString &pierwszy, const QString &drugi)
{
    for(auto czujka : daneWybranejCzujki) {
        if (czujka.numerPierwszy == pierwszy && czujka.numerDrugi == drugi)
            return true;
    }
    return false;
}
