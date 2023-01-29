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
    setIloscCzujek(0);

    setDrogaoptycznaCmaxCmin(0);
    setMaksymalneNapieciaTolerancjaNapiecia(0);
    setMaksymalnyCzasOczekiwaniaPowtarzalnosc1Test(0);
    setMinimalneNapieciaTolerancjaNapiecia(0);
    setMinimalnyCzasOczekiwaniaPowtarzalnosc1Test(0);
    setOdtwarzalnoscCmaxCrep(0);
    setOdtwarzalnoscCrepCmin(0);
    setOdtwarzalnoscCmin(0);
    setTolerancjaNapieciaZasilaniaCmaxCmin(0);

    numbersCzujki.clear();
    
    testy.clear();

    DaneTestu odtwarzalnosc;
    odtwarzalnosc.setId(REPRODUCIBILITY);
    odtwarzalnosc.setName("Odtwarzalność");
    testy[REPRODUCIBILITY] = odtwarzalnosc;

    DaneTestu powtarzalnosc;
    powtarzalnosc.setId(REPEATABILITY);
    powtarzalnosc.setName("Powtarzalność");
    testy[REPEATABILITY] = powtarzalnosc;

    DaneTestu zaleznosckierunkowa;
    zaleznosckierunkowa.setId(TOLERANCE_TO_BEAM_MISALIGNMENT);
    zaleznosckierunkowa.setName("Zależność kierunkowa");
    testy[TOLERANCE_TO_BEAM_MISALIGNMENT] = zaleznosckierunkowa;

    DaneTestu szybkiezmiany;
    szybkiezmiany.setId(RAPID_CHANGES_IN_ATTENUATION);
    szybkiezmiany.setName("Szybkie zmiany tłumienia");
    testy[RAPID_CHANGES_IN_ATTENUATION] = szybkiezmiany;


    DaneTestu dlugoscdrogioptycznej;
    dlugoscdrogioptycznej.setId(OPTICAL_PATH_LENGTH_DEPEDENCE);
    dlugoscdrogioptycznej.setName("Zależność od długości drogi optycznej");
    testy[OPTICAL_PATH_LENGTH_DEPEDENCE] = dlugoscdrogioptycznej;

    DaneTestu rozproszoneSwiatlo;
    rozproszoneSwiatlo.setId(STRAY_LIGHT);
    rozproszoneSwiatlo.setName("Rozproszone Światło");
    testy[STRAY_LIGHT] = rozproszoneSwiatlo;

    DaneTestu tolerancjaNapieciaZasilania;
    tolerancjaNapieciaZasilania.setId(TOLERANCE_TO_SUPPLY_VOLTAGE);
    tolerancjaNapieciaZasilania.setName("Zmienność parametrów zasilania");
    testy[TOLERANCE_TO_SUPPLY_VOLTAGE] = tolerancjaNapieciaZasilania;

    DaneTestu sucheCieplo;
    sucheCieplo.setId(DRY_HEAT);
    sucheCieplo.setName("Odporność na suche gorąco");
    testy[DRY_HEAT] = sucheCieplo;

    DaneTestu sucheZimno;
    sucheZimno.setId(COLD);
    sucheZimno.setName("Odporność na zimno");
    testy[COLD] = sucheZimno;
        
    DaneTestu odpornoscWilgotnoscGoraco;
    odpornoscWilgotnoscGoraco.setId(DAMP_HEAT_STADY_STATE_OPERATIONAL);
    odpornoscWilgotnoscGoraco.setName("Odporność na wilgotne gorąco, stan ustalony");
    testy[DAMP_HEAT_STADY_STATE_OPERATIONAL] = odpornoscWilgotnoscGoraco;

    DaneTestu wytrzymaloscWilgotnoscGoraco;
    wytrzymaloscWilgotnoscGoraco.setId(DAMP_HEAT_STADY_STATE_ENDURANCE);
    wytrzymaloscWilgotnoscGoraco.setName("Odporność na wilgotne gorąco, stan ustalony");
    testy[DAMP_HEAT_STADY_STATE_ENDURANCE] = wytrzymaloscWilgotnoscGoraco;
        
    DaneTestu wibracje;
    wibracje.setId(VIBRATION);
    wibracje.setName("Wytrzymałość na wibracje");
    testy[VIBRATION] = wibracje;        
    
    DaneTestu uderzenie;
    uderzenie.setId(IMPACT);
    uderzenie.setName("Odporność na uderzenie");
    testy[IMPACT] = uderzenie;        
    
    DaneTestu korozjaS02;
    korozjaS02.setId(SULPHUR_DIOXIDE_SO2_CORROSION);
    korozjaS02.setName("Wytrzymałość na korozję powodowaną przez dwutlenek siarki");
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
    numbersCzujki.clear();
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

