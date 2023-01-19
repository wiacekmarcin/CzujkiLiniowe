
#include "parametrybadania_gen.h"

#include <QApplication>


QDataStream &operator<<(QDataStream &out, const ParametryBadaniaGen &d)
{
    QDataStream::FloatingPointPrecision prev = out.floatingPointPrecision();
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    d.save(out);
    out.setFloatingPointPrecision(prev);
    return out;
}

QDataStream &operator>>(QDataStream &in, ParametryBadaniaGen &d)
{
    QDataStream::FloatingPointPrecision prev = in.floatingPointPrecision();
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    d.load(in);
    in.setFloatingPointPrecision(prev);
    return in;
}

ParametryBadaniaGen::ParametryBadaniaGen() 
{
    change = false;
}

ParametryBadaniaGen::~ParametryBadaniaGen()
{
    
}
        

void ParametryBadaniaGen::load(QDataStream &in)
{
    in   >> numerZlecenia
		 >> numerTestu
		 >> osobaOdpowiedzialna
		 >> uwagi
		 >> haslo
		 >> zasilanieCzujekCentrala
		 >> zasilanieCzujekTypCentrali
		 >> zasilanieCzujekZasilaczZewnetrzny
		 >> napiecieZasilaniaCzujki_mV
		 >> czasStabilizacjiCzujki_s
		 >> czasStabilizacjiPoResecie_s
		 >> wyzwalanieAlarmuPrzekaznikiem
		 >> wyzwalanieAlarmuPradem
		 >> przekroczeniePraduZasilania_mA
		 >> dlugoscFaliFiltrow
		 >> czasPomZmianaTlumenia_s
		 >> systemOdbiornikNadajnik
		 >> producentCzujki
		 >> rozstawienieMinCzujki
		 >> rozstawienieMaxCzujki
		 >> maksKatowaNieWspolPionowaNadajnika
		 >> maksKatowaNieWspolPoziomaNadajnika
		 >> maksKatowaNieWspolPionowaOdbiornika
		 >> maksKatowaNieWspolPoziomaOdbiornika
		 >> iloscCzujek
		 >> testOdtwarzalnosci
		 >> nazwaNumerTransmitter
		 >> nazwaNumerReceiver
		 >> nazwaTransmitter
		 >> nazwaReceiver
		 >> nazwaTransmitter_a
		 >> nazwaReceiver_a
		 >> typTransmitter
		 >> typReceiver
		 >> odtwarzalnoscCmaxCrep
		 >> odtwarzalnoscCrepCmin
		 >> odtwarzalnoscCmin
		 >> powtarzalnoscCmaxCmin ;
}

void ParametryBadaniaGen::save(QDataStream &out) const
{
    out  << numerZlecenia
		 << numerTestu
		 << osobaOdpowiedzialna
		 << uwagi
		 << haslo
		 << zasilanieCzujekCentrala
		 << zasilanieCzujekTypCentrali
		 << zasilanieCzujekZasilaczZewnetrzny
		 << napiecieZasilaniaCzujki_mV
		 << czasStabilizacjiCzujki_s
		 << czasStabilizacjiPoResecie_s
		 << wyzwalanieAlarmuPrzekaznikiem
		 << wyzwalanieAlarmuPradem
		 << przekroczeniePraduZasilania_mA
		 << dlugoscFaliFiltrow
		 << czasPomZmianaTlumenia_s
		 << systemOdbiornikNadajnik
		 << producentCzujki
		 << rozstawienieMinCzujki
		 << rozstawienieMaxCzujki
		 << maksKatowaNieWspolPionowaNadajnika
		 << maksKatowaNieWspolPoziomaNadajnika
		 << maksKatowaNieWspolPionowaOdbiornika
		 << maksKatowaNieWspolPoziomaOdbiornika
		 << iloscCzujek
		 << testOdtwarzalnosci
		 << nazwaNumerTransmitter
		 << nazwaNumerReceiver
		 << nazwaTransmitter
		 << nazwaReceiver
		 << nazwaTransmitter_a
		 << nazwaReceiver_a
		 << typTransmitter
		 << typReceiver
		 << odtwarzalnoscCmaxCrep
		 << odtwarzalnoscCrepCmin
		 << odtwarzalnoscCmin
		 << powtarzalnoscCmaxCmin ;
}
QString ParametryBadaniaGen::getNumerZlecenia() const
{
	return numerZlecenia;
}

void ParametryBadaniaGen::setNumerZlecenia(const QString & value)
{
	numerZlecenia = value;
	change = true;
}

QString ParametryBadaniaGen::getNumerTestu() const
{
	return numerTestu;
}

void ParametryBadaniaGen::setNumerTestu(const QString & value)
{
	numerTestu = value;
	change = true;
}

QString ParametryBadaniaGen::getOsobaOdpowiedzialna() const
{
	return osobaOdpowiedzialna;
}

void ParametryBadaniaGen::setOsobaOdpowiedzialna(const QString & value)
{
	osobaOdpowiedzialna = value;
	change = true;
}

QString ParametryBadaniaGen::getUwagi() const
{
	return uwagi;
}

