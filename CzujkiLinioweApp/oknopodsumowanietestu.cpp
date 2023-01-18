#include "oknopodsumowanietestu.h"
#include "ui_oknopodsumowanietestu.h"
#include <QDateTime>
#include "ustawienia.h"

OknoPodsumowanieTestu::OknoPodsumowanieTestu(DaneTestu &daneTestu, const ParametryBadania & badanie,
                                             const Ustawienia & ust_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoPodsumowanieTestu),
    ust(ust_)

{
    ui->setupUi(this);
    ui->testName->setText(daneTestu.getName());
    qDebug() << "id" << daneTestu.getId();
    QString pierwszy = daneTestu.getNazwaNumerPierwszego();
    QString  drugi = daneTestu.getNazwaNumerDrugiego();

    ui->result->setText(daneTestu.getOk() ? "POZYTYWNY" : "NEGATYWNY");
    if (daneTestu.getId() == REPRODUCIBILITY) {
        obliczOdtwarzalnosc(daneTestu);
        ui->stackedWidget->setCurrentWidget(ui->odtwarzalnosc);
        odtwarzalnoscHeadTable(ui->odtwarzalnoscframeTable, ui->odtwarzalnoscGridLayoutResults, "odtwarzalnosc",
                               pierwszy, drugi);

        ui->odtwarzalnoscCrep->setText(QString::number(daneTestu.getCrep(), 'f', 2) + " dB");
        ui->odtwarzalnoscCrep2->setText("0 %");
        ui->odtwarzalnoscCmin->setText(QString::number(daneTestu.getCmin(), 'f', 2) + " dB");
        ui->odtwarzalnoscCmin2->setText("0 %");
        ui->odtwarzalnoscCmax->setText(QString::number(daneTestu.getCmax(), 'f', 2) + " dB");
        ui->odtwarzalnoscCmax2->setText("0 %");
        ui->odtwarzalnoscCmaxCrep->setText(QString::number(daneTestu.getCmaxCrep(), 'f', 2));
        ui->odtwarzalnoscCrepCmin->setText(QString::number(daneTestu.getCrepCmin(), 'f', 2));
        if (daneTestu.getCmaxCrep() > ust.getOdtwarzalnoscCmaxCrep())
            ui->odtwarzalnoscCmaxCrep->setStyleSheet("background-color:red");
        if (daneTestu.getCrepCmin() > ust.getOdtwarzalnoscCrepCmin())
            ui->odtwarzalnoscCrepCmin->setStyleSheet("background-color:red");

        short num = 0;

        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            odtwarzalnoscAddRekord(ui->odtwarzalnoscframeTable, ui->odtwarzalnoscGridLayoutResults, "odtwarzalnosc",
                                  num+1, dane.nrCzujki, dane.numerNadajnika, dane.numerOdbiornika,
                                    dane.value_dB, "0.0", dane.ok, dane.error);
            num++;
        }
        ui->odtwarzalnoscGridLayoutResults->setVerticalSpacing(0);
        ui->odtwarzalnoscGridLayoutResults->setHorizontalSpacing(0);
        ui->odtwarzalnoscGridLayoutResults->setSpacing(0);
        //badanie->s
    } else if (daneTestu.getId() == REPEATABILITY) {
        obliczPowtarzalnosc(daneTestu);
        ui->stackedWidget->setCurrentWidget(ui->powtarzalnosc);
        powtarzalnoscHeadTable(ui->frPowatarzalnoscPrzbieg, ui->powtarzalnoscPrzebiegrGridLayout, "powtarzalnosc");

        ui->powtarzalnoscCmin->setText(QString::number(daneTestu.getCmin(), 'f', 2) + " dB");
        ui->powtarzalnoscCmin2->setText("0 %");
        ui->powtarzalnoscCmax->setText(QString::number(daneTestu.getCmax(), 'f', 2) + " dB");
        ui->powtarzalnoscCmax2->setText("0 %");
        ui->powtarzalnoscCmaxCmin->setText(QString::number(daneTestu.getCmaxCmin(), 'f', 2));

        if (daneTestu.getCmaxCmin() > ust.getPowtarzalnoscCmaxCmin())
            ui->powtarzalnoscCmaxCmin->setStyleSheet("background-color:red");

        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            powtarzalnoscAddRekord(ui->frPowatarzalnoscPrzbieg, ui->powtarzalnoscPrzebiegrGridLayout, "powtarzalnosc",
                                  num, dane.value_dB, "0.0", dane.ok, dane.error);
            num++;
        }
        ui->powtarzalnoscPrzebiegrGridLayout->setVerticalSpacing(0);
        ui->powtarzalnoscPrzebiegrGridLayout->setHorizontalSpacing(0);
        ui->powtarzalnoscPrzebiegrGridLayout->setSpacing(0);
     }


    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
}

