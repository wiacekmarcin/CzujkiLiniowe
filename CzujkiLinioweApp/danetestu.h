#ifndef DANETESTU_H
#define DANETESTU_H

#include <QObject>
#include <QSet>

typedef enum _testId {
    REPRODUCIBILITY = 0,
    REPEATABILITY,
    TOLERANCE_TO_BEAM_MISALIGNMENT
} TestIdType;

struct ListaTestow {
    ListaTestow();
    QStringList nazwyTestow;
    bool odtwarzalnosc;
    QSet<short> wykonane;
    short proccess;
    QSet<short> visible;
};

class DanePomiaru {
public:
    QString numerNadajnika;
    QString numerDrugi;
    short nrPomiaru;
    QString value;
    QString value2;
    bool ok;
    QString error;
};

QDataStream &operator<<(QDataStream &out, const DanePomiaru &dane);
QDataStream &operator>>(QDataStream &in, DanePomiaru &dane);
QDataStream &operator<<(QDataStream &out, const QList<DanePomiaru> &dane);
QDataStream &operator>>(QDataStream &in, QList<DanePomiaru> &dane);

class DaneTestu : public QObject
{
    Q_OBJECT
public:
    explicit DaneTestu(QObject *parent = nullptr);

    friend QDataStream &operator<<(QDataStream &out, const DaneTestu &dane);
    friend QDataStream &operator>>(QDataStream &in, DaneTestu &dane);

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

    QString getNumerNadajnika(short nrPomiaru) const;
    QString getNumerOdbiornika(short nrPomiaru) const;

    void addWybranaCzujka(const QString & pierwszy, const QString & drugi);
    bool sprawdzCzyBadanaCzujka(const QString & pierwszy, const QString & drugi);

    void setSuccessBadaniaCzujki(bool ok, const QString & value, const QString &error);
    const QList<DanePomiaru> &getDaneBadanCzujek() const;

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
    QList<DanePomiaru> daneWybranejCzujki;
};

#endif // DANETESTU_H
