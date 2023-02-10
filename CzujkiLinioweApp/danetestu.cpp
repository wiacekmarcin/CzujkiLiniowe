#include "danetestu.h"
#include <QDate>
#include <QTime>
#include <QDebug>
#include "math.h"
#include "parametrybadania.h"

ListaTestow::ListaTestow()
{
    nazwyTestow << QString::fromUtf8("Odtwarzalność")
                << QString::fromUtf8("Powtarzalność")
                << QString::fromUtf8("Zależność kierunkowa")
                << QString::fromUtf8("Szybkie zmiany tłumienia")
                << QString::fromUtf8("Zależność od długości drogi optycznej")
                << QString::fromUtf8("Światło rozproszone")
                << QString::fromUtf8("Zmiana parametrów zasilania")
                /*<< QString::fromUtf8("Czułość na pożar")*/
                << QString::fromUtf8("Odporność na suche gorąco")
                << QString::fromUtf8("Odporność na zimno")
                << QString::fromUtf8("Odporność na wilgotne gorąco, stan ustalony")
                << QString::fromUtf8("Wytrzymałość na wilgotne gorąco, stan ustalony")
                << QString::fromUtf8("Wytrzymałość na wibracje")
                << QString::fromUtf8("Odporność na uderzenie")
                << QString::fromUtf8("Odporność na wyładowania elektrostatyczne")
                << QString::fromUtf8("Odporność na wypromieniowanie pola elektromagnetycznego")
                << QString::fromUtf8("Odporność na przewodzone zakłocenia indukowane przez pola elektromagnetyczne")
                << QString::fromUtf8("Odporność na szybkie impulsy przejściowe")
                << QString::fromUtf8("Odporność na powolne udary napęciowe wysokiej energii")
                << QString::fromUtf8("Wytrzymałość na korozję powodowaną przez dwutlenek siarki")

                << QString::fromUtf8("Czułość na pożar")

                << QString::fromUtf8("Testowe sprawdzenie czujki");

      
    
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
    CmaxCmin(0.0),
    ok(false),
    errStr(""),
    nazwaNumerTransmitter(""),
    nazwaNumerReceiver(""),
    nazwaTransmitter(""),
    nazwaReceiver(""),
    nazwaTransmitter_a(""),
    nazwaReceiver_a(""),
    dlugoscFali(0),
    czasPowtarzalnosci(0),
    minimalneRozstawienie("0"),
    maksymalneRozstawienie("0"),
    minimalneNapiecie("0"),
    maksymalneNapiecie("0")
{
    katyProducenta.nadajnik.pionowo = "0";
    katyProducenta.nadajnik.poziomo = "0";
    katyProducenta.odbiornik.pionowo = "0";
    katyProducenta.odbiornik.poziomo = "0";
}

QDataStream &operator<<(QDataStream &out, const DanePomiaru &dane)
{
    out << dane.nrCzujki
        << dane.nrSortCzujki
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
       >> dane.nrSortCzujki
       >> dane.numerNadajnika
       >> dane.numerOdbiornika
       >> dane.value_dB
       >> dane.ok
       >> dane.error
           ;
    return in;
}

QDataStream &operator<<(QDataStream &out, const PomiarKata &dane)
{
    out << dane.nazwaBadania
        << dane.ok
        << dane.errorStr
        << dane.errorDetail
        << dane.katZmierzony
        << dane.katProducenta
           ;
   return out;
}

QDataStream &operator>>(QDataStream &in, PomiarKata &dane)
{
    in  >> dane.nazwaBadania
        >> dane.ok
        >> dane.errorStr
        >> dane.errorDetail
        >> dane.katZmierzony
        >> dane.katProducenta

            ;
    return in;
}

/*
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
*/
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
        << dane.CmaxCmin
        << dane.ok
        << dane.errStr
        << dane.nazwaNumerTransmitter
        << dane.nazwaNumerReceiver
        << dane.nazwaTransmitter
        << dane.nazwaReceiver
        << dane.nazwaTransmitter_a
        << dane.nazwaReceiver_a
        << dane.dlugoscFali
        << dane.czasPowtarzalnosci
        << dane.katyProducenta.nadajnik.pionowo
        << dane.katyProducenta.nadajnik.poziomo
        << dane.katyProducenta.odbiornik.pionowo
        << dane.katyProducenta.odbiornik.poziomo
        << dane.pomiaryKatow
        << dane.minimalneRozstawienie
        << dane.maksymalneRozstawienie
        << dane.minimalneNapiecie
        << dane.maksymalneNapiecie
        << dane.wynikNarazenia
        << dane.infoNarazenia

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
        >> dane.CmaxCmin
        >> dane.ok
        >> dane.errStr
        >> dane.nazwaNumerTransmitter
        >> dane.nazwaNumerReceiver
        >> dane.nazwaTransmitter
        >> dane.nazwaReceiver
        >> dane.nazwaTransmitter_a
        >> dane.nazwaReceiver_a
        >> dane.dlugoscFali
        >> dane.czasPowtarzalnosci
        >> dane.katyProducenta.nadajnik.pionowo
        >> dane.katyProducenta.nadajnik.poziomo
        >> dane.katyProducenta.odbiornik.pionowo
        >> dane.katyProducenta.odbiornik.poziomo
        >> dane.pomiaryKatow
        >> dane.minimalneRozstawienie
        >> dane.maksymalneRozstawienie
        >> dane.minimalneNapiecie
        >> dane.maksymalneNapiecie
        >> dane.wynikNarazenia
        >> dane.infoNarazenia
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

QString DaneTestu::getNumerTransmitter() const
{
    if (danePomiarowe.size() == 0)
        return QString();
    return danePomiarowe[danePomiarowe.size()-1].numerNadajnika;
}

QString DaneTestu::getNumerReceiver() const
{
    if (danePomiarowe.size() == 0)
        return QString();
    return danePomiarowe[danePomiarowe.size()-1].numerOdbiornika;
}

void DaneTestu::addWybranaCzujka(short nrCzujki, const QString &nadajnik, const QString &odbiornik)
{
    DanePomiaru nowyPomiar;
    nowyPomiar.nrCzujki = nrCzujki;
    nowyPomiar.nrSortCzujki = nrCzujki;
    nowyPomiar.numerNadajnika = nadajnik;
    nowyPomiar.numerOdbiornika = odbiornik;
    nowyPomiar.value_dB = "0.0";
    nowyPomiar.error = "";
    nowyPomiar.ok = false;
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

QString DaneTestu::getNumerCzujki() const
{
    if (danePomiarowe.size() > 0) {
        return QString("-");
    }
    else
        return QString::number(danePomiarowe.first().nrCzujki);
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

void DaneTestu::setDataZakonczenia()
{
    dataZakonczenia = QDate::currentDate().toString("yyyy-MM-dd") + QString(" ") + QTime::currentTime().toString("HH:mm");
}



QString DaneTestu::getErrStr() const
{
    return errStr;
}

void DaneTestu::setErrStr(const QString & newErr)
{
    errStr = newErr;
}

const QString &DaneTestu::getNazwaNumerTransmitter() const
{
    return nazwaNumerTransmitter;
}

void DaneTestu::setNazwaNumerTransmitter(const QString &newNazwaPierwszego)
{
    nazwaNumerTransmitter = newNazwaPierwszego;
}

const QString &DaneTestu::getNazwaNumerReceiver() const
{
    return nazwaNumerReceiver;
}

void DaneTestu::setNazwaNumerReceiver(const QString &newNazwaReceiverego)
{
    nazwaNumerReceiver = newNazwaReceiverego;
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
    nowyPomiar.nrSortCzujki = danePomiarowe.last().nrSortCzujki;
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

const QString &DaneTestu::getNazwaTransmitter() const
{
    return nazwaTransmitter;
}

void DaneTestu::setNazwaTransmitter(const QString &newNazwaTransmitter)
{
    nazwaTransmitter = newNazwaTransmitter;
}

const QString &DaneTestu::getNazwaReceiver() const
{
    return nazwaReceiver;
}

void DaneTestu::setNazwaReceiver(const QString &newNazwaTypReceiverego)
{
    nazwaReceiver = newNazwaTypReceiverego;
}

float DaneTestu::getCmaxCmin() const
{
    return CmaxCmin;
}

void DaneTestu::setCmaxCmin(float newCmaxCmin)
{
    CmaxCmin = newCmaxCmin;
}

const QVector<PomiarKata> &DaneTestu::getPomiaryKatow() const
{
    return pomiaryKatow;
}

void DaneTestu::setPomiaryKatow(const QVector<PomiarKata> &newPomiaryKatow)
{
    pomiaryKatow = newPomiaryKatow;
}

void DaneTestu::dodajPomiarKata(const PomiarKata & kat)
{
    pomiaryKatow.append(kat);
}

void DaneTestu::obliczZaleznoscKatowa(const Ustawienia &ust)
{
    bool ok = true;
    QStringList errors;
    for (auto & wynik : pomiaryKatow) {
        if (wynik.ok) {
            double val = abs(wynik.katProducenta);
            double delta = ust.getNiewspolosiowoscMinimalnyKatProducentMierzony();
            if (val < delta) {
                ok = false;
                errors << QString::fromUtf8("Kąt <%1").arg(delta,2,'f',1);
            }
        } else {
            ok = false;
        }
    }
    setOk(ok);
    if (errors.size() > 0)
        setErrStr(errors.size() > 1 ? QString::fromUtf8("Wystąpiło wiele błędów") : errors.at(0));
    setWykonany(true);
}

void DaneTestu::obliczOdtwarzalnosc(ParametryBadania * badanie, const Ustawienia & ust)
{
    bool badanieOk = true;
    int cntAvg = 0;
    float Cavg = 0;

    float Cmin = 100;
    float Cmax = -100;


    for (DanePomiaru & dane : getDanePomiarowe())
    {
        bool ok;
        double C = dane.value_dB.toDouble(&ok);

        if (!dane.ok) {
            badanieOk = false;
            dane.nrSortCzujki = 0;
            continue;
        }

        if (!ok) {
            badanieOk = false;
            setErrStr(QString::fromUtf8("Błędna wartość C"));
            if (dane.error.isEmpty())
                dane.error = QString::fromUtf8("Błędna wartość C");
            dane.ok = false;
            dane.nrSortCzujki = 0;
            continue;
        } else {
            if (C > Cmax)
                Cmax = C;
            if (C < Cmin)
                Cmin = C;

            Cavg += C;
            ++cntAvg;
        }

        if (badanieOk && !dane.ok) {
            setErrStr(dane.error);
            badanieOk = false;
            continue;
        }

        if (!dane.ok) {
            badanieOk = false;
            dane.nrSortCzujki = 0;
            continue;
        }

        if (C < ust.getMinimalnaWartoscCzujkiCn()) {
            if (badanieOk) {
                setErrStr(QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1));
                badanieOk = false;
            }
            dane.nrSortCzujki = 0;
            dane.ok = false;
            dane.error = QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1);
        }
    }
    if (cntAvg == 0) {
        setOk(false);
        setErrStr(badanieOk ? "" : "Żadna czujka nie przeszła badanie");
    }
    if (cntAvg)
        Cavg = Cavg/cntAvg;

    setOk(badanieOk);
    setErrStr(badanieOk ? "" : "Czujka(i) nie przeszły badania");

    if (cntAvg != 0 && Cavg != 0 && Cmin != 0) {
        setCrep(Cavg);
        setCmin(Cmin);
        setCmax(Cmax);
        setCmaxCrep(Cmax/Cavg);
        setCrepCmin(Cavg/Cmin);
    } else {
        setCrep(0);
        setCmin(0);
        setCmax(0);
        setCmaxCrep(0);
        setCrepCmin(0);
    }
    setWykonany(true);

    if (getCmaxCrep() > ust.getOdtwarzalnoscCmaxCrep()) {
        setOk(false);
        setErrStr(QString("Cmax/Crep>%1").arg(ust.getOdtwarzalnoscCmaxCrep(), 3, 'f', 2));
    } else if (getCrepCmin() > ust.getOdtwarzalnoscCrepCmin()) {
        setOk(false);
        setErrStr(QString("Crep/Cmin>%1").arg(ust.getOdtwarzalnoscCrepCmin(), 3, 'f', 2));
    } else {
        setOk(badanieOk);
    }

    posortuj(badanie);
}

void DaneTestu::obliczPowtarzalnosc(const Ustawienia & ust)
{
    bool badanieOk = true;

    float Cmin = 100;
    float Cmax = -100;


    for (DanePomiaru & dane : getDanePomiarowe())
    {
        if (!dane.ok) {
            badanieOk = false;
            setErrStr(dane.error);
            continue;
        }
        bool ok;
        double C = dane.value_dB.toDouble(&ok);

        if (!ok) {
            badanieOk = false;
            setErrStr(QString::fromUtf8("Błędna wartość C"));
            dane.ok = false;
            continue;
        }

        if (!dane.ok) {
            badanieOk = false;
            setErrStr(QString::fromUtf8("Czujka nie zadziałała"));
            continue;
        }

        if (C > Cmax)
            Cmax = C;
        if (C < Cmin)
            Cmin = C;

        if (C < ust.getMinimalnaWartoscCzujkiCn()) {
            badanieOk = false;
            setErrStr(QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1));
            dane.ok = false;
            dane.error = QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1);
        }
    }


    setOk(badanieOk);
    setErrStr(badanieOk ? "" : "Czujka(i) nie przeszły badania");

    if (Cmin) {
        setCmin(Cmin);
        setCmax(Cmax);
        setCmaxCmin(Cmax/Cmin);
    } else {
        setCmin(0);
        setCmax(0);
        setCmaxCmin(0);
    }
    setWykonany(true);

    if (badanieOk && getCmaxCmin() > ust.getPowtarzalnoscCmaxCmin()) {
        setOk(false);
        setErrStr(QString("Cmax/Cmin>%1").arg(ust.getPowtarzalnoscCmaxCmin(), 3, 'f', 2));
    }
}

