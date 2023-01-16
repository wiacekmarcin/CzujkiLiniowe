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

    DaneTestu odtwarzalnosc;
    odtwarzalnosc.setId(REPRODUCIBILITY);
    odtwarzalnosc.setName("Odtwarzalność");
    testy[REPRODUCIBILITY] = odtwarzalnosc;

    DaneTestu powtarzalnosc;
    powtarzalnosc.setId(REPEATABILITY);
    powtarzalnosc.setName("Powtarzalność");
    testy[REPEATABILITY] = powtarzalnosc;

    DaneTestu zaleznosckierunkowa;
    zaleznosckierunkowa.setId(REPEATABILITY);
    zaleznosckierunkowa.setName("Zależność kierunkowa");
    testy[TOLERANCE_TO_BEAM_MISALIGNMENT] = zaleznosckierunkowa;



}

ParametryBadania::ParametryBadania(const ParametryBadania &e):
    ParametryBadaniaGen(e)
{
    this->numbersCzujki.clear();
    this->numbersCzujki = e.numbersCzujki;
    this->sortedId.clear();
    this->sortedId = e.sortedId;
    this->testy.clear();
    this->testy = e.testy;
    this->nazwaNumerPierwszego = e.nazwaNumerPierwszego;
    this->nazwaNumerDrugiego = e.nazwaNumerDrugiego;
    this->nazwaPierwszy = e.nazwaPierwszy;
    this->nazwaDrugi = e.nazwaDrugi;
};

ParametryBadania &ParametryBadania::operator=(const ParametryBadania &e)
{
    ParametryBadaniaGen::operator=(e);
    this->numbersCzujki.clear();
    this->numbersCzujki = e.numbersCzujki;
    this->sortedId.clear();
    this->sortedId = e.sortedId;
    this->testy.clear();
    this->testy = e.testy;
    this->nazwaNumerPierwszego = e.nazwaNumerPierwszego;
    this->nazwaNumerDrugiego = e.nazwaNumerDrugiego;
    this->nazwaPierwszy = e.nazwaPierwszy;
    this->nazwaDrugi = e.nazwaDrugi;
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

const QMap<int, DaneTestu> &ParametryBadania::getTesty() const
{
    return testy;
}

const QString &ParametryBadania::getNazwaNumerPierwszego() const
{
    return nazwaNumerPierwszego;
}

void ParametryBadania::setNazwaPierwszego(const QString &newNazwaPierwszego)
{
    nazwaNumerPierwszego = newNazwaPierwszego;
}

const QString &ParametryBadania::getNazwaNumerDrugiego() const
{
    return nazwaNumerDrugiego;
}

void ParametryBadania::setNazwaDrugiego(const QString &newNazwaDrugiego)
{
    nazwaNumerDrugiego = newNazwaDrugiego;
}

void ParametryBadania::setDaneTestu(short id, const DaneTestu &dane)
{
    if (id == REPRODUCIBILITY)
        setTestOdtwarzalnosci(true);
    testy[id] = dane;
}

void ParametryBadania::posortuj()
{
    short tmp[7] = { -1, -1, -1, -1, -1, -1, -1};
    short wyk[7] = { -1, -1, -1, -1, -1, -1, -1};

    if (!testy[REPRODUCIBILITY].getWykonany())
        return;

    if (testy[REPRODUCIBILITY].getDaneBadanCzujek().size() < 2)
        return;

    auto pomiary = testy[REPRODUCIBILITY].getDaneBadanCzujek();
    float max1 = 0, max2 = 0;
    short pmax1 = -1, pmax2 = -1;
    for (int pos = 0; pos < pomiary.size(); ++pos) {

        if (pomiary[pos].value_dB.isEmpty())
            continue;
        bool ok;
        float val = pomiary[pos].value_dB.toFloat(&ok);
        if (!ok) continue;
        wyk[pos] = pomiary[pos].nrCzujki;
        if (val > max1) {
            pmax1 = pos;
            max1 = val;
        }
    }
    for (int pos = 0; pos < pomiary.size(); ++pos) {
        if (wyk[pos] == -1)
            continue;
        float val = pomiary[pos].value_dB.toFloat();

        if (pos == pmax1) continue;
        if (val > max2) {
            pmax2 = pos;
            max2 = val;
        }
    }
    for (int pos = 0; pos < pomiary.size(); ++pos) {
        if (pos == pmax1 || pos == pmax2)
            continue;
        if (wyk[pos] == -1)
        sortedId.append(wyk[pos]);
    }
    if (pmax2 != -1)
        sortedId.append(wyk[pmax2]);
    if (pmax1 != -1)
        sortedId.append(wyk[pmax1]);
}

const QString &ParametryBadania::getNazwaPierwszy() const
{
    return nazwaPierwszy;
}

void ParametryBadania::setNazwaPierwszy(const QString &newNazwaPierwszy)
{
    nazwaPierwszy = newNazwaPierwszy;
}

const QString &ParametryBadania::getNazwaDrugi() const
{
    return nazwaDrugi;
}

void ParametryBadania::setNazwaDrugi(const QString &newNazwaDrugi)
{
    nazwaDrugi = newNazwaDrugi;
}

short ParametryBadania::getSortedId(short index) const
{
    if (index >= numbersCzujki.size() )
        return -1;
    if (index >= sortedId.size())
        return -1;
    return sortedId[index];
}

QDataStream &operator<<(QDataStream &out, const ParametryBadania &dane)
{
    dane.ParametryBadaniaGen::save(out);
    out << dane.testOdtwarzalnosci;
    out << dane.numbersCzujki;
    out << dane.sortedId;
    out << dane.nazwaNumerPierwszego;
    out << dane.nazwaNumerDrugiego;
    out << dane.nazwaPierwszy;
    out << dane.nazwaDrugi;
    out << dane.testy;
    return out;
}

QDataStream &operator>>(QDataStream &in, ParametryBadania &dane)
{
    dane.ParametryBadaniaGen::load(in);
    in >> dane.testOdtwarzalnosci;
    in >> dane.numbersCzujki;
    in >> dane.sortedId;
    in >> dane.nazwaNumerPierwszego;
    in >> dane.nazwaNumerDrugiego;
    in >> dane.nazwaPierwszy;
    in >> dane.nazwaDrugi;
    in >> dane.testy;
    return in;
}


