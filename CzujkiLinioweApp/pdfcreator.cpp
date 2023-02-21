#include "pdfcreator.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include <hpdf.h>
#include <QTextCodec>
#include <QString>
#include <QTextCodec>
#include <QDebug>
#include <QDateTime>

#include "parametrybadania.h"
jmp_buf env;

static const int PAGE_WIDTH = 420;
static const int PAGE_HEIGHT = 400;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler (HPDF_STATUS   error_no,
               HPDF_STATUS   detail_no,
               void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);
    PdfCreator::error = QString("ERROR: error_no=%1, detail_no=%2").arg((HPDF_UINT)error_no, 16).arg((HPDF_UINT)detail_no);
    longjmp(env, 1);
}

QString PdfCreator::error;

PdfCreator::PdfCreator()
{
    codec = QTextCodec::codecForName("ISO 8859-2");
    urzadzenie = codec->fromUnicode(QString::fromUtf8("Stanowisko do badania czujek liniowych dymu - BCL, SN:1-23"));
}

void PdfCreator::setData(const ParametryBadania & badanie, bool all, short id = 0)
{
    numerZlecenia = codec->fromUnicode(badanie.getNumerZlecenia());
    numerBadania = codec->fromUnicode(badanie.getNumerTestu());
    osobaOdpowiedzialna1 = codec->fromUnicode(badanie.getOsobaOdpowiedzialna());
    uwagi1 = codec->fromUnicode(badanie.getUwagi());
    dataBadania = codec->fromUnicode(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"));

    if (badanie.getSystemOdbiornikNadajnik()) {
        typSystemu = codec->fromUnicode(QString::fromUtf8("Nadajnik + Odbiornik"));
    } else {
        typSystemu = codec->fromUnicode(QString::fromUtf8("Nadajnik-Odbiornik + Reflektor"));
    }

    producent = codec->fromUnicode(badanie.getProducentCzujki());

    eTypTransmitter = codec->fromUnicode(QString::fromUtf8("Typ %1").arg(badanie.getNazwaTransmitter_a()));
    eTypReceiver = codec->fromUnicode(QString::fromUtf8("Typ %1").arg(badanie.getNazwaReceiver_a()));
    eTransmitter = codec->fromUnicode(badanie.getNazwaTransmitter());
    eReceiver = codec->fromUnicode(badanie.getNazwaReceiver());
    eNrTransmitter = codec->fromUnicode(badanie.getNazwaNumerTransmitter());
    eNrReceiver = codec->fromUnicode(badanie.getNazwaNumerReceiver());

    typTransmitter = codec->fromUnicode(badanie.getTypTransmitter());
    typReceiver = codec->fromUnicode(badanie.getTypReceiver());
    rozstawienieMinimalne = codec->fromUnicode(QString::fromUtf8("%1 m").arg(badanie.getRozstawienieMinCzujki()).replace('.', ','));
    rozstawienieMaksymalne = codec->fromUnicode(QString::fromUtf8("%1 m").arg(badanie.getRozstawienieMaxCzujki()).replace('.', ','));

    nadajnikKatPionowy = codec->fromUnicode(badanie.getMaksKatowaNieWspolPionowaNadajnika().replace('.',',')+ ' ' + QChar(0xb0));
    nadajnikKatPoziomy = codec->fromUnicode(badanie.getMaksKatowaNieWspolPoziomaNadajnika().replace('.',',')+ ' ' +QChar(0xb0));
    odbiornikKatPionowy = codec->fromUnicode(badanie.getMaksKatowaNieWspolPionowaOdbiornika().replace('.',',')+ ' ' +QChar(0xb0));
    odbiornikKatPoziomy = codec->fromUnicode(badanie.getMaksKatowaNieWspolPoziomaOdbiornika().replace('.',',')+ ' ' +QChar(0xb0));

    zasilaczZewnetrzny = badanie.getZasilanieCzujekZasilaczZewnetrzny();
    zasilaczCentrala = badanie.getZasilanieCzujekCentrala();
    wartoscNapieciaZasilania = codec->fromUnicode(QString::fromUtf8("%1 V").
                                                  arg(0.001*badanie.getNapiecieZasilaniaCzujki_mV(), 7, 'f', 3).
                                                  replace('.', ','));
    nazwaCentraliPPOZ = codec->fromUnicode(badanie.getZasilanieCzujekTypCentrali());
    wyzwalaniePradem = badanie.getWyzwalanieAlarmuPradem();
    wyzwalaniePrzekaznikiem = badanie.getWyzwalanieAlarmuPrzekaznikiem();
    wartoscPrądu = codec->fromUnicode(QString("%1 mA").arg(badanie.getPrzekroczeniePraduZasilania_mA()).replace('.', ','));
    czasStabilizacjiCzujki = codec->fromUnicode(QString("%1 s").arg(badanie.getCzasStabilizacjiCzujki_s()));
    czasStabilizacjiPoResecie = codec->fromUnicode(QString("%1 s").arg(badanie.getCzasStabilizacjiPoResecie_s()));

    for (const auto & czujka : badanie.getWszystkieCzujki()) {
        CzujkaInfo cz;
        cz.oznaczenie = codec->fromUnicode(badanie.getNumerSortedCzujki(czujka.first, czujka.second));
        cz.transmiter = codec->fromUnicode(czujka.first);
        cz.receiver = codec->fromUnicode(czujka.second);
        czujki << cz;
    }


    short NrTestu = 1;
    for (const auto & test : badanie.getTesty()) {
        if (!test.getWykonany())
            continue;

        if (all || test.getId() == id) {
            TestWszystko daneTestu;
            daneTestu.odtwarzalnosc = test.getId() == REPRODUCIBILITY;

            daneTestu.testName = getTestName(test, NrTestu++);

            daneTestu.ogolne.osobaWykonujaca = codec->fromUnicode(test.getOsobaWykonujaca());
            daneTestu.ogolne.dataRozpoczecia = codec->fromUnicode(test.getDataRozpoczecia());
            daneTestu.ogolne.dataZakonczenia = codec->fromUnicode(test.getDataZakonczenia());
            daneTestu.ogolne.temperatura = codec->fromUnicode(QString("%1 %2C").arg(test.getTemperatura(),QChar(0xb0)).replace('.',','));
            daneTestu.ogolne.cisnienie = codec->fromUnicode(QString("%1 hPa").arg(test.getCisnienie()).replace('.',','));
            daneTestu.ogolne.wilgotnosc = codec->fromUnicode(QString("%1 %").arg(test.getWilgotnosc()).replace('.',','));
            daneTestu.ogolne.uwagi = codec->fromUnicode(test.getUwagi());
            daneTestu.ogolne.receiver = codec->fromUnicode(test.getNumerReceiver());
            daneTestu.ogolne.transmiter = codec->fromUnicode(test.getNumerTransmitter());
            if (test.getOk()) {
                daneTestu.ogolne.wynikTestu = codec->fromUnicode(QString::fromUtf8("POZYTYWNY"));
            } else {
                daneTestu.ogolne.wynikTestu = codec->fromUnicode(QString::fromUtf8("NEGATYWNY - %1").arg(test.getErrStr()));
            }
            daneTestu.wynikiC.CMaxCmin = codec->fromUnicode(QString::fromUtf8("%1").arg(test.getCmaxCmin(), 4, 'f', 2).replace('.',','));
            daneTestu.wynikiC.CMaxCrep = codec->fromUnicode(QString::fromUtf8("%1").arg(test.getCmaxCrep(), 4, 'f', 2).replace('.',','));
            daneTestu.wynikiC.CRepCMin = codec->fromUnicode(QString::fromUtf8("%1").arg(test.getCrepCmin(), 4, 'f', 2).replace('.',','));
            daneTestu.wynikiC.Cmin1 = codec->fromUnicode(QString::fromUtf8("%1 dB").arg(test.getCmin(), 4, 'f', 2).replace('.',','));
            daneTestu.wynikiC.Cmax1 = codec->fromUnicode(QString::fromUtf8("%1 dB").arg(test.getCmax(), 4, 'f', 2).replace('.',','));
            daneTestu.wynikiC.Crep1 = codec->fromUnicode(QString::fromUtf8("%1 dB").arg(test.getCrep(), 4, 'f', 2).replace('.',','));
            daneTestu.wynikiC.Cmin2 = codec->fromUnicode(QString::fromUtf8("%1 %").arg(d2p(test.getCmin()), 4, 'f', 2).replace('.',','));
            daneTestu.wynikiC.Cmax2 = codec->fromUnicode(QString::fromUtf8("%1 %").arg(d2p(test.getCmax()), 4, 'f', 2).replace('.',','));
            daneTestu.wynikiC.Crep2 = codec->fromUnicode(QString::fromUtf8("%1 %").arg(d2p(test.getCrep()), 4, 'f', 2).replace('.',','));
            setTableData(test, daneTestu.dane,
                             daneTestu.narazenie, daneTestu.czyNarazenie,
                             daneTestu.narazenie2, daneTestu.czyNarazenie2,
                             daneTestu.showC);

            testy << daneTestu;
        }
    }
}

QByteArray PdfCreator::getTestName(const DaneTestu & test, short NrTestu)
{
    return codec->fromUnicode(QString::fromUtf8("TEST NR %1: %2").arg(NrTestu).arg(test.getName()));
}

void PdfCreator::setTableData(const DaneTestu &test, TabelaDane &dane,
                              NarazenieDane & narazenie, bool &czyNarazenie,
                              NarazenieDane2 & narazenie2, bool &czyNarazenie2,
                              bool & showC)
{
    if (test.getId() == REPRODUCIBILITY) {
        czyNarazenie = false;
        czyNarazenie2 = false;
        showC = true;
        dane.leftMargin = 30;
        dane.head << codec->fromUnicode(QString::fromUtf8("Próba"));
        dane.head << eTransmitter;
        dane.head << eReceiver;
        dane.head << codec->fromUnicode(QString::fromUtf8("C[n]"));
        dane.head << codec->fromUnicode(QString::fromUtf8("C[n]"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Wynik"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Uwagi"));

        dane.colwidth << 40 << 100 << 100 << 65 << 55 << 70 << 100;
        dane.colAlign << 0  << -1  << -1  << 1  << 1  << 0  << -1;

        short num = 0;
        for (const auto & d : test.getDaneBadanCzujek())
        {
            QVector<QByteArray> row;
            row << codec->fromUnicode(QString::fromUtf8("%1").arg(num+1));
            row << codec->fromUnicode(d.numerNadajnika);
            row << codec->fromUnicode(d.numerOdbiornika);
            row << codec->fromUnicode(QString::fromUtf8("%1 dB").arg(d.value_dB).replace('.',','));
            row << codec->fromUnicode(QString::fromUtf8("%1 %").arg(d2p(d.value_dB)).replace('.',','));
            if (d.ok) {
                row << codec->fromUnicode(QString::fromUtf8("POPRAWNY")) << "-";
            }
            else {
                row << codec->fromUnicode(QString::fromUtf8("NIE POPRAWNY")) << codec->fromUnicode(d.error);
            }
            dane.dane << row;
            ++num;
        }
        for (short s = num; s < 7; ++s) {
            QVector<QByteArray> row;
            row << "-";
            row << "-";
            row << "-";
            row << "-";
            row << "-";
            row << "-";
            row << "-";
            dane.dane << row;
        }
    } else if (test.getId() == REPEATABILITY) {
        czyNarazenie = false;
        czyNarazenie2 = false;
        showC = true;
        dane.leftMargin = 50;
        dane.head << codec->fromUnicode(QString::fromUtf8("Próba"));
        dane.head << codec->fromUnicode(QString::fromUtf8("C[n]"));
        dane.head << codec->fromUnicode(QString::fromUtf8("C[n]"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Wynik"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Uwagi"));

        dane.colwidth << 40 << 90 << 90 << 80 << 200;
        dane.colAlign << 0  << 1  << 1  << 0  << -1;

        short num = 0;
        for (const auto & d : test.getDaneBadanCzujek())
        {
            QVector<QByteArray> row;
            row << codec->fromUnicode(QString::fromUtf8("%1").arg(num+1));
            row << codec->fromUnicode(QString::fromUtf8("%1 dB").arg(d.value_dB).replace('.',','));
            row << codec->fromUnicode(QString::fromUtf8("%1 %").arg(d2p(d.value_dB)).replace('.',','));
            if (d.ok) {
                row << codec->fromUnicode(QString::fromUtf8("POPRAWNY")) << "-";
            }
            else {
                row << codec->fromUnicode(QString::fromUtf8("NIE POPRAWNY")) << codec->fromUnicode(d.error);
            }
            dane.dane << row;
            num++;
        }
    } else if (test.getId() == TOLERANCE_TO_BEAM_MISALIGNMENT) {
        czyNarazenie = false;
        czyNarazenie2 = false;
        showC = true;
        dane.leftMargin = 20;
        dane.head << codec->fromUnicode(QString::fromUtf8("Nazwa pomiaru"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Kąt prod."));
        dane.head << codec->fromUnicode(QString::fromUtf8("Kąt zmierz."));
        dane.head << codec->fromUnicode(QString::fromUtf8("Wynik"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Błąd"));

        dane.colwidth << 150 << 70 << 70 << 80 << 150;
        dane.colAlign << 0   << 1  << 1  << 0  << -1;

        for (const auto & wynik : test.getPomiaryKatow()) {
            QVector<QByteArray> row;
            row << codec->fromUnicode(wynik.nazwaBadania);

            row << codec->fromUnicode(QString("%1 %2").arg(wynik.katProducenta, 4, 'f', 2).arg(QChar(0xb0)).replace('.',','));
            row << codec->fromUnicode(QString("%1 %2").arg(wynik.katZmierzony, 4, 'f', 2).arg(QChar(0xb0)).replace('.',','));
            if (wynik.ok) {
                row << codec->fromUnicode(QString::fromUtf8("POPRAWNY")) << "-";
            }
            else {
                row << codec->fromUnicode(QString::fromUtf8("NIE POPRAWNY")) << codec->fromUnicode(wynik.errorStr);
            }
            dane.dane << row;
        }
    } else if (test.getId() == RAPID_CHANGES_IN_ATTENUATION) {
        czyNarazenie = false;
        czyNarazenie2 = false;
        showC = false;
        dane.leftMargin = 50;

        dane.head << codec->fromUnicode(QString::fromUtf8("Próba"));
        dane.head << codec->fromUnicode(QString::fromUtf8("C[n]"));
        dane.head << codec->fromUnicode(QString::fromUtf8("C[n]"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Wynik"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Uwagi"));

        dane.colwidth << 40 << 70 << 70 << 80 << 250;
        dane.colAlign << 0  << 1  << 1  << 0  << -1;

        short num = 0;
        for (const auto & d : test.getDaneBadanCzujek())
        {
            QVector<QByteArray> row;
            row << codec->fromUnicode(QString::fromUtf8("%1").arg(num+1));
            row << codec->fromUnicode(QString::fromUtf8("%1 dB").arg(d.value_dB).replace('.',','));
            row << codec->fromUnicode(QString::fromUtf8("%1 %").arg(d2p(d.value_dB)).replace('.',','));
            if (d.ok) {
                row << codec->fromUnicode(QString::fromUtf8("POPRAWNY")) << "-";
            }
            else {
                row << codec->fromUnicode(QString::fromUtf8("NIE POPRAWNY")) << codec->fromUnicode(d.error);
            }
            dane.dane << row;
            num++;
        }
    } else if (test.getId() == OPTICAL_PATH_LENGTH_DEPEDENCE) {
        czyNarazenie = false;
        czyNarazenie2 = false;
        showC = true;
        dane.leftMargin = 40;
        dane.head << codec->fromUnicode(QString::fromUtf8("Próba"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Rozstawienie"));
        dane.head << codec->fromUnicode(QString::fromUtf8("C[n]"));
        dane.head << codec->fromUnicode(QString::fromUtf8("C[n]"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Wynik"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Uwagi"));

        dane.colwidth << 40 << 90 << 70 << 70 << 80 << 180;
        dane.colAlign << 0  << 1  << 1  << 1  << 0  << -1;

        short num = 0;
        for (const auto & d : test.getDaneBadanCzujek())
        {
            QVector<QByteArray> row;
            row << codec->fromUnicode(QString::fromUtf8("%1").arg(num+1));
            if (num == 0) {
                row << codec->fromUnicode(QString::fromUtf8("%1 m").arg(test.getMinimalneRozstawienie()).replace('.',','));
            } else {
                row << codec->fromUnicode(QString::fromUtf8("%1 m").arg(test.getMaksymalneRozstawienie()).replace('.',','));
            }
            row << codec->fromUnicode(QString::fromUtf8("%1 dB").arg(d.value_dB).replace('.',','));
            row << codec->fromUnicode(QString::fromUtf8("%1 %").arg(d2p(d.value_dB)).replace('.',','));
            if (d.ok) {
                row << codec->fromUnicode(QString::fromUtf8("POPRAWNY")) << "-";
            }
            else {
                row << codec->fromUnicode(QString::fromUtf8("NIE POPRAWNY")) << codec->fromUnicode(d.error);
            }
            dane.dane << row;
            num++;
        }
    } else if (test.getId() == STRAY_LIGHT) {
        czyNarazenie2 = false;
        narazeniaWynik(test, dane, narazenie, czyNarazenie, showC);
    } else if (test.getId() == TOLERANCE_TO_SUPPLY_VOLTAGE) {
        czyNarazenie = false;
        czyNarazenie2 = false;
        showC = true;
        dane.leftMargin = 40;
        dane.head << codec->fromUnicode(QString::fromUtf8("Próba"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Napięcie zasilania"));
        dane.head << codec->fromUnicode(QString::fromUtf8("C[n]"));
        dane.head << codec->fromUnicode(QString::fromUtf8("C[n]"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Wynik"));
        dane.head << codec->fromUnicode(QString::fromUtf8("Uwagi"));

        dane.colwidth << 40 << 100 << 70 << 70 << 80 << 160;
        dane.colAlign << 0  << 1    << 1  << 1  << 0  << -1;

        short num = 0;
        for (const auto & d : test.getDaneBadanCzujek())
        {
            QVector<QByteArray> row;
            row << codec->fromUnicode(QString::fromUtf8("%1").arg(num+1));
            if (num == 0) {
                row << codec->fromUnicode(QString::fromUtf8("%1 V").arg(test.getMinimalneNapiecie()).replace('.',','));
            } else {
                row << codec->fromUnicode(QString::fromUtf8("%1 V").arg(test.getMaksymalneNapiecie()).replace('.',','));
            }
            row << codec->fromUnicode(QString::fromUtf8("%1 dB").arg(d.value_dB).replace('.',','));
            row << codec->fromUnicode(QString::fromUtf8("%1 %").arg(d2p(d.value_dB)).replace('.',','));
            if (d.ok) {
                row << codec->fromUnicode(QString::fromUtf8("POPRAWNY")) << "-";
            }
            else {
                row << codec->fromUnicode(QString::fromUtf8("NIE POPRAWNY")) << codec->fromUnicode(d.error);
            }
            dane.dane << row;
            num++;
        }
    }

    else if (test.getId() == FIRE_SENSITIVITY) {
        czyNarazenie = false;
        czyNarazenie2 = false;
        showC = false;
        dane.leftMargin = 40;

        const auto & dane = test.getDaneBadanCzujek();
        if (dane.size() > 1)  {
            czyNarazenie2 = true;
            narazenie2.first.wynik = dane[0].ok ? codec->fromUnicode(QString::fromUtf8("POZYTYWNY")) : codec->fromUnicode(QString::fromUtf8("NEGATYWNY"));
            narazenie2.first.transmiter = codec->fromUnicode(dane[0].numerNadajnika);
            narazenie2.first.receiver = codec->fromUnicode(dane[0].numerOdbiornika);
            narazenie2.first.oznaczenie = codec->fromUnicode(QString::number(dane[0].nrSortCzujki));
            narazenie2.first.opis = codec->fromUnicode(test.getOpisNarazenia());
            narazenie2.first.uwagi = codec->fromUnicode(dane[0].error);
            narazenie2.second.wynik = dane[1].ok ? codec->fromUnicode(QString::fromUtf8("POZYTYWNY")) : codec->fromUnicode(QString::fromUtf8("NEGATYWNY"));
            narazenie2.second.transmiter = codec->fromUnicode(dane[1].numerNadajnika);
            narazenie2.second.receiver = codec->fromUnicode(dane[1].numerOdbiornika);
            narazenie2.second.oznaczenie = codec->fromUnicode(QString::number(dane[1].nrSortCzujki));
            narazenie2.second.opis = codec->fromUnicode(test.getOpisNarazenia());
            narazenie2.second.uwagi = codec->fromUnicode(dane[1].error);
        }
    }

    else if (test.getId() == DRY_HEAT) {
        czyNarazenie2 = false;
        narazeniaWynik(test, dane, narazenie, czyNarazenie, showC);
    }

    else if (test.getId() == COLD) {
        czyNarazenie2 = false;
        narazeniaWynik(test, dane, narazenie, czyNarazenie, showC);
    }

    else if (test.getId() == DAMP_HEAT_STADY_STATE_OPERATIONAL) {
        narazeniaWynik(test, dane, narazenie, czyNarazenie, showC);
    }

    else if (test.getId() == DAMP_HEAT_STADY_STATE_ENDURANCE) {
        czyNarazenie2 = false;
        narazeniaWynik(test, dane, narazenie, czyNarazenie, showC);
    }

    else if (test.getId() == VIBRATION) {
        czyNarazenie2 = false;
        narazeniaWynik(test, dane, narazenie, czyNarazenie, showC);
    }

    else if (test.getId() == IMPACT) {
        czyNarazenie2 = false;
        narazeniaWynik(test, dane, narazenie, czyNarazenie, showC);
    }

    else if (test.getId() == ELECTROMAGNETIC_ELEKTROSTATIC_DISCHARGE) {
        czyNarazenie2 = false;
        narazeniaWynik(test, dane, narazenie, czyNarazenie, showC);
    }

    else if (test.getId() == ELECTROMAGNETIC_RADIATED_ELEKTROMAGNETIC_FIELDS) {
        czyNarazenie2 = false;
        narazeniaWynik(test, dane, narazenie, czyNarazenie, showC);
    }

    else if (test.getId() == ELECTROMAGNETIC_CONDUCTED_DISTURBANCE_INDUCED) {
        czyNarazenie2 = false;
        narazeniaWynik(test, dane, narazenie, czyNarazenie, showC);
    }

    else if (test.getId() == ELECTROMAGNETIC_FAST_TRANSIENT_BURSTS) {
        czyNarazenie2 = false;
        narazeniaWynik(test, dane, narazenie, czyNarazenie, showC);
    }

    else if (test.getId() == ELECTROMAGNETIC_SLOW_HIGH_ENERGY_VOLTAGE_SURGES) {
        czyNarazenie2 = false;
        narazeniaWynik(test, dane, narazenie, czyNarazenie, showC);
    }

    else if (test.getId() == SULPHUR_DIOXIDE_SO2_CORROSION) {
        czyNarazenie2 = false;
        narazeniaWynik(test, dane, narazenie, czyNarazenie, showC);
    }
}

void PdfCreator::narazeniaWynik(const DaneTestu &test, TabelaDane &dane,
                                NarazenieDane &narazenie, bool &czyNarazenie, bool & showC)
{
    dane.leftMargin = 40;
    czyNarazenie = true;
    showC = true;

    dane.head << codec->fromUnicode(QString::fromUtf8("Stan czujki"));
    dane.head << codec->fromUnicode(QString::fromUtf8("C[n]"));
    dane.head << codec->fromUnicode(QString::fromUtf8("C[n]"));
    dane.head << codec->fromUnicode(QString::fromUtf8("Wynik"));
    dane.head << codec->fromUnicode(QString::fromUtf8("Uwagi"));

    dane.colwidth << 120 << 70 << 70 << 80 << 180;
    dane.colAlign << 0   << 1  << 1  << 0  << -1;

    short num = 0;
    for (const auto & d : test.getDaneBadanCzujek())
    {
        QVector<QByteArray> row;
        if (num == 0) {
            row << codec->fromUnicode(QString::fromUtf8("Przed narażeniem"));
        } else {
            row << codec->fromUnicode(QString::fromUtf8("Po narażeniu"));
        }
        row << codec->fromUnicode(QString::fromUtf8("%1 dB").arg(d.value_dB).replace('.',','));
        row << codec->fromUnicode(QString::fromUtf8("%1 %").arg(d2p(d.value_dB)).replace('.',','));
        if (d.ok) {
            row << codec->fromUnicode(QString::fromUtf8("POPRAWNY")) << "-";
        }
        else {
            row << codec->fromUnicode(QString::fromUtf8("NIE POPRAWNY")) << codec->fromUnicode(d.error);
        }
        dane.dane << row;
        num++;
    }


    narazenie.wynik = test.getWynikNarazenia() ? codec->fromUnicode(QString::fromUtf8("POZYTYWNY")) :
                                                 codec->fromUnicode(QString::fromUtf8("NEGATYWNY"));
    narazenie.uwagi = codec->fromUnicode(test.getInfoNarazenia());
    narazenie.opis = codec->fromUnicode(test.getOpisNarazenia());
}



void PdfCreator::create(const QString & data)
{


    HPDF_Doc  pdf;
    HPDF_Font font, font2;
    HPDF_Page page;


    pdf = HPDF_NewEx (error_handler,  NULL, NULL, 0, NULL);
    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
        errCode = -1;
        return;
    }

    if (setjmp(env)) {
        printf ("error: setjump");
        HPDF_Free (pdf);
        errCode = -1;
        return;
    }

    /* set compression mode */
    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

    /* set encoding */
    HPDF_SetCurrentEncoder(pdf, "ISO8859-2");

    /* get default font */
    font = HPDF_GetFont (pdf, HPDF_LoadTTFontFromFile(pdf, "czcionka1.ttf", false), "ISO8859-2");
    font2 = HPDF_GetFont (pdf, HPDF_LoadTTFontFromFile(pdf, "czcionka2.ttf", false), "ISO8859-2");

    iloscStron = testy.size() + 1;

    /* add a new main page object. */
    page = HPDF_AddPage (pdf);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 8);
    QByteArray strona = codec->fromUnicode(QString::fromUtf8("Strona 1 z %1").arg(iloscStron));
    float tw = HPDF_Page_TextWidth (page, strona.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw) / 2,
                25, strona.data());
    HPDF_Page_EndText (page);

    HPDF_Image image = HPDF_LoadJpegImageFromFile (pdf, "logo.jpg");

    /* Draw image to the canvas. */
    //HPDF_Page_DrawImage (page, image, x, 0, HPDF_Image_GetWidth (image),
    //            HPDF_Image_GetHeight (image));
    HPDF_Page_DrawImage (page, image, (HPDF_Page_GetWidth(page) - 60) / 2,
                         HPDF_Page_GetHeight(page)-70, 60, 60);



    /* draw grid to the page */
    //print_grid  (pdf, page);
    float endY = HPDF_Page_GetHeight(page)-70;
    endY = createHead(page, font, endY);
    endY = createInfoBadanie(page, font, font2, endY - 32, true, true);
    endY = createInformacje(page, font, font2, endY);
    endY = createInformacjeKat(page, font, font2, endY - 20);
    endY = createTablicaCzujek(page, font, font2, endY - 20);
    endY = createDodatkoweParametry(page, font, font2, endY - 20);

    for (short n = 0; n < testy.size(); ++n)
    {
        page = HPDF_AddPage (pdf);
        /* draw grid to the page */
        //print_grid  (pdf, page);


        HPDF_Page_BeginText (page);
        HPDF_Page_SetFontAndSize (page, font, 8);
        strona = codec->fromUnicode(QString::fromUtf8("Strona %1 z %2").arg(n+2).arg(iloscStron));
        tw = HPDF_Page_TextWidth (page, strona.data());
        HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw) / 2,
                    25, strona.data());
        HPDF_Page_EndText (page);


        HPDF_Page_BeginText (page);
        HPDF_Page_SetFontAndSize (page, font, 10);
        HPDF_Page_EndText (page);

        float endY = createInfoBadanie(page, font, font2, HPDF_Page_GetHeight (page) - 60, false, false);
        createPageTest(page, font, font2, endY - 60, testy.at(n));
    }

    /* save the document to a file */
    errCode = HPDF_SaveToFile (pdf, data.toStdString().data());

    /* clean up */
    HPDF_Free (pdf);

}

void PdfCreator::createPageTest(HPDF_Page page, HPDF_Font font, HPDF_Font font2, float startY,
                                     const TestWszystko & testPage)
{
    float endY = createTestInfo(page, font, font2, startY, testPage.ogolne, testPage.testName);

    if (testPage.czyNarazenie) {
        endY = createNarazenie(page, font, font2, endY - 15, testPage.narazenie);
    }

    if (!testPage.odtwarzalnosc && !testPage.czyNarazenie2)
        endY = createCzujka(page, font, font2, endY-15, eTransmitter, eReceiver,
                        testPage.ogolne.transmiter, testPage.ogolne.receiver);

    if (testPage.showC)
        endY = createCminCmax(page, font, font2, endY - 15, testPage.wynikiC, testPage.odtwarzalnosc);

    if (testPage.dane.dane.size() > 0)
        endY = createTable(page, font, font2, testPage.dane.leftMargin, endY - 30,
                       testPage.dane.head, testPage.dane.colwidth, testPage.dane.colAlign,
                       testPage.dane.dane);

    if (testPage.czyNarazenie2) {
        endY = createNarazenie2(page, font, font2, endY - 15,
                                eTransmitter, eReceiver,
                                testPage.narazenie2);

    }


}

float PdfCreator::createHead(HPDF_Page page, HPDF_Font font, float startY)
{
    QByteArray p_title1 = codec->fromUnicode(QString::fromUtf8("CENTRUM NAUKOWO BADAWCZE OCHRONY PRZECIWPOŻAROWEJ"));
    QByteArray p_title2 = codec->fromUnicode(QString::fromUtf8("im. Józefa Tuliszkowskiego w Józefowie"));
    QByteArray p_title3 = codec->fromUnicode(QString::fromUtf8("Państwowy Instytut Badawczy"));
    QByteArray p_title4 = codec->fromUnicode(QString::fromUtf8("ZAKRES BADAŃ POSIADAJĄCY AKREDYTACJĘ POLSKIEGO CENTRUM AKREDYTACJI"));
    QByteArray p_title5 = codec->fromUnicode(QString::fromUtf8("Numer certyfikatu akredytacji - AB 207"));

    float tw1, tw2, tw3, tw4, tw5;

    /* print the title of the page (with positioning center). */
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 16);
    tw1 = HPDF_Page_TextWidth (page, p_title1.data());
    tw2 = HPDF_Page_TextWidth (page, p_title2.data());
    tw3 = HPDF_Page_TextWidth (page, p_title3.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw1) / 2,
                HPDF_Page_GetHeight (page) - 90, p_title1.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw2) / 2,
                HPDF_Page_GetHeight (page) - 105, p_title2.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw3) / 2,
                HPDF_Page_GetHeight (page) - 120, p_title3.data());
    HPDF_Page_EndText (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 13);
    tw4 = HPDF_Page_TextWidth (page, p_title4.data());
    tw5 = HPDF_Page_TextWidth (page, p_title5.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw4) / 2,
                HPDF_Page_GetHeight (page) - 145, p_title4.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw5) / 2,
                HPDF_Page_GetHeight (page) - 160, p_title5.data());

    HPDF_Page_EndText (page);
    return HPDF_Page_GetHeight (page) - 160;
}

