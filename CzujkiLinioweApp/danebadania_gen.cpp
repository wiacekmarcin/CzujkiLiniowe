
#include "danebadania_gen.h"

#include <QApplication>


QDataStream &operator<<(QDataStream &out, const DaneBadaniaGen &d)
{
    QDataStream::FloatingPointPrecision prev = out.floatingPointPrecision();
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    d.save(out);
    out.setFloatingPointPrecision(prev);
    return out;
}

QDataStream &operator>>(QDataStream &in, DaneBadaniaGen &d)
{
    QDataStream::FloatingPointPrecision prev = in.floatingPointPrecision();
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    d.load(in);
    in.setFloatingPointPrecision(prev);
    return in;
}

DaneBadaniaGen::DaneBadaniaGen() 
{
    change = false;
}

DaneBadaniaGen::~DaneBadaniaGen()
{
    
}
        

void DaneBadaniaGen::load(QDataStream &in)
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

void DaneBadaniaGen::save(QDataStream &out) const
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
QString DaneBadaniaGen::getNumerZlecenia() const
{
	return numerZlecenia;
}

void DaneBadaniaGen::setNumerZlecenia(const QString & value)
{
	numerZlecenia = value;
	change = true;
}

QString DaneBadaniaGen::getNumerTestu() const
{
	return numerTestu;
}

void DaneBadaniaGen::setNumerTestu(const QString & value)
{
	numerTestu = value;
	change = true;
}

QString DaneBadaniaGen::getOsobaOdpowiedzialna() const
{
	return osobaOdpowiedzialna;
}

void DaneBadaniaGen::setOsobaOdpowiedzialna(const QString & value)
{
	osobaOdpowiedzialna = value;
	change = true;
}

QString DaneBadaniaGen::getUwagi() const
{
	return uwagi;
}

void DaneBadaniaGen::setUwagi(const QString & value)
{
	uwagi = value;
	change = true;
}

QString DaneBadaniaGen::getHaslo() const
{
	return haslo;
}

void DaneBadaniaGen::setHaslo(const QString & value)
{
	haslo = value;
	change = true;
}

bool DaneBadaniaGen::getZasCzujekWbudZasilacz() const
{
	return zasCzujekWbudZasilacz;
}

void DaneBadaniaGen::setZasCzujekWbudZasilacz(const bool & value)
{
	zasCzujekWbudZasilacz = value;
	change = true;
}

unsigned int DaneBadaniaGen::getNapiecieZasCzujki_mV() const
{
	return napiecieZasCzujki_mV;
}

void DaneBadaniaGen::setNapiecieZasCzujki_mV(const unsigned int & value)
{
	napiecieZasCzujki_mV = value;
	change = true;
}

QString DaneBadaniaGen::getTypCentraliSygnPoz() const
{
	return typCentraliSygnPoz;
}

void DaneBadaniaGen::setTypCentraliSygnPoz(const QString & value)
{
	typCentraliSygnPoz = value;
	change = true;
}

unsigned int DaneBadaniaGen::getCzasStabilizacjiCzujki_s() const
{
	return czasStabilizacjiCzujki_s;
}

void DaneBadaniaGen::setCzasStabilizacjiCzujki_s(const unsigned int & value)
{
	czasStabilizacjiCzujki_s = value;
	change = true;
}

bool DaneBadaniaGen::getWyzwalanieAlarmuPrzekaznik() const
{
	return wyzwalanieAlarmuPrzekaznik;
}

void DaneBadaniaGen::setWyzwalanieAlarmuPrzekaznik(const bool & value)
{
	wyzwalanieAlarmuPrzekaznik = value;
	change = true;
}

QString DaneBadaniaGen::getPrzekroczeniePraduZasilania_mA() const
{
	return przekroczeniePraduZasilania_mA;
}

void DaneBadaniaGen::setPrzekroczeniePraduZasilania_mA(const QString & value)
{
	przekroczeniePraduZasilania_mA = value;
	change = true;
}

