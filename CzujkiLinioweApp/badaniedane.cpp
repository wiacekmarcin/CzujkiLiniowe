#include "badaniedane.h"

BadanieDane::BadanieDane(QObject *parent)
    : QObject{parent}
{

}

const QString &BadanieDane::getNumerZlecenia() const
{
    return numerZlecenia;
}

void BadanieDane::setNumerZlecenia(const QString &newNumerZlecenia)
{
    numerZlecenia = newNumerZlecenia;
}

const QString &BadanieDane::getNumerTestu() const
{
    return numerTestu;
}

void BadanieDane::setNumerTestu(const QString &newNumerTestu)
{
    numerTestu = newNumerTestu;
}

const QString &BadanieDane::getUwagi() const
{
    return uwagi;
}

void BadanieDane::setUwagi(const QString &newUwagi)
{
    uwagi = newUwagi;
}

const QString &BadanieDane::getPassword() const
{
    return password;
}

void BadanieDane::setPassword(const QString &newPassword)
{
    password = newPassword;
}

bool BadanieDane::getWewnetrzyZasilacz() const
{
    return wewnetrzyZasilacz;
}

void BadanieDane::setWewnetrzyZasilacz(bool newWewnetrzyZasilacz)
{
    wewnetrzyZasilacz = newWewnetrzyZasilacz;
    centralaZasilanie = !newWewnetrzyZasilacz;
}

unsigned int BadanieDane::getZasilanie_mV() const
{
    return zasilanie_mV;
}

void BadanieDane::setZasilanie_mV(unsigned int newZasilanie_mV)
{
    zasilanie_mV = newZasilanie_mV;
}

bool BadanieDane::getCentralaZasilanie() const
{
    return centralaZasilanie;
}

void BadanieDane::setCentralaZasilanie(bool newCentralaZasilanie)
{
    centralaZasilanie = newCentralaZasilanie;
    wewnetrzyZasilacz = !newCentralaZasilanie;
}

const QString &BadanieDane::getCentralaTyp() const
{
    return centralaTyp;
}

void BadanieDane::setCentralaTyp(const QString &newCentralaTyp)
{
    centralaTyp = newCentralaTyp;
}

unsigned int BadanieDane::getCzasStabilizacji_ms() const
{
    return czasStabilizacji_ms;
}

void BadanieDane::setCzasStabilizacji_ms(unsigned int newCzasStabilizacji_ms)
{
    czasStabilizacji_ms = newCzasStabilizacji_ms;
}

bool BadanieDane::getWyzwalanieAlarmuPrzekaznik() const
{
    return wyzwalanieAlarmuPrzekaznik;
}

void BadanieDane::setWyzwalanieAlarmuPrzekaznik(bool newWyzwalanieAlarmuPrzekaznik)
{
    wyzwalanieAlarmuPrzekaznik = newWyzwalanieAlarmuPrzekaznik;
    wyzwalanieAlarmuPrad = !newWyzwalanieAlarmuPrzekaznik;
}

bool BadanieDane::getWyzwalanieAlarmuPrad() const
{
    return wyzwalanieAlarmuPrad;
}

void BadanieDane::setWyzwalanieAlarmuPrad(bool newWyzwalanieAlarmuPrad)
{
    wyzwalanieAlarmuPrad = newWyzwalanieAlarmuPrad;
    wyzwalanieAlarmuPrzekaznik = !newWyzwalanieAlarmuPrad;
}

unsigned int BadanieDane::getPradWyzwolenia_mA() const
{
    return pradWyzwolenia_mA;
}

void BadanieDane::setPradWyzwolenia_mA(unsigned int newPradWyzwolenia_mA)
{
    pradWyzwolenia_mA = newPradWyzwolenia_mA;
}

unsigned int BadanieDane::getDlugoscFaliFiltrow() const
{
    return dlugoscFaliFiltrow;
}

void BadanieDane::setDlugoscFaliFiltrow(unsigned int newDlugoscFaliFiltrow)
{
    dlugoscFaliFiltrow = newDlugoscFaliFiltrow;
}

unsigned int BadanieDane::getCzasPomiedzyZmianaTlumenia_ms() const
{
    return czasPomiedzyZmianaTlumenia_ms;
}

void BadanieDane::setCzasPomiedzyZmianaTlumenia_ms(unsigned int newCzasPomiedzyZmianaTlumenia_ms)
{
    czasPomiedzyZmianaTlumenia_ms = newCzasPomiedzyZmianaTlumenia_ms;
}

const QString &BadanieDane::getOsobaOdpowiedzialna() const
{
    return osobaOdpowiedzialna;
}

void BadanieDane::setOsobaOdpowiedzialna(const QString &newOsobaOdpowiedzialna)
{
    osobaOdpowiedzialna = newOsobaOdpowiedzialna;
}

