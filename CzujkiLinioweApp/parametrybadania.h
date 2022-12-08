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

    friend QDataStream &operator<<(QDataStream &out, const ParametryBadania &dane);
    friend QDataStream &operator>>(QDataStream &in, ParametryBadania &dane);

    void load(const QString & fileName);
    void save(const QString & fileName);

    void addNumberCzujki(const QString & first, const QString & second);
    QString getNumberFirstCzujkiNominal(unsigned int index);
    QString getNumberSecondCzujkiNominal(unsigned int index);
    QPair<QString, QString> getNumberCzujkiNominal(unsigned int index);

    QString getNumberFirstCzujkiSorted(unsigned int index);
    QString getNumberSecondCzujkiSorted(unsigned int index);
    QPair<QString, QString> getNumberCzujkiSorted(unsigned int index);

    QString getNumberFirst(unsigned int index);
    QString getNumberSecondCzujki(unsigned int index);
    QPair<QString, QString> getNumberCzujki(unsigned int index);


    void clearNumberCzujki();


    bool getOdtwarzalnosc() const;
    void setOdtwarzalnosc(bool newOdtwarzalnosc);

private:
    bool odtwarzalnosc;
    QVector<QPair<QString, QString> > numbersCzujki;
    QVector<short> sortedId;

};

#endif // PARAMETRYBADANIA_H
