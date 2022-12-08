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
        qDebug() << "Could not open bin file for reading";
        return;
    }

    QDataStream out(&file);
    out << *this;
    file.close();
}

void ParametryBadania::addNumberCzujki(const QString &first, const QString &second)
{
    numbersCzujki.push_back(qMakePair(first, second));
}

QString ParametryBadania::getNumberFirstCzujkiNominal(unsigned int index)
{
    if (index >= numbersCzujki.size())
        return QString();
    return numbersCzujki[index].first;
}

QString ParametryBadania::getNumberSecondCzujkiNominal(unsigned int index)
{
    if (index >= numbersCzujki.size())
        return QString();
    return numbersCzujki[index].second;
}

QPair<QString, QString> ParametryBadania::getNumberCzujkiNominal(unsigned int index)
{
    if (index >= numbersCzujki.size())
        return qMakePair(QString(), QString());
    return numbersCzujki[index];
}

QString ParametryBadania::getNumberFirstCzujkiSorted(unsigned int index)
{
    if (index >= sortedId.size())
        return QString();
    return getNumberFirstCzujkiNominal(sortedId[index]);
}

QString ParametryBadania::getNumberSecondCzujkiSorted(unsigned int index)
{
    if (index >= sortedId.size())
        return QString();
    return getNumberSecondCzujkiNominal(sortedId[index]);
}

QPair<QString, QString> ParametryBadania::getNumberCzujkiSorted(unsigned int index)
{
    if (index >= sortedId.size())
        return qMakePair(QString(), QString());
    return getNumberCzujkiNominal(sortedId[index]);
}

QString ParametryBadania::getNumberFirst(unsigned int index)
{
    return odtwarzalnosc ? getNumberFirstCzujkiSorted(index) : getNumberFirstCzujkiSorted(index);
}

QString ParametryBadania::getNumberSecondCzujki(unsigned int index)
{
    return odtwarzalnosc ? getNumberSecondCzujkiSorted(index) : getNumberSecondCzujkiSorted(index);
}

QPair<QString, QString> ParametryBadania::getNumberCzujki(unsigned int index)
{
    return odtwarzalnosc ? getNumberCzujkiSorted(index) : getNumberCzujkiSorted(index);
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


