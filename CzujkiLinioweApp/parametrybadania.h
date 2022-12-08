#ifndef PARAMETRYBADANIA_H
#define PARAMETRYBADANIA_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <QString>
#include "parametrybadania_gen.h"


class ParametryBadania : public QObject, public ParametryBadaniaGen
{
    Q_OBJECT

public:
    explicit ParametryBadania();
    ParametryBadania(const ParametryBadania & e);
    ParametryBadania & operator=(const ParametryBadania & e);


    friend QDataStream &operator<<(QDataStream &out, const ParametryBadania &dane);
    friend QDataStream &operator>>(QDataStream &in, ParametryBadania &dane);

    void load(const QString & fileName);
    void save(const QString & fileName);

    void addNumeryCzujki(const QString & first, const QString & second);
    QString getNumerPierwszyCzujkiNominal(unsigned int index) const;
    QString getNumerDrugiCzujkiNominal(unsigned int index) const;
    QPair<QString, QString> getNumeryCzujkiNominal(unsigned int index) const;

    QString getNumerPierwszyCzujkiSorted(unsigned int index) const;
    QString getNumerDrugiCzujkiSorted(unsigned int index) const;
    QPair<QString, QString> getNumeryCzujkiSorted(unsigned int index) const;

    QString getNumerPierwszyCzujki(unsigned int index) const;
    QString getNumerDrugiCzujki(unsigned int index) const;
    QPair<QString, QString> getNumeryCzujki(unsigned int index) const;


    void clearNumberCzujki();


    bool getOdtwarzalnosc() const;
    void setOdtwarzalnosc(bool newOdtwarzalnosc);

private:
    bool odtwarzalnosc;
    QVector<QPair<QString, QString> > numbersCzujki;
    QVector<short> sortedId;

};

#endif // PARAMETRYBADANIA_H
