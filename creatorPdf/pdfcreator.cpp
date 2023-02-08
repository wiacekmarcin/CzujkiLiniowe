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
    longjmp(env, 1);
}

PdfCreator::PdfCreator()
{
    codec = QTextCodec::codecForName("ISO 8859-2");
    //numerZlecenia = codec->fromUnicode(QString::fromUtf8("6/CNBOP-PIB/BA/2014"));
    numerZlecenia = codec->fromUnicode(QString::fromUtf8("6/CNBOP-PIB/BA/2014"));
    numerBadania = codec->fromUnicode(QString::fromUtf8("001"));
    osobaOdpowiedzialna1 = codec->fromUnicode(QString::fromUtf8("Marcin Wiącek"));
    uwagi1 = codec->fromUnicode(QString::fromUtf8("Uwagi"));
    dataBadania = codec->fromUnicode(QString::fromUtf8("2023-01-12"));

    typSystemu = codec->fromUnicode(QString::fromUtf8("Nadajnik-odbiornik + Reflektor"));
    producent = codec->fromUnicode(QString::fromUtf8("Fire Fighting Enterprises Ltd"));

    eTypTransmitter = codec->fromUnicode(QString::fromUtf8("Typ nadajnika-odbiornika"));
    eTypReceiver = codec->fromUnicode(QString::fromUtf8("Typ reflektora"));
    eTransmitter = codec->fromUnicode(QString::fromUtf8("Nadajnik-Odbiornik"));
    eReceiver = codec->fromUnicode(QString::fromUtf8("Reflektor"));
    eNrTransmitter = codec->fromUnicode(QString::fromUtf8("Numer nadajnika-odbiornika"));
    eNrReceiver = codec->fromUnicode(QString::fromUtf8("Numer reflektora"));

    typTransmitter = codec->fromUnicode(QString::fromUtf8("FD 705R/FD 710"));
    typReceiver = codec->fromUnicode(QString::fromUtf8("RL 107"));
    rozstawienieMinimalne = codec->fromUnicode(QString::fromUtf8("10,0 m"));
    rozstawienieMaksymalne = codec->fromUnicode(QString::fromUtf8("100,0 m"));

    nadajnikKatPionowy = codec->fromUnicode(QString::fromUtf8("1,23")+QChar(0xb0));
    nadajnikKatPoziomy = codec->fromUnicode(QString::fromUtf8("1,43")+QChar(0xb0));
    odbiornikKatPionowy = codec->fromUnicode(QString::fromUtf8("2,45")+QChar(0xb0));
    odbiornikKatPoziomy = codec->fromUnicode(QString::fromUtf8("2.55")+QChar(0xb0));

    czujki.clear();
    czujki.append(czujkaInfo{QByteArray("1"),QByteArray("0102-0717"),QByteArray("1")});
    czujki.append(czujkaInfo{QByteArray("2"),QByteArray("0102-0718"),QByteArray("2")});
    //czujki.append(czujkaInfo{QByteArray("--"),QByteArray("--"),QByteArray("--")});
    //czujki.append(czujkaInfo{QByteArray("--"),QByteArray("--"),QByteArray("--")});
    //czujki.append(czujkaInfo{QByteArray("--"),QByteArray("--"),QByteArray("--")});
    //czujki.append(czujkaInfo{QByteArray("--"),QByteArray("--"),QByteArray("--")});
    //czujki.append(czujkaInfo{QByteArray("--"),QByteArray("--"),QByteArray("--")});

    zasilaczZewnetrzny = true;
    zasilaczCentrala = false;

    wartoscNapieciaZasilania = codec->fromUnicode(QString::fromUtf8("24,000 V"));
    nazwaCentraliPPOZ  = codec->fromUnicode(QString::fromUtf8("Centrala PPOZ"));

    wyzwalaniePradem = true;
    wyzwalaniePrzekaznikiem = false;

    wartoscPrądu = codec->fromUnicode(QString::fromUtf8("30,0 mA"));

    czasStabilizacjiCzujki =  codec->fromUnicode(QString::fromUtf8("900 s"));
    czasStabilizacjiPoResecie = codec->fromUnicode(QString::fromUtf8("5 s"));;



    iloscStron = 2;

    create();
}

