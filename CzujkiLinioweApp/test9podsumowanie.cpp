#include "test9podsumowanie.h"
#include "ui_test9podsumowanie.h"
#include <QDateTime>

#include <QMessageBox>
#define DEBUG

#define ADDHLINE(N,R) QFrame * lvert##N = new QFrame(ui->frameTable);\
    lvert##N->setObjectName(QString("lvert%1").arg(N)); \
    lvert##N->setFrameShape(QFrame::HLine); \
    lvert##N->setFrameShadow(QFrame::Sunken);\
    lvert##N->setLineWidth(3);\
    ui->gridLayoutResults->addWidget(lvert##N, R, 0, 14, 1);


Test9Podsumowanie::Test9Podsumowanie(const DaneTestu &daneTestu, const ParametryBadania & badanie, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test9Podsumowanie),
    powtorzPomiar(false)

{
    ui->setupUi(this);
    ui->testName->setText(daneTestu.getName());
QString pierwszy, drugi;
#ifdef DEBUG
    pierwszy = "Nadajnik";
    drugi = "Odbiornik";
#else

    //short nrBadania = 1;

    if (badanie.getSystemOdbiornikNadajnik()) { //odb <-> nad
        pierwszy = "Nadajnik";
        drugi = "Odbiornik";
    } else { //odb+nad <-> ref
        pierwszy = "Nadajnik+Odbiornik";
        drugi = "Reflektor";
    }
#endif
    headTable(pierwszy, drugi);
    short num = 0;
#ifdef DEBUG
    addOneRekordTable(0, 1, "AXA1", "AYA1", "-", "-", false, "Błąd sprzętowy");
    addOneRekordTable(1, 2, "AXA2", "AYA2", "0.2", "34", false, "Crep<0.3");
    addOneRekordTable(2, 3, "AXA3", "AYA3", "0.5", "45", true, "");
    addOneRekordTable(3, 4, "AXA4", "AYA4", "1.5", "45", true, "");
    addOneRekordTable(4, 5, "AXA5", "AYA5", "3.5", "45", true, "");
    addOneRekordTable(5, 6, "AXA6", "AYA6", "2.5", "45", true, "");
    addOneRekordTable(6, 7, "AXA7", "AYA7", "0.6", "45", true, "");
    
#endif

    for (const auto & dane : daneTestu.getDaneBadanCzujek())
    {
        //QString inne;
        addOneRekordTable(num++, dane.nrPomiaru, dane.numerNadajnika, dane.numerDrugi, dane.value, dane.value2, dane.ok, dane.error);
    }





    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
}

Test9Podsumowanie::~Test9Podsumowanie()
{
    delete ui;
}

void Test9Podsumowanie::addOneRekordTable(short row, short nrProby, const QString & nadajnik, const QString & odbiornik,
                                          const QString &tlumienie_db, const QString &tlumienie_per,
                                          bool ok, const QString &inneText)
{

    short col = 0;
    short row = 2*row+3;
    
    addLine(QString("line_%1_%2").arg(row).arg(col), false, row, col++, 1, 1);
    oneTableTd(QString("label_%1_%2").arg(row).arg(col), QString::number(nrPozycji), row, col++);
    addLine(QString("line_%1_%2").arg(row).arg(col), false, row, col++, 1, 1);
    oneTableFrame(QString("frame_%1_%2").arg(row, col), nadajnik, row, col++);
    addLine(QString("line_%1_%2").arg(row).arg(col), false, row, col++, 1, 1);
    oneTableFrame(QString("frame_%1_%2").arg(row, col), odbiornik, row, col++);
    addLine(QString("line_%1_%2").arg(row).arg(col), false, row, col++, 1, 1);
    oneTableFrame(QString("frame_%1_%2").arg(row, col), tlumienie_db, row, col++);
    addLine(QString("line_%1_%2").arg(row).arg(col), false, row, col++, 1, 1);
    oneTableFrame(QString("frame_%1_%2").arg(row, col), tlumienie_per, row, col++);
    addLine(QString("line_%1_%2").arg(row).arg(col), false, row, col++, 1, 1);
    oneTableTd(QString("label_%1_%2").arg(row).arg(col), ok ? "POZYTYWNY" : "NEGATYWNY", row, col++);
    addLine(QString("line_%1_%2").arg(row).arg(col), false, row, col++, 1, 1);
    oneTableTd(QString("label_%1_%2").arg(row).arg(col), inneText, row, col++);
    addLine(QString("line_%1_%2").arg(row).arg(col), false, row, col++, 1, 1);
    
    addLine(QString("vertline_%1").arg(row).arg(col), false, row+1, 0, 1, col);

}

void Test9Podsumowanie::addRow(const ) {
    
}

void Test9Podsumowanie::headTable(const QString & nadajnik, const QString & odbiornik)
{
    short col = 0;
    
    addLine("lhead0", false, 1, col++, 1, 1);
    oneHeadRecord("etProba", QString::fromUtf8("Próba"), 1, col++);    
    addLine("lhead1", false, 1, col++, 1, 1);
    oneHeadRecord("etNadajnik", nadajnik, 1, col++);
    addLine("lhead2", false, 1, col++, 1, 1);
    oneHeadRecord("etOdbiornik", odbiornik, 1, col++);
    addLine("lhead3", false, 1, col++, 1, 1);
    oneHeadRecord("etCndB", "<html><body><b>C<sub>[n]</sub></b> <i>[dB]</i></body></html>", 1, col++);
    addLine("lhead4", false, 1, col++, 1, 1);
    oneHeadRecord("etCnPer", "<html><body><b>C<sub>[n]</sub></b> <i>[%]</i></body></html>", 1, col++);
    addLine("lhead5", false, 1, col++, 1, 1);
    oneHeadRecord("etResult", "Wynik", 1, col++);
    addLine("lhead6", false, 1, col++, 1, 1);
    oneHeadRecord("etUwagi", "Uwagi", 1, col++);
    addLine("lhead7", false, 1, col++, 1, 1);

    addLine("lheadUp",   true, 0, 0, 1, col);
    addLine("lheadDown", true, 2, 0, 1, col);
}


void Test9Podsumowanie::feetTable(short nrPomiarow)
{
    ADDVLINE(1, 0, nrPomiarow);
    ADDVLINE(2, 2, nrPomiarow);
    ADDVLINE(3, 6, nrPomiarow);
    ADDVLINE(4, 8, nrPomiarow);
    ADDVLINE(5, 10, nrPomiarow);
    ADDVLINE(6, 12, nrPomiarow);
    ADDVLINE(7, 14, nrPomiarow);
}

void Test9Podsumowanie::oneHeadRecord(const QString & objectName, const QString & text, int row, int col)
{
    QLabel * lh = new QLabel(ui->frameTable);
    lh->setObjectName(objectName);
    lh->setText(QString::fromUtf8(text));
    lh->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    QFont f(lh->font());
    f.setBold(true);
    lh->setFont(f);
    ui->gridLayoutResults->addWidget(lh, row, col, 1, 1);
}

void Test9Podsumowanie::oneTableTd(const QString & objectName, const QString & text, int row, int col)
{
    QLabel * l = new QLabel(ui->frameTable);
    l->setObjectName(objectName));
    l->setText(text);
    ui->gridLayoutResults->addWidget(l, row, col, 1, 1);
}

void Test9Podsumowanie::oneTableFrame(const QString & objectName, const QString & text, int row, int col)
{
    QFrame * frame = new QFrame(ui->frameTable);
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
    ui->gridLayoutResults->addWidget(frame, row, col, 1, 1);
}

void Test9Podsumowanie::addLine(const QString & objectName, bool vert, int row, int col, int rowspan, int colspan)
{
    QFrame * line = new QFrame(ui->frameTable);
    line->setObjectName(objectName); 
    line->setFrameShape(vert ? QFrame::VLine : QFrame::HLine); 
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(3);
    ui->gridLayoutResults->addWidget(line, row, col, rowspan, colspan);
}