float PdfCreator::createInfoBadanie(HPDF_Page page, HPDF_Font font, HPDF_Font font2,
                                   float endY, bool showUwagi, bool showPodpis)
{
    float col1width = 130; //lewy margines etykiet
    QByteArray eNumerZlecenia = codec->fromUnicode(QString::fromUtf8("Numer zlecenia:"));
    QByteArray eNumerBadania = codec->fromUnicode(QString::fromUtf8("Numer badania:"));
    QByteArray eOsobaOdpowiedzialna = codec->fromUnicode(QString::fromUtf8("Osoba odpowiedzialna:"));
    QByteArray eUwagi = codec->fromUnicode(QString::fromUtf8("Uwagi:"));
    QByteArray eData = codec->fromUnicode(QString::fromUtf8("Data:"));
    QByteArray ePodpis = codec->fromUnicode(QString::fromUtf8("Podpis:"));
    QByteArray eTypUrzadzenia = codec->fromUnicode(QString::fromUtf8("Typ urządzenia:"));

    float startY = endY;
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);

    float tw1 = HPDF_Page_TextWidth (page, eNumerZlecenia.data());
    float tw2 = HPDF_Page_TextWidth (page, eNumerBadania.data());
    float tw3 = HPDF_Page_TextWidth (page, eOsobaOdpowiedzialna.data());
    float tw4 = HPDF_Page_TextWidth (page, eUwagi.data());
    float tw5 = HPDF_Page_TextWidth (page, eTypUrzadzenia.data());
    float twD, twP;

    //176
    HPDF_Page_TextOut (page, col1width - tw1, startY, eNumerZlecenia.data());
    HPDF_Page_TextOut (page, col1width - tw2, startY -16, eNumerBadania.data());
    HPDF_Page_TextOut (page, col1width - tw3, startY -32, eOsobaOdpowiedzialna.data());
    if (showUwagi) {
        HPDF_Page_TextOut (page, col1width - tw4, startY -48, eUwagi.data());
        HPDF_Page_TextOut (page, col1width - tw5, startY -96, eTypUrzadzenia.data());
    }
    HPDF_Page_EndText (page);

    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);

    float width1Table = 285;
    //176
    HPDF_Page_Rectangle(page, col1width + 5, startY -4 , width1Table, 16);
    HPDF_Page_Rectangle(page, col1width + 5, startY -20 , width1Table, 16);
    HPDF_Page_Rectangle(page, col1width + 5, startY -36 , width1Table, 16);
    if (showUwagi) {
        HPDF_Page_Rectangle(page, col1width + 5, startY -84 , width1Table, 48);
        HPDF_Page_Rectangle(page, col1width + 5, startY -116 , width1Table, 32);
    }
    HPDF_Page_Stroke (page);


    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);
    //176
    drawTextInBoxLeft (page, col1width + 10, startY, numerZlecenia.data(), width1Table-10);
    drawTextInBoxLeft (page, col1width + 10, startY-16, numerBadania.data(), width1Table-10);
    drawTextInBoxLeft (page, col1width + 10, startY-32, osobaOdpowiedzialna1.data(), width1Table-10);
    if (showUwagi && uwagi1.size() > 0) {
        QByteArray safeUwagi = uwagi1;
        if (safeUwagi.size() > 200)
            safeUwagi = safeUwagi.first(200);
        HPDF_Page_TextRect (page, col1width + 10, startY -39,
                            col1width + 10 + width1Table-10, startY -83,
                        safeUwagi.data(), HPDF_TALIGN_LEFT, NULL);
    }
    if (showUwagi && urzadzenie.size() > 0) {
        QByteArray safeUrzadzenie = urzadzenie;
        if (safeUrzadzenie.size() > 130)
            safeUrzadzenie = safeUrzadzenie.first(130);
        HPDF_Page_TextRect (page, col1width + 10, startY -87,
                            col1width + 10 + width1Table-10, startY -115,
                        safeUrzadzenie.data(), HPDF_TALIGN_LEFT, NULL);
    }
    HPDF_Page_EndText (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);
    twD = HPDF_Page_TextWidth(page, eData.data());
    twP = HPDF_Page_TextWidth(page, ePodpis.data());
    float boxDataPodpisWidth = 120;
    HPDF_Page_TextOut (page, 500-twD/2, startY, eData.data());
    if (showPodpis)
        HPDF_Page_TextOut (page, 500-twP/2, startY-37, ePodpis.data());
    HPDF_Page_EndText (page);

    HPDF_Page_Rectangle(page, 500-boxDataPodpisWidth/2, startY -20 , boxDataPodpisWidth, 16);
    if (showPodpis)
        HPDF_Page_Rectangle(page, 500-boxDataPodpisWidth/2, startY -86 , boxDataPodpisWidth, 44);
    HPDF_Page_Stroke (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);
    //twD = HPDF_Page_TextWidth(page, dataBadania.data());
    //drawTextInBoxCenter (page, 500-twD/2, startY-15, dataBadania.data(), boxDataPodpisWidth-10);
    drawTextInBoxCenter (page, 500-boxDataPodpisWidth/2+5, startY -15, dataBadania.data(), boxDataPodpisWidth-10);
    HPDF_Page_EndText (page);
    return showUwagi ? startY -148 : startY -36;
}

