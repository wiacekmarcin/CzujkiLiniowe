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
    feetTable(7);
#endif

    for (const auto & dane : daneTestu.getDaneBadanCzujek())
    {
        //QString inne;
        addOneRekordTable(num++, dane.nrPomiaru, dane.numerPierwszy, dane.numerDrugi, dane.value, dane.value2, dane.ok, dane.error);
    }
    feetTable(2*daneTestu.getDaneBadanCzujek().size()+3);




    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->accept(); });
}

Test9Podsumowanie::~Test9Podsumowanie()
{
    delete ui;
}

#define STR(N) #N
#define FBOX(N) frame_##N->setObjectName(QString("frame_%1_%2").arg(STR(N)).arg(nrPozycji)); \
    frame_##N->setFrameShape(QFrame::StyledPanel); \
    frame_##N->setFrameShadow(QFrame::Raised); \
    QHBoxLayout * horizontalLayout_##N = new QHBoxLayout(frame_##N); \
    horizontalLayout_##N->setSpacing(3); \
    horizontalLayout_##N->setObjectName(QString("horizontalLayout_%1_%2").arg(STR(N)).arg(nrPozycji)); \
    horizontalLayout_##N->setContentsMargins(5, 5, 5, 5); \
    QLabel * label_##N = new QLabel(frame_##N); \
    label_##N->setObjectName(QString("label_tlumienie_")+QString::number(nrPozycji)); \
    horizontalLayout_##N->addWidget(label_##N);\
    label_##N->setText(N##Text);

void Test9Podsumowanie::addOneRekordTable(short nrPozycji, short nrProby, const QString & pierwszyText, const QString & drugiText,
                                          const QString &tlumienieText, const QString &tlumienie2Text,
                                          bool ok, const QString &inneText)
{
    QLabel * nrCzujki = new QLabel(ui->frameTable);
    nrCzujki->setObjectName(QString("nrCzujki")+QString::number(nrPozycji));
    nrCzujki->setText(QString::number(nrProby));
    ui->gridLayoutResults->addWidget(nrCzujki, 2*nrPozycji+3, 1, 1, 1);

    QFrame * frame_pierwszy = new QFrame(ui->frameTable);
    FBOX(pierwszy);
    ui->gridLayoutResults->addWidget(frame_pierwszy, 2*nrPozycji+3, 3, 1, 1);

    QFrame * frame_drugi = new QFrame(ui->frameTable);
    FBOX(drugi);
    ui->gridLayoutResults->addWidget(frame_drugi, 2*nrPozycji+3, 5, 1, 1);

    QFrame * frame_tlumienie = new QFrame(ui->frameTable);
    FBOX(tlumienie);
    ui->gridLayoutResults->addWidget(frame_tlumienie, 2*nrPozycji+3, 7, 1, 1);

    QFrame * frame_tlumienie2 = new QFrame(ui->frameTable);
    FBOX(tlumienie2);
    ui->gridLayoutResults->addWidget(frame_tlumienie2, 2*nrPozycji+3, 9, 1, 1);

    QLabel * result = new QLabel(ui->frameTable);
    result->setObjectName(QString("result_")+QString::number(nrPozycji));
    result->setText(ok ? "POZYTYWNY" : "NEGATYWNY");
    ui->gridLayoutResults->addWidget(result, 2*nrPozycji+3, 11, 1, 1);

    QLabel * inne = new QLabel(ui->frameTable);
    inne->setText(inneText);
    inne->setObjectName(QString("inne_")+QString::number(nrPozycji));

    ui->gridLayoutResults->addWidget(inne, 2*nrPozycji+3, 13, 1, 1);

    ADDHLINE(nrPozycji, 2*nrPozycji+4);

}

void Test9Podsumowanie::headTable(const QString & pierwszy, const QString & drugi)
{
    ADDHLINE(0, 0);
    QLabel * etProba = new QLabel(ui->frameTable);
    etProba->setObjectName("etProba");
    etProba->setText(QString::fromUtf8("Próba"));
    etProba->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    QFont f1(etProba->font());
    f1.setBold(true);
    etProba->setFont(f1);
    ui->gridLayoutResults->addWidget(etProba, 1, 1, 1, 1);

    QLabel * etPierwszy = new QLabel(ui->frameTable);
    etPierwszy->setObjectName("etPierwszy");
    etPierwszy->setText(pierwszy);
    etPierwszy->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    QFont f2(etPierwszy->font());
    f2.setBold(true);
    etPierwszy->setFont(f2);
    ui->gridLayoutResults->addWidget(etPierwszy, 1, 3, 1, 1);

    QLabel * etDrugi = new QLabel(ui->frameTable);
    etDrugi->setObjectName("etDrugi");
    etDrugi->setText(drugi);
    etDrugi->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    QFont f3(etDrugi->font());
    f3.setBold(true);
    etDrugi->setFont(f3);
    ui->gridLayoutResults->addWidget(etDrugi, 1, 5, 1, 1);

    QLabel * cn1 = new QLabel(ui->frameTable);
    cn1->setObjectName("etCndB");
    cn1->setText("<html><body><b>C<sub>[n]</sub></b> <i>[dB]</i></body></html>");
    cn1->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    QFont f4(cn1->font());
    f4.setBold(true);
    cn1->setFont(f4);
    ui->gridLayoutResults->addWidget(cn1, 1, 7, 1, 1);

    QLabel * cn2 = new QLabel(ui->frameTable);
    cn2->setObjectName("etCnpercent");
    cn2->setText("<html><body><b>C<sub>[n]</sub></b> <i>[%]</i></body></html>");
    cn2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    QFont f5(cn2->font());
    f5.setBold(true);
    cn2->setFont(f5);
    ui->gridLayoutResults->addWidget(cn2, 1, 9, 1, 1);

    QLabel * result = new QLabel(ui->frameTable);
    result->setObjectName("etResult");
    result->setText(QString::fromUtf8("Wynik"));
    result->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    QFont f6(result->font());
    f6.setBold(true);
    result->setFont(f6);
    ui->gridLayoutResults->addWidget(result, 1, 11, 1, 1);

    QLabel * uwagi = new QLabel(ui->frameTable);
    uwagi->setObjectName("etUwagi");
    uwagi->setText(QString::fromUtf8("Uwagi"));
    uwagi->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    QFont f7(uwagi->font());
    f7.setBold(true);
    uwagi->setFont(f7);
    ui->gridLayoutResults->addWidget(uwagi, 1, 13, 1, 1);

    ADDHLINE(1, 2);
}

#define ADDVLINE(N,C,S) QFrame * lhead##N = new QFrame(ui->frameTable);\
                      lhead##N->setObjectName(QString("lhead%1").arg(N)); \
                      lhead##N->setFrameShape(QFrame::VLine); \
                      lhead##N->setFrameShadow(QFrame::Sunken);\
                      lhead##N->setLineWidth(3);\
                      ui->gridLayoutResults->addWidget(lhead##N, 0, C, 1, S+4);

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