void PdfCreator::create()
{


    HPDF_Doc  pdf;
    HPDF_Font font, font2;
    HPDF_Page page;
    char fname[256];
    HPDF_Outline root;


    strcpy (fname, "atest.pdf");

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
    font = HPDF_GetFont (pdf, "Helvetica", "ISO8859-2");
    font2 = HPDF_GetFont (pdf, "Courier", "ISO8859-2");

    /* load font object */
    //#ifdef __WIN32__
    //font_name = HPDF_LoadType1FontFromFile (pdf, "a010013l.afm",
    //        "a010013l.pfb");
    //#else
    //font_name = HPDF_LoadType1FontFromFile (pdf, "a010013l.afm",
    //        "a010013l.pfb");
    //#endif


    //font = HPDF_GetFont (pdf, font_name, "ISO8859-2");

    iloscStron = 3;
    /* add a new page object. */
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

    createHead(page, font);
    createInfoBadanie(page, font, font2, 176, true, true);
    createInformacje(page, font, font2);
    createInformacjeKat(page, font, font2);
    createTablicaCzujek(page, font, font2);
    createDodatkoweParametry(page, font, font2);




    page = HPDF_AddPage (pdf);
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 8);
    strona = codec->fromUnicode(QString::fromUtf8("Strona 2 z %1").arg(iloscStron));
    tw = HPDF_Page_TextWidth (page, strona.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw) / 2,
                25, strona.data());
    HPDF_Page_EndText (page);

    /* draw grid to the page */
    //print_grid  (pdf, page);
    createInfoBadanie(page, font, font2, 60, false, false);




    OgolneParametryTestu test;
    test.osobaWykonujaca = codec->fromUnicode(QString::fromUtf8("Michał Wiącek"));
    test.wynikTestu = codec->fromUnicode(QString::fromUtf8("POZYTYWNY"));
    test.dataRozpoczecia = codec->fromUnicode(QString::fromUtf8("2023-01-19 17:34"));
    test.dataZakonczenia = codec->fromUnicode(QString::fromUtf8("2023-01-19 22:57"));
    test.temperatura = codec->fromUnicode(QString::fromUtf8("21,5 ")+QChar(0xb0)+ QChar('C'));
    test.wilgotnosc = codec->fromUnicode(QString::fromUtf8("67 %"));
    test.cisnienie = codec->fromUnicode(QString::fromUtf8("1012 mBar"));
    test.uwagi = codec->fromUnicode(QString::fromUtf8("Uwago do testu"));

    CMinCMaxCRep wynik;
    wynik.Cmin1 = codec->fromUnicode(QString::fromUtf8("2,00 dB"));
    wynik.Cmin2 = codec->fromUnicode(QString::fromUtf8("36,92 %"));
    wynik.Cmax1 = codec->fromUnicode(QString::fromUtf8("2,00 dB"));
    wynik.Cmax2 = codec->fromUnicode(QString::fromUtf8("36,92 %"));
    wynik.Crep1 = codec->fromUnicode(QString::fromUtf8("2,00 dB"));
    wynik.Crep2 = codec->fromUnicode(QString::fromUtf8("36,92 %"));
    wynik.CRepCMin = codec->fromUnicode(QString::fromUtf8("1,00"));
    wynik.CMaxCrep = codec->fromUnicode(QString::fromUtf8("1,00"));

    QVector<QVector<QByteArray> > dane;
    QVector<QByteArray> r;
    r << codec->fromUnicode(QString::fromUtf8("1"));
    r << codec->fromUnicode(QString::fromUtf8("0102-0717"));
    r << codec->fromUnicode(QString::fromUtf8("1"));
    r << codec->fromUnicode(QString::fromUtf8("2,00 dB"));
    r << codec->fromUnicode(QString::fromUtf8("36,9 %"));
    r << codec->fromUnicode(QString::fromUtf8("POZYTYWNY"));
    r << codec->fromUnicode(QString::fromUtf8("-"));
    dane << r;
    r.clear();
    r << codec->fromUnicode(QString::fromUtf8("2"));
    r << codec->fromUnicode(QString::fromUtf8("0102-0718"));
    r << codec->fromUnicode(QString::fromUtf8("2"));
    r << codec->fromUnicode(QString::fromUtf8("2,00 dB"));
    r << codec->fromUnicode(QString::fromUtf8("36,9 %"));
    r << codec->fromUnicode(QString::fromUtf8("POZYTYWNY"));
    r << codec->fromUnicode(QString::fromUtf8("-"));
    dane << r;
    r.clear();
    r << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-"));
    r << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-"));
    r << codec->fromUnicode(QString::fromUtf8("-"));
    dane << r;
    r.clear();
    r << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-"));
    r << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-"));
    r << codec->fromUnicode(QString::fromUtf8("-"));
    dane << r;
    r.clear();
    r << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-"));
    r << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-"));
    r << codec->fromUnicode(QString::fromUtf8("-"));
    dane << r;
    r.clear();
    r << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-"));
    r << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-"));
    r << codec->fromUnicode(QString::fromUtf8("-"));
    dane << r;
    r.clear();
    r << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-"));
    r << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-")) << codec->fromUnicode(QString::fromUtf8("-"));
    r << codec->fromUnicode(QString::fromUtf8("-"));
    dane << r;
    r.clear();
    createOdtwarzalnosc(page, font, font2, test, wynik, dane);

    page = HPDF_AddPage (pdf);
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 8);
    strona = codec->fromUnicode(QString::fromUtf8("Strona 3 z %1").arg(iloscStron));
    tw = HPDF_Page_TextWidth (page, strona.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw) / 2,
                25, strona.data());
    HPDF_Page_EndText (page);

    /* draw grid to the page */
    //print_grid  (pdf, page);
    createInfoBadanie(page, font, font2, 60, false, false);
{
        OgolneParametryTestu test;
        test.osobaWykonujaca = codec->fromUnicode(QString::fromUtf8("Mateusz Wiącek"));
        test.wynikTestu = codec->fromUnicode(QString::fromUtf8("POZYTYWNY"));
        test.dataRozpoczecia = codec->fromUnicode(QString::fromUtf8("2023-02-19 11:34"));
        test.dataZakonczenia = codec->fromUnicode(QString::fromUtf8("2023-02-23 15:57"));
        test.temperatura = codec->fromUnicode(QString::fromUtf8("27,5 ")+QChar(0xb0)+ QChar('C'));
        test.wilgotnosc = codec->fromUnicode(QString::fromUtf8("87 %"));
        test.cisnienie = codec->fromUnicode(QString::fromUtf8("912 mBar"));
        test.uwagi = codec->fromUnicode(QString::fromUtf8("Badania testowe"));
        test.transmiter = codec->fromUnicode(QString::fromUtf8("0102-717"));
        test.receiver = codec->fromUnicode(QString::fromUtf8("2"));

        CMinCMaxCRep wynik;
        wynik.Cmin1 = codec->fromUnicode(QString::fromUtf8("2,00 dB"));
        wynik.Cmin2 = codec->fromUnicode(QString::fromUtf8("36,92 %"));
        wynik.Cmax1 = codec->fromUnicode(QString::fromUtf8("2,00 dB"));
        wynik.Cmax2 = codec->fromUnicode(QString::fromUtf8("36,92 %"));
        wynik.Crep1 = codec->fromUnicode(QString::fromUtf8("2,00 dB"));
        wynik.Crep2 = codec->fromUnicode(QString::fromUtf8("36,92 %"));
        wynik.CRepCMin = codec->fromUnicode(QString::fromUtf8("1,00"));
        wynik.CMaxCrep = codec->fromUnicode(QString::fromUtf8("1,00"));
        wynik.CMaxCmin = codec->fromUnicode(QString::fromUtf8("1,00"));

        QVector<QVector<QByteArray> > dane;
        QVector<QByteArray> r;
        r << codec->fromUnicode(QString::fromUtf8("1"));
        r << codec->fromUnicode(QString::fromUtf8("2,00 dB"));
        r << codec->fromUnicode(QString::fromUtf8("36,9 %"));
        r << codec->fromUnicode(QString::fromUtf8("POZYTYWNY"));
        r << codec->fromUnicode(QString::fromUtf8("-"));
        dane << r;
        r.clear();
        r << codec->fromUnicode(QString::fromUtf8("2"));
        r << codec->fromUnicode(QString::fromUtf8("2,00 dB"));
        r << codec->fromUnicode(QString::fromUtf8("36,9 %"));
        r << codec->fromUnicode(QString::fromUtf8("POZYTYWNY"));
        r << codec->fromUnicode(QString::fromUtf8("-"));
        dane << r;
        r.clear();
        r << codec->fromUnicode(QString::fromUtf8("3"));
        r << codec->fromUnicode(QString::fromUtf8("2,00 dB"));
        r << codec->fromUnicode(QString::fromUtf8("36,9 %"));
        r << codec->fromUnicode(QString::fromUtf8("POZYTYWNY"));
        r << codec->fromUnicode(QString::fromUtf8("-"));
        dane << r;
        r.clear();
        r << codec->fromUnicode(QString::fromUtf8("4"));
        r << codec->fromUnicode(QString::fromUtf8("2,00 dB"));
        r << codec->fromUnicode(QString::fromUtf8("36,9 %"));
        r << codec->fromUnicode(QString::fromUtf8("POZYTYWNY"));
        r << codec->fromUnicode(QString::fromUtf8("-"));
        dane << r;

        createPowtarzalnosc(page, font, font2, test, wynik, dane);
}
    /* save the document to a file */
    errCode = HPDF_SaveToFile (pdf, fname);

    /* clean up */
    HPDF_Free (pdf);

}

