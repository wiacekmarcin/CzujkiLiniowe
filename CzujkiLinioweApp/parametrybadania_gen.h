
#ifndef PARAMETRYBADANIAGEN_H
#define PARAMETRYBADANIAGEN_H

#include <QString>
#include <QSettings>
#include <QObject>
#include <QDataStream>

class ParametryBadaniaGen
{
public:
    ParametryBadaniaGen();
    ~ParametryBadaniaGen();

protected:
    void load(QDataStream &in);
    void save(QDataStream &out) const;
    bool change;
public:    

    friend QDataStream &operator<<(QDataStream &, const ParametryBadaniaGen &);
    friend QDataStream &operator>>(QDataStream &, ParametryBadaniaGen &);
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
	bool getZasilanieCzujekCentrala() const;
	void setZasilanieCzujekCentrala(const bool & zasilanieCzujekCentrala);
	QString getZasilanieCzujekTypCentrali() const;
	void setZasilanieCzujekTypCentrali(const QString & zasilanieCzujekTypCentrali);
	bool getZasilanieCzujekZasilaczZewnetrzny() const;
	void setZasilanieCzujekZasilaczZewnetrzny(const bool & zasilanieCzujekZasilaczZewnetrzny);
	unsigned int getNapiecieZasilaniaCzujki_mV() const;
	void setNapiecieZasilaniaCzujki_mV(const unsigned int & napiecieZasilaniaCzujki_mV);
	unsigned int getCzasStabilizacjiCzujki_s() const;
	void setCzasStabilizacjiCzujki_s(const unsigned int & czasStabilizacjiCzujki_s);
	bool getWyzwalanieAlarmuPrzekaznikiem() const;
	void setWyzwalanieAlarmuPrzekaznikiem(const bool & wyzwalanieAlarmuPrzekaznikiem);
	bool getWyzwalanieAlarmuPradem() const;
	void setWyzwalanieAlarmuPradem(const bool & wyzwalanieAlarmuPradem);
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
	QString getMaksKatowaNieWspolPionowaNadajnika() const;
	void setMaksKatowaNieWspolPionowaNadajnika(const QString & maksKatowaNieWspolPionowaNadajnika);
	QString getMaksKatowaNieWspolPoziomaNadajnika() const;
	void setMaksKatowaNieWspolPoziomaNadajnika(const QString & maksKatowaNieWspolPoziomaNadajnika);
	QString getMaksKatowaNieWspolPionowaOdbiornika() const;
	void setMaksKatowaNieWspolPionowaOdbiornika(const QString & maksKatowaNieWspolPionowaOdbiornika);
	QString getMaksKatowaNieWspolPoziomaOdbiornika() const;
	void setMaksKatowaNieWspolPoziomaOdbiornika(const QString & maksKatowaNieWspolPoziomaOdbiornika);
	unsigned short getIloscCzujek() const;
	void setIloscCzujek(const unsigned short & iloscCzujek);
protected:

private:

	QString numerZlecenia;
	QString numerTestu;
	QString osobaOdpowiedzialna;
	QString uwagi;
	QString haslo;
	bool zasilanieCzujekCentrala;
	QString zasilanieCzujekTypCentrali;
	bool zasilanieCzujekZasilaczZewnetrzny;
	unsigned int napiecieZasilaniaCzujki_mV;
	unsigned int czasStabilizacjiCzujki_s;
	bool wyzwalanieAlarmuPrzekaznikiem;
	bool wyzwalanieAlarmuPradem;
	QString przekroczeniePraduZasilania_mA;
	unsigned int dlugoscFaliFiltrow;
	unsigned int czasPomZmianaTlumenia_s;
	bool systemOdbiornikNadajnik;
	QString producentCzujki;
	QString typPierwszejCzujki;
	QString typDrugiejCzujki;
	QString rozstawienieMinCzujki;
	QString rozstawienieMaxCzujki;
	QString maksKatowaNieWspolPionowaNadajnika;
	QString maksKatowaNieWspolPoziomaNadajnika;
	QString maksKatowaNieWspolPionowaOdbiornika;
	QString maksKatowaNieWspolPoziomaOdbiornika;
	unsigned short iloscCzujek;
};
#endif