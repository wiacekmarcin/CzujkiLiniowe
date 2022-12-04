#ifndef DANEBADANIA_H
#define DANEBADANIA_H

#include <QObject>
#include "danebadania_gen.h"

#include <QVector>
#include <QPair>
#include <QString>

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
};

#endif // DANEBADANIA_H
