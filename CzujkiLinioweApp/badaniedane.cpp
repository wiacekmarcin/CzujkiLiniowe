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