float PdfCreator::createInformacje(HPDF_Page page, HPDF_Font font, HPDF_Font font2, float endY)
{
    QByteArray p_title = codec->fromUnicode(QString::fromUtf8("INFORMACJE O BADANYCH CZUJKACH"));

    float col1width = 150; //lewy margines etykiet
    QByteArray eRodzajSystemu = codec->fromUnicode(QString::fromUtf8("Rodzaj systemu:"));
    QByteArray urzadzenie = codec->fromUnicode(QString::fromUtf8("Stanowisko do badania czujek liniowych dymu - BCL, SN:1-23"));
    QByteArray eProducent = codec->fromUnicode(QString::fromUtf8("Producent:"));
    QByteArray eTransmitter = eTypTransmitter + ':';
    QByteArray eReceiver = eTypReceiver + ':';

    QByteArray erozstawienieMinimalne = codec->fromUnicode(QString::fromUtf8("Rozstawienie minimalne:"));
    QByteArray erozstawienieMaksymalne = codec->fromUnicode(QString::fromUtf8("Rozstawienie maksymalne:"));
    float width1Table = 380;
    float widthRozstBox = 75;
    float startY = endY;

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 12);
    float tw = HPDF_Page_TextWidth (page, p_title.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw) / 2,
                startY, p_title.data());
    HPDF_Page_EndText (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);
    float tw1 = HPDF_Page_TextWidth (page, eRodzajSystemu.data());
    float tw2 = HPDF_Page_TextWidth (page, eProducent.data());
    float tw3 = HPDF_Page_TextWidth (page, eTransmitter.data());
    float tw4 = HPDF_Page_TextWidth (page, eReceiver.data());
    float twRmin = HPDF_Page_TextWidth (page, erozstawienieMinimalne.data());
    float twRmax = HPDF_Page_TextWidth (page, erozstawienieMaksymalne.data());

    startY -= 24;
    short nRow = 0;
    HPDF_Page_TextOut (page, col1width - tw1, startY -(nRow++)*16 , eRodzajSystemu.data());
    HPDF_Page_TextOut (page, col1width - tw2, startY -(nRow++)*16, eProducent.data());
    HPDF_Page_TextOut (page, col1width - tw3, startY -(nRow++)*16, eTransmitter.data());
    HPDF_Page_TextOut (page, col1width - tw4, startY - (nRow++)*16, eReceiver.data());
    HPDF_Page_TextOut (page, col1width - twRmin, startY -(nRow)*16, erozstawienieMinimalne.data());
    HPDF_Page_TextOut (page, col1width + 5 + width1Table - 5 - twRmax - widthRozstBox,
                       startY -(nRow)*16, erozstawienieMaksymalne.data());
    HPDF_Page_EndText (page);

    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);

    nRow = 0;
    HPDF_Page_Rectangle(page, col1width + 5, startY -(nRow++)*16-4 , width1Table, 16);
    HPDF_Page_Rectangle(page, col1width + 5, startY -(nRow++)*16-4 , width1Table, 16);
    HPDF_Page_Rectangle(page, col1width + 5, startY -(nRow++)*16-4 , width1Table, 16);
    HPDF_Page_Rectangle(page, col1width + 5, startY -(nRow++)*16-4 , width1Table, 16);
    HPDF_Page_Rectangle(page, col1width + 5, startY -(nRow)*16-4 , widthRozstBox, 16);
    HPDF_Page_Rectangle(page, col1width + 5 + width1Table - widthRozstBox, startY -(nRow)*16-4 , widthRozstBox, 16);
    HPDF_Page_Stroke (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);

    nRow = 0;
    drawTextInBoxLeft (page, col1width + 10, startY -(nRow++)*16, typSystemu.data(), width1Table-10);
    drawTextInBoxLeft (page, col1width + 10, startY -(nRow++)*16, producent.data(), width1Table-10);
    drawTextInBoxLeft (page, col1width + 10, startY -(nRow++)*16, typTransmitter.data(), width1Table-10);
    drawTextInBoxLeft (page, col1width + 10, startY -(nRow++)*16, typReceiver.data(), width1Table-10);

    //float twRminV = HPDF_Page_TextWidth (page, rozstawienieMinimalne.data());
    //drawTextInBox (page, col1width + 5 + widthRozstBox - 10 - twRminV,
    //               startY-64, rozstawienieMinimalne.data(), widthRozstBox-10);
    drawTextInBoxRight (page, col1width + 10,
                   startY -(nRow)*16, rozstawienieMinimalne.data(), widthRozstBox-10);


    //float twRmaxV = HPDF_Page_TextWidth (page, rozstawienieMaksymalne.data());
    //drawTextInBox (page, col1width + 5 + width1Table - 10 - twRmaxV,
    //               startY-64, rozstawienieMaksymalne.data(), widthRozstBox-10);

    drawTextInBoxRight (page, col1width + 10 + width1Table - widthRozstBox,
                   startY -(nRow)*16, rozstawienieMaksymalne.data(), widthRozstBox-10);

    HPDF_Page_EndText (page);
    return startY -(5)*16 -4;
}