bool BadanieDane::getRodzajSystemuOdbiornikNadajnik() const
{
    return rodzajSystemuOdbiornikNadajnik;
}

void BadanieDane::setRodzajSystemuOdbiornikNadajnik(bool newRodzajSystemuOdbiornikNadajnik)
{
    rodzajSystemuOdbiornikNadajnik = newRodzajSystemuOdbiornikNadajnik;
    rodzajSystemuOdbiornikReflektor= !newRodzajSystemuOdbiornikNadajnik;
}

bool BadanieDane::getRodzajSystemuOdbiornikReflektor() const
{
    return rodzajSystemuOdbiornikReflektor;
}

void BadanieDane::setRodzajSystemuOdbiornikReflektor(bool newRodzajSystemuOdbiornikReflektor)
{
    rodzajSystemuOdbiornikReflektor = newRodzajSystemuOdbiornikReflektor;
    rodzajSystemuOdbiornikNadajnik = !newRodzajSystemuOdbiornikReflektor;
}

const QString &BadanieDane::getProducent() const
{
    return producent;
}

void BadanieDane::setProducent(const QString &newProducent)
{
    producent = newProducent;
}

const QString &BadanieDane::getTypNadajnika() const
{
    return typNadajnika;
}

void BadanieDane::setTypNadajnika(const QString &newTypNadajnika)
{
    typNadajnika = newTypNadajnika;
}

const QString &BadanieDane::getTypOdbiornika() const
{
    return typOdbiornika;
}

void BadanieDane::setTypOdbiornika(const QString &newTypOdbiornika)
{
    typOdbiornika = newTypOdbiornika;
}

long BadanieDane::getRozstawienieMinimalne_mm() const
{
    return rozstawienieMinimalne_mm;
}

void BadanieDane::setRozstawienieMinimalne_mm(long newRozstawienieMinimalne_mm)
{
    rozstawienieMinimalne_mm = newRozstawienieMinimalne_mm;
}

long BadanieDane::getRozstawienieMaksymalne_mm() const
{
    return rozstawienieMaksymalne_mm;
}

void BadanieDane::setRozstawienieMaksymalne_mm(long newRozstawienieMaksymalne_mm)
{
    rozstawienieMaksymalne_mm = newRozstawienieMaksymalne_mm;
}

float BadanieDane::getMaksKatNieWspolOdbiornikPion() const
{
    return maksKatNieWspolOdbiornikPion;
}

void BadanieDane::setMaksKatNieWspolOdbiornikPion(float newMaksKatNieWspolOdbiornikPion)
{
    maksKatNieWspolOdbiornikPion = newMaksKatNieWspolOdbiornikPion;
}

float BadanieDane::getMaksKatNieWspolOdbiornikPoz() const
{
    return maksKatNieWspolOdbiornikPoz;
}

void BadanieDane::setMaksKatNieWspolOdbiornikPoz(float newMaksKatNieWspolOdbiornikPoz)
{
    maksKatNieWspolOdbiornikPoz = newMaksKatNieWspolOdbiornikPoz;
}

float BadanieDane::getMaksKatNieWspolNadajnikPion() const
{
    return maksKatNieWspolNadajnikPion;
}

void BadanieDane::setMaksKatNieWspolNadajnikPion(float newMaksKatNieWspolNadajnikPion)
{
    maksKatNieWspolNadajnikPion = newMaksKatNieWspolNadajnikPion;
}

float BadanieDane::getMaksKatNieWspolNadajnikPaz() const
{
    return maksKatNieWspolNadajnikPaz;
}

void BadanieDane::setMaksKatNieWspolNadajnikPaz(float newMaksKatNieWspolNadajnikPaz)
{
    maksKatNieWspolNadajnikPaz = newMaksKatNieWspolNadajnikPaz;
}

int BadanieDane::getIloscNadajnikOdbiornik() const
{
    return iloscNadajnikOdbiornik;
}

void BadanieDane::setIloscNadajnikOdbiornik(int newIloscNadajnikOdbiornik)
{
    iloscNadajnikOdbiornik = newIloscNadajnikOdbiornik;
}

const QString &BadanieDane::getNumerNadajnika(const unsigned int & numer) const
{
    Q_ASSERT(numer > 7);
    return numerNadajnika[numer];
}

void BadanieDane::setNumerNadajnika(const unsigned int & numer, const QString &newNumerNadajnika)
{
    Q_ASSERT(numer > 7);
    numerNadajnika[numer] = newNumerNadajnika;
}

const QString &BadanieDane::getNumerOdbiornika(const unsigned int & numer) const
{
    Q_ASSERT(numer > 7);
    return numerOdbiornika[numer];
}

void BadanieDane::setNumerOdbiornika(const unsigned int & numer, const QString &newNumerOdbiornika)
{
    Q_ASSERT(numer > 7);
    numerOdbiornika[numer] = newNumerOdbiornika;
}