OknoPodsumowanieTestu::~OknoPodsumowanieTestu()
{
    delete ui;
}

#define ADDLINETABLETD(S)  addLine(fr, lay, true, row, col, 1, 1, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col));\
                        ++col;\
                        oneTableTd(ok, fr, lay, S, row, col, QString("label_%1_%2_%3").arg(suffix).arg(row).arg(col));\
                        ++col;

void OknoPodsumowanieTestu::odtwarzalnoscAddRekord(
        QFrame * fr, QGridLayout * lay, const QString & suffix,
        short r, short nrCzujki, const QString & nadajnik, const QString & odbiornik,
                                          const QString &tlumienie_db, const QString &tlumienie_per,
                                          bool ok, const QString &inneText)
{

    short col = 0;
    short row = 2*r+3;

    ADDLINETABLETD(QString::number(r));
    ADDLINETABLETD(QString::number(nrCzujki));
    ADDLINETABLETD(nadajnik);
    ADDLINETABLETD(odbiornik);
    ADDLINETABLETD(tlumienie_db);
    ADDLINETABLETD(tlumienie_per);
    ADDLINETABLETD(ok ? "POZYTYWNY" : "NEGATYWNY");
    addLine(fr, lay, true, row, col++, 1, 1, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col));
    if (inneText.isEmpty())
        oneTableTd(ok, fr, lay, inneText, row, col, QString("label_%1_%2_%3").arg(suffix).arg(row).arg(col));
    else
        oneTableFrame(ok, fr, lay, inneText, row, col, QString("frame_%1_%2_%3").arg(suffix).arg(row).arg(col));
    ++col;
    addLine(fr, lay, true, row, col, 1, 1, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col));
    ++col;

    addLine(fr, lay, false, row+1, 0, 1, col, QString("vertline_%1_%2").arg(suffix).arg(row));

}

#define ADDLINETABLEHEADTD(S,O,P) addLine(fr, lay, true, 1, col++, 1, 1, QString("%1 %2").arg(suffix,O)); \
                              oneHeadRecord(fr, lay, S, 1, col++, QString("%1 %2").arg(suffix,P));



void OknoPodsumowanieTestu::odtwarzalnoscHeadTable(QFrame * fr, QGridLayout * lay,
                                      const QString & suffix,
                                      const QString & nadajnik, const QString & odbiornik)
{
    short col = 0;
    ADDLINETABLEHEADTD("Nr Próby", "lhead0", "etProba");
    ADDLINETABLEHEADTD("Nr Czujki", "lhead1", "etCzujkaNr");
    ADDLINETABLEHEADTD(nadajnik, "lhead2", "etNadajnik");
    ADDLINETABLEHEADTD(odbiornik, "lhead3", "etOdbiornik");
    ADDLINETABLEHEADTD("<html><body><b>C<sub>[n]</sub></b> <i>[dB]</i></body></html>", "lhead4", "etCndB");
    ADDLINETABLEHEADTD("<html><body><b>C<sub>[n]</sub></b> <i>[%]</i></body></html>", "lhead5", "etCndPer");
    ADDLINETABLEHEADTD("Wynik", "lhead6", "etResult");
    ADDLINETABLEHEADTD("Uwagi", "lhead7", "etUwagi");
    addLine(fr, lay, true, 1, col++, 1, 1, QString("%1 %2").arg(suffix,"lhead8"));
    addLine(fr, lay, false, 0, 0, 1, col, QString("%1 %2").arg(suffix, "lheadUp"));
    addLine(fr, lay, false, 2, 0, 1, col, QString("%1 %2").arg(suffix, "lheadDown"));
}

