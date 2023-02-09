#ifndef PDFCREATOR_H
#define PDFCREATOR_H

#include <QString>
#include <QByteArray>
#include <QVector>
#include <hpdf.h>
#include <QVector>
#include "parametrybadania.h"
#include "danetestu.h"

class QTextCodec;

struct CzujkaInfo {
    QByteArray oznaczenie;
    QByteArray transmiter;
    QByteArray receiver;
};



struct OgolneParametryTestu
{
    QByteArray osobaWykonujaca;
    QByteArray wynikTestu;
    QByteArray dataRozpoczecia;
    QByteArray dataZakonczenia;
    QByteArray temperatura;
    QByteArray cisnienie;
    QByteArray wilgotnosc;
    QByteArray uwagi;
    QByteArray receiver;
    QByteArray transmiter;
};

struct CMinCMaxCRep {
    QByteArray Cmin1;
    QByteArray Cmin2;
    QByteArray Cmax1;
    QByteArray Cmax2;
    QByteArray Crep1;
    QByteArray Crep2;
    QByteArray CMaxCmin;
    QByteArray CMaxCrep;
    QByteArray CRepCMin;

};

struct NarazenieDane {
    QByteArray wynik;
    QByteArray opis;
    QByteArray uwagi;
};

struct TabelaDane {
    QVector<QByteArray> head;
    QVector<int> colwidth;
    QVector<short> colAlign;
    QVector<QVector<QByteArray> > dane;
    float leftMargin;
};

struct TestWszystko {
    CMinCMaxCRep wynikiC;
    OgolneParametryTestu ogolne;
    bool odtwarzalnosc;
    CzujkaInfo czujka;
    TabelaDane dane;
    NarazenieDane narazenie;
    bool czyNarazenie;
    QByteArray testName;
    bool showC;
    bool czyNarazenie2;
    CzujkaInfo czujka2;
    NarazenieDane narazenie2;
};

class PdfCreator
{
public:
    static QString error;

    PdfCreator();
    void create(const QString &data);
    void setData(const ParametryBadania & badanie, bool all, short id);

    void print_grid(HPDF_Doc pdf, HPDF_Page page);
    short getErrCode() const;
    void setErrCode(short newErrCode);
protected:
    void createPageTest(HPDF_Page page, HPDF_Font font, HPDF_Font font2,
                            const TestWszystko &testPage);


    void setTableData(const DaneTestu & test, TabelaDane & dane, NarazenieDane &narazenie, bool &czyNarazenie, bool &showC);
    QByteArray getTestName(const DaneTestu &test, short NrTestu);
private:
    //void drawTextInBox(HPDF_Page page, float posX, float posY, const QByteArray & text, float maxWidth);
    void drawTextInBoxRight(HPDF_Page page, float posX, float posY, const QByteArray & text, float maxWidth);
    void drawTextInBoxCenter(HPDF_Page page, float posX, float posY, const QByteArray &text, float maxWidth);
    void drawTextInBoxLeft(HPDF_Page page, float posX, float posY, const QByteArray &text, float maxWidth);
    void createInfoBadanie(HPDF_Page page, HPDF_Font font, HPDF_Font font2,
                           float startY, bool showUwagi, bool showPodpis);
    void createHead(HPDF_Page page, HPDF_Font font);
    void createInformacje(HPDF_Page page, HPDF_Font font, HPDF_Font font2);
    void createInformacjeKat(HPDF_Page page, HPDF_Font font, HPDF_Font font2);
    void createTablicaCzujek(HPDF_Page page, HPDF_Font font, HPDF_Font font2);
    void createDodatkoweParametry(HPDF_Page page, HPDF_Font font, HPDF_Font font2);

    float createTestInfo(HPDF_Page page, HPDF_Font font, HPDF_Font font2,
                                    const OgolneParametryTestu & test, const QByteArray &nazwaTest);
    float createTable(HPDF_Page page, HPDF_Font font, HPDF_Font font2, float marginl, float startY,
                      const QVector<QByteArray> & head,
                      const QVector<int> &colwidth, const QVector<short> &colCenter,
                      const QVector<QVector<QByteArray> > &dane);

    float createCminCmax(HPDF_Page page, HPDF_Font font, HPDF_Font font2, float startY, const CMinCMaxCRep & test, bool CRep);
    float createCzujka(HPDF_Page page, HPDF_Font font, HPDF_Font font2, float startY,
                       const QByteArray &etTransmiter, const QByteArray &etReceiver,
                       const QByteArray &transmiter, const QByteArray &receiver);
    float createNarazenie(HPDF_Page page, HPDF_Font font, HPDF_Font font2, float startY, const NarazenieDane &narazenie);

    void narazeniaWynik(const DaneTestu &test, TabelaDane &dane, NarazenieDane &narazenie, bool &czyNarazenie, bool &showC);

    double d2p(const double & val);
    QString d2p(const QString &val);
private:
    short errCode;
    QByteArray numerZlecenia;
    QByteArray numerBadania;
    QByteArray rodzajSystemu;
    QByteArray osobaOdpowiedzialna1;
    QByteArray uwagi1;
    QByteArray dataBadania;

    QByteArray typSystemu;
    QByteArray producent;
    QByteArray eTypTransmitter;
    QByteArray eTypReceiver;
    QByteArray eTransmitter;
    QByteArray eReceiver;
    QByteArray eNrTransmitter;
    QByteArray eNrReceiver;

    QByteArray typTransmitter;
    QByteArray typReceiver;
    QByteArray rozstawienieMinimalne;
    QByteArray rozstawienieMaksymalne;

    QByteArray nadajnikKatPionowy;
    QByteArray nadajnikKatPoziomy;
    QByteArray odbiornikKatPionowy;
    QByteArray odbiornikKatPoziomy;

    QVector<CzujkaInfo> czujki;

    short iloscStron;

    bool zasilaczZewnetrzny;
    bool zasilaczCentrala;
    QByteArray wartoscNapieciaZasilania;
    QByteArray nazwaCentraliPPOZ;
    bool wyzwalaniePradem;
    bool wyzwalaniePrzekaznikiem;
    QByteArray wartoscPrądu;
    QByteArray czasStabilizacjiCzujki;
    QByteArray czasStabilizacjiPoResecie;

    QVector<TestWszystko> testy;

    QTextCodec *codec;


};

#endif // PDFCREATOR_H
