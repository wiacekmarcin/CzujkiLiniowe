#include "parametrybadania.h"
#include <QFile>

#include <QDebug>

ParametryBadania::ParametryBadania()
    : ParametryBadaniaGen()
{
    setCzasStabilizacjiCzujki_s(900);
    setCzasPomZmianaTlumenia_s(15);
    setCzasStabilizacjiCzujki_s(60);
    setCzasStabilizacjiPoResecie_s(15);
    setNapiecieZasilaniaCzujki_mV(24000);
    setPrzekroczeniePraduZasilania_mA("50");
    setZasilanieCzujekZasilaczZewnetrzny(true);
    setWyzwalanieAlarmuPrzekaznikiem(true);
    setMaksKatowaNieWspolPionowaNadajnika("0.0");
    setMaksKatowaNieWspolPionowaOdbiornika("0.0");
    setMaksKatowaNieWspolPoziomaNadajnika("0.0");
    setMaksKatowaNieWspolPoziomaOdbiornika("0.0");
    setRozstawienieMinCzujki("1.0");
    setRozstawienieMaxCzujki("10.0");
    setSystemOdbiornikNadajnik(true);
    setTestOdtwarzalnosci(false);
    setNazwaTransmitter("Nadajnik");
    setNazwaReceiver("Odbiornik");
    setNazwaNumerTransmitter("Numer nadajnika");
    setNazwaNumerReceiver("Numer odbiornika");
    setNazwaTransmitter_a("Nadajnika");
    setNazwaReceiver_a("Odbiornika");
    setNumerZlecenia("-");
    setNumerTestu("-");
    setOsobaOdpowiedzialna("-");
    setUwagi("");
    setHaslo("");
    setZasilanieCzujekCentrala(false);
    setZasilanieCzujekTypCentrali("-");

    setWyzwalanieAlarmuPradem(false);
    setDlugoscFaliFiltrow(880);
    setSystemOdbiornikNadajnik(true);
    setProducentCzujki("-");
    setTypTransmitter("-");
    setTypReceiver("-");

    setDrogaoptycznaCmaxCmin(0);
    setMaksymalneNapieciaTolerancjaNapiecia(0);
    setMaksymalnyCzasOczekiwaniaPowtarzalnosc1Test(0);
    setMinimalneNapieciaTolerancjaNapiecia(0);
    setMinimalnyCzasOczekiwaniaPowtarzalnosc1Test(0);
    setOdtwarzalnoscCmaxCrep(0);
    setOdtwarzalnoscCrepCmin(0);
    setOdtwarzalnoscCmin(0);
    setTolerancjaNapieciaZasilaniaCmaxCmin(0);

    posortowaneCzujki.clear();
    wszystkieCzujki.clear();
    
    testy.clear();

    DaneTestu odtwarzalnosc;
    odtwarzalnosc.setId(REPRODUCIBILITY);
    odtwarzalnosc.setName(QString::fromUtf8("Odtwarzalność"));
    testy[REPRODUCIBILITY] = odtwarzalnosc;

    DaneTestu powtarzalnosc;
    powtarzalnosc.setId(REPEATABILITY);
    powtarzalnosc.setName(QString::fromUtf8("Powtarzalność"));
    testy[REPEATABILITY] = powtarzalnosc;

    DaneTestu zaleznosckierunkowa;
    zaleznosckierunkowa.setId(TOLERANCE_TO_BEAM_MISALIGNMENT);
    zaleznosckierunkowa.setName(QString::fromUtf8("Zależność kierunkowa"));
    testy[TOLERANCE_TO_BEAM_MISALIGNMENT] = zaleznosckierunkowa;

    DaneTestu szybkiezmiany;
    szybkiezmiany.setId(RAPID_CHANGES_IN_ATTENUATION);
    szybkiezmiany.setName(QString::fromUtf8("Szybkie zmiany tłumienia"));
    testy[RAPID_CHANGES_IN_ATTENUATION] = szybkiezmiany;


    DaneTestu dlugoscdrogioptycznej;
    dlugoscdrogioptycznej.setId(OPTICAL_PATH_LENGTH_DEPEDENCE);
    dlugoscdrogioptycznej.setName(QString::fromUtf8("Zależność od długości drogi optycznej"));
    testy[OPTICAL_PATH_LENGTH_DEPEDENCE] = dlugoscdrogioptycznej;

    DaneTestu rozproszoneSwiatlo;
    rozproszoneSwiatlo.setId(STRAY_LIGHT);
    rozproszoneSwiatlo.setName(QString::fromUtf8("Rozproszone Światło"));
    testy[STRAY_LIGHT] = rozproszoneSwiatlo;

    DaneTestu tolerancjaNapieciaZasilania;
    tolerancjaNapieciaZasilania.setId(TOLERANCE_TO_SUPPLY_VOLTAGE);
    tolerancjaNapieciaZasilania.setName(QString::fromUtf8("Zmienność parametrów zasilania"));
    testy[TOLERANCE_TO_SUPPLY_VOLTAGE] = tolerancjaNapieciaZasilania;

    DaneTestu czuloscNaPozar;
    czuloscNaPozar.setId(FIRE_SENSITIVITY);
    czuloscNaPozar.setName(QString::fromUtf8("Czułość na pożar"));
    testy[FIRE_SENSITIVITY] = czuloscNaPozar;

    DaneTestu sucheCieplo;
    sucheCieplo.setId(DRY_HEAT);
    sucheCieplo.setName(QString::fromUtf8("Odporność na suche gorąco"));
    testy[DRY_HEAT] = sucheCieplo;

    DaneTestu sucheZimno;
    sucheZimno.setId(COLD);
    sucheZimno.setName(QString::fromUtf8("Odporność na zimno"));
    testy[COLD] = sucheZimno;
        
    DaneTestu odpornoscWilgotnoscGoraco;
    odpornoscWilgotnoscGoraco.setId(DAMP_HEAT_STADY_STATE_OPERATIONAL);
    odpornoscWilgotnoscGoraco.setName(QString::fromUtf8("Odporność na wilgotne gorąco, stan ustalony"));
    testy[DAMP_HEAT_STADY_STATE_OPERATIONAL] = odpornoscWilgotnoscGoraco;

    DaneTestu wytrzymaloscWilgotnoscGoraco;
    wytrzymaloscWilgotnoscGoraco.setId(DAMP_HEAT_STADY_STATE_ENDURANCE);
    wytrzymaloscWilgotnoscGoraco.setName(QString::fromUtf8("Odporność na wilgotne gorąco, stan ustalony"));
    testy[DAMP_HEAT_STADY_STATE_ENDURANCE] = wytrzymaloscWilgotnoscGoraco;
        
    DaneTestu wibracje;
    wibracje.setId(VIBRATION);
    wibracje.setName(QString::fromUtf8("Wytrzymałość na wibracje"));
    testy[VIBRATION] = wibracje;        
    
    DaneTestu uderzenie;
    uderzenie.setId(IMPACT);
    uderzenie.setName(QString::fromUtf8("Odporność na uderzenie"));
    testy[IMPACT] = uderzenie;        

    DaneTestu emc1;
    emc1.setId(ELECTROMAGNETIC_ELEKTROSTATIC_DISCHARGE);
    emc1.setName(QString::fromUtf8("Odporność na wyładowania elektrostatyczne"));
    testy[ELECTROMAGNETIC_ELEKTROSTATIC_DISCHARGE] = emc1;

    DaneTestu emc2;
    emc2.setId(ELECTROMAGNETIC_RADIATED_ELEKTROMAGNETIC_FIELDS);
    emc2.setName(QString::fromUtf8("Odporność na wypromieniowanie pola elektromagnetycznego"));
    testy[ELECTROMAGNETIC_RADIATED_ELEKTROMAGNETIC_FIELDS] = emc2;

    DaneTestu emc3;
    emc3.setId(ELECTROMAGNETIC_CONDUCTED_DISTURBANCE_INDUCED);
    emc3.setName(QString::fromUtf8("Odporność na przewodzone zakłocenia indukowane przez pola elektromagnetyczne"));
    testy[ELECTROMAGNETIC_CONDUCTED_DISTURBANCE_INDUCED] = emc3;

    DaneTestu emc4;
    emc4.setId(ELECTROMAGNETIC_FAST_TRANSIENT_BURSTS);
    emc4.setName(QString::fromUtf8("Odporność na szybkie impulsy przejściowe"));
    testy[ELECTROMAGNETIC_FAST_TRANSIENT_BURSTS] = emc4;

    DaneTestu emc5;
    emc5.setId(ELECTROMAGNETIC_SLOW_HIGH_ENERGY_VOLTAGE_SURGES);
    emc5.setName(QString::fromUtf8("Odporność na powolne udary napęciowe wysokiej energii"));
    testy[ELECTROMAGNETIC_SLOW_HIGH_ENERGY_VOLTAGE_SURGES] = emc5;

    DaneTestu korozjaS02;
    korozjaS02.setId(SULPHUR_DIOXIDE_SO2_CORROSION);
    korozjaS02.setName(QString::fromUtf8("Wytrzymałość na korozję powodowaną przez dwutlenek siarki"));
    testy[SULPHUR_DIOXIDE_SO2_CORROSION] = korozjaS02;        
}

