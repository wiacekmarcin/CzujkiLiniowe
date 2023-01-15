#include "oknopodsumowanietestu.h"
#include "ui_oknopodsumowanietestu.h"
#include <QDateTime>


OknoPodsumowanieTestu::OknoPodsumowanieTestu(DaneTestu &daneTestu, const ParametryBadania & badanie, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoPodsumowanieTestu),
    powtorzPomiar(false)

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
        headTable(ui->odtwarzalnoscframeTable, ui->odtwarzalnoscGridLayoutResults, "odtwarzalnosc", pierwszy, drugi);

        ui->odtwarzalnoscCrep->setText(QString::number(daneTestu.getCrep(), 'f', 2) + " dB");
        ui->odtwarzalnoscCrep2->setText("0 %");
        ui->odtwarzalnoscCmin->setText(QString::number(daneTestu.getCmin(), 'f', 2) + " dB");
        ui->odtwarzalnoscCmin2->setText("0 %");
        ui->odtwarzalnoscCmax->setText(QString::number(daneTestu.getCmax(), 'f', 2) + " dB");
        ui->odtwarzalnoscCmax2->setText("0 %");
        ui->odtwarzalnoscCmaxCrep->setText(QString::number(daneTestu.getCmaxCrep(), 'f', 2));
        ui->odtwarzalnoscCrepCmin->setText(QString::number(daneTestu.getCrepCmin(), 'f', 2));
        if (daneTestu.getCmaxCrep() > 1.33)
            ui->odtwarzalnoscCmaxCrep->setStyleSheet("background-color:red");
        if (daneTestu.getCrepCmin() > 1.5)
            ui->odtwarzalnoscCrepCmin->setStyleSheet("background-color:red");

        short num = 0;
        for (const auto & dane : daneTestu.getDaneBadanCzujek())
        {
            addRekordPodsumowanie(ui->odtwarzalnoscframeTable, ui->odtwarzalnoscGridLayoutResults, "odtwarzalnosc",
                                  num, dane.nrCzujki, dane.numerNadajnika, dane.numerOdbiornika,
                                    dane.value_dB, "0.0", dane.ok, dane.error);
            num++;
        }
        ui->odtwarzalnoscGridLayoutResults->setVerticalSpacing(0);
        ui->odtwarzalnoscGridLayoutResults->setHorizontalSpacing(0);
        ui->odtwarzalnoscGridLayoutResults->setSpacing(0);
        //badanie->s
    } else if (daneTestu.getId() == REPEATABILITY) {

    }


    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
}

OknoPodsumowanieTestu::~OknoPodsumowanieTestu()
{
    delete ui;
}

void OknoPodsumowanieTestu::addRekordPodsumowanie(
        QFrame * fr, QGridLayout * lay, const QString & suffix,
        short r, short nrCzujki, const QString & nadajnik, const QString & odbiornik,
                                          const QString &tlumienie_db, const QString &tlumienie_per,
                                          bool ok, const QString &inneText)
{

    short col = 0;
    short row = 2*r+3;

    addLine(fr, lay, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col), true, row, col, 1, 1);++col;
    oneTableTd(ok, fr, lay, QString("label_%1_%2").arg(suffix).arg(row).arg(col), QString::number(nrCzujki), row, col);++col;
    addLine(fr, lay, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col), true, row, col, 1, 1);++col;
    oneTableFrame(ok, fr, lay, QString("frame_%1_%2_%3").arg(suffix).arg(row, col), nadajnik, row, col);++col;
    addLine(fr, lay, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col), true, row, col, 1, 1);++col;
    oneTableFrame(ok, fr, lay, QString("frame_%1_%2_%3").arg(suffix).arg(row, col), odbiornik, row, col);++col;
    addLine(fr, lay, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col), true, row, col, 1, 1);++col;
    oneTableFrame(ok, fr, lay, QString("frame_%1_%2_%3").arg(suffix).arg(row, col), tlumienie_db, row, col);++col;
    addLine(fr, lay, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col), true, row, col, 1, 1);++col;
    oneTableFrame(ok, fr, lay, QString("frame_%1_%2_%3").arg(suffix).arg(row, col), tlumienie_per, row, col);++col;
    addLine(fr, lay, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col), true, row, col, 1, 1);++col;
    oneTableFrame(ok, fr, lay, QString("label_%1_%2_%3").arg(suffix).arg(row).arg(col), ok ? "POZYTYWNY" : "NEGATYWNY", row, col);++col;
    addLine(fr, lay, QString("line_%1_%2_%3").arg(suffix).arg(row).arg(col), true, row, col, 1, 1);++col;
    if (inneText.isEmpty())
        oneTableTd(ok, fr, lay, QString("label_%1_%2_%3").arg(suffix).arg(row).arg(col), inneText, row, col);
    else
        oneTableFrame(ok, fr, lay, QString("frame_%1_%2_%3").arg(suffix).arg(row).arg(col), inneText, row, col);
    ++col;
    addLine(fr, lay, QString("line_%1_%2_%3").arg(suffix).arg(suffix).arg(row).arg(col), true, row, col, 1, 1);++col;

    addLine(fr, lay, QString("vertline_%1_%2_%3").arg(suffix).arg(row), false, row+1, 0, 1, col);

}

