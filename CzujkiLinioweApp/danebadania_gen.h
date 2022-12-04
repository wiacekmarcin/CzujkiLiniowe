
#ifndef DANEBADANIAGEN_H
#define DANEBADANIAGEN_H

#include <QString>
#include <QSettings>
#include <QObject>
#include <QDataStream>

class DaneBadaniaGen
{
public:
    DaneBadaniaGen();
    ~DaneBadaniaGen();

protected:
    void load(QDataStream &in);
    void save(QDataStream &out) const;
    bool change;
public:    

    friend QDataStream &operator<<(QDataStream &, const DaneBadaniaGen &);
    friend QDataStream &operator>>(QDataStream &, DaneBadaniaGen &);
/********** GET SET **********/
	QString getNumerZlecenia() const;
	void setNumerZlecenia(const QString & numerZlecenia);
	QString getNumerTestu() const;
	void setNumerTestu(const QString & numerTestu);
	QString getOsobaOdpowiedzialna() const;
	void setOsobaOdpowiedzialna(const QString & osobaOdpowiedzialna);
	QString getUwagi() const;
	void setUwagi(const QString & uwagi);
	QString getHaslo() const;
	void setHaslo(const QString & haslo);
	bool getZasCzujekWbudZasilacz() const;
	void setZasCzujekWbudZasilacz(const bool & zasCzujekWbudZasilacz);
	unsigned int getNapiecieZasCzujki_mV() const;
	void setNapiecieZasCzujki_mV(const unsigned int & napiecieZasCzujki_mV);
	QString getTypCentraliSygnPoz() const;
	void setTypCentraliSygnPoz(const QString & typCentraliSygnPoz);
	unsigned int getCzasStabilizacjiCzujki_s() const;
	void setCzasStabilizacjiCzujki_s(const unsigned int & czasStabilizacjiCzujki_s);
	bool getWyzwalanieAlarmuPrzekaznik() const;
	void setWyzwalanieAlarmuPrzekaznik(const bool & wyzwalanieAlarmuPrzekaznik);
	QString getPrzekroczeniePraduZasilania_mA() const;
	void setPrzekroczeniePraduZasilania_mA(const QString & przekroczeniePraduZasilania_mA);
	unsigned int getDlugoscFaliFiltrow() const;
	void setDlugoscFaliFiltrow(const unsigned int & dlugoscFaliFiltrow);
	unsigned int getCzasPomZmianaTlumenia_s() const;
	void setCzasPomZmianaTlumenia_s(const unsigned int & czasPomZmianaTlumenia_s);
	bool getSystemOdbiornikNadajnik() const;
	void setSystemOdbiornikNadajnik(const bool & systemOdbiornikNadajnik);
	QString getProducentCzujki() const;
	void setProducentCzujki(const QString & producentCzujki);
	QString getTypPierwszejCzujki() const;
	void setTypPierwszejCzujki(const QString & typPierwszejCzujki);
	QString getTypDrugiejCzujki() const;
	void setTypDrugiejCzujki(const QString & typDrugiejCzujki);
	QString getRozstawienieMinCzujki() const;
	void setRozstawienieMinCzujki(const QString & rozstawienieMinCzujki);
	QString getRozstawienieMaxCzujki() const;
	void setRozstawienieMaxCzujki(const QString & rozstawienieMaxCzujki);
	double getMaksKatowaNieWspolPionPierwszejCzuj() const;
	void setMaksKatowaNieWspolPionPierwszejCzuj(const double & maksKatowaNieWspolPionPierwszejCzuj);
	double getMaksKatowaNieWspolPozPierwszejCzuj() const;
	void setMaksKatowaNieWspolPozPierwszejCzuj(const double & maksKatowaNieWspolPozPierwszejCzuj);
	double getMaksKatowaNieWspolPionDrugiejCzuj() const;
	void setMaksKatowaNieWspolPionDrugiejCzuj(const double & maksKatowaNieWspolPionDrugiejCzuj);
	double getMaksKatowaNieWspolPozDrugiejCzuj() const;
	void setMaksKatowaNieWspolPozDrugiejCzuj(const double & maksKatowaNieWspolPozDrugiejCzuj);
	unsigned short getIloscCzujek() const;
	void setIloscCzujek(const unsigned short & iloscCzujek);
protected:

private:

	QString numerZlecenia;
	QString numerTestu;
	QString osobaOdpowiedzialna;
	QString uwagi;
	QString haslo;
	bool zasCzujekWbudZasilacz;
	unsigned int napiecieZasCzujki_mV;
	QString typCentraliSygnPoz;
	unsigned int czasStabilizacjiCzujki_s;
	bool wyzwalanieAlarmuPrzekaznik;
	QString przekroczeniePraduZasilania_mA;
	unsigned int dlugoscFaliFiltrow;
	unsigned int czasPomZmianaTlumenia_s;
	bool systemOdbiornikNadajnik;
	QString producentCzujki;
	QString typPierwszejCzujki;
	QString typDrugiejCzujki;
	QString rozstawienieMinCzujki;
	QString rozstawienieMaxCzujki;
	double maksKatowaNieWspolPionPierwszejCzuj;
	double maksKatowaNieWspolPozPierwszejCzuj;
	double maksKatowaNieWspolPionDrugiejCzuj;
	double maksKatowaNieWspolPozDrugiejCzuj;
	unsigned short iloscCzujek;
};
#endif