void ParametryBadania::load(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        //qDebug() << "Could not open bin file for reading";
        return;
    }

    QDataStream in(&file);
    wszystkieCzujki.clear();
    posortowaneCzujki.clear();
    testy.clear();
    in >> *this;
    file.close();
}

void ParametryBadania::save(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open bin file for saving";
        return;
    }

    QDataStream out(&file);
    out << *this;
    file.close();
}

bool ParametryBadania::dodajCzujki(const QString &nadajnik, const QString &odbiornik)
{
    if (nadajnik.isEmpty() && odbiornik.isEmpty())
        return false;
    if (nadajnik == "" && odbiornik == "")
        return false;
    for (const auto & p : wszystkieCzujki) {
        if (p.first == nadajnik && p.second == odbiornik)
            return false;
    }
    wszystkieCzujki.push_back(qMakePair(nadajnik, odbiornik));
    posortowaneCzujki.push_back(qMakePair(nadajnik, odbiornik));
    return true;
}

QString ParametryBadania::getNumerTransmitter(unsigned int index) const
{
    //if (getTestOdtwarzalnosci()) {
        if ((int)index >= posortowaneCzujki.size())
            return QString();
        else
            return posortowaneCzujki[index].first;
    /*} else {
        if (index >= wszystkieCzujki.size() )
            return QString();
        else
            return wszystkieCzujki[index].first;
    }*/
}

