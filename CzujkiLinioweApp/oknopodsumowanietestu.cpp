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
     } else if (daneTestu.getId() == TOLERANCE_TO_BEAM_MISALIGNMENT) {
        ui->stackedWidget->setCurrentWidget(ui->zaleznosckatowa);
        zaleznoscKatowaVector.push_back(ZaleznoscKatowaLabelsPtr{ui->zaleznosckatowa_1katproducent, ui->zaleznosckatowa_1_kat, ui->zaleznosckatowa_wyniki_1});
        zaleznoscKatowaVector.push_back(ZaleznoscKatowaLabelsPtr{ui->zaleznosckatowa_2katproducent, ui->zaleznosckatowa_2_kat, ui->zaleznosckatowa_wyniki_2});
        zaleznoscKatowaVector.push_back(ZaleznoscKatowaLabelsPtr{ui->zaleznosckatowa_3katproducent, ui->zaleznosckatowa_3_kat, ui->zaleznosckatowa_wyniki_3});
        zaleznoscKatowaVector.push_back(ZaleznoscKatowaLabelsPtr{ui->zaleznosckatowa_4katproducent, ui->zaleznosckatowa_4_kat, ui->zaleznosckatowa_wyniki_4});
        zaleznoscKatowaVector.push_back(ZaleznoscKatowaLabelsPtr{ui->zaleznosckatowa_5katproducent, ui->zaleznosckatowa_5_kat, ui->zaleznosckatowa_wyniki_5});
        zaleznoscKatowaVector.push_back(ZaleznoscKatowaLabelsPtr{ui->zaleznosckatowa_6katproducent, ui->zaleznosckatowa_6_kat, ui->zaleznosckatowa_wyniki_6});
        zaleznoscKatowaVector.push_back(ZaleznoscKatowaLabelsPtr{ui->zaleznosckatowa_7katproducent, ui->zaleznosckatowa_7_kat, ui->zaleznosckatowa_wyniki_7});
        zaleznoscKatowaVector.push_back(ZaleznoscKatowaLabelsPtr{ui->zaleznosckatowa_8katproducent, ui->zaleznosckatowa_8_kat, ui->zaleznosckatowa_wyniki_8});
        ui->zaleznosckatowaframeerror->setVisible(!daneTestu.getOk());
        ui->zaleznosckatowaetnadajnik->setText(pierwszy);
        ui->zaleznosckatowaetodbiornik->setText(drugi);
        short id = 0;
        QString err;
        for (const auto & wynik : daneTestu.getPomiaryKatow()) {
            if (!wynik.errorStr.isEmpty() || !wynik.errorDetail.isEmpty()) {
                ui->zaleznosckatowa_errinfo->setText(wynik.errorStr);
                ui->zaleznosckatowa_errExt->setText(wynik.errorDetail);
            }
            zaleznoscKatowaVector[id].katmierzony->setText(QString("<html><body>%1 &deg'</body></html>").arg(wynik.katZmierzony, 2, 'f', 1));
            zaleznoscKatowaVector[id].katproducenta->setText(QString("<html><body>%1 &deg'</body></html>").arg(wynik.katProducenta, 2, 'f', 1));
            if (!wynik.ok) {
                zaleznoscKatowaVector[id].wynik->setText(wynik.errorStr);
            }
        }
    } else if (daneTestu.getId() == RAPID_CHANGES_IN_ATTENUATION) {
        ui->stackedWidget->setCurrentWidget(ui->szybkiezmianytlumienia);
        short num = 0;
        szybkieZmianyTlumieniaHeadTable(ui->frSzybkieZmianyPrzebieg, ui->szybkiezmianytlumieniagridlayout,"szybkiezmiany");
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            powtarzalnoscAddRekord(ui->frSzybkieZmianyPrzebieg, ui->szybkiezmianytlumieniagridlayout, "szybkiezmiany",
                                  num, dane.value_dB, "0.0", dane.ok, dane.error);
            num++;
        }
        ui->szybkiezmianytlumieniagridlayout->setVerticalSpacing(0);
        ui->szybkiezmianytlumieniagridlayout->setHorizontalSpacing(0);
        ui->szybkiezmianytlumieniagridlayout->setSpacing(0);
    } else if (daneTestu.getId() == OPTICAL_PATH_LENGTH_DEPEDENCE) {
        ui->stackedWidget->setCurrentWidget(ui->dlugoscdrogioptycznej);
        ui->dlugoscdrogioptycznejCmin->setText(QString::number(daneTestu.getCmin(), 'g', 1));
        ui->dlugoscdrogioptycznejCmax->setText(QString::number(daneTestu.getCmax(), 'g', 1));
        ui->dlugoscdrogioptycznejCmaxCmin->setText(QString::number(daneTestu.getCmaxCmin(), 'g', 2));
        short num = 0;
        dlugoscdrogioptycznejHeadTable(ui->frSzybkieZmianyPrzebieg, ui->dlugoscDrogiOptycznejGridLayout,"dlugoscdrogioptycznej");
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            dlugoscdrogioptycznejAddRekord(ui->frSzybkieZmianyPrzebieg, ui->dlugoscDrogiOptycznejGridLayout, "dlugoscdrogioptycznej",
                              num, dane.value_dB, "0.0",
                                           (num == 0 ? daneTestu.getMinimalneRozstawienie() : daneTestu.getMaksymalneRozstawienie()) ,
                                           dane.ok, dane.error);
            num++;
        }
        ui->dlugoscDrogiOptycznejGridLayout->setVerticalSpacing(0);
        ui->dlugoscDrogiOptycznejGridLayout->setHorizontalSpacing(0);
        ui->dlugoscDrogiOptycznejGridLayout->setSpacing(0);
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
    addLine(fr, lay, true, row, col, 1, 1, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col));
    ++col;
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
    addLine(fr, lay, true, row, col, 1, 1, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col));
    ++col;
    if (inneText.isEmpty())
        oneTableTd(ok, fr, lay, inneText, row, col, QString("label_%1_%2_%3").arg(suffix).arg(row).arg(col));
    else
        oneTableFrame(ok, fr, lay, inneText, row, col, QString("frame_%1_%2_%3").arg(suffix).arg(row).arg(col));
    ++col;
    addLine(fr, lay, true, row, col, 1, 1, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col));
    ++col;

    addLine(fr, lay, false, row+1, 0, 1, col, QString("vertline_%1_%2").arg(suffix).arg(row));

}

