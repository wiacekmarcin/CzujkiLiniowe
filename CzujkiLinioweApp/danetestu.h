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
    QString numerOdbiornika;
    short nrPomiaru;
    QString value_dB;
    QString value_perc;
    bool ok;
    QString error;
};

QDataStream &operator<<(QDataStream &out, const DanePomiaru &dane);
QDataStream &operator>>(QDataStream &in, DanePomiaru &dane);
QDataStream &operator<<(QDataStream &out, const QList<DanePomiaru> &dane);
QDataStream &operator>>(QDataStream &in, QList<DanePomiaru> &dane);

class DaneTestu
{
public:
    explicit DaneTestu();
    //DaneTestu(const DaneTestu & test);
    //DaneTestu& operator=(const DaneTestu & test);

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

    const QString &getDataRozpoczecia() const;
    void setDataRozpoczecia(const QString &newRozpoczeto);

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

    void addWybranaCzujka(const QString & nadajnik, const QString & odbiornik);
    bool sprawdzCzyBadanaCzujka(const QString & odbiornik, const QString & nadajnik);

    void setSuccessBadaniaCzujki(bool ok, const QString & value, const float &valper, const QString &error);
    const QList<DanePomiaru> &getDaneBadanCzujek() const;

    float getCrep() const;
    void setCrep(float newCrep);

    float getCmin() const;
    void setCmin(float newCmin);

    float getCmax() const;
    void setCmax(float newCmax);

    float getCmaxCrep() const;
    void setCmaxCrep(float newCmaxCrep);

    float getCrepCmin() const;
    void setCrepCmin(float newCrepCmin);

    const QString &getDataZakonczenia() const;
    void setDataZakonczenia(const QString &newDataZakonczenia);

    QString getWynik() const;

    bool getOk() const;
    void setOk(bool newOk);

    QString getErrStr() const;
    void setErrStr(const QString & err);

    const QString &getNazwaPierwszego() const;
    void setNazwaPierwszego(const QString &newNazwaPierwszego);

    const QString &getNazwaDrugiego() const;
    void setNazwaDrugiego(const QString &newNazwaDrugiego);

    int getDlugoscFali() const;
    void setDlugoscFali(int newDlugoscFali);

    float getCrep2() const;
    void setCrep2(float newCrep2);

    float getCmin2() const;
    void setCmin2(float newCmin2);

    float getCmax2() const;
    void setCmax2(float newCmax2);

    unsigned int getCzasPowtarzalnosci() const;
    void setCzasPowtarzalnosci(unsigned int newCzasPowtarzalnosci);

    void addNextPomiar();

private:
    short id;
    QString name;
    bool wykonany;
    QString osobaWykonujaca;
    QString dataRozpoczecia;
    QString dataZakonczenia;
    QString temperatura;
    QString wilgotnosc;
    QString cisnienie;
    QString uwagi;
    QList<DanePomiaru> daneWybranejCzujki;
    float Crep;
    float Cmin;
    float Cmax;
    float Crep2;
    float Cmin2;
    float Cmax2;
    float CmaxCrep;
    float CrepCmin;
    bool ok;
    QString errStr;
    QString nazwaPierwszego;
    QString nazwaDrugiego;
    int dlugoscFali;
    unsigned int czasPowtarzalnosci;

};

#endif // DANETESTU_H
