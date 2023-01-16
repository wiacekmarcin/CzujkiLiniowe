#ifndef PARAMETRYBADANIA_H
#define PARAMETRYBADANIA_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <QString>
#include "danetestu.h"
#include "parametrybadania_gen.h"

class ParametryBadania : public ParametryBadaniaGen
{

public:
    explicit ParametryBadania();
    ParametryBadania(const ParametryBadania &e);
    ParametryBadania &operator=(const ParametryBadania &e);

    friend QDataStream &operator<<(QDataStream &out, const ParametryBadania &dane);
    friend QDataStream &operator>>(QDataStream &in, ParametryBadania &dane);

    void load(const QString &fileName);
    void save(const QString &fileName);

    void dodajCzujki(const QString &odbiornik, const QString &nadajnik);
    QString getNumerNadajnika(unsigned int index, bool sorted) const;
    QString getNumerOdbiornika(unsigned int index, bool sorted) const;
    QPair<QString, QString> getNumeryCzujki(unsigned int index, bool sorted) const
    { return qMakePair(getNumerNadajnika(index, sorted), getNumerOdbiornika(index, sorted)); }

    void wyczyscCzujki();


    bool getTestOdtwarzalnosci() const;
    void setTestOdtwarzalnosci(bool newTestOdtwarzalnossci);

    const QMap<int, DaneTestu> &getTesty() const;

    const QString &getNazwaNumerPierwszego() const;
    void setNazwaPierwszego(const QString &newNazwaPierwszego);

    const QString &getNazwaNumerDrugiego() const;
    void setNazwaDrugiego(const QString &newNazwaDrugiego);

    void setDaneTestu(short id, const DaneTestu & dane);

    void posortuj();
    const QString &getNazwaPierwszy() const;
    void setNazwaPierwszy(const QString &newNazwaPierwszy);

    const QString &getNazwaDrugi() const;
    void setNazwaDrugi(const QString &newNazwaDrugi);

    short getSortedId(short id) const;

private:
    bool testOdtwarzalnosci;
    QVector<QPair<QString, QString>> numbersCzujki;
    QVector<short> sortedId;
    QMap<int, DaneTestu> testy;
    QString nazwaNumerPierwszego;
    QString nazwaNumerDrugiego;
    QString nazwaPierwszy;
    QString nazwaDrugi;
};

#endif // PARAMETRYBADANIA_H