void ParametryBadaniaGen::setUwagi(const QString & value)
{
	uwagi = value;
	change = true;
}

QString ParametryBadaniaGen::getHaslo() const
{
	return haslo;
}

void ParametryBadaniaGen::setHaslo(const QString & value)
{
	haslo = value;
	change = true;
}

bool ParametryBadaniaGen::getZasilanieCzujekCentrala() const
{
	return zasilanieCzujekCentrala;
}

void ParametryBadaniaGen::setZasilanieCzujekCentrala(const bool & value)
{
	zasilanieCzujekCentrala = value;
	change = true;
}

QString ParametryBadaniaGen::getZasilanieCzujekTypCentrali() const
{
	return zasilanieCzujekTypCentrali;
}

void ParametryBadaniaGen::setZasilanieCzujekTypCentrali(const QString & value)
{
	zasilanieCzujekTypCentrali = value;
	change = true;
}

bool ParametryBadaniaGen::getZasilanieCzujekZasilaczZewnetrzny() const
{
	return zasilanieCzujekZasilaczZewnetrzny;
}

void ParametryBadaniaGen::setZasilanieCzujekZasilaczZewnetrzny(const bool & value)
{
	zasilanieCzujekZasilaczZewnetrzny = value;
	change = true;
}

unsigned int ParametryBadaniaGen::getNapiecieZasilaniaCzujki_mV() const
{
	return napiecieZasilaniaCzujki_mV;
}

void ParametryBadaniaGen::setNapiecieZasilaniaCzujki_mV(const unsigned int & value)
{
	napiecieZasilaniaCzujki_mV = value;
	change = true;
}

unsigned int ParametryBadaniaGen::getCzasStabilizacjiCzujki_s() const
{
	return czasStabilizacjiCzujki_s;
}

void ParametryBadaniaGen::setCzasStabilizacjiCzujki_s(const unsigned int & value)
{
	czasStabilizacjiCzujki_s = value;
	change = true;
}

unsigned int ParametryBadaniaGen::getCzasStabilizacjiPoResecie_s() const
{
	return czasStabilizacjiPoResecie_s;
}

void ParametryBadaniaGen::setCzasStabilizacjiPoResecie_s(const unsigned int & value)
{
	czasStabilizacjiPoResecie_s = value;
	change = true;
}

bool ParametryBadaniaGen::getWyzwalanieAlarmuPrzekaznikiem() const
{
	return wyzwalanieAlarmuPrzekaznikiem;
}

void ParametryBadaniaGen::setWyzwalanieAlarmuPrzekaznikiem(const bool & value)
{
	wyzwalanieAlarmuPrzekaznikiem = value;
	change = true;
}

bool ParametryBadaniaGen::getWyzwalanieAlarmuPradem() const
{
	return wyzwalanieAlarmuPradem;
}

void ParametryBadaniaGen::setWyzwalanieAlarmuPradem(const bool & value)
{
	wyzwalanieAlarmuPradem = value;
	change = true;
}

QString ParametryBadaniaGen::getPrzekroczeniePraduZasilania_mA() const
{
	return przekroczeniePraduZasilania_mA;
}

void ParametryBadaniaGen::setPrzekroczeniePraduZasilania_mA(const QString & value)
{
	przekroczeniePraduZasilania_mA = value;
	change = true;
}

unsigned int ParametryBadaniaGen::getDlugoscFaliFiltrow() const
{
	return dlugoscFaliFiltrow;
}

void ParametryBadaniaGen::setDlugoscFaliFiltrow(const unsigned int & value)
{
	dlugoscFaliFiltrow = value;
	change = true;
}

unsigned int ParametryBadaniaGen::getCzasPomZmianaTlumenia_s() const
{
	return czasPomZmianaTlumenia_s;
}

void ParametryBadaniaGen::setCzasPomZmianaTlumenia_s(const unsigned int & value)
{
	czasPomZmianaTlumenia_s = value;
	change = true;
}

bool ParametryBadaniaGen::getSystemOdbiornikNadajnik() const
{
	return systemOdbiornikNadajnik;
}

void ParametryBadaniaGen::setSystemOdbiornikNadajnik(const bool & value)
{
	systemOdbiornikNadajnik = value;
	change = true;
}

QString ParametryBadaniaGen::getProducentCzujki() const
{
	return producentCzujki;
}

void ParametryBadaniaGen::setProducentCzujki(const QString & value)
{
	producentCzujki = value;
	change = true;
}

QString ParametryBadaniaGen::getRozstawienieMinCzujki() const
{
	return rozstawienieMinCzujki;
}

void ParametryBadaniaGen::setRozstawienieMinCzujki(const QString & value)
{
	rozstawienieMinCzujki = value;
	change = true;
}

QString ParametryBadaniaGen::getRozstawienieMaxCzujki() const
{
	return rozstawienieMaxCzujki;
}

void ParametryBadaniaGen::setRozstawienieMaxCzujki(const QString & value)
{
	rozstawienieMaxCzujki = value;
	change = true;
}

QString ParametryBadaniaGen::getMaksKatowaNieWspolPionowaNadajnika() const
{
	return maksKatowaNieWspolPionowaNadajnika;
}

