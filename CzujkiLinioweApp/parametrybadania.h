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

    QString getTypNadajnika() const;
    QString getTypOdbiornika() const;

    void setTypNadajnika(const QString &nadajnik);
    void setTypOdbiornika(const QString &odbiornik);

    void wyczyscCzujki();


    bool getTestOdtwarzalnosci() const;
    void setTestOdtwarzalnosci(bool newTestOdtwarzalnossci);

    void addTest(short testId);

    const QVector<DaneTestu> &getTesty() const;

private:
    bool testOdtwarzalnosci;
    QVector<QPair<QString, QString>> numbersCzujki;
    QVector<short> sortedId;
    QVector<DaneTestu> testy;
};

#endif // PARAMETRYBADANIA_H
