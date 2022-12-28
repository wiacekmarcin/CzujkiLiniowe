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
    setOdtwarzalnosc(false);
}

ParametryBadania::ParametryBadania(const ParametryBadania &e):
    ParametryBadaniaGen(e)
{
    this->odtwarzalnosc = e.odtwarzalnosc;
    this->numbersCzujki.clear();
    this->numbersCzujki = e.numbersCzujki;
    this->sortedId.clear();
    this->sortedId = e.sortedId;
}

ParametryBadania &ParametryBadania::operator=(const ParametryBadania &e)
{
    ParametryBadaniaGen::operator=(e);
    this->odtwarzalnosc = e.odtwarzalnosc;
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

void ParametryBadania::addNumeryCzujki(const QString &first, const QString &second)
{
    numbersCzujki.push_back(qMakePair(first, second));
}

QString ParametryBadania::getNumerPierwszyCzujkiNominal(unsigned int index) const
{
    if (index >= numbersCzujki.size())
        return QString();
    return numbersCzujki[index].first;
}

QString ParametryBadania::getNumerDrugiCzujkiNominal(unsigned int index) const
{
    if (index >= numbersCzujki.size())
        return QString();
    return numbersCzujki[index].second;
}

QPair<QString, QString> ParametryBadania::getNumeryCzujkiNominal(unsigned int index) const
{
    if (index >= numbersCzujki.size())
        return qMakePair(QString(), QString());
    return numbersCzujki[index];
}

QString ParametryBadania::getNumerPierwszyCzujkiSorted(unsigned int index) const
{
    if (index >= sortedId.size())
        return QString();
    return getNumerPierwszyCzujkiNominal(sortedId[index]);
}

QString ParametryBadania::getNumerDrugiCzujkiSorted(unsigned int index) const
{
    if (index >= sortedId.size())
        return QString();
    return getNumerDrugiCzujkiNominal(sortedId[index]);
}

QPair<QString, QString> ParametryBadania::getNumeryCzujkiSorted(unsigned int index) const
{
    if (index >= sortedId.size())
        return qMakePair(QString(), QString());
    return getNumeryCzujkiNominal(sortedId[index]);
}

QString ParametryBadania::getNumerPierwszyCzujki(unsigned int index) const
{
    return odtwarzalnosc ? getNumerPierwszyCzujkiSorted(index) : getNumerPierwszyCzujkiNominal(index);
}

QString ParametryBadania::getNumerDrugiCzujki(unsigned int index) const
{
    return odtwarzalnosc ? getNumerDrugiCzujkiSorted(index) : getNumerDrugiCzujkiNominal(index);
}

QPair<QString, QString> ParametryBadania::getNumeryCzujki(unsigned int index) const
{
    return odtwarzalnosc ? getNumeryCzujkiSorted(index) : getNumeryCzujkiNominal(index);
}


void ParametryBadania::clearNumberCzujki()
{
    numbersCzujki.clear();
}

bool ParametryBadania::getOdtwarzalnosc() const
{
    return odtwarzalnosc;
}

void ParametryBadania::setOdtwarzalnosc(bool newOdtwarzalnosc)
{
    odtwarzalnosc = newOdtwarzalnosc;
}

QDataStream &operator<<(QDataStream &out, const ParametryBadania &dane)
{
    dane.ParametryBadaniaGen::save(out);
    out << dane.odtwarzalnosc;
    out << dane.numbersCzujki;
    out << dane.sortedId;
    return out;
}

QDataStream &operator>>(QDataStream &in, ParametryBadania &dane)
{
    dane.ParametryBadaniaGen::load(in);
    in << dane.odtwarzalnosc;
    in >> dane.numbersCzujki;
    in >> dane.sortedId;
    return in;
}