void DaneTestu::obliczDlugoscOptyczna(const Ustawienia &ust)
{
    if (!getOk())
        return;
    bool badanieOk = true;

    float Cmin = 100;
    float Cmax = -100;


    for (DanePomiaru & dane : getDanePomiarowe())
    {
        if (!dane.ok) {
            badanieOk = false;
            setErrStr(dane.error);
            continue;
        }
        bool ok;
        double C = dane.value_dB.toDouble(&ok);

        if (!ok) {
            badanieOk = false;
            setErrStr(QString::fromUtf8("Błędna wartość C"));
            dane.ok = false;
            continue;
        }

        if (!dane.ok) {
            badanieOk = false;
            setErrStr(QString::fromUtf8("Czujka nie zadziałała"));
            continue;
        }

        if (C > Cmax)
            Cmax = C;
        if (C < Cmin)
            Cmin = C;

        if (C < ust.getMinimalnaWartoscCzujkiCn()) {
            badanieOk = false;
            setErrStr(QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1));
            dane.ok = false;
            dane.error = QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1);
        }
    }


    setOk(badanieOk);
    setErrStr(badanieOk ? "" : "Czujka(i) nie przeszły badania");

    if (Cmin) {
        setCmin(Cmin);
        setCmax(Cmax);
        setCmaxCmin(Cmax/Cmin);
    } else {
        setCmin(0);
        setCmax(0);
        setCmaxCmin(0);
    }
    setWykonany(true);

    if (badanieOk && getCmaxCmin() > ust.getDlugoscDrogiOptycznejCmaxCmin()) {
        setOk(false);
        setErrStr(QString("Cmax/Cmin>%1").arg(ust.getDlugoscDrogiOptycznejCmaxCmin(), 3, 'f', 2));
    }
}

