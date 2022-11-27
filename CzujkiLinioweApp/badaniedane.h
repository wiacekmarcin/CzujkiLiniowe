#ifndef BADANIEDANE_H
#define BADANIEDANE_H

#include <QObject>

class BadanieDane : public QObject
{
    Q_OBJECT
public:
    explicit BadanieDane(QObject *parent = nullptr);

    const QString &getNumerZlecenia() const;
    void setNumerZlecenia(const QString &newNumerZlecenia);

    const QString &getNumerTestu() const;
    void setNumerTestu(const QString &newNumerTestu);

    const QString &getUwagi() const;
    void setUwagi(const QString &newUwagi);

    const QString &getPassword() const;
    void setPassword(const QString &newPassword);

    bool getWewnetrzyZasilacz() const;
    void setWewnetrzyZasilacz(bool newWewnetrzyZasilacz);

    unsigned int getZasilanie_mV() const;
    void setZasilanie_mV(unsigned int newZasilanie_mV);

    bool getCentralaZasilanie() const;
    void setCentralaZasilanie(bool newCentralaZasilanie);

    const QString &getCentralaTyp() const;
    void setCentralaTyp(const QString &newCentralaTyp);

    unsigned int getCzasStabilizacji_ms() const;
    void setCzasStabilizacji_ms(unsigned int newCzasStabilizacji_ms);

    bool getWyzwalanieAlarmuPrzekaznik() const;
    void setWyzwalanieAlarmuPrzekaznik(bool newWyzwalanieAlarmuPrzekaznik);

    bool getWyzwalanieAlarmuPrad() const;
    void setWyzwalanieAlarmuPrad(bool newWyzwalanieAlarmuPrad);

    unsigned int getPradWyzwolenia_mA() const;
    void setPradWyzwolenia_mA(unsigned int newPradWyzwolenia_mA);

    unsigned int getDlugoscFaliFiltrow() const;
    void setDlugoscFaliFiltrow(unsigned int newDlugoscFaliFiltrow);

    unsigned int getCzasPomiedzyZmianaTlumenia_ms() const;
    void setCzasPomiedzyZmianaTlumenia_ms(unsigned int newCzasPomiedzyZmianaTlumenia_ms);

    const QString &getOsobaOdpowiedzialna() const;
    void setOsobaOdpowiedzialna(const QString &newOsobaOdpowiedzialna);

    bool getRodzajSystemuOdbiornikNadajnik() const;
    void setRodzajSystemuOdbiornikNadajnik(bool newRodzajSystemuOdbiornikNadajnik);

    bool getRodzajSystemuOdbiornikReflektor() const;
    void setRodzajSystemuOdbiornikReflektor(bool newRodzajSystemuOdbiornikReflektor);

    const QString &getProducent() const;
    void setProducent(const QString &newProducent);

    const QString &getTypNadajnika() const;
    void setTypNadajnika(const QString &newTypNadajnika);

    const QString &getTypOdbiornika() const;
    void setTypOdbiornika(const QString &newTypOdbiornika);

    long getRozstawienieMinimalne_mm() const;
    void setRozstawienieMinimalne_mm(long newRozstawienieMinimalne_mm);

    long getRozstawienieMaksymalne_mm() const;
    void setRozstawienieMaksymalne_mm(long newRozstawienieMaksymalne_mm);

    float getMaksKatNieWspolOdbiornikPion() const;
    void setMaksKatNieWspolOdbiornikPion(float newMaksKatNieWspolOdbiornikPion);

    float getMaksKatNieWspolOdbiornikPoz() const;
    void setMaksKatNieWspolOdbiornikPoz(float newMaksKatNieWspolOdbiornikPoz);

    float getMaksKatNieWspolNadajnikPion() const;
    void setMaksKatNieWspolNadajnikPion(float newMaksKatNieWspolNadajnikPion);

    float getMaksKatNieWspolNadajnikPaz() const;
    void setMaksKatNieWspolNadajnikPaz(float newMaksKatNieWspolNadajnikPaz);

    int getIloscNadajnikOdbiornik() const;
    void setIloscNadajnikOdbiornik(int newIloscNadajnikOdbiornik);

    const QString &getNumerNadajnika(const unsigned int &numer) const;
    void setNumerNadajnika(const unsigned int &numer, const QString &newNumerNadajnika);

    const QString &getNumerOdbiornika(const unsigned int & numer) const;
    void setNumerOdbiornika(const unsigned int & numer, const QString &newNumerOdbiornika);

signals:



private:
    QString numerZlecenia;
    QString numerTestu;
    QString osobaOdpowiedzialna;
    QString uwagi;
    QString password;
    bool wewnetrzyZasilacz;
    unsigned int zasilanie_mV;
    bool centralaZasilanie;
    QString centralaTyp;
    unsigned int czasStabilizacji_ms;
    bool wyzwalanieAlarmuPrzekaznik;
    bool wyzwalanieAlarmuPrad;
    unsigned int pradWyzwolenia_mA;
    unsigned int dlugoscFaliFiltrow;
    unsigned int czasPomiedzyZmianaTlumenia_ms;

    bool rodzajSystemuOdbiornikNadajnik;
    bool rodzajSystemuOdbiornikReflektor;
    QString producent;
    QString typNadajnika;
    QString typOdbiornika;
    long rozstawienieMinimalne_mm;
    long rozstawienieMaksymalne_mm;
    float maksKatNieWspolOdbiornikPion;
    float maksKatNieWspolOdbiornikPoz;
    float maksKatNieWspolNadajnikPion;
    float maksKatNieWspolNadajnikPaz;

    QString numerNadajnika[7];
    QString numerOdbiornika[7];
    int iloscNadajnikOdbiornik;
};

#endif // BADANIEDANE_H
