
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
		 >> zasCzujekWbudZasilacz
		 >> napiecieZasCzujki_mV
		 >> typCentraliSygnPoz
		 >> czasStabilizacjiCzujki_s
		 >> wyzwalanieAlarmuPrzekaznik
		 >> przekroczeniePraduZasilania_mA
		 >> dlugoscFaliFiltrow
		 >> czasPomZmianaTlumenia_s
		 >> systemOdbiornikNadajnik
		 >> producentCzujki
		 >> typPierwszejCzujki
		 >> typDrugiejCzujki
		 >> rozstawienieMinCzujki
		 >> rozstawienieMaxCzujki
		 >> maksKatowaNieWspolPionPierwszejCzuj
		 >> maksKatowaNieWspolPozPierwszejCzuj
		 >> maksKatowaNieWspolPionDrugiejCzuj
		 >> maksKatowaNieWspolPozDrugiejCzuj
		 >> iloscCzujek ;
}

void ParametryBadaniaGen::save(QDataStream &out) const
{
    out  << numerZlecenia
		 << numerTestu
		 << osobaOdpowiedzialna
		 << uwagi
		 << haslo
		 << zasCzujekWbudZasilacz
		 << napiecieZasCzujki_mV
		 << typCentraliSygnPoz
		 << czasStabilizacjiCzujki_s
		 << wyzwalanieAlarmuPrzekaznik
		 << przekroczeniePraduZasilania_mA
		 << dlugoscFaliFiltrow
		 << czasPomZmianaTlumenia_s
		 << systemOdbiornikNadajnik
		 << producentCzujki
		 << typPierwszejCzujki
		 << typDrugiejCzujki
		 << rozstawienieMinCzujki
		 << rozstawienieMaxCzujki
		 << maksKatowaNieWspolPionPierwszejCzuj
		 << maksKatowaNieWspolPozPierwszejCzuj
		 << maksKatowaNieWspolPionDrugiejCzuj
		 << maksKatowaNieWspolPozDrugiejCzuj
		 << iloscCzujek ;
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

bool ParametryBadaniaGen::getZasCzujekWbudZasilacz() const
{
	return zasCzujekWbudZasilacz;
}

void ParametryBadaniaGen::setZasCzujekWbudZasilacz(const bool & value)
{
	zasCzujekWbudZasilacz = value;
	change = true;
}

unsigned int ParametryBadaniaGen::getNapiecieZasCzujki_mV() const
{
	return napiecieZasCzujki_mV;
}

void ParametryBadaniaGen::setNapiecieZasCzujki_mV(const unsigned int & value)
{
	napiecieZasCzujki_mV = value;
	change = true;
}

QString ParametryBadaniaGen::getTypCentraliSygnPoz() const
{
	return typCentraliSygnPoz;
}

void ParametryBadaniaGen::setTypCentraliSygnPoz(const QString & value)
{
	typCentraliSygnPoz = value;
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

bool ParametryBadaniaGen::getWyzwalanieAlarmuPrzekaznik() const
{
	return wyzwalanieAlarmuPrzekaznik;
}

void ParametryBadaniaGen::setWyzwalanieAlarmuPrzekaznik(const bool & value)
{
	wyzwalanieAlarmuPrzekaznik = value;
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

QString ParametryBadaniaGen::getTypPierwszejCzujki() const
{
	return typPierwszejCzujki;
}

void ParametryBadaniaGen::setTypPierwszejCzujki(const QString & value)
{
	typPierwszejCzujki = value;
	change = true;
}

QString ParametryBadaniaGen::getTypDrugiejCzujki() const
{
	return typDrugiejCzujki;
}

void ParametryBadaniaGen::setTypDrugiejCzujki(const QString & value)
{
	typDrugiejCzujki = value;
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

double ParametryBadaniaGen::getMaksKatowaNieWspolPionPierwszejCzuj() const
{
	return maksKatowaNieWspolPionPierwszejCzuj;
}

void ParametryBadaniaGen::setMaksKatowaNieWspolPionPierwszejCzuj(const double & value)
{
	maksKatowaNieWspolPionPierwszejCzuj = value;
	change = true;
}

double ParametryBadaniaGen::getMaksKatowaNieWspolPozPierwszejCzuj() const
{
	return maksKatowaNieWspolPozPierwszejCzuj;
}

void ParametryBadaniaGen::setMaksKatowaNieWspolPozPierwszejCzuj(const double & value)
{
	maksKatowaNieWspolPozPierwszejCzuj = value;
	change = true;
}

double ParametryBadaniaGen::getMaksKatowaNieWspolPionDrugiejCzuj() const
{
	return maksKatowaNieWspolPionDrugiejCzuj;
}

void ParametryBadaniaGen::setMaksKatowaNieWspolPionDrugiejCzuj(const double & value)
{
	maksKatowaNieWspolPionDrugiejCzuj = value;
	change = true;
}

double ParametryBadaniaGen::getMaksKatowaNieWspolPozDrugiejCzuj() const
{
	return maksKatowaNieWspolPozDrugiejCzuj;
}

void ParametryBadaniaGen::setMaksKatowaNieWspolPozDrugiejCzuj(const double & value)
{
	maksKatowaNieWspolPozDrugiejCzuj = value;
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
