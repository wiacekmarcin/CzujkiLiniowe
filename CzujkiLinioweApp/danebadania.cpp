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
    return out;
}

QDataStream &operator>>(QDataStream &in, DaneBadania &dane)
{
    dane.DaneBadaniaGen::load(in);
    in >> dane.numbersCzujki;
    in >> dane.sortedId;
    return in;
}