void PdfCreator::createHead(HPDF_Page page, HPDF_Font font)
{
    QByteArray p_title1 = codec->fromUnicode(QString::fromUtf8("CENTRUM NAUKOWO BADAWCZE OCHRONY PRZECIWPOŻAROWEJ"));
    QByteArray p_title2 = codec->fromUnicode(QString::fromUtf8("im. Józefa Tuliszkowskiego w Józefowie"));
    QByteArray p_title3 = codec->fromUnicode(QString::fromUtf8("ZAKRES BADAŃ POSIADAJĄCY AKREDYTACJĘ POLSKIEGO CENTRUM AKREDYTACJI"));
    QByteArray p_title4 = codec->fromUnicode(QString::fromUtf8("Numer certyfikatu akredytacji - AB 207"));

    float tw1, tw2, tw3, tw4;

    /* print the title of the page (with positioning center). */
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 16);
    tw1 = HPDF_Page_TextWidth (page, p_title1.data());
    tw2 = HPDF_Page_TextWidth (page, p_title2.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw1) / 2,
                HPDF_Page_GetHeight (page) - 90, p_title1.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw2) / 2,
                HPDF_Page_GetHeight (page) - 105, p_title2.data());
    HPDF_Page_EndText (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 13);
    tw3 = HPDF_Page_TextWidth (page, p_title3.data());
    tw4 = HPDF_Page_TextWidth (page, p_title4.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw3) / 2,
                HPDF_Page_GetHeight (page) - 130, p_title3.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw4) / 2,
                HPDF_Page_GetHeight (page) - 145, p_title4.data());

    HPDF_Page_EndText (page);
}

