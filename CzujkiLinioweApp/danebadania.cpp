#include "danebadania.h"
#include <QFile>

#include <QDebug>

DaneBadania::DaneBadania()
    : QObject(nullptr),
      DaneBadaniaGen()
{
    setCzasStabilizacjiCzujki_s(900);
    setCzasPomZmianaTlumenia_s(15);
    setNapiecieZasCzujki_mV(24000);
    setPrzekroczeniePraduZasilania_mA("50");
    setZasCzujekWbudZasilacz(true);
    setWyzwalanieAlarmuPrzekaznik(true);
    setMaksKatowaNieWspolPionPierwszejCzuj(0.0);
    setMaksKatowaNieWspolPionDrugiejCzuj(0.0);
    setMaksKatowaNieWspolPozPierwszejCzuj(0.0);
    setMaksKatowaNieWspolPozDrugiejCzuj(0.0);
    setSystemOdbiornikNadajnik(true);
    QStringList nazwyTestow;
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
                << "Odporność na uderzenie";
    for (int p = 0 ; p < nazwyTestow.size(); ++p)
    {
        Test t;
        t.setId(p);
        t.setName(nazwyTestow.at(p));
    }

}

void DaneBadania::load(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open bin file for reading";
        return;
    }

    QDataStream in(&file);
    in >> *this;
    file.close();
}

void DaneBadania::save(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open bin file for reading";
        return;
    }

    QDataStream out(&file);
    out << *this;
    file.close();
}

void DaneBadania::addNumberCzujki(const QString &first, const QString &second)
{
    numbersCzujki.push_back(qMakePair(first, second));
}

QString DaneBadania::getNumberFirstCzujkiNominal(unsigned int index)
{
    if (index >= numbersCzujki.size())
        return QString();
    return numbersCzujki[index].first;
}

QString DaneBadania::getNumberSecondCzujkiNominal(unsigned int index)
{
    if (index >= numbersCzujki.size())
        return QString();
    return numbersCzujki[index].second;
}

QPair<QString, QString> DaneBadania::getNumberCzujkiNominal(unsigned int index)
{
    if (index >= numbersCzujki.size())
        return qMakePair(QString(), QString());
    return numbersCzujki[index];
}

QString DaneBadania::getNumberFirstCzujkiSorted(unsigned int index)
{
    if (index >= sortedId.size())
        return QString();
    return getNumberFirstCzujkiNominal(sortedId[index]);
}

QString DaneBadania::getNumberSecondCzujkiSorted(unsigned int index)
{
    if (index >= sortedId.size())
        return QString();
    return getNumberSecondCzujkiNominal(sortedId[index]);
}

QPair<QString, QString> DaneBadania::getNumberCzujkiSorted(unsigned int index)
{
    if (index >= sortedId.size())
        return qMakePair(QString(), QString());
    return getNumberCzujkiNominal(sortedId[index]);
}

void DaneBadania::clearNumberCzujki()
{
    numbersCzujki.clear();
}

QDataStream &operator<<(QDataStream &out, const DaneBadania &dane)
{
    dane.DaneBadaniaGen::save(out);
    out << dane.numbersCzujki;
    out << dane.sortedId;
    out << dane.tests;
    return out;
}

QDataStream &operator>>(QDataStream &in, DaneBadania &dane)
{
    dane.DaneBadaniaGen::load(in);
    in >> dane.numbersCzujki;
    in >> dane.sortedId;
    in >> dane.tests;
    return in;
}

/****************/
Test::Test()
    : id(-1),
      name("Brak testów")

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
