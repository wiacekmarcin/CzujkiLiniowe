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

    friend QDataStream &operator<<(QDataStream &out, const ParametryBadania &dane);
    friend QDataStream &operator>>(QDataStream &in, ParametryBadania &dane);

    void load(const QString &fileName);
    void save(const QString &fileName);

    void dodajCzujki(const QString &odbiornik, const QString &nadajnik);
    QString getNumerTransmitter(unsigned int index, bool sorted) const;
    QString getNumerReceiver(unsigned int index, bool sorted) const;
    QString getNumerCzujki(const QString &nadajnik, const QString &odbiornik) const;

    QPair<QString, QString> getNumeryCzujki(unsigned int index, bool sorted) const
    { return qMakePair(getNumerTransmitter(index, sorted), getNumerReceiver(index, sorted)); }

    void wyczyscCzujki();
    const QMap<int, DaneTestu> &getTesty() const;
    void setDaneTestu(short id, const DaneTestu & dane);
    void posortuj();
    short getSortedId(short id) const;

    DanePomiaru getDaneDlaCzujki(const QString &nadajnik, const QString &odbiornik) const;

private:
    QVector<QPair<QString, QString>> numbersCzujki;
    QVector<int> sortedId;
    QMap<int, DaneTestu> testy;

};

#endif // PARAMETRYBADANIA_H