void DaneTestu::obliczSzybkieZmianyTlumienia(const Ustawienia &)
{
    for (DanePomiaru & dane : getDanePomiarowe())
    {
        if (!dane.ok) {
            errStr = dane.error;
            ok = false;
            return;
        }
    }
    ok = true;
    errStr = "";
}

void DaneTestu::obliczZaleznoscNapieciaZasilania(const Ustawienia &ust)
{
    if (!getOk())
        return;
    bool badanieOk = true;

    float Cmin = 100;
    float Cmax = -100;


    for (DanePomiaru & dane : getDanePomiarowe())
    {
        if (!dane.ok) {
            badanieOk = false;
            setErrStr(dane.error);
            continue;
        }
        bool ok;
        double C = dane.value_dB.toDouble(&ok);

        if (!ok) {
            badanieOk = false;
            setErrStr(QString::fromUtf8("Błędna wartość C"));
            dane.ok = false;
            continue;
        }

        if (!dane.ok) {
            badanieOk = false;
            setErrStr(QString::fromUtf8("Czujka nie zadziałała"));
            continue;
        }

        if (C > Cmax)
            Cmax = C;
        if (C < Cmin)
            Cmin = C;

        if (C < ust.getMinimalnaWartoscCzujkiCn()) {
            badanieOk = false;
            setErrStr(QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1));
            dane.ok = false;
            dane.error = QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1);
        }
    }


    setOk(badanieOk);
    setErrStr(badanieOk ? "" : "Czujka(i) nie przeszły badania");

    if (Cmin) {
        setCmin(Cmin);
        setCmax(Cmax);
        setCmaxCmin(Cmax/Cmin);
    } else {
        setCmin(0);
        setCmax(0);
        setCmaxCmin(0);
    }
    setWykonany(true);
    if (badanieOk && getCmaxCmin() > ust.getTolerancjaNapieciaZasilaniaCmaxCmin()) {
        setOk(false);
        setErrStr(QString("Cmax/Cmin>%1").arg(ust.getTolerancjaNapieciaZasilaniaCmaxCmin(), 3, 'f', 2));
    }
}