float PdfCreator::createInformacjeKat(HPDF_Page page, HPDF_Font font, HPDF_Font font2, float endY)
{

    float marginleft = 35; //lewy margines etykiet
    float width1Column = 200;
    float width2Column = 150;
    float width3Column = 150;
    float startY = endY;
    QByteArray ePionowa = codec->fromUnicode(QString::fromUtf8("Pionowa"));
    QByteArray ePozioma = codec->fromUnicode(QString::fromUtf8("Pozioma"));
    QByteArray title = codec->fromUnicode(QString::fromUtf8("Maksymalna kątowa niewspółosiowość"));

    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);

    HPDF_Page_Rectangle(page, marginleft, startY, width1Column, 16);
    HPDF_Page_Rectangle(page, marginleft, startY-16, width1Column, 16);
    HPDF_Page_Rectangle(page, marginleft, startY-32, width1Column, 16);

    HPDF_Page_Rectangle(page, marginleft+width1Column, startY, width2Column, 16);
    HPDF_Page_Rectangle(page, marginleft+width1Column, startY-16, width2Column, 16);
    HPDF_Page_Rectangle(page, marginleft+width1Column, startY-32, width2Column, 16);

    HPDF_Page_Rectangle(page, marginleft+width1Column+width2Column, startY, width3Column, 16);
    HPDF_Page_Rectangle(page, marginleft+width1Column+width2Column, startY-16, width3Column, 16);
    HPDF_Page_Rectangle(page, marginleft+width1Column+width2Column, startY-32, width3Column, 16);
    HPDF_Page_Stroke (page);


    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);

    //float tw1 = HPDF_Page_TextWidth(page, title.data());
    //drawTextInBox (page, marginleft + (width1Column-tw1)/2,
    //               startY+5, title.data(), width1Column-10);

    drawTextInBoxCenter (page, marginleft + 5,
                   startY+5, title.data(), width1Column-10);

    //float tw2 = HPDF_Page_TextWidth(page, eTransmitter.data());
    //drawTextInBox (page, marginleft + width1Column - tw2 - 5,
    //               startY-11, eTransmitter.data(), width1Column-10);
    drawTextInBoxRight (page, marginleft + 5,
                   startY-11, eTransmitter.data(), width1Column-10);

    //float tw3 = HPDF_Page_TextWidth(page, eReceiver.data());
    //drawTextInBox (page, marginleft + width1Column - tw3 - 5,
    //               startY-27, eReceiver.data(), width1Column-10);
    drawTextInBoxRight (page, marginleft + 5,
                   startY-27, eReceiver.data(), width1Column-10);

    //float tw4 = HPDF_Page_TextWidth(page, ePionowa.data());
    //drawTextInBox (page, marginleft + width1Column + (width2Column-tw4)/2,
    //               startY+5, ePionowa.data(), width2Column-10);
    drawTextInBoxCenter (page, marginleft + width1Column + 5,
                   startY+5, ePionowa.data(), width2Column-10);

    //float tw5 = HPDF_Page_TextWidth(page, ePozioma.data());
    //drawTextInBox (page, marginleft + width1Column + width2Column + (width3Column-tw5)/2,
    //               startY+5, ePozioma.data(), width3Column-10);
    drawTextInBoxCenter (page, marginleft + width1Column + width2Column + 5,
                   startY+5, ePozioma.data(), width3Column-10);

    HPDF_Page_EndText (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);

    //float twk1 = HPDF_Page_TextWidth(page, nadajnikKatPionowy.data());
    //drawTextInBox (page, marginleft + width1Column + width2Column - twk1 - 5,
    //               startY-11, nadajnikKatPionowy.data(), width2Column-10);
    drawTextInBoxRight (page, marginleft + width1Column + 5,
                   startY-11, nadajnikKatPionowy.data(), width2Column-10);

    //float twk2 = HPDF_Page_TextWidth(page, nadajnikKatPoziomy.data());
    //drawTextInBox (page, marginleft + width1Column + width2Column + width3Column - twk2 - 5,
    //               startY-11, nadajnikKatPoziomy.data(), width3Column-10);
    drawTextInBoxRight (page, marginleft + width1Column + width2Column + 5,
                   startY-11, nadajnikKatPionowy.data(), width2Column-10);

    //float twk3 = HPDF_Page_TextWidth(page, odbiornikKatPionowy.data());
    //drawTextInBox (page, marginleft + width1Column + width2Column - twk3 - 5,
    //               startY-27, odbiornikKatPionowy.data(), width2Column-10);
    drawTextInBoxRight (page, marginleft + width1Column + 5,
                   startY-27, odbiornikKatPionowy.data(), width2Column-10);

    //float twk4 = HPDF_Page_TextWidth(page, odbiornikKatPoziomy.data());
    //drawTextInBox (page, marginleft + width1Column + width2Column + width3Column - twk4 - 5,
    //               startY-27, odbiornikKatPoziomy.data(), width3Column-10);
    drawTextInBoxRight (page, marginleft + width1Column + width2Column + 5,
                   startY-27, odbiornikKatPoziomy.data(), width3Column-10);

    HPDF_Page_EndText (page);
    return startY-32;
}

