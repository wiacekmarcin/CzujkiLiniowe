#include "danetestu.h"
#include <QDate>
#include <QTime>


ListaTestow::ListaTestow()
{
    nazwyTestow << "Odtwarzalność" << "Powtarzalność" << "Zależność kierunkowa"
    << "Szybkie zmiany tłumienia" << "Zależność od długości drogi optycznej"
    << "Światło rozproszone" << "Zmiana parametrów zasilania";


    /*
    << "Czułość na pożar" <<  << "Odporność na suche gorąco"
    << "Odporność na zimno" << "Odporność na wilgotne gorąco, stan ustalony"
    << "Wytrzymałość na wilgotne gorąco, stan ustalony" << "Wytrzymałość na wibracje"
    << "Odporność na wyładowania elektrostatyczne"
    << "Odporność na wypromieniowanie pola elektromagnetycznego"
    << "Odporność na przewodzone zakłocenia indukowane przez pola elektromagnetyczne"
    << "Odporność na szybkie impulsy przejściowe"
    << "Odporność na powolne udary napęciowe wysokiej energii"
    << "Wytrzymałość na korozję powodowaną przez dwutlenek siarki"
    << "Odporność na uderzenie"*/
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
    qDebug() << __FILE__ << __LINE__ << kat.katProducenta << kat.ok << kat.katZmierzony;
    pomiaryKatow.append(kat);
}

void DaneTestu::obliczZaleznoscKatowa(const Ustawienia &ust)
{
    for (auto & wynik : pomiaryKatow) {
        if (wynik.ok) {
            double val = wynik.katZmierzony - wynik.katProducenta;
            double delta = ust.getNiewspolosiowoscMinimalnyKatProducentMierzony();
            if (val < delta || val < -delta) {
                wynik.ok = false;
                wynik.errorStr = QString::fromUtf8("Różnica<%1").arg(delta,2,'f',1);
            }
        }
    }
}

void DaneTestu::obliczOdtwarzalnosc(const Ustawienia & ust)
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

        if (!ok) {
            badanieOk = false;
            setErrStr(QString::fromUtf8("Błędna wartość C"));
            dane.ok = false;
            continue;
        } else {
            if (C > Cmax)
                Cmax = C;
            if (C < Cmin)
                Cmin = C;

            Cavg += C;
            ++cntAvg;
        }

        if (!dane.ok) {
            badanieOk = false;
            setErrStr(dane.error);
            continue;
        }

        if (C < ust.getMinimalnaWartoscCzujkiCn()) {
            badanieOk = false;
            setErrStr(QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1));
            dane.ok = false;
            dane.error = QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1);
        }
    }
    if (cntAvg)
        Cavg = Cavg/cntAvg;

    setOk(badanieOk);
    setErrStr(badanieOk ? "" : "Czujka(i) nie przeszły badania");

    if (cntAvg && Cavg) {
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

    if (getCmaxCmin() > ust.getPowtarzalnoscCmaxCmin()) {
        setOk(false);
        setErrStr(QString("Cmax/Cmin>%1").arg(ust.getPowtarzalnoscCmaxCmin(), 3, 'f', 2));
    } else {
        setOk(badanieOk);
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

    if (getCmaxCmin() > ust.getDlugoscDrogiOptycznejCmaxCmin()) {
        setOk(false);
        setErrStr(QString("Cmax/Cmin>%1").arg(ust.getDlugoscDrogiOptycznejCmaxCmin(), 3, 'f', 2));
    } else {
        setOk(badanieOk);
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

    if (getCmaxCmin() > ust.getTolerancjaNapieciaZasilaniaCmaxCmin()) {
        setOk(false);
        setErrStr(QString("Cmax/Cmin>%1").arg(ust.getTolerancjaNapieciaZasilaniaCmaxCmin(), 3, 'f', 2));
    } else {
        setOk(badanieOk);
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

DanePomiaru DaneTestu::getDaneDlaCzujki(const QString &nadajnik, const QString &odbiornik)
{
    for(const auto & czujka : danePomiarowe) {
        if (czujka.numerNadajnika == nadajnik && czujka.numerOdbiornika == odbiornik)
            return czujka;
    }
    DanePomiaru p;
    p.error = "Nie znaleziono";
    p.ok = false;
    p.value_dB = 0;
    p.numerNadajnika = "";
    p.numerOdbiornika = "";
    p.nrCzujki = 0;

    return p;
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