void ParametryBadaniaGen::setMaksKatowaNieWspolPionowaNadajnika(const QString & value)
{
	maksKatowaNieWspolPionowaNadajnika = value;
	change = true;
}

QString ParametryBadaniaGen::getMaksKatowaNieWspolPoziomaNadajnika() const
{
	return maksKatowaNieWspolPoziomaNadajnika;
}

void ParametryBadaniaGen::setMaksKatowaNieWspolPoziomaNadajnika(const QString & value)
{
	maksKatowaNieWspolPoziomaNadajnika = value;
	change = true;
}

QString ParametryBadaniaGen::getMaksKatowaNieWspolPionowaOdbiornika() const
{
	return maksKatowaNieWspolPionowaOdbiornika;
}

void ParametryBadaniaGen::setMaksKatowaNieWspolPionowaOdbiornika(const QString & value)
{
	maksKatowaNieWspolPionowaOdbiornika = value;
	change = true;
}

QString ParametryBadaniaGen::getMaksKatowaNieWspolPoziomaOdbiornika() const
{
	return maksKatowaNieWspolPoziomaOdbiornika;
}

void ParametryBadaniaGen::setMaksKatowaNieWspolPoziomaOdbiornika(const QString & value)
{
	maksKatowaNieWspolPoziomaOdbiornika = value;
	change = true;
}

unsigned short ParametryBadaniaGen::getIloscCzujek() const
{
	return iloscCzujek;
}

void ParametryBadaniaGen::setIloscCzujek(const unsigned short & value)
{
	iloscCzujek = value;
	change = true;
}

bool ParametryBadaniaGen::getTestOdtwarzalnosci() const
{
	return testOdtwarzalnosci;
}

void ParametryBadaniaGen::setTestOdtwarzalnosci(const bool & value)
{
	testOdtwarzalnosci = value;
	change = true;
}

QString ParametryBadaniaGen::getNazwaNumerTransmitter() const
{
	return nazwaNumerTransmitter;
}

void ParametryBadaniaGen::setNazwaNumerTransmitter(const QString & value)
{
	nazwaNumerTransmitter = value;
	change = true;
}

QString ParametryBadaniaGen::getNazwaNumerReceiver() const
{
	return nazwaNumerReceiver;
}

void ParametryBadaniaGen::setNazwaNumerReceiver(const QString & value)
{
	nazwaNumerReceiver = value;
	change = true;
}

QString ParametryBadaniaGen::getNazwaTransmitter() const
{
	return nazwaTransmitter;
}

void ParametryBadaniaGen::setNazwaTransmitter(const QString & value)
{
	nazwaTransmitter = value;
	change = true;
}

QString ParametryBadaniaGen::getNazwaReceiver() const
{
	return nazwaReceiver;
}

void ParametryBadaniaGen::setNazwaReceiver(const QString & value)
{
	nazwaReceiver = value;
	change = true;
}

QString ParametryBadaniaGen::getNazwaTransmitter_a() const
{
	return nazwaTransmitter_a;
}

void ParametryBadaniaGen::setNazwaTransmitter_a(const QString & value)
{
	nazwaTransmitter_a = value;
	change = true;
}

QString ParametryBadaniaGen::getNazwaReceiver_a() const
{
	return nazwaReceiver_a;
}

void ParametryBadaniaGen::setNazwaReceiver_a(const QString & value)
{
	nazwaReceiver_a = value;
	change = true;
}

QString ParametryBadaniaGen::getTypTransmitter() const
{
	return typTransmitter;
}

void ParametryBadaniaGen::setTypTransmitter(const QString & value)
{
	typTransmitter = value;
	change = true;
}

QString ParametryBadaniaGen::getTypReceiver() const
{
	return typReceiver;
}

void ParametryBadaniaGen::setTypReceiver(const QString & value)
{
	typReceiver = value;
	change = true;
}

float ParametryBadaniaGen::getOdtwarzalnoscCmaxCrep() const
{
	return odtwarzalnoscCmaxCrep;
}

void ParametryBadaniaGen::setOdtwarzalnoscCmaxCrep(const float & value)
{
	odtwarzalnoscCmaxCrep = value;
	change = true;
}

float ParametryBadaniaGen::getOdtwarzalnoscCrepCmin() const
{
	return odtwarzalnoscCrepCmin;
}

void ParametryBadaniaGen::setOdtwarzalnoscCrepCmin(const float & value)
{
	odtwarzalnoscCrepCmin = value;
	change = true;
}

float ParametryBadaniaGen::getOdtwarzalnoscCmin() const
{
	return odtwarzalnoscCmin;
}

void ParametryBadaniaGen::setOdtwarzalnoscCmin(const float & value)
{
	odtwarzalnoscCmin = value;
	change = true;
}

float ParametryBadaniaGen::getPowtarzalnoscCmaxCmin() const
{
	return powtarzalnoscCmaxCmin;
}

void ParametryBadaniaGen::setPowtarzalnoscCmaxCmin(const float & value)
{
	powtarzalnoscCmaxCmin = value;
	change = true;
}