void DaneTestu::obliczTestNarazenia(short testId, const Ustawienia &ust)
{
    float Cmin = 100;
    float Cmax = -100;

    bool badanieOk = true;
    float cmaxcmin = 1.6;
    switch (testId) {
        case STRAY_LIGHT: cmaxcmin = ust.getRozproszoneSwiatloCmaxCmin(); break;
        case DRY_HEAT : cmaxcmin = ust.getOdpornoscSucheGoraceCmaxCmin(); break;
        case COLD : cmaxcmin = ust.getOdpornoscZimnoCmaxCmin(); break;
        case DAMP_HEAT_STADY_STATE_OPERATIONAL : cmaxcmin = ust.getOdpornoscWilgotneGoraceCmaxCmin(); break;
        case DAMP_HEAT_STADY_STATE_ENDURANCE : cmaxcmin = ust.getWytrzymaloscWilgotneGoraceCmaxCmin(); break;
        case VIBRATION : cmaxcmin = ust.getWytrzymaloscWibracjeCmaxCmin(); break;
        case IMPACT : cmaxcmin = ust.getOdpornoscUderzoniowaCmaxCmin(); break;
        case ELECTROMAGNETIC_RADIATED_ELEKTROMAGNETIC_FIELDS:
        case ELECTROMAGNETIC_CONDUCTED_DISTURBANCE_INDUCED:
        case ELECTROMAGNETIC_FAST_TRANSIENT_BURSTS:
        case ELECTROMAGNETIC_SLOW_HIGH_ENERGY_VOLTAGE_SURGES:
        case ELECTROMAGNETIC_ELEKTROSTATIC_DISCHARGE :
            cmaxcmin = ust.getOdpornoscElektroMagnetycznaCmaxCmin(); break;
        case SULPHUR_DIOXIDE_SO2_CORROSION: cmaxcmin = ust.getWytrzymaloscKorozyjnaSO2CmaxCmin(); break;
    default: break;
    }

    for (DanePomiaru & dane : getDanePomiarowe())
    {
        if (!dane.ok) {
            if (errStr.isEmpty())
                setErrStr(dane.error);
            setOk(false);
            setCmin(0);
            setCmax(0);
            setCmaxCmin(0);
            setWykonany(true);
            return;
        }
        bool ok;
        double C = dane.value_dB.toDouble(&ok);

        if (!ok) {
            if (errStr.isEmpty())
                setErrStr(QString::fromUtf8("Błędna wartość C"));
            dane.ok = false;
            setOk(false);
            setCmin(0);
            setCmax(0);
            setCmaxCmin(0);
            setWykonany(true);
            return;
        }

        if (!dane.ok) {
            if (errStr.isEmpty())
                setErrStr(QString::fromUtf8("Czujka nie zadziałała"));
            setOk(false);
            setCmin(0);
            setCmax(0);
            setCmaxCmin(0);
            setWykonany(true);
            return;
        }

        if (C > Cmax)
            Cmax = C;
        if (C < Cmin)
            Cmin = C;

        if (C < ust.getMinimalnaWartoscCzujkiCn()) {
            badanieOk = false;
            if (errStr.isEmpty())
                setErrStr(QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1));
            dane.ok = false;
            dane.error = QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1);
        }
    }

    if (!getWynikNarazenia()) {
        setOk(false);
        badanieOk = false;
        setErrStr(QString::fromUtf8("Czujka nie przeszła testu narażenia"));
    }

    if (Cmin) {
        setCmin(Cmin);
        setCmax(Cmax);
        setCmaxCmin(Cmax/Cmin);
    } else {
        setCmin(0);
        setCmax(0);
        setCmaxCmin(0);
    }
    setWykonany(true);

    if (getCmaxCmin() > cmaxcmin) {
        setOk(false);
        setErrStr(QString("Cmax/Cmin>%1").arg(cmaxcmin, 3, 'f', 2));
    } else {
        setOk(badanieOk);
    }
}

