
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
	unsigned int getCzasStabilizacjiPoResecie_s() const;
	void setCzasStabilizacjiPoResecie_s(const unsigned int & czasStabilizacjiPoResecie_s);
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
	bool getTestOdtwarzalnosci() const;
	void setTestOdtwarzalnosci(const bool & testOdtwarzalnosci);
	QString getNazwaNumerTransmitter() const;
	void setNazwaNumerTransmitter(const QString & nazwaNumerTransmitter);
	QString getNazwaNumerReceiver() const;
	void setNazwaNumerReceiver(const QString & nazwaNumerReceiver);
	QString getNazwaTransmitter() const;
	void setNazwaTransmitter(const QString & nazwaTransmitter);
	QString getNazwaReceiver() const;
	void setNazwaReceiver(const QString & nazwaReceiver);
	QString getNazwaTransmitter_a() const;
	void setNazwaTransmitter_a(const QString & nazwaTransmitter_a);
	QString getNazwaReceiver_a() const;
	void setNazwaReceiver_a(const QString & nazwaReceiver_a);
	QString getTypTransmitter() const;
	void setTypTransmitter(const QString & typTransmitter);
	QString getTypReceiver() const;
	void setTypReceiver(const QString & typReceiver);
	float getOdtwarzalnoscCmaxCrep() const;
	void setOdtwarzalnoscCmaxCrep(const float & odtwarzalnoscCmaxCrep);
	float getOdtwarzalnoscCrepCmin() const;
	void setOdtwarzalnoscCrepCmin(const float & odtwarzalnoscCrepCmin);
	float getOdtwarzalnoscCmin() const;
	void setOdtwarzalnoscCmin(const float & odtwarzalnoscCmin);
	float getPowtarzalnoscCmaxCmin() const;
	void setPowtarzalnoscCmaxCmin(const float & powtarzalnoscCmaxCmin);
	float getDrogaoptycznaCmaxCmin() const;
	void setDrogaoptycznaCmaxCmin(const float & drogaoptycznaCmaxCmin);
	float getSzybkieZmianyWartoscTlumnikaA() const;
	void setSzybkieZmianyWartoscTlumnikaA(const float & szybkieZmianyWartoscTlumnikaA);
	float getSzybkieZmianyWartoscTlumnikaB() const;
	void setSzybkieZmianyWartoscTlumnikaB(const float & szybkieZmianyWartoscTlumnikaB);
	float getWartoscTlumienieDlaKataNieWspolosiowosci() const;
	void setWartoscTlumienieDlaKataNieWspolosiowosci(const float & wartoscTlumienieDlaKataNieWspolosiowosci);
	float getNiewspolosiowoscMinimalnyKatProducentMierzony() const;
	void setNiewspolosiowoscMinimalnyKatProducentMierzony(const float & niewspolosiowoscMinimalnyKatProducentMierzony);
	float getRozproszoneSwiatloCmaxCmin() const;
	void setRozproszoneSwiatloCmaxCmin(const float & rozproszoneSwiatloCmaxCmin);
	float getTolerancjaNapieciaZasilaniaCmaxCmin() const;
	void setTolerancjaNapieciaZasilaniaCmaxCmin(const float & tolerancjaNapieciaZasilaniaCmaxCmin);
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
	unsigned int czasStabilizacjiPoResecie_s;
	bool wyzwalanieAlarmuPrzekaznikiem;
	bool wyzwalanieAlarmuPradem;
	QString przekroczeniePraduZasilania_mA;
	unsigned int dlugoscFaliFiltrow;
	unsigned int czasPomZmianaTlumenia_s;
	bool systemOdbiornikNadajnik;
	QString producentCzujki;
	QString rozstawienieMinCzujki;
	QString rozstawienieMaxCzujki;
	QString maksKatowaNieWspolPionowaNadajnika;
	QString maksKatowaNieWspolPoziomaNadajnika;
	QString maksKatowaNieWspolPionowaOdbiornika;
	QString maksKatowaNieWspolPoziomaOdbiornika;
	unsigned short iloscCzujek;
	bool testOdtwarzalnosci;
	QString nazwaNumerTransmitter;
	QString nazwaNumerReceiver;
	QString nazwaTransmitter;
	QString nazwaReceiver;
	QString nazwaTransmitter_a;
	QString nazwaReceiver_a;
	QString typTransmitter;
	QString typReceiver;
	float odtwarzalnoscCmaxCrep;
	float odtwarzalnoscCrepCmin;
	float odtwarzalnoscCmin;
	float powtarzalnoscCmaxCmin;
	float drogaoptycznaCmaxCmin;
	float szybkieZmianyWartoscTlumnikaA;
	float szybkieZmianyWartoscTlumnikaB;
	float wartoscTlumienieDlaKataNieWspolosiowosci;
	float niewspolosiowoscMinimalnyKatProducentMierzony;
	float rozproszoneSwiatloCmaxCmin;
	float tolerancjaNapieciaZasilaniaCmaxCmin;
};
#endif