float PdfCreator::createTablicaCzujek(HPDF_Page page, HPDF_Font font, HPDF_Font font2, float endY)
{
    QByteArray tytul = codec->fromUnicode(QString::fromUtf8("NUMERY SERYJNE BADANYCH CZUJEK"));
    QByteArray eOznaczenie = codec->fromUnicode(QString::fromUtf8("Oznaczenie"));

    float startY = endY;

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 12);

    float tw = HPDF_Page_TextWidth(page, tytul.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw)/2, startY, tytul.data());
    HPDF_Page_EndText (page);

    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);

    float wCol1 = 75;
    float wCol2 = 200;
    float wCol3 = 200;
    float mleft = (HPDF_Page_GetWidth(page) - wCol1 - wCol2 - wCol3)/2;
    startY -= 25;
    for (short i = 0 ; i < 8; ++i) {
        HPDF_Page_Rectangle(page, mleft, startY-16*i, wCol1, 16);
        HPDF_Page_Rectangle(page, mleft + wCol1, startY-16*i, wCol2, 16);
        HPDF_Page_Rectangle(page, mleft + wCol1 + wCol2, startY-16*i, wCol3, 16);
    }
    HPDF_Page_Stroke (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);
    float tw1 = HPDF_Page_TextWidth(page, eOznaczenie.data());
    float tw2 = HPDF_Page_TextWidth(page, eNrTransmitter.data());
    float tw3 = HPDF_Page_TextWidth(page, eNrReceiver.data());
    HPDF_Page_TextOut (page, mleft + (wCol1 - tw1)/2, startY + 5, eOznaczenie.data());
    HPDF_Page_TextOut (page, mleft + wCol1 + (wCol2 - tw2)/2, startY + 5, eNrTransmitter.data());
    HPDF_Page_TextOut (page, mleft + wCol1 + wCol2 + (wCol3 - tw3)/2, startY + 5, eNrReceiver.data());
    HPDF_Page_EndText (page);

    /*dane wlasciwe czujki */

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);

    for (short i = 0 ; i < 7; ++i) {
        QByteArray c1, c2, c3;
        if (i < czujki.size()) {
            c1 = czujki.at(i).oznaczenie;
            c2 = czujki.at(i).transmiter;
            c3 = czujki.at(i).receiver;
        } else {
            c1 = "---";
            c2 = "---";
            c3 = "---";
        }

        //float rtw1 = (wCol1 - HPDF_Page_TextWidth(page, c1))/2;
        //float rtw2 = wCol1 + (wCol2 - HPDF_Page_TextWidth(page, c2))/2;
        //float rtw3 = wCol1 + wCol2 + (wCol3 - HPDF_Page_TextWidth(page, c3))/2;

        //drawTextInBox(page, mleft + rtw1, startY-16*(i+1)+5, c1.data(), wCol1);
        //drawTextInBox(page, mleft + rtw2, startY-16*(i+1)+5, c2.data(), wCol2);
        //drawTextInBox(page, mleft + rtw3, startY-16*(i+1)+5, c3.data(), wCol3);

        drawTextInBoxCenter(page, mleft + 5, startY-16*(i+1)+5, c1.data(), wCol1-10);
        drawTextInBoxCenter(page, mleft + 5 + wCol1, startY-16*(i+1)+5, c2.data(), wCol2-10);
        drawTextInBoxCenter(page, mleft + 5 + wCol1 + wCol2, startY-16*(i+1)+5, c3.data(), wCol3-10);

    }
    HPDF_Page_EndText (page);
    return startY-16*7;
}