void ParametryBadania::dodajCzujki(const QString &odbiornik, const QString &nadajnik)
{
    numbersCzujki.push_back(qMakePair(odbiornik, nadajnik));
}

QString ParametryBadania::getNumerTransmitter(unsigned int index, bool sorted) const
{
    if (!sorted && index >= numbersCzujki.size() )
        return QString();

    if ( sorted )
    {
        short sPos = getNumerSortedCzujki(index);
        if (sPos == -1)
            return QString();
        else
            return getNumerTransmitter(sPos, false);
    }
    else
        return numbersCzujki[index].first;
}

QString ParametryBadania::getNumerReceiver(unsigned int index, bool sorted) const
{
    if (!sorted && index >= numbersCzujki.size() )
        return QString();
    if ( sorted )
    {
        short sPos = getNumerSortedCzujki(index);
        if (sPos == -1)
            return QString();
        else
            return getNumerReceiver(sPos, false);
    }
    else
        return numbersCzujki[index].second;
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
    numbersCzujki.clear();
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
        numbersCzujki.clear();
        const auto & daneCzujek = dane.getDaneBadanCzujek();
        numbersCzujki = QVector<QPair<QString, QString>>(daneCzujek.size());
        setIloscCzujek(daneCzujek.size());
        for (short i = 0; i < daneCzujek.size(); ++i ) {
            const auto & pomiar = daneCzujek.at(i);
            numbersCzujki[pomiar.nrCzujki-1] = qMakePair(pomiar.numerNadajnika, pomiar.numerOdbiornika);
        }
    }
}
/*
void ParametryBadania::posortuj()
{
    short wyk[7] = { -1, -1, -1, -1, -1, -1, -1};

    if (!testy[REPRODUCIBILITY].getWykonany())
        return;

    if (testy[REPRODUCIBILITY].getDaneBadanCzujek().size() < 2) {
        return;
    }

    auto pomiary = testy[REPRODUCIBILITY].getDaneBadanCzujek();
    float max1 = 0, max2 = 0;
    short pmax1 = -1, pmax2 = -1;
    for (int pos = 0; pos < pomiary.size(); ++pos) {

        if (pomiary[pos].value_dB.isEmpty())
            continue;
        bool ok;
        float val = pomiary[pos].value_dB.toFloat(&ok);
        if (!ok) continue;
        wyk[pos] = pomiary[pos].nrCzujki;
        if (val >= max1) {
            pmax1 = pos;
            max1 = val;
        }
    }
    for (int pos = 0; pos < pomiary.size(); ++pos) {
        if (wyk[pos] == -1)
            continue;
        float val = pomiary[pos].value_dB.toFloat();

        if (pos == pmax1) continue;
        if (val >= max2) {
            pmax2 = pos;
            max2 = val;
        }
    }
    QVector<int> sorted;
    for (int pos = 0; pos < pomiary.size(); ++pos) {
        if (pos == pmax1 || pos == pmax2)
            continue;

        if (wyk[pos] == -1)
            continue;

        sorted.append(wyk[pos]);
    }
    if (pmax2 != -1)
        sorted.append(wyk[pmax2]);
    if (pmax1 != -1)
        sorted.append(wyk[pmax1]);

    for (int pos = 0; pos < pomiary.size(); ++pos) {
        pomiary[pos].nrSortCzujki = sorted[pos];
    }
}
*/

short ParametryBadania::getSortedId(short index) const
{
    if (index > testy[REPRODUCIBILITY].getDanePomiarowe().size())
        return -1;

    return testy[REPRODUCIBILITY].getDanePomiarowe()[index].nrSortCzujki;
}

DanePomiaru ParametryBadania::getDaneDlaCzujki(const QString &nadajnik, const QString &odbiornik) const
{
    if (!testy[REPRODUCIBILITY].getWykonany()) {
        DanePomiaru p ;
        p.error = "Nie wykonany";
        p.nrCzujki = -1;
        p.numerNadajnika = p.numerOdbiornika = "-";
        p.ok = false;
        p.value_dB = 0;
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



QDataStream &operator<<(QDataStream &out, const ParametryBadania &dane)
{
    dane.ParametryBadaniaGen::save(out);
    out << dane.numbersCzujki;
    out << dane.testy;
    return out;
}

QDataStream &operator>>(QDataStream &in, ParametryBadania &dane)
{
    dane.ParametryBadaniaGen::load(in);
    in >> dane.numbersCzujki;
    in >> dane.testy;
    return in;
}


