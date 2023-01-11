#include "parametrybadania.h"
#include <QFile>

#include <QDebug>

ParametryBadania::ParametryBadania()
    : QObject(nullptr),
      ParametryBadaniaGen()
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
    setTestOdtwarzalnosci(false);
    setTypNadajnika("Nadajnik");
    setTypOdbiornika("Odbiornik");
}

ParametryBadania::ParametryBadania(const ParametryBadania &e):
    ParametryBadaniaGen(e)
{
    this->numbersCzujki.clear();
    this->numbersCzujki = e.numbersCzujki;
    this->sortedId.clear();
    this->sortedId = e.sortedId;
}

ParametryBadania &ParametryBadania::operator=(const ParametryBadania &e)
{
    ParametryBadaniaGen::operator=(e);
    this->numbersCzujki.clear();
    this->numbersCzujki = e.numbersCzujki;
    this->sortedId.clear();
    this->sortedId = e.sortedId;
    return *this;
}


void ParametryBadania::load(const QString &fileName)
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

void ParametryBadania::save(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open bin file for saving";
        return;
    }

    QDataStream out(&file);
    out << *this;
    file.close();
}

void ParametryBadania::dodajCzujki(const QString &odbiornik, const QString &nadajnik)
{
    numbersCzujki.push_back(qMakePair(odbiornik, nadajnik));
}

QString ParametryBadania::getNumerNadajnika(unsigned int index, bool sorted) const
{
    if (!sorted && index >= numbersCzujki.size() )
        return QString();
    if (sorted && index >= sortedId.size())
        return QString();
    if (sorted)
        return getNumerNadajnika(sortedId[index], false);
    else
        return numbersCzujki[index].first;
}

QString ParametryBadania::getNumerOdbiornika(unsigned int index, bool sorted) const
{
    if (!sorted && index >= numbersCzujki.size() )
        return QString();
    if (sorted && index >= sortedId.size())
        return QString();
    if (sorted)
        return getNumerNadajnika(sortedId[index], false);
    else
        return numbersCzujki[index].second;
}

void ParametryBadania::wyczyscCzujki()
{
    numbersCzujki.clear();
}

QString ParametryBadania::getTypNadajnika() const
{
    return getTypPierwszejCzujki();
}

QString ParametryBadania::getTypOdbiornika() const
{
    return getTypDrugiejCzujki();
}

void setTypNadajnika(const QString & nadajnik)
{
    setTypPiewszejCzujki(nadajnik);
}

void setTypOdbiornika(const QString & odbiornik)
{
    setTypDrugiejCzujki(odbiornik);
}

QDataStream &operator<<(QDataStream &out, const ParametryBadania &dane)
{
    dane.ParametryBadaniaGen::save(out);
    out << dane.numbersCzujki;
    out << dane.sortedId;
    return out;
}

QDataStream &operator>>(QDataStream &in, ParametryBadania &dane)
{
    dane.ParametryBadaniaGen::load(in);
    in >> dane.numbersCzujki;
    in >> dane.sortedId;
    return in;
}