QString DaneTestu::getOpisNarazenia() const {
    QString narazenieOpis;
    switch(getId()) {
    case STRAY_LIGHT:
        narazenieOpis = QString::fromUtf8("Narażenia na światło rozproszone");
        break;
    case FIRE_SENSITIVITY:
        narazenieOpis = QString::fromUtf8("Narażenia na pożar");
        break;
    
    case DRY_HEAT :
        narazenieOpis = QString::fromUtf8("Narażenie na gorące i suche powietrze");
        break;

    case COLD :
        narazenieOpis = QString::fromUtf8("Narażenie na zimne powietrze");
        break;

    case DAMP_HEAT_STADY_STATE_OPERATIONAL :
        narazenieOpis = QString::fromUtf8("Narażenie na długotrwałe ciepłe i wilgotne powietrze");
        break;

    case DAMP_HEAT_STADY_STATE_ENDURANCE :
        narazenieOpis = QString::fromUtf8("Narażenie na krótkotrwałe gorące i wilgotne powietrze");
        break;

    case VIBRATION :
        narazenieOpis = QString::fromUtf8("Narażenie na wibracje");
        break;

    case IMPACT :
        narazenieOpis = QString::fromUtf8("Narażenie na uderzenie");
        break;

    case ELECTROMAGNETIC_RADIATED_ELEKTROMAGNETIC_FIELDS:
        narazenieOpis = QString::fromUtf8("Narażenie na promieniowanie pola elektromagnetycznego");
        break;

    case ELECTROMAGNETIC_CONDUCTED_DISTURBANCE_INDUCED:
        narazenieOpis = QString::fromUtf8("Narażenie na zakłócenia wywołane indukcją pola elektromagnetycznego");
        break;

    case ELECTROMAGNETIC_FAST_TRANSIENT_BURSTS:
        narazenieOpis = QString::fromUtf8("Narażenie na szybkie zmiany dużych poziomów pola elektromagnetycznego");
        break;

    case ELECTROMAGNETIC_SLOW_HIGH_ENERGY_VOLTAGE_SURGES:
        narazenieOpis = QString::fromUtf8("Narażenie na powolne skoki o dużej energii napięcia");
        break;

    case ELECTROMAGNETIC_ELEKTROSTATIC_DISCHARGE :
        narazenieOpis = QString::fromUtf8("Narażenie na wyładowania elektrostatyczne");
        break;

    case SULPHUR_DIOXIDE_SO2_CORROSION:
        narazenieOpis = QString::fromUtf8("Narażenie na dwutlenek siarki (S02) korozja");
        break;

    default:
        narazenieOpis = QString::fromUtf8("Inne narażenie");
    break;
    }
    return narazenieOpis;
}

