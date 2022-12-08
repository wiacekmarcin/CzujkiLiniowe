#ifndef DANETESTU_H
#define DANETESTU_H

#include <QObject>
#include <QSet>

struct ListaTestow {
    ListaTestow();
    QStringList nazwyTestow;
    bool odtwarzalnosc;
    QSet<short> wykonane;
    QSet<short> proccess;
    QSet<short> visible;
};

class DaneTestu : public QObject
{
    Q_OBJECT
public:
    explicit DaneTestu(QObject *parent = nullptr);

signals:

};

class Test {
public:
    Test();

    friend QDataStream &operator<<(QDataStream &out, const Test &dane);
    friend QDataStream &operator>>(QDataStream &in, Test &dane);


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


#endif // DANETESTU_H