QString ParametryBadania::getNumerReceiver(unsigned int index) const
{
    //if (getTestOdtwarzalnosci()) {
        if ((int)index >= posortowaneCzujki.size())
            return QString();
        else
            return posortowaneCzujki[index].second;
    /*} else {
        if (index >= wszystkieCzujki.size() )
            return QString();
        else
            return wszystkieCzujki[index].second;
    }*/
}

QString ParametryBadania::getNumerCzujki(const QString &nadajnik, const QString &odbiornik) const
{
    if (!testy[REPRODUCIBILITY].getWykonany()) {
        return "-";
    } else {
        return QString::number(testy[REPRODUCIBILITY].getDaneDlaCzujki(nadajnik, odbiornik).nrCzujki);
    }
}

QString ParametryBadania::getNumerSortedCzujki(const QString &nadajnik, const QString &odbiornik) const
{
    if (!testy[REPRODUCIBILITY].getWykonany()) {
        return "-";
    } else {
        return QString::number(testy[REPRODUCIBILITY].getDaneDlaCzujki(nadajnik, odbiornik).nrSortCzujki);
    }
}

short ParametryBadania::getNumerSortedCzujki(short sortId) const
{
    if (!testy[REPRODUCIBILITY].getWykonany()) {
        return -1;
    } else {
        return testy[REPRODUCIBILITY].getSortedPos(sortId);
    }
}

void ParametryBadania::wyczyscCzujki()
{
    wszystkieCzujki.clear();
}

const QMap<int, DaneTestu> &ParametryBadania::getTesty() const
{
    return testy;
}

void ParametryBadania::setDaneTestu(short id, const DaneTestu &dane)
{
    testy[id] = dane;
    if (id == REPRODUCIBILITY) {
        setTestOdtwarzalnosci(true);
        //wszystkieCzujki.clear();
        //const auto & daneCzujek = dane.getDaneBadanCzujek();
        //setIloscWszystkichCzujek(daneCzujek.size());
        //posortowaneCzujki = QVector<QPair<QString, QString>>(daneCzujek.size());
        //short cnt = 0;
        //for ( const auto & dane : daneCzujek) {
        //    if (dane.nrSortCzujki != 0) {
        //        posortowaneCzujki[dane.nrSortCzujki-1] = qMakePair(dane.numerNadajnika, dane.numerOdbiornika);
        //        ++cnt;
        //    }
        //    wszystkieCzujki.append(qMakePair(dane.numerNadajnika, dane.numerOdbiornika));
        //}
        //for (short r = cnt; r < daneCzujek.size(); ++r) {
        //    posortowaneCzujki.pop_back();
        //}
    }
}

short ParametryBadania::getSortedId(short index) const
{
    if (index > testy[REPRODUCIBILITY].getDaneBadanCzujek().size())
        return -1;

    return testy[REPRODUCIBILITY].getDaneBadanCzujek()[index].nrSortCzujki;
}

DanePomiaru ParametryBadania::getDaneDlaCzujki(const QString &nadajnik, const QString &odbiornik) const
{
    if (!testy[REPRODUCIBILITY].getWykonany()) {
        DanePomiaru p ;
        p.error = "Nie wykonany";
        p.nrCzujki = -1;
        p.numerNadajnika = p.numerOdbiornika = "-";
        p.ok = false;
        p.value_dB = "0";
        return p;
    } else {
        return testy[REPRODUCIBILITY].getDaneDlaCzujki(nadajnik, odbiornik);
    }
}

short ParametryBadania::getNumerPozycji(short posortowane)
{
    if (!testy[REPRODUCIBILITY].getWykonany()) {
        return -1;
    } else {

        return testy[REPRODUCIBILITY].getSortedPos(posortowane);
    }
}

void ParametryBadania::setPosortowaneCzujki(const QVector<QPair<QString, QString>> & posortowaneCzujki)
{
    this->posortowaneCzujki = posortowaneCzujki;
}

QVector<QPair<QString, QString> > ParametryBadania::getWszystkieCzujki() const
{
    return wszystkieCzujki;
}

QDataStream &operator<<(QDataStream &out, const ParametryBadania &dane)
{
    dane.ParametryBadaniaGen::save(out);
    out << dane.wszystkieCzujki;
    out << dane.posortowaneCzujki;
    out << dane.testy;
    return out;
}

QDataStream &operator>>(QDataStream &in, ParametryBadania &dane)
{
    dane.ParametryBadaniaGen::load(in);
    in >> dane.wszystkieCzujki;
    in >> dane.posortowaneCzujki;
    in >> dane.testy;
    return in;
}


