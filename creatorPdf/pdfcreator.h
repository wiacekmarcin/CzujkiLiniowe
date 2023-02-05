#ifndef PDFCREATOR_H
#define PDFCREATOR_H

#include <QString>
#include <QByteArray>
#include <QVector>
#include <hpdf.h>

class QTextCodec;

struct czujkaInfo {
    QByteArray oznaczenie;
    QByteArray transmiter;
    QByteArray receiver;
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
    void drawTextInBox(HPDF_Page page, float posX, float posY, const QByteArray & text, float maxWidth);
    void createInfoBadanie(HPDF_Page page, HPDF_Font font, HPDF_Font font2,
                           float startY, bool showUwagi, bool showPodpis);
    void createHead(HPDF_Page page, HPDF_Font font);
    void createInformacje(HPDF_Page page, HPDF_Font font, HPDF_Font font2);
    void createInformacjeKat(HPDF_Page page, HPDF_Font font, HPDF_Font font2);
    void createTablicaCzujek(HPDF_Page page, HPDF_Font font, HPDF_Font font2);
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
    QTextCodec *codec;

};

#endif // PDFCREATOR_H