float PdfCreator::createDodatkoweParametry(HPDF_Page page, HPDF_Font font, HPDF_Font font2, float endY)
{
    QByteArray eTytul = codec->fromUnicode(QString::fromUtf8("SPOSÓB ZASILANIA CZUJEK"));
    QByteArray eZasilanieZasilacza = codec->fromUnicode(QString::fromUtf8("Z zasilacza zewnętrznego"));
    QByteArray eZasilanieNapiecie = codec->fromUnicode(QString::fromUtf8("Napięcie zasilania:"));
    QByteArray eCentralaZasilanie = codec->fromUnicode(QString::fromUtf8("Z centrali sygnalizacji pożarowej"));
    QByteArray eTypCentralaZasilanie = codec->fromUnicode(QString::fromUtf8("Typ centrali:"));
    QByteArray ePrzekaznik = codec->fromUnicode(QString::fromUtf8("Centrala lub przekaźnik NO"));
    QByteArray ePrad = codec->fromUnicode(QString::fromUtf8("Przekroczenie prądu zasilania:"));
    QByteArray eCzasStabilizacji = codec->fromUnicode(QString::fromUtf8("Czas stabilizacji czujki po włączeniu zasilania:"));
    QByteArray eCzasPoResecie = codec->fromUnicode(QString::fromUtf8("Czas stabilizacji czujki po zresetowaniu zasilania:"));
    QByteArray epoZresetowaniuZasilania = codec->fromUnicode(QString::fromUtf8("po zresetowaniu zasilania:"));

    float startY = endY;
    float lMargin = 30;
    float rMargin = HPDF_Page_GetWidth(page)/2 + 30;
    float boxSize = 15;
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 12);
    float tw = HPDF_Page_TextWidth (page, eTytul.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw) / 2,
                startY, eTytul.data());

    startY -= 30;

    //zasilacz centrala
    HPDF_Page_TextOut (page, lMargin+5+boxSize, startY, eZasilanieZasilacza.data());
    HPDF_Page_TextOut (page, rMargin+5+boxSize, startY, eCentralaZasilanie.data());
    HPDF_Page_EndText (page);

    //boxy dla centrali lub zasilacza
    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);
    HPDF_Page_Rectangle(page, lMargin, startY-2 , boxSize, boxSize);
    HPDF_Page_Rectangle(page, rMargin, startY-2 , boxSize, boxSize);

    //zaznaczenie boxow centrali lub zasilacza
    if (zasilaczZewnetrzny) {
        HPDF_Page_MoveTo (page, lMargin + 2, startY);
        HPDF_Page_LineTo (page, lMargin + boxSize - 2, startY-2 + boxSize-2);
        HPDF_Page_MoveTo (page, lMargin + boxSize - 2, startY);
        HPDF_Page_LineTo (page, lMargin + 2, startY-2 + boxSize-2);
    }

    if (zasilaczCentrala) {
        HPDF_Page_MoveTo (page, rMargin + 2, startY);
        HPDF_Page_LineTo (page, rMargin + boxSize - 2, startY-2 + boxSize-2);
        HPDF_Page_MoveTo (page, rMargin + boxSize - 2, startY);
        HPDF_Page_LineTo (page, rMargin + 2, startY-2 + boxSize-2);
    }
    HPDF_Page_Stroke (page);

    //napiecie zasilania i typ centrali
    startY -= 20;
    float boxNapiecieSize = 100;
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 12);
    float twn = HPDF_Page_TextWidth(page, eZasilanieNapiecie.data());
    float twc = HPDF_Page_TextWidth(page, eTypCentralaZasilanie.data());

    HPDF_Page_TextOut (page, rMargin-boxNapiecieSize-10-5-twn, startY, eZasilanieNapiecie.data());
    HPDF_Page_TextOut (page, rMargin, startY, eTypCentralaZasilanie.data());
    HPDF_Page_EndText (page);

    //text bosy dla zsialnia i typu centrali

    float boxCentralaSize = 165;
    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);
    //HPDF_Page_Rectangle(page, lMargin+twn+5, startY-2, boxNapiecieSize, 16);
    HPDF_Page_Rectangle(page, rMargin-boxNapiecieSize-10, startY-2, boxNapiecieSize, 16);
    HPDF_Page_Rectangle(page, rMargin+twc+5, startY-2, boxCentralaSize, 16);
    HPDF_Page_Stroke (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 12);
    //float tw_z1 = HPDF_Page_TextWidth(page, wartoscNapieciaZasilania.data());
    //drawTextInBox(page, rMargin-10-5-tw_z1, startY+3, wartoscNapieciaZasilania.data(), boxNapiecieSize-10);
    drawTextInBoxRight(page, rMargin-boxNapiecieSize-10+5, startY+3, wartoscNapieciaZasilania.data(), boxNapiecieSize-10);

    //drawTextInBox(page, rMargin+twc+5+5, startY+3, nazwaCentraliPPOZ.data(), boxCentralaSize-10);
    drawTextInBoxLeft(page, rMargin+twc+5+5, startY+3, nazwaCentraliPPOZ.data(), boxCentralaSize-10);
    HPDF_Page_EndText (page);

    //alarm czujki prad lub przekaznik
    startY -= 20;
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 12);
    HPDF_Page_TextOut (page, lMargin+5+boxSize, startY, ePrad.data());
    HPDF_Page_TextOut (page, rMargin+5+boxSize, startY, ePrzekaznik.data());
    HPDF_Page_EndText (page);

    float boxPradSize = 100;
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 12);
    //float tw_pp = HPDF_Page_TextWidth(page, wartoscPrądu.data());
    //drawTextInBox(page, rMargin-10-5-tw_pp, startY+3, wartoscPrądu.data(), boxPradSize-10);
    drawTextInBoxRight(page, rMargin-10+5-boxPradSize, startY+3, wartoscPrądu.data(), boxPradSize-10);
    HPDF_Page_EndText (page);

    //boxy dla pradu i przekaznik
    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);

    HPDF_Page_Rectangle(page, lMargin, startY-2 , boxSize, boxSize);
    HPDF_Page_Rectangle(page, rMargin, startY-2 , boxSize, boxSize);
    if (wyzwalaniePradem) {
        HPDF_Page_MoveTo (page, lMargin + 2, startY);
        HPDF_Page_LineTo (page, lMargin + boxSize - 2, startY-2 + boxSize-2);
        HPDF_Page_MoveTo (page, lMargin + boxSize - 2, startY);
        HPDF_Page_LineTo (page, lMargin + 2, startY-2 + boxSize-2);
    }

    if (wyzwalaniePrzekaznikiem) {
        HPDF_Page_MoveTo (page, rMargin + 2, startY);
        HPDF_Page_LineTo (page, rMargin + boxSize - 2, startY-2 + boxSize-2);
        HPDF_Page_MoveTo (page, rMargin + boxSize - 2, startY);
        HPDF_Page_LineTo (page, rMargin + 2, startY-2 + boxSize-2);
    }

    HPDF_Page_Rectangle(page, rMargin-boxPradSize-10, startY-2, boxPradSize, 16);
    HPDF_Page_Stroke (page);

    startY -= 30;
    float czasBoxSize = 75;
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 12);
    float twst1 = HPDF_Page_TextWidth(page, eCzasStabilizacji.data());
    float twst2 = HPDF_Page_TextWidth(page, epoZresetowaniuZasilania.data());
    HPDF_Page_TextOut (page, lMargin, startY, eCzasStabilizacji.data());
    HPDF_Page_TextOut (page, lMargin+10+twst1+czasBoxSize, startY, ",");
    HPDF_Page_TextOut (page, lMargin+twst1-twst2 , startY - 20, epoZresetowaniuZasilania.data());
    HPDF_Page_EndText (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 12);
    float twstv1 = HPDF_Page_TextWidth(page, czasStabilizacjiCzujki.data());
    float twstv2 = HPDF_Page_TextWidth(page, czasStabilizacjiPoResecie.data());
    HPDF_Page_TextOut (page, lMargin+twst1+10+czasBoxSize-5-twstv1-5, startY-1, czasStabilizacjiCzujki.data());
    HPDF_Page_TextOut (page, lMargin+twst1+10+czasBoxSize-5-twstv2-5 , startY-19, czasStabilizacjiPoResecie.data());
    HPDF_Page_EndText (page);

    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);
    HPDF_Page_Rectangle(page, lMargin+twst1+5, startY-3, czasBoxSize, 16);
    HPDF_Page_Rectangle(page, lMargin+twst1+5, startY-23, czasBoxSize, 16);
    HPDF_Page_Stroke (page);
    return startY-23;

}

float PdfCreator::createTestInfo(HPDF_Page page, HPDF_Font font, HPDF_Font font2,
                                float endY, const OgolneParametryTestu & test, const QByteArray & tytulTestu)
{
    QByteArray eOsobaOdpowiedzialna = codec->fromUnicode(QString::fromUtf8("Osoba wykonująca:"));
    QByteArray eDataRozpoczecia = codec->fromUnicode(QString::fromUtf8("Data i czas rozpoczęcia:"));
    QByteArray eDataZakonczenia = codec->fromUnicode(QString::fromUtf8("Data i czas zakończenia:"));
    QByteArray eTemperatura = codec->fromUnicode(QString::fromUtf8("Temperatura:"));
    QByteArray eWilgotnosc = codec->fromUnicode(QString::fromUtf8("Wilgotność:"));
    QByteArray ecisnienie = codec->fromUnicode(QString::fromUtf8("Ciśnienie:"));
    QByteArray ewynikTestu = codec->fromUnicode(QString::fromUtf8("Wynik testu:"));
    QByteArray eUwagiTestu = codec->fromUnicode(QString::fromUtf8("Uwagi:"));

    //float startY = HPDF_Page_GetHeight (page) - 140;
    float startY = endY;
    float marginl = 130;

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 14);
    HPDF_Page_TextOut (page, marginl-75, startY, tytulTestu.data());
    HPDF_Page_EndText (page);

    startY -= 30;
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);

    float tw1 = HPDF_Page_TextWidth (page, eOsobaOdpowiedzialna.data());
    float tw2 = HPDF_Page_TextWidth (page, ewynikTestu.data());
    float tw3 = HPDF_Page_TextWidth (page, eDataRozpoczecia.data());
    float tw4 = HPDF_Page_TextWidth (page, eDataZakonczenia.data());
    float tw5 = HPDF_Page_TextWidth (page, eTemperatura.data());
    float tw6 = HPDF_Page_TextWidth (page, eWilgotnosc.data());
    float tw7 = HPDF_Page_TextWidth (page, ecisnienie.data());
    float tw8 = HPDF_Page_TextWidth (page, eUwagiTestu.data());

    short nrRow = 0;
    HPDF_Page_TextOut (page, marginl - tw1, startY - (nrRow++)*16, eOsobaOdpowiedzialna.data());
    HPDF_Page_TextOut (page, marginl - tw2, startY - (nrRow++)*16, ewynikTestu.data());
    HPDF_Page_TextOut (page, marginl - tw3, startY - (nrRow++)*16, eDataRozpoczecia.data());
    HPDF_Page_TextOut (page, marginl - tw4, startY - (nrRow++)*16, eDataZakonczenia.data());
    HPDF_Page_TextOut (page, marginl - tw5, startY - (nrRow++)*16, eTemperatura.data());
    HPDF_Page_TextOut (page, marginl - tw6, startY - (nrRow++)*16, eWilgotnosc.data());
    HPDF_Page_TextOut (page, marginl - tw7, startY - (nrRow++)*16, ecisnienie.data());
    HPDF_Page_TextOut (page, marginl - tw8, startY - (nrRow++)*16, eUwagiTestu.data());
    HPDF_Page_EndText (page);

    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);

    float wBox = 285;
    //176
    nrRow = 0;
    HPDF_Page_Rectangle(page, marginl + 5, startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5, startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5, startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5, startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5, startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5, startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5, startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5, startY - (nrRow++)*16 -4 -50+16 , wBox, 50);
    HPDF_Page_Stroke (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);
    nrRow = 0;
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, test.osobaWykonujaca.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, test.wynikTestu.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, test.dataRozpoczecia.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, test.dataZakonczenia.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, test.temperatura.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, test.wilgotnosc.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, test.cisnienie.data(), wBox-10);
    if (test.uwagi.size() > 0) {
        QByteArray safeUwagi = test.uwagi ;
        if (safeUwagi.size() > 200)
            safeUwagi = safeUwagi.first(200);
        HPDF_Page_TextRect(page, marginl + 10, startY - nrRow * 16 + 10,
                       marginl + 10 + wBox - 10, nrRow * 16 - 50 + 10,
                       safeUwagi.data(), HPDF_TALIGN_LEFT, NULL);
    }
    HPDF_Page_EndText (page);
    return startY - nrRow * 16 - 50;
}