float DaneTestu::getCmaxCminNarazenia(const Ustawienia & ust) const
{
    switch(getId()) {
    case STRAY_LIGHT:
        return ust.getRozproszoneSwiatloCmaxCmin();

    case DRY_HEAT :
        return ust.getOdpornoscSucheGoraceCmaxCmin();

    case COLD :
        return ust.getOdpornoscZimnoCmaxCmin();

    case DAMP_HEAT_STADY_STATE_OPERATIONAL :
        return  ust.getOdpornoscWilgotneGoraceCmaxCmin();

    case DAMP_HEAT_STADY_STATE_ENDURANCE :
        return ust.getWytrzymaloscWilgotneGoraceCmaxCmin();

    case VIBRATION :
        return ust.getWytrzymaloscWibracjeCmaxCmin();

    case IMPACT :
        return ust.getOdpornoscUderzoniowaCmaxCmin();

    case ELECTROMAGNETIC_RADIATED_ELEKTROMAGNETIC_FIELDS:
    case ELECTROMAGNETIC_CONDUCTED_DISTURBANCE_INDUCED:
    case ELECTROMAGNETIC_FAST_TRANSIENT_BURSTS:
    case ELECTROMAGNETIC_SLOW_HIGH_ENERGY_VOLTAGE_SURGES:
    case ELECTROMAGNETIC_ELEKTROSTATIC_DISCHARGE :
        return ust.getOdpornoscElektroMagnetycznaCmaxCmin();

    case SULPHUR_DIOXIDE_SO2_CORROSION:
        return ust.getWytrzymaloscKorozyjnaSO2CmaxCmin();
    default:
        return 1.6;
    }
}