void OknoPodsumowanieTestu::powtarzalnoscHeadTable(QFrame * fr, QGridLayout * lay,
                                      const QString & suffix)
{
    short col = 0;
    ADDLINETABLEHEADTD("Nr Próby", "lhead0", "etProba");
    ADDLINETABLEHEADTD("<html><body><b>C<sub>[n]</sub></b> <i>[dB]</i></body></html>", "lhead4", "etCndB");
    ADDLINETABLEHEADTD("<html><body><b>C<sub>[n]</sub></b> <i>[%]</i></body></html>", "lhead5", "etCndPer");
    ADDLINETABLEHEADTD("Wynik", "lhead6", "etResult");
    ADDLINETABLEHEADTD("Uwagi", "lhead6", "etUwagi");
    addLine(fr, lay, true, 1, col++, 1, 1, QString("%1 %2").arg(suffix,"lhead8"));

    addLine(fr, lay, false, 0, 0, 1, col, QString("%1 %2").arg(suffix,"lheadUp"));
    addLine(fr, lay, false, 2, 0, 1, col, QString("%1 %2").arg(suffix,"lheadDown"));
}

void OknoPodsumowanieTestu::powtarzalnoscAddRekord(
        QFrame * fr, QGridLayout * lay, const QString & suffix,
        short r, const QString &tlumienie_db, const QString &tlumienie_per,
                                          bool ok, const QString &inneText)
{

    short col = 0;
    short row = 2*r+3;

    ADDLINETABLETD(QString::number(r+1));
    ADDLINETABLETD(tlumienie_db);
    ADDLINETABLETD(tlumienie_per);
    ADDLINETABLETD(ok ? "POZYTYWNY" : "NEGATYWNY");
    addLine(fr, lay, true, row, col++, 1, 1, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col));
    if (inneText.isEmpty())
        oneTableTd(ok, fr, lay, inneText, row, col, QString("label_%1_%2_%3").arg(suffix).arg(row).arg(col));
    else
        oneTableFrame(ok, fr, lay, inneText, row, col, QString("frame_%1_%2_%3").arg(suffix).arg(row).arg(col));
    ++col;
    addLine(fr, lay, true, row, col, 1, 1, QString("line_%1_%2_%3").arg(suffix).arg(suffix).arg(row).arg(col));
    ++col;

    addLine(fr, lay, false, row+1, 0, 1, col, QString("vertline_%1_%2").arg(suffix).arg(row));

}

void OknoPodsumowanieTestu::oneHeadRecord(QFrame * frameTable, QGridLayout * layout,
                                          const QString & text, int row, int col, const QString & objectName)
{
    QLabel * lh = new QLabel(frameTable);
    lh->setObjectName(objectName);
    lh->setText(text);
    lh->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    QFont f(lh->font());
    f.setBold(true);
    lh->setFont(f);
    layout->addWidget(lh, row, col, 1, 1);
}

void OknoPodsumowanieTestu::oneTableTd(bool err, QFrame * frameTable, QGridLayout * layout, const QString & text, int row, int col,
                                       const QString & objectName)
{
    QLabel * l = new QLabel(frameTable);
    l->setObjectName(objectName);
    l->setText(text);
    if (!err) {
        l->setStyleSheet("background-color:red");
    }
    layout->addWidget(l, row, col, 1, 1);
}

void OknoPodsumowanieTestu::oneTableFrame(bool err, QFrame * frameTable, QGridLayout * layout,
                                          const QString & text, int row, int col, const QString & objectName)
{
    QFrame * frame = new QFrame(frameTable);
    frame->setObjectName(objectName);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    QHBoxLayout * horizontalLayout = new QHBoxLayout(frame);
    horizontalLayout->setSpacing(3);
    horizontalLayout->setObjectName(QString("horizontalLayout_%1").arg(objectName));
    horizontalLayout->setContentsMargins(5, 5, 5, 5);
    QLabel * label = new QLabel(frame);
    label->setObjectName(QString("label_%1").arg(objectName));
    horizontalLayout->addWidget(label);
    label->setText(text);
    if (!err) {
        frame->setStyleSheet("background-color:red");
        label->setStyleSheet("background-color:red");
    }
    layout->addWidget(frame, row, col, 1, 1);
}

