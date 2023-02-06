#ifndef PDFCREATOR_H
#define PDFCREATOR_H

#include <QString>
#include <QByteArray>
#include <QVector>
#include <hpdf.h>
#include <QVector>

class QTextCodec;

struct czujkaInfo {
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

class PdfCreator
{
public:
    PdfCreator();
    void create();

    void print_grid(HPDF_Doc pdf, HPDF_Page page);
    short getErrCode() const;
    void setErrCode(short newErrCode);
protected:
    void createOdtwarzalnosc(HPDF_Page page, HPDF_Font font, HPDF_Font font2,
                            const OgolneParametryTestu & daneOgolne,
                            const CMinCMaxCRep & wyniki,
                            const QVector<QVector<QByteArray> > &dane);
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
                                    const OgolneParametryTestu & test, const QString & nazwaTest);
    float createTable(HPDF_Page page, HPDF_Font font, HPDF_Font font2, float marginl, float startY,
                      const QVector<QByteArray> & head,
                      const QVector<int> &colwidth, const QVector<short> &colCenter,
                      const QVector<QVector<QByteArray> > &dane);

    float createCminCmax(HPDF_Page page, HPDF_Font font, HPDF_Font font2, float startY, const CMinCMaxCRep & test, bool CRep);

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

    QVector<czujkaInfo> czujki;

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


    QTextCodec *codec;

};

#endif // PDFCREATOR_H