void OknoPodsumowanieTestu::headTable(QFrame * fr, QGridLayout * lay,
                                      const QString & suffix,
                                      const QString & nadajnik, const QString & odbiornik)
{
    short col = 0;
    addLine(fr, lay, QString("%1 %2").arg(suffix).arg("lhead0"), true, 1, col++, 1, 1);
    oneHeadRecord(fr, lay, QString("%1 %2").arg(suffix).arg("etProba"), QString::fromUtf8("Próba"), 1, col++);
    addLine(fr, lay, QString("%1 %2").arg(suffix).arg("lhead1"), true, 1, col++, 1, 1);
    oneHeadRecord(fr, lay, QString("%1 %2").arg(suffix).arg("etNadajnik"), nadajnik, 1, col++);
    addLine(fr, lay, QString("%1 %2").arg(suffix).arg("lhead2"), true, 1, col++, 1, 1);
    oneHeadRecord(fr, lay, QString("%1 %2").arg(suffix).arg("etOdbiornik"), odbiornik, 1, col++);
    addLine(fr, lay, QString("%1 %2").arg(suffix).arg("lhead3"), true, 1, col++, 1, 1);
    oneHeadRecord(fr, lay, QString("%1 %2").arg(suffix).arg("etCndB"), "<html><body><b>C<sub>[n]</sub></b> <i>[dB]</i></body></html>", 1, col++);
    addLine(fr, lay, QString("%1 %2").arg(suffix).arg("lhead4"), true, 1, col++, 1, 1);
    oneHeadRecord(fr, lay, QString("%1 %2").arg(suffix).arg("etCnPer"), "<html><body><b>C<sub>[n]</sub></b> <i>[%]</i></body></html>", 1, col++);
    addLine(fr, lay, QString("%1 %2").arg(suffix).arg("lhead5"), true, 1, col++, 1, 1);
    oneHeadRecord(fr, lay, QString("%1 %2").arg(suffix).arg("etResult"), "Wynik", 1, col++);
    addLine(fr, lay, QString("%1 %2").arg(suffix).arg("lhead6"), true, 1, col++, 1, 1);
    oneHeadRecord(fr, lay, QString("%1 %2").arg(suffix).arg("etUwagi"), "Uwagi", 1, col++);
    addLine(fr, lay, QString("%1 %2").arg(suffix).arg("lhead7"), true, 1, col++, 1, 1);

    addLine(fr, lay, QString("%1 %2").arg(suffix).arg("lheadUp"),   false, 0, 0, 1, col);
    addLine(fr, lay, QString("%1 %2").arg(suffix).arg("lheadDown"), false, 2, 0, 1, col);
}

void OknoPodsumowanieTestu::oneHeadRecord(QFrame * frameTable, QGridLayout * layout,
                                          const QString & objectName, const QString & text, int row, int col)
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

void OknoPodsumowanieTestu::oneTableTd(bool err, QFrame * frameTable, QGridLayout * layout,
                                       const QString & objectName, const QString & text, int row, int col)
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
                                          const QString & objectName, const QString & text, int row, int col)
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
                                                  const QString & objectName, bool vert, int row, int col,
                                                  int rowspan, int colspan)
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
        if (C > Cmax)
            Cmax = C;
        if (C < Cmin)
            Cmin = C;

        if (dane.ok) {
            Cavg += C;
            ++cntAvg;
        }

        if (C < 0.4) {
            badanieOk = false;
            daneTestu.setErrStr(QString::fromUtf8("Cn < 0.4"));
            dane.ok = false;
            dane.error = QString::fromUtf8("Cn < 0.4");
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

    if (daneTestu.getCmaxCrep() > 1.33) {
        daneTestu.setOk(false);
        daneTestu.setErrStr("Cmax/Crep>1.33");
    } else if (daneTestu.getCrepCmin() > 1.5) {
        daneTestu.setOk(false);
        daneTestu.setErrStr("Crep/Cmin<1.5");
    } else {
        daneTestu.setOk(badanieOk);
    }
}