void PdfCreator::createInfoBadanie(HPDF_Page page, HPDF_Font font, HPDF_Font font2,
                                   float startY, bool showUwagi, bool showPodpis)
{
    float col1width = 130; //lewy margines etykiet
    QByteArray eNumerZlecenia = codec->fromUnicode(QString::fromUtf8("Numer zlecenia:"));
    QByteArray eNumerBadania = codec->fromUnicode(QString::fromUtf8("Numer badania:"));
    QByteArray eOsobaOdpowiedzialna = codec->fromUnicode(QString::fromUtf8("Osoba odpowiedzialna:"));
    QByteArray eUwagi = codec->fromUnicode(QString::fromUtf8("Uwagi:"));
    QByteArray eData = codec->fromUnicode(QString::fromUtf8("Data:"));
    QByteArray ePodpis = codec->fromUnicode(QString::fromUtf8("Podpis:"));


    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);

    float tw1 = HPDF_Page_TextWidth (page, eNumerZlecenia.data());
    float tw2 = HPDF_Page_TextWidth (page, eNumerBadania.data());
    float tw3 = HPDF_Page_TextWidth (page, eOsobaOdpowiedzialna.data());
    float tw4 = HPDF_Page_TextWidth (page, eUwagi.data());
    float twD, twP;

    //176
    HPDF_Page_TextOut (page, col1width - tw1, HPDF_Page_GetHeight(page) -startY, eNumerZlecenia.data());
    HPDF_Page_TextOut (page, col1width - tw2, HPDF_Page_GetHeight(page) -startY -16, eNumerBadania.data());
    HPDF_Page_TextOut (page, col1width - tw3, HPDF_Page_GetHeight(page) -startY -32, eOsobaOdpowiedzialna.data());
    if (showUwagi)
        HPDF_Page_TextOut (page, col1width - tw4,  HPDF_Page_GetHeight (page) - startY - 48, eUwagi.data());
    HPDF_Page_EndText (page);

    HPDF_Page_SetLineWidth (page, 2);
    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);

    float width1Table = 285;
    //176
    HPDF_Page_Rectangle(page, col1width + 5, HPDF_Page_GetHeight(page) -startY -4 , width1Table, 16);
    HPDF_Page_Rectangle(page, col1width + 5, HPDF_Page_GetHeight(page) -startY -20 , width1Table, 16);
    HPDF_Page_Rectangle(page, col1width + 5, HPDF_Page_GetHeight(page) -startY -36 , width1Table, 16);
    if (showUwagi)
        HPDF_Page_Rectangle(page, col1width + 5, HPDF_Page_GetHeight(page) -startY -86 , width1Table, 50);
    HPDF_Page_Stroke (page);


    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);
    //176
    drawTextInBoxLeft (page, col1width + 10, HPDF_Page_GetHeight(page) -startY, numerZlecenia.data(), width1Table-10);
    drawTextInBoxLeft (page, col1width + 10, HPDF_Page_GetHeight(page) -startY-16, numerBadania.data(), width1Table-10);
    drawTextInBoxLeft (page, col1width + 10, HPDF_Page_GetHeight(page) -startY-32, osobaOdpowiedzialna1.data(), width1Table-10);
    if (showUwagi)
        HPDF_Page_TextRect (page, col1width + 10, HPDF_Page_GetHeight (page) -startY-39,
                            col1width + 10 + width1Table-10, HPDF_Page_GetHeight(page) -startY-83,
                        uwagi1.first(200).data(), HPDF_TALIGN_LEFT, NULL);
    HPDF_Page_EndText (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);
    twD = HPDF_Page_TextWidth(page, eData.data());
    twP = HPDF_Page_TextWidth(page, ePodpis.data());
    float boxDataPodpisWidth = 120;
    HPDF_Page_TextOut (page, 500-twD/2, HPDF_Page_GetHeight(page) -startY, eData.data());
    if (showPodpis)
        HPDF_Page_TextOut (page, 500-twP/2, HPDF_Page_GetHeight(page) -startY-37, ePodpis.data());
    HPDF_Page_EndText (page);

    HPDF_Page_Rectangle(page, 500-boxDataPodpisWidth/2, HPDF_Page_GetHeight (page) -startY-20 , boxDataPodpisWidth, 16);
    if (showPodpis)
        HPDF_Page_Rectangle(page, 500-boxDataPodpisWidth/2, HPDF_Page_GetHeight (page) -startY-86 , boxDataPodpisWidth, 44);
    HPDF_Page_Stroke (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);
    //twD = HPDF_Page_TextWidth(page, dataBadania.data());
    //drawTextInBoxCenter (page, 500-twD/2, HPDF_Page_GetHeight(page) -startY-15, dataBadania.data(), boxDataPodpisWidth-10);
    drawTextInBoxCenter (page, 500-boxDataPodpisWidth/2+5, HPDF_Page_GetHeight(page) -startY-15, dataBadania.data(), boxDataPodpisWidth-10);
    HPDF_Page_EndText (page);

}