float PdfCreator::createTable(HPDF_Page page, HPDF_Font font, HPDF_Font font2,
                              float marginl,
                              float startY,
                              const QVector<QByteArray> &head,
                              const QVector<int> &colwidth,
                              const QVector<short> &colCenter,
                              const QVector<QVector<QByteArray> > &dane)
{
    float actY = startY+16;
    float actPosX = marginl;
    float ret = 0;

    HPDF_Page_SetLineWidth (page, 1);
    HPDF_Page_SetRGBStroke (page, 0, 0, 0);

    for (short nrRow = 0; nrRow <= dane.size(); ++nrRow) {
        actPosX = marginl;
        for (short nrCol = 0; nrCol < colwidth.size(); ++nrCol) {
            HPDF_Page_Rectangle(page, actPosX, actY-16, colwidth.at(nrCol), 16);
            actPosX += colwidth.at(nrCol);
        }
        actY -= 16;
    }
    ret = actY;
    HPDF_Page_Stroke (page);

    //naglowki
    HPDF_Page_BeginText (page);
    actY = startY + 5;
    actPosX = marginl;
    HPDF_Page_SetFontAndSize (page, font, 10);
    for (short nrCol = 0; nrCol < head.size(); ++nrCol) {
        if (nrCol >= colwidth.size())
            break;
        drawTextInBoxCenter(page, actPosX+5, actY, head.at(nrCol).data(), colwidth.at(nrCol)-10);
        actPosX += colwidth.at(nrCol);
    }
    HPDF_Page_EndText (page);

    //dane
    HPDF_Page_BeginText (page);
    actY = startY + 5 - 16;
    HPDF_Page_SetFontAndSize (page, font2, 10);
    for (short row = 0; row < dane.size(); ++row) {
        actPosX = marginl;
        for (short col = 0; col < dane.at(row).size(); ++col) {
            if (col >= colwidth.size())
                break;

            short calign = 0;
            if (col < colCenter.size()) {
                calign = colCenter.at(col);
            }
            if (calign == -1) {
                drawTextInBoxLeft (page, actPosX + 5, actY, dane.at(row).at(col), colwidth.at(col)-10);
            } else if (calign == 0) {
                drawTextInBoxCenter (page, actPosX + 5, actY, dane.at(row).at(col), colwidth.at(col)-10);
            } else if (calign == 1) {
                drawTextInBoxRight (page, actPosX + 5, actY, dane.at(row).at(col), colwidth.at(col)-10);
            }
            actPosX += colwidth.at(col);
        }
        actY -= 16;
    }
    HPDF_Page_EndText (page);

    return ret;
}

float PdfCreator::createCminCmax(HPDF_Page page,  HPDF_Font font, HPDF_Font font2, float startY, const CMinCMaxCRep & test, bool crep)
{
    QByteArray eCmin = codec->fromUnicode(QString::fromUtf8("Cmin = "));
    QByteArray eCMax = codec->fromUnicode(QString::fromUtf8("Cmax = "));
    QByteArray eCRep = codec->fromUnicode(QString::fromUtf8("Crep = "));
    QByteArray eCmaxCrep = codec->fromUnicode(QString::fromUtf8("Cmax/Crep = "));
    QByteArray eCrepCmin = codec->fromUnicode(QString::fromUtf8("Crep/Cmin = "));
    QByteArray eCmaxCmin = codec->fromUnicode(QString::fromUtf8("Cmax/Cmin = "));

    float marginl = 130;

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);

    float tw1 = HPDF_Page_TextWidth (page, eCmin.data());
    float tw2 = HPDF_Page_TextWidth (page, eCMax.data());
    float tw3 = HPDF_Page_TextWidth (page, eCRep.data());
    float tw4 = HPDF_Page_TextWidth (page, eCmaxCrep.data());
    float tw5 = HPDF_Page_TextWidth (page, eCrepCmin.data());
    float tw6 = HPDF_Page_TextWidth (page, eCmaxCmin.data());

    short nrRow = 0;
    HPDF_Page_TextOut (page, marginl - tw1, startY - (nrRow++)*16, eCmin.data());
    HPDF_Page_TextOut (page, marginl - tw2, startY - (nrRow++)*16, eCMax.data());
    if (crep) {
        HPDF_Page_TextOut (page, marginl - tw3, startY - (nrRow++)*16, eCRep.data());
        HPDF_Page_TextOut (page, marginl - tw4, startY - (nrRow++)*16, eCmaxCrep.data());
        HPDF_Page_TextOut (page, marginl - tw5, startY - (nrRow++)*16, eCrepCmin.data());
    } else {
        HPDF_Page_TextOut (page, marginl - tw6, startY - (nrRow++)*16, eCmaxCmin.data());
    }
    HPDF_Page_EndText (page);

    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);

    float wBox = 75;
    //176
    nrRow = 0;
    HPDF_Page_Rectangle(page, marginl + 5,          startY - (nrRow)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5 + wBox,   startY - (nrRow++)*16 -4 , wBox, 16);

    HPDF_Page_Rectangle(page, marginl + 5,          startY - (nrRow)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5 + wBox,   startY - (nrRow++)*16 -4 , wBox, 16);
    if (crep) {
        HPDF_Page_Rectangle(page, marginl + 5,          startY - (nrRow)*16 -4 , wBox, 16);
        HPDF_Page_Rectangle(page, marginl + 5 + wBox,   startY - (nrRow++)*16 -4 , wBox, 16);
        HPDF_Page_Rectangle(page, marginl + 5,          startY - (nrRow++)*16 -4 , wBox, 16);
        HPDF_Page_Rectangle(page, marginl + 5,          startY - (nrRow++)*16 -4 , wBox, 16);
    } else {
        HPDF_Page_Rectangle(page, marginl + 5,          startY - (nrRow++)*16 -4 , wBox, 16);
    }
    HPDF_Page_Stroke (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);
    nrRow = 0;
    drawTextInBoxRight (page, marginl + 10 + wBox,       startY - (nrRow)*16, test.Cmin2.data(), wBox-10);
    drawTextInBoxRight (page, marginl + 10,              startY - (nrRow++)*16, test.Cmin1.data(), wBox-10);
    drawTextInBoxRight (page, marginl + 10,              startY - (nrRow)*16, test.Cmax1.data(), wBox-10);
    drawTextInBoxRight (page, marginl + 10 + wBox,       startY - (nrRow++)*16, test.Cmax2.data(), wBox-10);
    if (crep) {
        drawTextInBoxRight (page, marginl + 10,          startY - (nrRow)*16, test.Crep1.data(), wBox-10);
        drawTextInBoxRight (page, marginl + 10 + wBox,   startY - (nrRow++)*16, test.Crep2.data(), wBox-10);
        drawTextInBoxRight (page, marginl + 10,          startY - (nrRow++)*16, test.CMaxCrep.data(), wBox-10);
        drawTextInBoxRight (page, marginl + 10,          startY - (nrRow++)*16, test.CRepCMin.data(), wBox-10);
    } else {
        drawTextInBoxRight (page, marginl + 10,          startY - (nrRow++)*16, test.CMaxCmin.data(), wBox-10);
    }
    HPDF_Page_EndText (page);
    return startY - nrRow*16;
}

float PdfCreator::createCzujka(HPDF_Page page,  HPDF_Font font, HPDF_Font font2,
                               float startY,
                               const QByteArray & etTransmiter, const QByteArray & etReceiver,
                               const QByteArray & transmiter, const QByteArray & receiver)
{

    float marginl = 130;

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);

    float tw1 = HPDF_Page_TextWidth (page, etTransmiter.data());
    float tw2 = HPDF_Page_TextWidth (page, etReceiver.data());

    short nrRow = 0;
    HPDF_Page_TextOut (page, marginl - tw1, startY - (nrRow++)*16, etTransmiter.data());
    HPDF_Page_TextOut (page, marginl - tw2, startY - (nrRow++)*16, etReceiver.data());
    HPDF_Page_EndText (page);

    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);

    float wBox = 200;
    //176
    nrRow = 0;
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Stroke (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);
    nrRow = 0;
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, transmiter.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, receiver.data(), wBox-10);
    HPDF_Page_EndText (page);
    return startY - nrRow*16;
}

float PdfCreator::createNarazenie(HPDF_Page page,  HPDF_Font font, HPDF_Font font2,
                               float startY, const NarazenieDane & narazenie)
{
    float marginl = 130;
    QByteArray eOpis = codec->fromUnicode(QString::fromUtf8("Opis narażenia:"));
    QByteArray eWynik = codec->fromUnicode(QString::fromUtf8("Wynik narażenia:"));
    QByteArray eUwagi = codec->fromUnicode(QString::fromUtf8("Uwagi dotyczące narażenia:"));


    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);

    float tw1 = HPDF_Page_TextWidth (page, eOpis.data());
    float tw2 = HPDF_Page_TextWidth (page, eWynik.data());
    float tw3 = HPDF_Page_TextWidth (page, eUwagi.data());

    short nrRow = 0;
    HPDF_Page_TextOut (page, marginl - tw1, startY - (nrRow++)*16, eOpis.data());
    HPDF_Page_TextOut (page, marginl - tw2, startY - (nrRow++)*16, eWynik.data());
    HPDF_Page_TextOut (page, marginl - tw3, startY - (nrRow++)*16, eUwagi.data());
    HPDF_Page_EndText (page);

    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);

    float wBox = 400;

    nrRow = 0;
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++ + 2)*16 -4 , wBox, 3*16);
    HPDF_Page_Stroke (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);
    nrRow = 0;
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, narazenie.opis.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, narazenie.wynik.data(), wBox-10);
    if (narazenie.uwagi.size() > 0) {
        QByteArray safeUwagi = narazenie.uwagi ;
        if (safeUwagi.size() > 200)
            safeUwagi = safeUwagi.first(200);
        HPDF_Page_TextRect(page, marginl + 10, startY - nrRow * 16 + 10,
                       marginl + 10 + wBox - 10, nrRow * 16 - 50 + 10,
                       safeUwagi.data(), HPDF_TALIGN_LEFT, NULL);
    }
    HPDF_Page_EndText (page);
    return startY - (nrRow+3)*16;
}

