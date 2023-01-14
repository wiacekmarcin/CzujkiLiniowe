#include "danetestu.h"

ListaTestow::ListaTestow()
{
    nazwyTestow << "Odtwarzalność" << "Powtarzalność" << "Zależność kierunkowa" << "Zmiana parametrów zasilania"
    << "Szybkie zmiany tłumienia" << "Powolne zmiany tłumienia" << "Zależność od długości drogi optycznej"
    << "Czułość na pożar" << "Światło rozproszone" << "Odporność na suche gorąco"
    << "Odporność na zimno" << "Odporność na wilgotne gorąco, stan ustalony"
    << "Wytrzymałość na wilgotne gorąco, stan ustalony" << "Wytrzymałość na wibracje"
    << "Odporność na wyładowania elektrostatyczne"
    << "Odporność na wypromieniowanie pola elektromagnetycznego"
    << "Odporność na przewodzone zakłocenia indukowane przez pola elektromagnetyczne"
    << "Odporność na szybkie impulsy przejściowe"
    << "Odporność na powolne udary napęciowe wysokiej energii"
    << "Wytrzymałość na korozję powodowaną przez dwutlenek siarki"
    << "Odporność na uderzenie"
    ;
    odtwarzalnosc = false;

}



DaneTestu::DaneTestu():
    id(-1),
    name(""),
    wykonany(false),
    osobaWykonujaca(""),
    dataRozpoczecia(""),
    dataZakonczenia(""),
    temperatura(""),
    wilgotnosc(""),
    cisnienie(""),
    uwagi(""),
    danePomiarowe{QList<DanePomiaru>()},
    Crep(0.0),
    Cmin(0.0),
    Cmax(0.0),
    CmaxCrep(0.0),
    CrepCmin(0.0),
    ok(false),
    errStr(""),
    nazwaPierwszego(""),
    nazwaDrugiego(""),
    dlugoscFali(0),
    czasPowtarzalnosci(0)
{
    katyProducenta.nadajnik.pionowo = "0";
    katyProducenta.nadajnik.poziomo = "0";
    katyProducenta.odbiornik.pionowo = "0";
    katyProducenta.odbiornik.poziomo = "0";
}

QDataStream &operator<<(QDataStream &out, const DanePomiaru &dane)
{
    out << dane.nrCzujki
        << dane.numerNadajnika
        << dane.numerOdbiornika
        << dane.value_dB
        << dane.ok
        << dane.error

           ;
    return out;
}

QDataStream &operator>>(QDataStream &in, DanePomiaru &dane)
{
    in >> dane.nrCzujki
       >> dane.numerNadajnika
       >> dane.numerOdbiornika
       >> dane.value_dB
       >> dane.ok
       >> dane.error
           ;
    return in;
}

