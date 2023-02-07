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

    bool dodajCzujki(const QString &nadajnik, const QString &odbiornik);
    QString getNumerTransmitter(unsigned int index) const;
    QString getNumerReceiver(unsigned int index) const;
    QString getNumerCzujki(const QString &nadajnik, const QString &odbiornik) const;

    inline  QPair<QString, QString> getNumeryCzujki(unsigned int index) const
    { return qMakePair(getNumerTransmitter(index), getNumerReceiver(index)); }

    void wyczyscCzujki();
    const QMap<int, DaneTestu> &getTesty() const;
    void setDaneTestu(short id, const DaneTestu & dane);

    DanePomiaru getDaneDlaCzujki(const QString &nadajnik, const QString &odbiornik) const;
    short getNumerPozycji(short posortowane);

    short getNumerSortedCzujki(short sortId) const;
    QString getNumerSortedCzujki(const QString &nadajnik, const QString &odbiornik) const;
    short getSortedId(short index) const;

    short getIloscCzujek() const { return posortowaneCzujki.size(); }
    inline const QList<DanePomiaru> &getDaneBadanCzujek() const { return testy[REPRODUCIBILITY].getDaneBadanCzujek(); }

    void setPosortowaneCzujki(const QVector<QPair<QString, QString>> & posortowaneCzujki);

    inline unsigned short getIloscWszystkichCzujek() const { return wszystkieCzujki.size(); };

    QVector<QPair<QString, QString> > getWszystkieCzujki() const;

private:
    QVector<QPair<QString, QString>> wszystkieCzujki;
    QVector<QPair<QString, QString>> posortowaneCzujki;
    QMap<int, DaneTestu> testy;

};

#endif // PARAMETRYBADANIA_H
