#include "oknopotwierdzenieemcnarazenie.h"
#include "danetestu.h"
#include "ui_oknopotwierdzenieemcnarazenie.h"


//Nazwa testu [Ten ekran nie pojawi się dla badań odwarzalności, powatarzalności, katowych , zasilania
#include "danetestu.h"

inline void CenterWidgets(QWidget *widget, QWidget *host = 0) {
    if (!host)
        host = widget->parentWidget();

    if (host) {
        auto hostRect = host->geometry();
        widget->move(hostRect.center() - widget->rect().center());
    }
    else {
        //QRect screenGeometry = host->screen()->availableSize();
        //int x = (screenGeometry.width() - widget->width()) / 2;
        //int y = (screenGeometry.height() - widget->height()) / 2;
        widget->move(0, 0);
    }
}

#include <QMessageBox>

OknoPotwierdzenieEMCNarazenie::OknoPotwierdzenieEMCNarazenie(const DaneTestu & daneTestu,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoPotwierdzenieEMCNarazenie),
    idTest(daneTestu.getId())
{
    ui->setupUi(this);
    ui->osobaWykonujacaTest->setText(daneTestu.getOsobaWykonujaca());
    ui->numerNadajnika->setText(daneTestu.getNumerTransmitter());
    ui->numerReceiver->setText(daneTestu.getNumerReceiver());
    ui->testName->setText(daneTestu.getName());

    ui->opisNarazenia->setText(daneTestu.getOpisNarazenia());

    ui->eTransmitter->setText(daneTestu.getNazwaNumerTransmitter());
    ui->eReceiver->setText(daneTestu.getNazwaNumerReceiver());

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->done(QDialog::Accepted); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    connect(ui->cbAlarmYesNo, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int index) { this->changeComboBox(0,index); });
    connect(ui->cbUszkodzenie, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int index) { this->changeComboBox(1,index); });


    adjustSize();
    CenterWidgets(this, parent);
}

OknoPotwierdzenieEMCNarazenie::~OknoPotwierdzenieEMCNarazenie()
{
    delete ui;
}

void OknoPotwierdzenieEMCNarazenie::changeComboBox(short nrCombo, int index)
{
    if (idTest == DAMP_HEAT_STADY_STATE_ENDURANCE)
        return;

    QString text = ui->komentarz->toPlainText();
    if (nrCombo == 0) {
        if (index == 1) {
            text.append(QString::fromUtf8("[Czujka zgłosiła alarm podczas narażania]"));
        } else {
            text.remove(QString::fromUtf8("[Czujka zgłosiła podczas narażania]"));
        }
    }
    else if (nrCombo == 2) {
        if (index == 1) {
            text.append(QString::fromUtf8("[Czujka została uszkodzona]"));
        } else {
            text.remove(QString::fromUtf8("[Czujka została uszkodzona]"));
        }
    }

    if (ui->cbAlarmYesNo->currentIndex() == 0 && ui->cbUszkodzenie->currentIndex() == 0) {
        ui->wynik_narazenia->setText("POZYTYWNY");
    } else {
        ui->wynik_narazenia->setText("NEGATYWNY");
    }
}

bool OknoPotwierdzenieEMCNarazenie::getWynik() const
{
    return ui->cbAlarmYesNo->currentIndex() == 0 && ui->cbUszkodzenie->currentIndex() == 0;
}

QString OknoPotwierdzenieEMCNarazenie::getKomenatarz() const
{
    return ui->komentarz->toPlainText();
}

static int question(QWidget * parent, const QString & title, const QString & pytanie) {
    QMessageBox messageBox(QMessageBox::Question, title, pytanie, 
                        QMessageBox::Yes | QMessageBox::No, parent);

    messageBox.setButtonText(QMessageBox::Yes, QString::fromUtf8("Tak"));
    messageBox.setButtonText(QMessageBox::No, QString::fromUtf8("Nie"));
    return messageBox.exec();
}

void OknoPotwierdzenieEMCNarazenie::pbCancel_clicked()
{
    int ret = question(this, QString::fromUtf8("Badanie : %1").arg(ui->testName->text()),
                             QString::fromUtf8("Czy napewno chcesz przerwać badanie"));
    if (ret == QMessageBox::Yes)
        done(QDialog::Rejected);
}