void PdfCreator::createInformacje(HPDF_Page page, HPDF_Font font, HPDF_Font font2)
{
    QByteArray p_title = codec->fromUnicode(QString::fromUtf8("INFORMACJE O BADANYCH CZUJKACH"));

    float col1width = 150; //lewy margines etykiet
    QByteArray eRodzajSystemu = codec->fromUnicode(QString::fromUtf8("Rodzaj systemu:"));
    QByteArray eProducent = codec->fromUnicode(QString::fromUtf8("Producent:"));
    QByteArray eTransmitter = eTypTransmitter + ':';
    QByteArray eReceiver = eTypReceiver + ':';

    QByteArray erozstawienieMinimalne = codec->fromUnicode(QString::fromUtf8("Rozstawienie minimalne:"));
    QByteArray erozstawienieMaksymalne = codec->fromUnicode(QString::fromUtf8("Rozstawienie maksymalne:"));
    float width1Table = 380;
    float widthRozstBox = 75;

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 12);
    float tw = HPDF_Page_TextWidth (page, p_title.data());
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw) / 2,
                HPDF_Page_GetHeight (page) - 295, p_title.data());
    HPDF_Page_EndText (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);
    float tw1 = HPDF_Page_TextWidth (page, eRodzajSystemu.data());
    float tw2 = HPDF_Page_TextWidth (page, eProducent.data());
    float tw3 = HPDF_Page_TextWidth (page, eTransmitter.data());
    float tw4 = HPDF_Page_TextWidth (page, eReceiver.data());
    float twRmin = HPDF_Page_TextWidth (page, erozstawienieMinimalne.data());
    float twRmax = HPDF_Page_TextWidth (page, erozstawienieMaksymalne.data());

    float startY = HPDF_Page_GetHeight(page) - 320;
    HPDF_Page_TextOut (page, col1width - tw1, startY, eRodzajSystemu.data());
    HPDF_Page_TextOut (page, col1width - tw2, startY -16, eProducent.data());
    HPDF_Page_TextOut (page, col1width - tw3, startY -32, eTransmitter.data());
    HPDF_Page_TextOut (page, col1width - tw4, startY - 48, eReceiver.data());
    HPDF_Page_TextOut (page, col1width - twRmin, startY - 64, erozstawienieMinimalne.data());
    HPDF_Page_TextOut (page, col1width + 5 + width1Table - 5 - twRmax - widthRozstBox,
                       startY - 64, erozstawienieMaksymalne.data());
    HPDF_Page_EndText (page);

    HPDF_Page_SetLineWidth (page, 2);
    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);

    HPDF_Page_Rectangle(page, col1width + 5, startY -4 , width1Table, 16);
    HPDF_Page_Rectangle(page, col1width + 5, startY -20 , width1Table, 16);
    HPDF_Page_Rectangle(page, col1width + 5, startY -36 , width1Table, 16);
    HPDF_Page_Rectangle(page, col1width + 5, startY -52 , width1Table, 16);
    HPDF_Page_Rectangle(page, col1width + 5, startY -68 , widthRozstBox, 16);
    HPDF_Page_Rectangle(page, col1width + 5 + width1Table - widthRozstBox, startY -68 , widthRozstBox, 16);
    HPDF_Page_Stroke (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font2, 10);

    drawTextInBoxLeft (page, col1width + 10, startY, typSystemu.data(), width1Table-10);
    drawTextInBoxLeft (page, col1width + 10, startY-16, producent.data(), width1Table-10);
    drawTextInBoxLeft (page, col1width + 10, startY-32, typTransmitter.data(), width1Table-10);
    drawTextInBoxLeft (page, col1width + 10, startY-48, typReceiver.data(), width1Table-10);

    //float twRminV = HPDF_Page_TextWidth (page, rozstawienieMinimalne.data());
    //drawTextInBox (page, col1width + 5 + widthRozstBox - 10 - twRminV,
    //               startY-64, rozstawienieMinimalne.data(), widthRozstBox-10);
    drawTextInBoxRight (page, col1width + 10,
                   startY-64, rozstawienieMinimalne.data(), widthRozstBox-10);


    //float twRmaxV = HPDF_Page_TextWidth (page, rozstawienieMaksymalne.data());
    //drawTextInBox (page, col1width + 5 + width1Table - 10 - twRmaxV,
    //               startY-64, rozstawienieMaksymalne.data(), widthRozstBox-10);

    drawTextInBoxRight (page, col1width + 10 + width1Table - widthRozstBox + 5,
                   startY-64, rozstawienieMinimalne.data(), widthRozstBox-10);

    HPDF_Page_EndText (page);
}