float PdfCreator::createNarazenie2(HPDF_Page page,  HPDF_Font font, HPDF_Font font2,
                               float endY,
                               const QByteArray & etTransmiter,
                               const QByteArray & etReceiver,
                               const NarazenieDane2 & narazenie)
{
    float marginl = 130;
    QByteArray eOpis = codec->fromUnicode(QString::fromUtf8("Opis narażenia:"));
    QByteArray eWynik = codec->fromUnicode(QString::fromUtf8("Wynik narażenia:"));
    QByteArray eUwagi = codec->fromUnicode(QString::fromUtf8("Uwagi dotyczące narażenia:"));


    float startY = endY;
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);

    float tw1 = HPDF_Page_TextWidth (page, eOpis.data());
    float tw2 = HPDF_Page_TextWidth (page, etTransmiter.data());
    float tw3 = HPDF_Page_TextWidth (page, etReceiver.data());
    float tw4 = HPDF_Page_TextWidth (page, eWynik.data());
    float tw5 = HPDF_Page_TextWidth (page, eUwagi.data());
    float wBox = 400;
    //begin
    short nrRow = 0;
    HPDF_Page_TextOut (page, marginl - tw1, startY - (nrRow++)*16, eOpis.data());
    HPDF_Page_TextOut (page, marginl - tw2, startY - (nrRow++)*16, etTransmiter.data());
    HPDF_Page_TextOut (page, marginl - tw3, startY - (nrRow++)*16, etReceiver.data());
    HPDF_Page_TextOut (page, marginl - tw4, startY - (nrRow++)*16, eWynik.data());
    HPDF_Page_TextOut (page, marginl - tw5, startY - (nrRow++)*16, eUwagi.data());
    HPDF_Page_EndText (page);

    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);

    nrRow = 0;
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++ + 2)*16 -4 , wBox, 3*16);
    HPDF_Page_Stroke (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);
    nrRow = 0;
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, narazenie.first.opis.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, narazenie.first.transmiter.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, narazenie.first.receiver.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, narazenie.first.wynik.data(), wBox-10);
    if (narazenie.first.uwagi.size() > 0) {
        QByteArray safeUwagi = narazenie.first.uwagi ;
        if (safeUwagi.size() > 200)
            safeUwagi = safeUwagi.first(200);
        HPDF_Page_TextRect(page, marginl + 10, startY - nrRow * 16 + 10,
                       marginl + 10 + wBox - 10, nrRow * 16 - 50 + 10,
                       safeUwagi.data(), HPDF_TALIGN_LEFT, NULL);
    }
    HPDF_Page_EndText (page);
    //end

    startY -= (nrRow+7)*16;

    //begin
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);
    nrRow = 0;
    HPDF_Page_TextOut (page, marginl - tw1, startY - (nrRow++)*16, eOpis.data());
    HPDF_Page_TextOut (page, marginl - tw2, startY - (nrRow++)*16, etTransmiter.data());
    HPDF_Page_TextOut (page, marginl - tw3, startY - (nrRow++)*16, etReceiver.data());
    HPDF_Page_TextOut (page, marginl - tw4, startY - (nrRow++)*16, eWynik.data());
    HPDF_Page_TextOut (page, marginl - tw5, startY - (nrRow++)*16, eUwagi.data());
    HPDF_Page_EndText (page);

    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);
    nrRow = 0;
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++)*16 -4 , wBox, 16);
    HPDF_Page_Rectangle(page, marginl + 5,   startY - (nrRow++ + 2)*16 -4 , wBox, 3*16);
    HPDF_Page_Stroke (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);
    nrRow = 0;
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, narazenie.second.opis.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, narazenie.second.transmiter.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, narazenie.second.receiver.data(), wBox-10);
    drawTextInBoxLeft (page, marginl + 10, startY - (nrRow++)*16, narazenie.second.wynik.data(), wBox-10);
    if (narazenie.second.uwagi.size() > 0) {
        QByteArray safeUwagi = narazenie.second.uwagi ;
        if (safeUwagi.size() > 200)
            safeUwagi = safeUwagi.first(200);
        HPDF_Page_TextRect(page, marginl + 10, startY - nrRow * 16 + 10,
                       marginl + 10 + wBox - 10, nrRow * 16 - 50 + 10,
                       safeUwagi.data(), HPDF_TALIGN_LEFT, NULL);
    }
    HPDF_Page_EndText (page);
    //end

    return startY - (nrRow+6)*16;
}

void PdfCreator::print_grid(HPDF_Doc pdf, HPDF_Page    page)
{
    HPDF_REAL height = HPDF_Page_GetHeight (page);
    HPDF_REAL width = HPDF_Page_GetWidth (page);
    HPDF_Font font = HPDF_GetFont (pdf, "Helvetica", NULL);
    HPDF_UINT x, y;

    HPDF_Page_SetFontAndSize (page, font, 5);
    HPDF_Page_SetGrayFill (page, 0.5);
    HPDF_Page_SetGrayStroke (page, 0.8);

    /* Draw horizontal lines */
    y = 0;
    while (y < height) {
        if (y % 10 == 0)
            HPDF_Page_SetLineWidth (page, 0.5);
        else {
            if (HPDF_Page_GetLineWidth (page) != 0.25)
                HPDF_Page_SetLineWidth (page, 0.25);
        }

        HPDF_Page_MoveTo (page, 0, y);
        HPDF_Page_LineTo (page, width, y);
        HPDF_Page_Stroke (page);

        if (y % 10 == 0 && y > 0) {
            HPDF_Page_SetGrayStroke (page, 0.5);

            HPDF_Page_MoveTo (page, 0, y);
            HPDF_Page_LineTo (page, 5, y);
            HPDF_Page_Stroke (page);

            HPDF_Page_SetGrayStroke (page, 0.8);
        }

        y += 5;
    }


    /* Draw virtical lines */
    x = 0;
    while (x < width) {
        if (x % 10 == 0)
            HPDF_Page_SetLineWidth (page, 0.5);
        else {
            if (HPDF_Page_GetLineWidth (page) != 0.25)
                HPDF_Page_SetLineWidth (page, 0.25);
        }

        HPDF_Page_MoveTo (page, x, 0);
        HPDF_Page_LineTo (page, x, height);
        HPDF_Page_Stroke (page);

        if (x % 50 == 0 && x > 0) {
            HPDF_Page_SetGrayStroke (page, 0.5);

            HPDF_Page_MoveTo (page, x, 0);
            HPDF_Page_LineTo (page, x, 5);
            HPDF_Page_Stroke (page);

            HPDF_Page_MoveTo (page, x, height);
            HPDF_Page_LineTo (page, x, height - 5);
            HPDF_Page_Stroke (page);

            HPDF_Page_SetGrayStroke (page, 0.8);
        }

        x += 5;
    }

    /* Draw horizontal text */
    y = 0;
    while (y < height) {
        if (y % 10 == 0 && y > 0) {
            char buf[12];

            HPDF_Page_BeginText (page);
            HPDF_Page_MoveTextPos (page, 5, y - 2);
#ifdef __WIN32__
            _snprintf (buf, 12, "%u", y);
#else
            snprintf (buf, 12, "%u", y);
#endif
            HPDF_Page_ShowText (page, buf);
            HPDF_Page_EndText (page);
        }

        y += 5;
    }


    /* Draw virtical text */
    x = 0;
    while (x < width) {
        if (x % 50 == 0 && x > 0) {
            char buf[12];

            HPDF_Page_BeginText (page);
            HPDF_Page_MoveTextPos (page, x, 5);
#ifdef __WIN32__
            _snprintf (buf, 12, "%u", x);
#else
            snprintf (buf, 12, "%u", x);
#endif
            HPDF_Page_ShowText (page, buf);
            HPDF_Page_EndText (page);

            HPDF_Page_BeginText (page);
            HPDF_Page_MoveTextPos (page, x, height - 10);
            HPDF_Page_ShowText (page, buf);
            HPDF_Page_EndText (page);
        }

        x += 5;
    }

    HPDF_Page_SetGrayFill (page, 0);
    HPDF_Page_SetGrayStroke (page, 0);
}

short PdfCreator::getErrCode() const
{
    return errCode;
}

void PdfCreator::setErrCode(short newErrCode)
{
    errCode = newErrCode;
}

void PdfCreator::drawTextInBoxLeft(HPDF_Page page, float posX, float posY, const QByteArray & text, float maxWidth)
{
    float tw = HPDF_Page_TextWidth(page, text.data());
    if (tw < maxWidth)
        HPDF_Page_TextOut (page, posX, posY, text.data());
    else {
        float twdot = HPDF_Page_TextWidth(page, "... ");
        QByteArray newArr;
        int pos = 0;
        float tw = 0;
        do {
            newArr.append(text.at(pos++));
            tw = HPDF_Page_TextWidth(page, newArr.data());
        } while (tw < maxWidth - twdot);
        newArr.append("...");
        HPDF_Page_TextOut (page, posX, posY, newArr.data());
    }
}

void PdfCreator::drawTextInBoxRight(HPDF_Page page, float posX, float posY, const QByteArray &text, float maxWidth)
{
    float tw = HPDF_Page_TextWidth(page, text.data());
    if (tw < maxWidth) {
        HPDF_Page_TextOut (page, posX + maxWidth - tw, posY, text.data());
    } else {
        drawTextInBoxLeft(page, posX, posY, text, maxWidth);

    }
}

void PdfCreator::drawTextInBoxCenter(HPDF_Page page, float posX, float posY, const QByteArray &text, float maxWidth)
{
    float tw = HPDF_Page_TextWidth(page, text.data());
    if (tw < maxWidth) {
        HPDF_Page_TextOut (page, posX + (maxWidth - tw)/2, posY, text.data());
    } else {
        drawTextInBoxLeft(page, posX, posY, text, maxWidth);

    }
}

double PdfCreator::d2p(const double &val)
{
    double l = std::pow(10, val/10);
    if (l == 0)
        return 0;
    return 100.0/l;
}

QString PdfCreator::d2p(const QString &val)
{
    bool ok;
    double dval = val.toDouble(&ok);
    if (!ok)
        return "-";
    return QString::number(d2p(dval), 'f', 2);
}