unsigned int DaneBadaniaGen::getDlugoscFaliFiltrow() const
{
	return dlugoscFaliFiltrow;
}

void DaneBadaniaGen::setDlugoscFaliFiltrow(const unsigned int & value)
{
	dlugoscFaliFiltrow = value;
	change = true;
}

unsigned int DaneBadaniaGen::getCzasPomZmianaTlumenia_s() const
{
	return czasPomZmianaTlumenia_s;
}

void DaneBadaniaGen::setCzasPomZmianaTlumenia_s(const unsigned int & value)
{
	czasPomZmianaTlumenia_s = value;
	change = true;
}

bool DaneBadaniaGen::getSystemOdbiornikNadajnik() const
{
	return systemOdbiornikNadajnik;
}

void DaneBadaniaGen::setSystemOdbiornikNadajnik(const bool & value)
{
	systemOdbiornikNadajnik = value;
	change = true;
}

QString DaneBadaniaGen::getProducentCzujki() const
{
	return producentCzujki;
}

void DaneBadaniaGen::setProducentCzujki(const QString & value)
{
	producentCzujki = value;
	change = true;
}

QString DaneBadaniaGen::getTypPierwszejCzujki() const
{
	return typPierwszejCzujki;
}

void DaneBadaniaGen::setTypPierwszejCzujki(const QString & value)
{
	typPierwszejCzujki = value;
	change = true;
}

QString DaneBadaniaGen::getTypDrugiejCzujki() const
{
	return typDrugiejCzujki;
}

void DaneBadaniaGen::setTypDrugiejCzujki(const QString & value)
{
	typDrugiejCzujki = value;
	change = true;
}

QString DaneBadaniaGen::getRozstawienieMinCzujki() const
{
	return rozstawienieMinCzujki;
}

void DaneBadaniaGen::setRozstawienieMinCzujki(const QString & value)
{
	rozstawienieMinCzujki = value;
	change = true;
}

QString DaneBadaniaGen::getRozstawienieMaxCzujki() const
{
	return rozstawienieMaxCzujki;
}

void DaneBadaniaGen::setRozstawienieMaxCzujki(const QString & value)
{
	rozstawienieMaxCzujki = value;
	change = true;
}

double DaneBadaniaGen::getMaksKatowaNieWspolPionPierwszejCzuj() const
{
	return maksKatowaNieWspolPionPierwszejCzuj;
}

void DaneBadaniaGen::setMaksKatowaNieWspolPionPierwszejCzuj(const double & value)
{
	maksKatowaNieWspolPionPierwszejCzuj = value;
	change = true;
}

double DaneBadaniaGen::getMaksKatowaNieWspolPozPierwszejCzuj() const
{
	return maksKatowaNieWspolPozPierwszejCzuj;
}

void DaneBadaniaGen::setMaksKatowaNieWspolPozPierwszejCzuj(const double & value)
{
	maksKatowaNieWspolPozPierwszejCzuj = value;
	change = true;
}

double DaneBadaniaGen::getMaksKatowaNieWspolPionDrugiejCzuj() const
{
	return maksKatowaNieWspolPionDrugiejCzuj;
}

void DaneBadaniaGen::setMaksKatowaNieWspolPionDrugiejCzuj(const double & value)
{
	maksKatowaNieWspolPionDrugiejCzuj = value;
	change = true;
}

double DaneBadaniaGen::getMaksKatowaNieWspolPozDrugiejCzuj() const
{
	return maksKatowaNieWspolPozDrugiejCzuj;
}

void DaneBadaniaGen::setMaksKatowaNieWspolPozDrugiejCzuj(const double & value)
{
	maksKatowaNieWspolPozDrugiejCzuj = value;
	change = true;
}

unsigned short DaneBadaniaGen::getIloscCzujek() const
{
	return iloscCzujek;
}

void DaneBadaniaGen::setIloscCzujek(const unsigned short & value)
{
	iloscCzujek = value;
	change = true;
}