void PdfCreator::createInformacjeKat(HPDF_Page page, HPDF_Font font, HPDF_Font font2)
{

    float marginleft = 35; //lewy margines etykiet
    float width1Column = 200;
    float width2Column = 150;
    float width3Column = 150;
    float startY = HPDF_Page_GetHeight(page) - 415;;
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
}

void PdfCreator::createTablicaCzujek(HPDF_Page page, HPDF_Font font, HPDF_Font font2)
{
    QByteArray tytul = codec->fromUnicode(QString::fromUtf8("NUMERY SERYJNE BADANYCH CZUJEK"));
    QByteArray eOznaczenie = codec->fromUnicode(QString::fromUtf8("Oznaczenie"));

    float startY = HPDF_Page_GetHeight(page) - 470;

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

}

void PdfCreator::createDodatkoweParametry(HPDF_Page page, HPDF_Font font, HPDF_Font font2)
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

    float startY = HPDF_Page_GetHeight(page) - 635;
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

}

void PdfCreator::createOdtwarzalnosc(HPDF_Page page, HPDF_Font font, HPDF_Font font2,
                                     const OgolneParametryTestu & daneOgolne,
                                     const CMinCMaxCRep & wyniki,
                                     const QVector<QVector<QByteArray> > &dane)
{
    QString nazwaTestu = QString::fromUtf8("TEST NR 1: Odtwarzalność (Reproducibility)");
    float endY = createTestInfo(page, font, font2, daneOgolne, nazwaTestu);



    endY = createCminCmax(page, font, font2, endY - 15, wyniki, true);

    QVector<QByteArray> head;
    head << codec->fromUnicode(QString::fromUtf8("Próba"));
    head << eTransmitter;
    head << eReceiver;
    head << codec->fromUnicode(QString::fromUtf8("C[n]"));
    head << codec->fromUnicode(QString::fromUtf8("C[n]"));
    head << codec->fromUnicode(QString::fromUtf8("Wynik"));
    head << codec->fromUnicode(QString::fromUtf8("Uwagi"));

    QVector<int> colwidth;
    colwidth << 40 << 100 << 100 << 65 << 55 << 70 << 100;

    QVector<short> colAlign;
    colAlign << 0 << -1 << -1 << 1 << 1 << 0 << -1;

    endY = createTable(page, font, font2, 30, endY - 30, head, colwidth, colAlign, dane);

}