const QString &DaneTestu::getNazwaTransmitter_a() const
{
    return nazwaTransmitter_a;
}

void DaneTestu::setNazwaTransmitter_a(const QString &newNazwaTransmitter_a)
{
    nazwaTransmitter_a = newNazwaTransmitter_a;
}

const QString &DaneTestu::getNazwaReceiver_a() const
{
    return nazwaReceiver_a;
}

void DaneTestu::setNazwaReceiver_a(const QString &newNazwaReceiver_a)
{
    nazwaReceiver_a = newNazwaReceiver_a;
}

const QString &DaneTestu::getMinimalneNapiecie() const
{
    return minimalneNapiecie;
}

void DaneTestu::setMinimalneNapiecie(const QString &newMinimalneNapiecie)
{
    minimalneNapiecie = newMinimalneNapiecie;
}

const QString &DaneTestu::getMaksymalneNapiecie() const
{
    return maksymalneNapiecie;
}

void DaneTestu::setMaksymalneNapiecie(const QString &newMaksymalneNapiecie)
{
    maksymalneNapiecie = newMaksymalneNapiecie;
}

DanePomiaru DaneTestu::getDaneDlaCzujki(const QString &nadajnik, const QString &odbiornik) const
{
    for(const auto & czujka : danePomiarowe) {
        if (czujka.numerNadajnika == nadajnik && czujka.numerOdbiornika == odbiornik)
            return czujka;
    }
    DanePomiaru p;
    p.error = "Nie znaleziono";
    p.ok = false;
    p.value_dB = "0";
    p.numerNadajnika = "";
    p.numerOdbiornika = "";
    p.nrCzujki = 0;
    p.nrSortCzujki = 0;

    return p;
}