void OknoPodsumowanieTestu::addLine(QFrame * frameTable, QGridLayout * layout,
                                                  bool vert, int row, int col,
                                                  int rowspan, int colspan,
                                                  const QString & objectName)
{
    QFrame * line = new QFrame(frameTable);
    line->setObjectName(objectName);
    line->setFrameShape(vert ? QFrame::VLine : QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(1);
    layout->addWidget(line, row, col, rowspan, colspan);
}

void OknoPodsumowanieTestu::obliczOdtwarzalnosc(DaneTestu &daneTestu)
{
    bool badanieOk = true;
    int cntAvg = 0;
    float Cavg = 0;

    float Cmin = 100;
    float Cmax = -100;


    for (DanePomiaru & dane : daneTestu.getDanePomiarowe())
    {
        bool ok;
        double C = dane.value_dB.toDouble(&ok);

        if (!ok) {
            badanieOk = false;
            daneTestu.setErrStr(QString::fromUtf8("Błędna wartość C"));
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
            daneTestu.setErrStr(dane.error);
            continue;
        }

        if (C < ust.getMinimalnaWartoscCzujkiCn()) {
            badanieOk = false;
            daneTestu.setErrStr(QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1));
            dane.ok = false;
            dane.error = QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1);
        }
    }
    if (cntAvg)
        Cavg = Cavg/cntAvg;

    daneTestu.setOk(badanieOk);
    daneTestu.setErrStr(badanieOk ? "" : "Czujka(i) nie przeszły badania");

    if (cntAvg && Cavg) {
        daneTestu.setCrep(Cavg);
        daneTestu.setCmin(Cmin);
        daneTestu.setCmax(Cmax);
        daneTestu.setCmaxCrep(Cmax/Cavg);
        daneTestu.setCrepCmin(Cavg/Cmin);
    } else {
        daneTestu.setCrep(0);
        daneTestu.setCmin(0);
        daneTestu.setCmax(0);
        daneTestu.setCmaxCrep(0);
        daneTestu.setCrepCmin(0);
    }
    daneTestu.setWykonany(true);

    if (daneTestu.getCmaxCrep() > ust.getOdtwarzalnoscCmaxCrep()) {
        daneTestu.setOk(false);
        daneTestu.setErrStr(QString("Cmax/Crep>%1").arg(ust.getOdtwarzalnoscCmaxCrep(), 3, 'f', 2));
    } else if (daneTestu.getCrepCmin() > ust.getOdtwarzalnoscCrepCmin()) {
        daneTestu.setOk(false);
        daneTestu.setErrStr(QString("Crep/Cmin>%1").arg(ust.getOdtwarzalnoscCrepCmin(), 3, 'f', 2));
    } else {
        daneTestu.setOk(badanieOk);
    }
}

void OknoPodsumowanieTestu::obliczPowtarzalnosc(DaneTestu &daneTestu)
{
    bool badanieOk = true;

    float Cmin = 100;
    float Cmax = -100;


    for (DanePomiaru & dane : daneTestu.getDanePomiarowe())
    {
        if (!dane.ok) {
            badanieOk = false;
            daneTestu.setErrStr(dane.error);
            continue;
        }
        bool ok;
        double C = dane.value_dB.toDouble(&ok);

        if (!ok) {
            badanieOk = false;
            daneTestu.setErrStr(QString::fromUtf8("Błędna wartość C"));
            dane.ok = false;
            continue;
        }

        if (!dane.ok) {
            badanieOk = false;
            daneTestu.setErrStr(QString::fromUtf8("Czujka nie zadziałała"));
            continue;
        }

        if (C > Cmax)
            Cmax = C;
        if (C < Cmin)
            Cmin = C;

        if (C < ust.getMinimalnaWartoscCzujkiCn()) {
            badanieOk = false;
            daneTestu.setErrStr(QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1));
            dane.ok = false;
            dane.error = QString::fromUtf8("Cn<%1").arg(ust.getMinimalnaWartoscCzujkiCn(), 2, 'f', 1);
        }
    }


    daneTestu.setOk(badanieOk);
    daneTestu.setErrStr(badanieOk ? "" : "Czujka(i) nie przeszły badania");

    if (Cmin) {
        daneTestu.setCmin(Cmin);
        daneTestu.setCmax(Cmax);
        daneTestu.setCmaxCmin(Cmax/Cmin);
    } else {
        daneTestu.setCmin(0);
        daneTestu.setCmax(0);
        daneTestu.setCmaxCmin(0);
    }
    daneTestu.setWykonany(true);

    if (daneTestu.getCmaxCmin() > ust.getPowtarzalnoscCmaxCmin()) {
        daneTestu.setOk(false);
        daneTestu.setErrStr(QString("Cmax/Cmin>%1").arg(ust.getPowtarzalnoscCmaxCmin(), 3, 'f', 2));
    } else {
        daneTestu.setOk(badanieOk);
    }
}