void PdfCreator::createPowtarzalnosc(HPDF_Page page, HPDF_Font font, HPDF_Font font2,
                                     const OgolneParametryTestu & daneOgolne,
                                     const CMinCMaxCRep & wyniki,
                                     const QVector<QVector<QByteArray> > &dane)
{
    QString nazwaTestu = QString::fromUtf8("TEST NR 2: Powtarzalność (Repeatability)");
    float endY = createTestInfo(page, font, font2, daneOgolne, nazwaTestu);

    endY = createCzujka(page, font, font2, endY-15, eTransmitter, eReceiver, daneOgolne.transmiter, daneOgolne.receiver);


    endY = createCminCmax(page, font, font2, endY - 15, wyniki, false);

    QVector<QByteArray> head;
    head << codec->fromUnicode(QString::fromUtf8("Próba"));
    head << codec->fromUnicode(QString::fromUtf8("C[n]"));
    head << codec->fromUnicode(QString::fromUtf8("C[n]"));
    head << codec->fromUnicode(QString::fromUtf8("Wynik"));
    head << codec->fromUnicode(QString::fromUtf8("Uwagi"));

    QVector<int> colwidth;
    colwidth << 40 << 90 << 90 << 80 << 200;

    QVector<short> colAlign;
    colAlign << 0 << 1 << 1 << 0 << -1;

    endY = createTable(page, font, font2, 50, endY - 30, head, colwidth, colAlign, dane);

}

