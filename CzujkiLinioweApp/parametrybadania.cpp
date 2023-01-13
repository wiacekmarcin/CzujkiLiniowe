#include "parametrybadania.h"
#include <QFile>

#include <QDebug>

ParametryBadania::ParametryBadania()
    : ParametryBadaniaGen()
{
    setCzasStabilizacjiCzujki_s(900);
    setCzasPomZmianaTlumenia_s(15);
    setCzasStabilizacjiCzujki_s(60);
    setNapiecieZasilaniaCzujki_mV(24000);
    setPrzekroczeniePraduZasilania_mA("50");
    setZasilanieCzujekZasilaczZewnetrzny(true);
    setWyzwalanieAlarmuPrzekaznikiem(true);
    setMaksKatowaNieWspolPionowaNadajnika("0.0");
    setMaksKatowaNieWspolPionowaOdbiornika("0.0");
    setMaksKatowaNieWspolPoziomaNadajnika("0.0");
    setMaksKatowaNieWspolPionowaOdbiornika("0.0");
    setSystemOdbiornikNadajnik(true);
    setTestOdtwarzalnosci(false);
    setNazwaPierwszego("Nadajnik");
    setNazwaDrugiego("Odbiornik");

    setNumerZlecenia("-");
    setNumerTestu("-");
    setOsobaOdpowiedzialna("-");
    setUwagi("");
    setHaslo("");
    setZasilanieCzujekCentrala(false);
    setZasilanieCzujekTypCentrali("-");

    setWyzwalanieAlarmuPradem(false);
    setDlugoscFaliFiltrow(880);
    setSystemOdbiornikNadajnik(true);
    setProducentCzujki("-");
    setTypPierwszejCzujki("-");
    setTypDrugiejCzujki("-");
    setRozstawienieMinCzujki(0);
    setRozstawienieMaxCzujki(0);
    setIloscCzujek(0);
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

bool ParametryBadania::getTestOdtwarzalnosci() const
{
    return testOdtwarzalnosci;
}

void ParametryBadania::setTestOdtwarzalnosci(bool newTestOdtwarzalnossci)
{
    testOdtwarzalnosci = newTestOdtwarzalnossci;
}

void ParametryBadania::addTest(short testId)
{
    ListaTestow lt;
    DaneTestu test;
    test.setId(testId);
    test.setWykonany(false);
    test.setName(lt.nazwyTestow.at(testId));
    testy.append(test);
}

const QVector<DaneTestu> &ParametryBadania::getTesty() const
{
    return testy;
}

const QString &ParametryBadania::getNazwaPierwszego() const
{
    return nazwaPierwszego;
}

void ParametryBadania::setNazwaPierwszego(const QString &newNazwaPierwszego)
{
    nazwaPierwszego = newNazwaPierwszego;
}

const QString &ParametryBadania::getNazwaDrugiego() const
{
    return nazwaDrugiego;
}

void ParametryBadania::setNazwaDrugiego(const QString &newNazwaDrugiego)
{
    nazwaDrugiego = newNazwaDrugiego;
}

void ParametryBadania::setDaneTestu(short id, const DaneTestu &dane)
{
    if (id == REPRODUCIBILITY)
        setTestOdtwarzalnosci(true);
    testy[id] = dane;
}

QDataStream &operator<<(QDataStream &out, const ParametryBadania &dane)
{
    dane.ParametryBadaniaGen::save(out);
    out << dane.testOdtwarzalnosci;
    out << dane.numbersCzujki;
    out << dane.sortedId;
    out << dane.testy;
    out << dane.nazwaPierwszego;
    out << dane.nazwaDrugiego;
    return out;
}

QDataStream &operator>>(QDataStream &in, ParametryBadania &dane)
{
    dane.ParametryBadaniaGen::load(in);
    in >> dane.testOdtwarzalnosci;
    in >> dane.numbersCzujki;
    in >> dane.sortedId;
    in >> dane.testy;
    in >> dane.nazwaPierwszego;
    in >> dane.nazwaDrugiego;
    return in;
}