QDataStream &operator<<(QDataStream &out, const QList<DanePomiaru> &dane)
{
    out << dane.size();
    for (const auto & d : dane) {
        out << d;
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, QList<DanePomiaru> &dane)
{
    dane.clear();
    unsigned int ss;
    in >> ss;
    DanePomiaru p;
    for (unsigned int s = 0; s < ss; ++s)
    {
        in >> p;
        dane.append(p);
    }
    return in;
}

QDataStream &operator<<(QDataStream &out, const DaneTestu &dane)
{
    out << dane.id
        << dane.name
        << dane.wykonany
        << dane.osobaWykonujaca
        << dane.dataRozpoczecia
        << dane.dataZakonczenia
        << dane.temperatura
        << dane.wilgotnosc
        << dane.cisnienie
        << dane.uwagi
        << dane.danePomiarowe
        << dane.Crep
        << dane.Cmin
        << dane.Cmax
        << dane.CmaxCrep
        << dane.CrepCmin
        << dane.ok
        << dane.errStr
        << dane.nazwaPierwszego
        << dane.nazwaDrugiego
        << dane.dlugoscFali
        << dane.czasPowtarzalnosci
        << dane.katyProducenta.nadajnik.pionowo
        << dane.katyProducenta.nadajnik.poziomo
        << dane.katyProducenta.odbiornik.pionowo
        << dane.katyProducenta.odbiornik.poziomo
           ;
    return out;
}

QDataStream &operator>>(QDataStream &in, DaneTestu &dane)
{
    in  >> dane.id
        >> dane.name
        >> dane.wykonany
        >> dane.osobaWykonujaca
        >> dane.dataRozpoczecia
        >> dane.dataZakonczenia
        >> dane.temperatura
        >> dane.wilgotnosc
        >> dane.cisnienie
        >> dane.uwagi
        >> dane.danePomiarowe
        >> dane.Crep
        >> dane.Cmin
        >> dane.Cmax
        >> dane.CmaxCrep
        >> dane.CrepCmin
        >> dane.ok
        >> dane.errStr
        >> dane.nazwaPierwszego
        >> dane.nazwaDrugiego
        >> dane.dlugoscFali
        >> dane.czasPowtarzalnosci
        >> dane.katyProducenta.nadajnik.pionowo
        >> dane.katyProducenta.nadajnik.poziomo
        >> dane.katyProducenta.odbiornik.pionowo
        >> dane.katyProducenta.odbiornik.poziomo
            ;
    return in;
}

short DaneTestu::getId() const
{
    return id;
}

void DaneTestu::setId(short newId)
{
    id = newId;
}

const QString &DaneTestu::getName() const
{
    return name;
}

void DaneTestu::setName(const QString &newName)
{
    name = newName;
}

bool DaneTestu::getWykonany() const
{
    return wykonany;
}

void DaneTestu::setWykonany(bool newWykonany)
{
    wykonany = newWykonany;
}

const QString &DaneTestu::getOsobaWykonujaca() const
{
    return osobaWykonujaca;
}

void DaneTestu::setOsobaWykonujaca(const QString &newOsobaWykonujaca)
{
    osobaWykonujaca = newOsobaWykonujaca;
}

const QString &DaneTestu::getDataRozpoczecia() const
{
    return dataRozpoczecia;
}

void DaneTestu::setDataRozpoczecia(const QString &newRozpoczeto)
{
    dataRozpoczecia = newRozpoczeto;
}

const QString &DaneTestu::getTemperatura() const
{
    return temperatura;
}

void DaneTestu::setTemperatura(const QString &newTemperatura)
{
    temperatura = newTemperatura;
}

const QString &DaneTestu::getWilgotnosc() const
{
    return wilgotnosc;
}

void DaneTestu::setWilgotnosc(const QString &newWilgotnosc)
{
    wilgotnosc = newWilgotnosc;
}

const QString &DaneTestu::getCisnienie() const
{
    return cisnienie;
}

void DaneTestu::setCisnienie(const QString &newCisnienie)
{
    cisnienie = newCisnienie;
}

const QString &DaneTestu::getUwagi() const
{
    return uwagi;
}

void DaneTestu::setUwagi(const QString &newUwagi)
{
    uwagi = newUwagi;
}

QString DaneTestu::getNumerNadajnika() const
{
    if (danePomiarowe.size() == 0)
        return QString();
    return danePomiarowe[danePomiarowe.size()-1].numerNadajnika;
}

QString DaneTestu::getNumerOdbiornika() const
{
    if (danePomiarowe.size() == 0)
        return QString();
    return danePomiarowe[danePomiarowe.size()-1].numerOdbiornika;
}

void DaneTestu::addWybranaCzujka(short nrCzujki, const QString &nadajnik, const QString &odbiornik)
{
    DanePomiaru nowyPomiar;
    nowyPomiar.nrCzujki = nrCzujki;
    nowyPomiar.numerNadajnika = nadajnik;
    nowyPomiar.numerOdbiornika = odbiornik;
    nowyPomiar.value_dB = "0.0";
    nowyPomiar.error = "";
    danePomiarowe.append(nowyPomiar);
}

bool DaneTestu::sprawdzCzyBadanaCzujka(const QString &nadajnik, const QString &odbiornik)
{
    for(const auto & czujka : danePomiarowe) {
        if (czujka.numerNadajnika == nadajnik && czujka.numerOdbiornika == odbiornik)
            return true;
    }
    return false;
}

void DaneTestu::setSuccessBadaniaCzujki(bool ok, const QString &value, const QString & error)
{
    danePomiarowe.last().ok = ok;
    danePomiarowe.last().value_dB = value;
    danePomiarowe.last().error = error;
}

const QList<DanePomiaru> &DaneTestu::getDaneBadanCzujek() const
{
    return danePomiarowe;
}

float DaneTestu::getCrep() const
{
    return Crep;
}

void DaneTestu::setCrep(float newCrep)
{
    Crep = newCrep;
}

float DaneTestu::getCmin() const
{
    return Cmin;
}

void DaneTestu::setCmin(float newCmin)
{
    Cmin = newCmin;
}

float DaneTestu::getCmax() const
{
    return Cmax;
}

void DaneTestu::setCmax(float newCmax)
{
    Cmax = newCmax;
}

float DaneTestu::getCmaxCrep() const
{
    return CmaxCrep;
}

void DaneTestu::setCmaxCrep(float newCmaxCrep)
{
    CmaxCrep = newCmaxCrep;
}

float DaneTestu::getCrepCmin() const
{
    return CrepCmin;
}

void DaneTestu::setCrepCmin(float newCrepCmin)
{
    CrepCmin = newCrepCmin;
}

const QString &DaneTestu::getDataZakonczenia() const
{
    return dataZakonczenia;
}

QString DaneTestu::getWynik() const
{
    if (wykonany)
        return ok ? "POZYTYWNY" : "NEGATYWNY";
    return "NIE WYKONANY";
}

bool DaneTestu::getOk() const
{
    return ok;
}

void DaneTestu::setOk(bool newOk)
{
    ok = newOk;
}

void DaneTestu::setDataZakonczenia(const QString &newDataZakonczen)
{
    dataZakonczenia = newDataZakonczen;
}

QString DaneTestu::getErrStr() const
{
    return errStr;
}

void DaneTestu::setErrStr(const QString & newErr)
{
    errStr = newErr;
}

const QString &DaneTestu::getNazwaPierwszego() const
{
    return nazwaPierwszego;
}

void DaneTestu::setNazwaPierwszego(const QString &newNazwaPierwszego)
{
    nazwaPierwszego = newNazwaPierwszego;
}

const QString &DaneTestu::getNazwaDrugiego() const
{
    return nazwaDrugiego;
}

void DaneTestu::setNazwaDrugiego(const QString &newNazwaDrugiego)
{
    nazwaDrugiego = newNazwaDrugiego;
}

int DaneTestu::getDlugoscFali() const
{
    return dlugoscFali;
}

void DaneTestu::setDlugoscFali(int newDlugoscFali)
{
    dlugoscFali = newDlugoscFali;
}

unsigned int DaneTestu::getCzasPowtarzalnosci() const
{
    return czasPowtarzalnosci;
}

void DaneTestu::setCzasPowtarzalnosci(unsigned int newCzasPowtarzalnosci)
{
    czasPowtarzalnosci = newCzasPowtarzalnosci;
}

void DaneTestu::addNextPomiar()
{
    DanePomiaru nowyPomiar;
    nowyPomiar.nrCzujki = danePomiarowe.last().nrCzujki;
    nowyPomiar.numerNadajnika = danePomiarowe.last().numerNadajnika;
    nowyPomiar.numerOdbiornika = danePomiarowe.last().numerOdbiornika;
    nowyPomiar.value_dB = "0.0";
    nowyPomiar.error = "";
    danePomiarowe.append(nowyPomiar);
}

void DaneTestu::removeLastPomiar()
{
    danePomiarowe.removeLast();
}

const NiewspolosiowoscOsUrzadzenie &DaneTestu::getKatyProducenta() const
{
    return katyProducenta;
}

void DaneTestu::setKatyProducenta(const NiewspolosiowoscOsUrzadzenie &newKatyProducenta)
{
    katyProducenta = newKatyProducenta;
}
