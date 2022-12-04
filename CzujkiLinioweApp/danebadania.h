#ifndef DANEBADANIA_H
#define DANEBADANIA_H

#include <QObject>
#include "danebadania_gen.h"

#include <QVector>
#include <QPair>
#include <QString>

class Test {
    Test();

    friend QDataStream &operator<<(QDataStream &out, const Test &dane);
    friend QDataStream &operator>>(QDataStream &in, Test &dane);

public:
    short getId() const;
    void setId(short newId);

    const QString &getName() const;
    void setName(const QString &newName);

    bool getWykonany() const;
    void setWykonany(bool newWykonany);

    const QString &getOsobaWykonujaca() const;
    void setOsobaWykonujaca(const QString &newOsobaWykonujaca);

    const QString &getRozpoczeto() const;
    void setRozpoczeto(const QString &newRozpoczeto);

    const QString &getTemperatura() const;
    void setTemperatura(const QString &newTemperatura);

    const QString &getWilgotnosc() const;
    void setWilgotnosc(const QString &newWilgotnosc);

    const QString &getCisnienie() const;
    void setCisnienie(const QString &newCisnienie);

    const QString &getUwagi() const;
    void setUwagi(const QString &newUwagi);

    const QString &getNumerPierwszy() const;
    void setNumerPierwszy(const QString &newNumerPierwszy);

    const QString &getNumerDrugi() const;
    void setNumerDrugi(const QString &newNumerDrugi);

private:
    short id;
    QString name;
    bool wykonany;
    QString osobaWykonujaca;
    QString rozpoczeto;
    QString temperatura;
    QString wilgotnosc;
    QString cisnienie;
    QString uwagi;
    QString numerPierwszy;
    QString numerDrugi;
};


class DaneBadania : public QObject, public DaneBadaniaGen
{
    Q_OBJECT

public:
    explicit DaneBadania();

    friend QDataStream &operator<<(QDataStream &out, const DaneBadania &dane);
    friend QDataStream &operator>>(QDataStream &in, DaneBadania &dane);

    void load(const QString & fileName);
    void save(const QString & fileName);

    void addNumberCzujki(const QString & first, const QString & second);
    QString getNumberFirstCzujkiNominal(unsigned int index);
    QString getNumberSecondCzujkiNominal(unsigned int index);
    QPair<QString, QString> getNumberCzujkiNominal(unsigned int index);
    QString getNumberFirstCzujkiSorted(unsigned int index);
    QString getNumberSecondCzujkiSorted(unsigned int index);
    QPair<QString, QString> getNumberCzujkiSorted(unsigned int index);
    void clearNumberCzujki();
signals:


private:
    QVector<QPair<QString, QString> > numbersCzujki;
    QVector<short> sortedId;
    QVector<Test> tests;
};

#endif // DANEBADANIA_H