float PdfCreator::createTestInfo(HPDF_Page page, HPDF_Font font, HPDF_Font font2,
                                const OgolneParametryTestu & test, const QString & nazwaTestu)
{
    QByteArray tytulTestu = codec->fromUnicode(nazwaTestu);
    QByteArray eOsobaOdpowiedzialna = codec->fromUnicode(QString::fromUtf8("Osoba wykonująca:"));
    QByteArray eDataRozpoczecia = codec->fromUnicode(QString::fromUtf8("Data i czas rozpoczęcia:"));
    QByteArray eDataZakonczenia = codec->fromUnicode(QString::fromUtf8("Data i czas zakończenia:"));
    QByteArray eTemperatura = codec->fromUnicode(QString::fromUtf8("Temperatura:"));
    QByteArray eWilgotnosc = codec->fromUnicode(QString::fromUtf8("Wilgotność:"));
    QByteArray ecisnienie = codec->fromUnicode(QString::fromUtf8("Ciśnienie:"));
    QByteArray ewynikTestu = codec->fromUnicode(QString::fromUtf8("Wynik testu:"));
    QByteArray eUwagiTestu = codec->fromUnicode(QString::fromUtf8("Uwagi:"));

    float startY = HPDF_Page_GetHeight (page) - 140;
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
    HPDF_Page_TextRect(page, marginl + 10, startY - nrRow * 16 + 10,
                       marginl + 10 + wBox - 10, nrRow * 16 - 50 + 10,
                       test.uwagi.first(200).data(), HPDF_TALIGN_LEFT, NULL);
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
    HPDF_Page_SetRGBStroke (page, 0, 0, 0);
    HPDF_Page_SetLineWidth (page, 1);

    float actY = startY+16;
    float actPosX = marginl;
    float ret = 0;
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
        drawTextInBoxRight (page, marginl + 10,          startY - (nrRow++)*16, test.CRepCMin.data(), wBox-10);
        drawTextInBoxRight (page, marginl + 10,          startY - (nrRow++)*16, test.CMaxCrep.data(), wBox-10);
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
        QByteArray newArr = text.first(26);
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