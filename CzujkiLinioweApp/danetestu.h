#ifndef DANETESTU_H
#define DANETESTU_H

#include "ustawienia.h"
#include <QObject>
#include <QSet>

struct NiewspolosiowoscOs {
    QString poziomo;
    QString pionowo;
};

struct NiewspolosiowoscOsUrzadzenie {
    NiewspolosiowoscOs odbiornik;
    NiewspolosiowoscOs nadajnik;
};


typedef enum _testId {
    REPRODUCIBILITY = 0,
    REPEATABILITY,
    TOLERANCE_TO_BEAM_MISALIGNMENT,
    RAPID_CHANGES_IN_ATTENUATION,
    OPTICAL_PATH_LENGTH_DEPEDENCE,
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
    short nrCzujki;
    QString value_dB;
    bool ok;
    QString error;
};

struct PomiarKata {
    QString nazwaBadania ; //ktora os, nadajnik/odbiornik
    bool ok;
    QString errorStr;
    QString errorDetail;
    double katZmierzony;
    double katProducenta;
};

QDataStream &operator<<(QDataStream &out, const DanePomiaru &dane);
QDataStream &operator>>(QDataStream &in, DanePomiaru &dane);
QDataStream &operator<<(QDataStream &out, const PomiarKata &dane);
QDataStream &operator>>(QDataStream &in, PomiarKata &dane);

//QDataStream &operator<<(QDataStream &out, const QList<DanePomiaru> &dane);
//QDataStream &operator>>(QDataStream &in, QList<DanePomiaru> &dane);

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

    QString getNumerNadajnika() const;
    QString getNumerOdbiornika() const;

    void addWybranaCzujka(short nrCzujki, const QString & nadajnik, const QString & odbiornik);
    bool sprawdzCzyBadanaCzujka(const QString & odbiornik, const QString & nadajnik);

    void setSuccessBadaniaCzujki(bool ok, const QString & value, const QString &error);
    const QList<DanePomiaru> &getDaneBadanCzujek() const;
    QList<DanePomiaru> &getDanePomiarowe() { return danePomiarowe; }

    QString getNumerCzujki() const;

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

    const QString &getNazwaNumerPierwszego() const;
    void setNazwaPierwszego(const QString &newNazwaPierwszego);

    const QString &getNazwaNumerDrugiego() const;
    void setNazwaDrugiego(const QString &newNazwaDrugiego);

    int getDlugoscFali() const;
    void setDlugoscFali(int newDlugoscFali);

    unsigned int getCzasPowtarzalnosci() const;
    void setCzasPowtarzalnosci(unsigned int newCzasPowtarzalnosci);

    void addNextPomiar();
    void removeLastPomiar();

    const NiewspolosiowoscOsUrzadzenie &getKatyProducenta() const;
    void setKatyProducenta(const NiewspolosiowoscOsUrzadzenie &newKatyProducenta);

    const QString &getNazwaTypPierwszego() const;
    void setNazwaTypPierwszego(const QString &newNazwaTypPierwszego);

    const QString &getNazwaTypDrugiego() const;
    void setNazwaTypDrugiego(const QString &newNazwaTypDrugiego);

    float getCmaxCmin() const;
    void setCmaxCmin(float newCmaxCmin);

    const QVector<PomiarKata> &getPomiaryKatow() const;
    void setPomiaryKatow(const QVector<PomiarKata> &newPomiaryKatow);

    const QString &getMinimalneRozstawienie() const;
    void setMinimalneRozstawienie(const QString &newMinimalneRozstawienie);

    const QString &getMaksymalneRozstawienie() const;
    void setMaksymalneRozstawienie(const QString &newMaksymalneRozstawienie);


    void dodajPomiarKata(const PomiarKata &kat);
    void setDataZakonczenia();
    void obliczZaleznoscKatowa(const Ustawienia & ust);
    void obliczOdtwarzalnosc(const Ustawienia & ust);
    void obliczPowtarzalnosc(const Ustawienia & ust);
    void obliczDlugoscOptyczna(const Ustawienia & ust);

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
    QList<DanePomiaru> danePomiarowe;
    float Crep;
    float Cmin;
    float Cmax;
    float CmaxCrep;
    float CrepCmin;
    float CmaxCmin;
    bool ok;
    QString errStr;
    QString nazwaNumerPierwszego;
    QString nazwaNumerDrugiego;
    QString nazwaTypPierwszego;
    QString nazwaTypDrugiego;

    int dlugoscFali;
    unsigned int czasPowtarzalnosci;

    NiewspolosiowoscOsUrzadzenie katyProducenta;
    QVector<PomiarKata> pomiaryKatow;

    QString minimalneRozstawienie;
    QString maksymalneRozstawienie;

};

#endif // DANETESTU_H