short DaneTestu::getSortedPos(short sortedId) const
{
    if (id > danePomiarowe.size())
        return -1;

    short pos = 0;
    for(const auto & czujka : danePomiarowe) {
        if (czujka.nrSortCzujki == sortedId)
            return pos;
        ++pos;
    }
    return -1;
}

void DaneTestu::setDanePomiarowe(const DanePomiaru &podtw)
{
    if (danePomiarowe.size() != 0)
        return;
    danePomiarowe.append(podtw);
}

const QString &DaneTestu::getMinimalneRozstawienie() const
{
    return minimalneRozstawienie;
}

void DaneTestu::setMinimalneRozstawienie(const QString &newMinimalneRozstawienie)
{
    minimalneRozstawienie = newMinimalneRozstawienie;
}

const QString &DaneTestu::getMaksymalneRozstawienie() const
{
    return maksymalneRozstawienie;
}

void DaneTestu::setMaksymalneRozstawienie(const QString &newMaksymalneRozstawienie)
{
    maksymalneRozstawienie = newMaksymalneRozstawienie;
}

void DaneTestu::posortuj(ParametryBadania * badanie)
{

    if (getDanePomiarowe().size() < 2)
        return;

    float max1 = -1, max2 = -1;
    short pmax1 = -1, pmax2 = -1;
    short pos = -1;
    auto & wszystkieDane = getDanePomiarowe();
    for (DanePomiaru & dane : wszystkieDane) {
        ++pos;
        if (!dane.ok)
            continue;
        float val = dane.value_dB.toFloat();
        if (val > max1) {
            pmax1 = pos;
            max1 = val;
        }
    }

    pos = -1;
    for (DanePomiaru & dane : wszystkieDane) {
        ++pos;
        if (!dane.ok)
            continue;
        float val = dane.value_dB.toFloat();
        if (pos == pmax1) continue;
        if (val > max2) {
            pmax2 = pos;
            max2 = val;
        }
    }

    pos = -1;
    short nrPom = 1;
    QVector<QPair<QString, QString>> listaPosortowana;

    for (DanePomiaru & dane : wszystkieDane) {
        ++pos;
        if (pos == pmax1 || pos == pmax2) {
            continue;
        }
        if (!dane.ok)
            continue;
        dane.nrSortCzujki = nrPom++;
        listaPosortowana.push_back(qMakePair(dane.numerNadajnika, dane.numerOdbiornika));
    }
    if (pmax2 != -1) {
        wszystkieDane[pmax2].nrSortCzujki = nrPom++;
        listaPosortowana.push_back(qMakePair(wszystkieDane[pmax2].numerNadajnika, wszystkieDane[pmax2].numerOdbiornika));
    }
    if (pmax1 != -1) {
        wszystkieDane[pmax1].nrSortCzujki = nrPom++;
        listaPosortowana.push_back(qMakePair(wszystkieDane[pmax1].numerNadajnika, wszystkieDane[pmax1].numerOdbiornika));

    }
    badanie->setPosortowaneCzujki(listaPosortowana);


}

QString DaneTestu::getInfoNarazenia() const
{
    return infoNarazenia;
}

void DaneTestu::setInfoNarazenia(const QString &newInfoNarazenia)
{
    infoNarazenia = newInfoNarazenia;
}

bool DaneTestu::getWynikNarazenia() const
{
    return wynikNarazenia;
}

void DaneTestu::setWynikNarazenia(bool newWynikNarazenia)
{
    wynikNarazenia = newWynikNarazenia;
}