void OknoPodsumowanieTestu::szybkieZmianyTlumieniaHeadTable(QFrame * fr, QGridLayout * lay,
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


void OknoPodsumowanieTestu::szybkiezmianytlumieniaAddRekord(
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
    addLine(fr, lay, true, row, col, 1, 1, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col));
    ++col;
    if (inneText.isEmpty())
        oneTableTd(ok, fr, lay, inneText, row, col, QString("label_%1_%2_%3").arg(suffix).arg(row).arg(col));
    else
        oneTableFrame(ok, fr, lay, inneText, row, col, QString("frame_%1_%2_%3").arg(suffix).arg(row).arg(col));
    ++col;
    addLine(fr, lay, true, row, col, 1, 1, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col));
    ++col;

    addLine(fr, lay, false, row+1, 0, 1, col, QString("vertline_%1_%2").arg(suffix).arg(row));

}

void OknoPodsumowanieTestu::dlugoscdrogioptycznejHeadTable(QFrame * fr, QGridLayout * lay,
                                      const QString & suffix)
{
    short col = 0;
    ADDLINETABLEHEADTD("Nr Próby", "lhead0", "etProba");
    ADDLINETABLEHEADTD("Rozstawienie", "lhead10", "etRozstawienie");
    ADDLINETABLEHEADTD("<html><body><b>C<sub>[n]</sub></b> <i>[dB]</i></body></html>", "lhead4", "etCndB");
    ADDLINETABLEHEADTD("<html><body><b>C<sub>[n]</sub></b> <i>[%]</i></body></html>", "lhead5", "etCndPer");
    ADDLINETABLEHEADTD("Wynik", "lhead6", "etResult");
    ADDLINETABLEHEADTD("Uwagi", "lhead6", "etUwagi");
    addLine(fr, lay, true, 1, col++, 1, 1, QString("%1 %2").arg(suffix,"lhead8"));

    addLine(fr, lay, false, 0, 0, 1, col, QString("%1 %2").arg(suffix,"lheadUp"));
    addLine(fr, lay, false, 2, 0, 1, col, QString("%1 %2").arg(suffix,"lheadDown"));
}


void OknoPodsumowanieTestu::dlugoscdrogioptycznejAddRekord(
        QFrame * fr, QGridLayout * lay, const QString & suffix,
        short r, const QString &tlumienie_db, const QString &tlumienie_per,
        const QString & rozstawienie, bool ok, const QString &inneText)
{

    short col = 0;
    short row = 2*r+3;

    ADDLINETABLETD(QString::number(r+1));
    ADDLINETABLETD(QString("%1 m").arg(rozstawienie));
    ADDLINETABLETD(tlumienie_db);
    ADDLINETABLETD(tlumienie_per);
    ADDLINETABLETD(ok ? "POZYTYWNY" : "NEGATYWNY");
    addLine(fr, lay, true, row, col, 1, 1, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col));
    ++col;
    if (inneText.isEmpty())
        oneTableTd(ok, fr, lay, inneText, row, col, QString("label_%1_%2_%3").arg(suffix).arg(row).arg(col));
    else
        oneTableFrame(ok, fr, lay, inneText, row, col, QString("frame_%1_%2_%3").arg(suffix).arg(row).arg(col));
    ++col;
    addLine(fr, lay, true, row, col, 1, 1, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col));
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




