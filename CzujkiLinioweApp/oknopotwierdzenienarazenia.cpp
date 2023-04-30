#include "oknopotwierdzenienarazenia.h"
#include "danetestu.h"
#include "ui_oknopotwierdzenienarazenia.h"


//Nazwa testu [Ten ekran nie pojawi się dla badań odwarzalności, powatarzalności, katowych , zasilania
#include "danetestu.h"

#include <QMessageBox>

static int questionQuit(const QString & title, const QString & pytanie, QWidget * parent) {
    QMessageBox messageBox(QMessageBox::Question, title, pytanie,
                        QMessageBox::Close | QMessageBox::Cancel, parent);


    messageBox.setButtonText(QMessageBox::Close, QString::fromUtf8("Zamknij"));
    messageBox.setButtonText(QMessageBox::Cancel, QString::fromUtf8("Anuluj"));

    return messageBox.exec();
}

inline void CenterWidgets(QWidget *widget, QWidget *host = 0) {
    if (!host)
        host = widget->parentWidget();

    if (host) {
        auto hostRect = host->geometry();
        widget->move(hostRect.center() - widget->rect().center());
    }
    else {
        //QRect screenGeometry = host()screen()->availableSize();
        //int x = (screenGeometry.width() - widget->width()) / 2;
        //int y = (screenGeometry.height() - widget->height()) / 2;
        //widget->move(x, y);
    }
}

#include <QMessageBox>

OknoPotwierdzenieNarazenia::OknoPotwierdzenieNarazenia(const DaneTestu & daneTestu,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoPotwierdzenieNarazenia),
    idTest(daneTestu.getId())
{
    ui->setupUi(this);
    ui->osobaWykonujacaTest->setText(daneTestu.getOsobaWykonujaca());
    ui->numerNadajnika->setText(daneTestu.getNumerTransmitter());
    ui->numerReceiver->setText(daneTestu.getNumerReceiver());
    ui->testName->setText(daneTestu.getName());

    ui->eTransmitter->setText(daneTestu.getNazwaNumerTransmitter());
    ui->eReceiver->setText(daneTestu.getNazwaNumerReceiver());

    ui->cbAlarmYesNo->setCurrentIndex(0);
    ui->cbTlumnik->setCurrentIndex(0);
    ui->cbUszkodzenie->setCurrentIndex(0);
    ui->cbCzujkaOk->setCurrentIndex(0);
    ui->komentarz->setPlainText("");

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->done(QDialog::Accepted); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    connect(ui->cbAlarmYesNo, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int index) { this->changeComboBox(0,index); });
    connect(ui->cbTlumnik, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int index) { this->changeComboBox(1,index); });
    connect(ui->cbUszkodzenie, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int index) { this->changeComboBox(2,index); });
    connect(ui->cbCzujkaOk, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int index) { this->changeComboBox(3,index); });

    if (daneTestu.getId() == FIRE_SENSITIVITY) {
        removeButtons(true, true, true, false);
        ui->narazenie->setText(QString::fromUtf8("<html><head/><body style=\"font-size:14pt;\"><p>"
                                       "Ocenę reakcji czujki na powolny wzrost gęstości dymu należy "
                                       "przeprowadzić poprzez analizę układu/oprogramowania, próbę "
                                       "fizyczną lub symulacje w celu sprawdzenia, czy czujka "
                                       "spełnia wymagania określone w pkt 4.3.5.</p></body></html>"));
    } else if (daneTestu.getId() == DRY_HEAT) {
        removeButtons(false, false, false, false);
        ui->narazenie->setText(QString::fromUtf8("<html><head/><body style=\"font-size:14pt;\"><p>"
                                       "Czujka powinna zostać podłączona a następnie ustawiona na maksymalną czułość i "
                                       "zostawiona na <b>16 godzin w temperaturze w 55 ± 2 °C</b> .</p>"
                                       "<p>Pod koniec czasu narażenia należy umieśić tłumnik 6 dB w torze optycznym.</p>"
                                       "<p>Czujka powinna zostać wyłączona i przeniesiona do pomieszczenia z warunkami "
                                       "pokojowymi na czas 1 godziny.</p></body></html>"));
    } else if (daneTestu.getId() == COLD) {
        removeButtons(false, false, false, false);
        ui->narazenie->setText(QString::fromUtf8("<html><head/><body style=\"font-size:14pt;\"><p>"
                                       "Czujka powinna zostać podłączona a następnie ustawiona na maksymalną czułość i "
                                       "zostawiona na <b>16 godzin w temperaturze w -10 ± 3 °C</b> .</p>"
                                       "<p>Należy pilnować aby na czujce nie pojawił się lód lub szron</p>"
                                       "<p>Pod koniec czasu narażenia należy umieśić tłumnik 6 dB w torze optycznym.</p>"
                                       "<p>Czujka powinna zostać wyłączona i przeniesiona do pomieszczenia z warunkami "
                                       "pokojowymi na czas 1 godziny.</p></body></html>"));
    } else if (daneTestu.getId() == DAMP_HEAT_STADY_STATE_OPERATIONAL) {
        removeButtons(true, true, false, false);
        ui->narazenie->setText(QString::fromUtf8("<html><head/><body style=\"font-size:14pt;\"><p>"
                                   "Czujka powinna zostać podłączona a następnie ustawiona na maksymalną czułość i "
                                   "zostawiona na <b>4 doby w temperaturze w 40 ± 2 °C i wilgotności względnej 93 ± 3 %</b> .</p>"
                                   "<p>Czujka powinna zostać wyłączona i przeniesiona do pomieszczenia z warunkami "
                                   "pokojowymi na czas 1 godziny.</p></body></html>"));
    } else if (daneTestu.getId() == DAMP_HEAT_STADY_STATE_ENDURANCE) {
        removeButtons(true, true, false, false);
        ui->narazenie->setText(QString::fromUtf8("<html><head/><body style=\"font-size:14pt;\"><p>"
                               "Czujki nie powinno się podłączać , tylko powinna zostać umieszoczna w pomieszczeniu "
                               " <b>na 21 dni w temperaturze w 40 ± 2 °C i wilgotności względnej 93 ± 3 %</b> .</p>"
                               "<p>Czujka powinna zostać przeniesiona do pomieszczenia z warunkami "
                               "pokojowymi na czas 1 godziny w celu regeneracji.</p></body></html>"));
    } else if (daneTestu.getId() == VIBRATION) {
        removeButtons(true, true, false, false);
        ui->narazenie->setText(QString::fromUtf8("<html><head/><body style=\"font-size:14pt;\"><p>"
                               "Czujki nie powinno się podłączać , ale powinna zostać zamontowana zgodnie z normą do sztywnego uchwytu.</p>"
                               "<p>Drgania należy przykładać kolejno w każdej z trzech wzajemnie prostopadłych osi. Element należy "
                               "zamontować tak, aby jedna z trzech osi była prostopadła do jego normalnej osi montażowej.</p>"
                               "<p>Stosuje się następujące warunki : zakres częstotliwości <b>od 10 Hz do 150 Hz</b>, "
                               "amplituda przyspieszenia <b>9,81 ms<sup>-2</sup> (1,0 gn)</b>, liczba osi <b>3</b>,"
                               " szybkość przemiatania <b>1 oktawa/min</b>, liczba cykli przemiatania <b>20</b></p>"
                               "<p>Należy zadbać o to, aby wyrównanie urządzeń wibracyjnych nie uległo znaczącym zmianom "
                               "po zakończeniu testu</p><p>Po zakończeniu testu nie należy regulować ustawienia kątowego</p></body></html>"));
    } else if (daneTestu.getId() == IMPACT) {
        removeButtons(true, true, false, false);
        ui->narazenie->setText(QString::fromUtf8("<html><head/><body style=\"font-size:14pt;\"><p>"
                                       "Czujka powinna zostać podłączona a następnie ustawiona na maksymalną czułość.</p>"
                               "<p>Stosuje się następujące warunki: energia uderzenia: <b>(0,5 ± 0,04) J</b>,"
                               "liczba uderzeń na punkt : <b>3</b></p>"
                               "<p>Dla każdego elementu czujki po kolei należy uderzać w każdy punkt elementu, "
                               "który uznano za podatny na uszkodzenia mechaniczne mogące zakłócić prawidłowe "
                               "działanie czujki, maksymalnie do 20 punktów na każdy element (np. soczewki, "
                               "okna i urządzenia służące do regulacji ustawienia mogą zostać uznane za "
                               "podatne na uszkodzenia). Żadne dwa punkty, w które przykładane są uderzenia, "
                               "nie mogą być oddalone od siebie o mniej niż 20 mm</p>"
                               "<p>Należy uważać, aby wyniki jednej serii trzech ciosów nie miały wpływu na kolejne serie. "
                               "W przypadku wątpliwości co do wpływu poprzednich uderzeń, wadę należy pominąć i "
                               "wykonać kolejne trzy uderzenia w to samo miejsce na nowej próbce</p></body></html>"));
     } else if (daneTestu.getId() == SULPHUR_DIOXIDE_SO2_CORROSION) {
        removeButtons(true, true, false, false);
        ui->narazenie->setText(QString::fromUtf8("<html><head/><body style=\"font-size:14pt;\"><p>"
                                       "Czujkę należy zamontować zgodnie z normą, jednak nie należy podłączać zasilania, "
                                       ", ale czujka musi mieć niecynowane przewody miedziane o odpowiedniej średnicy, "
                                       "podłączone do wystarczającej liczby zacisków, aby umożliwić dokonanie "
                                       "ostatecznych pomiarów, bez wykonywania dalszych połączeń z komponentem (elementami)</p>"
                                       "<p>Czujka powinna zostać podanna następującym warunkom <b>21 dni w temperaturze "
                                       "w 25 ± 2 °C i wilgotności względnej 93 ± 3 %, koncetracja SO<sub>2</sub> ułamek "
                                       "objętościowy 25 ± 5 * 10<sup>-6</sup></b> .</p>"
                                       "<p>Natychmiast po kondycjonowaniu próbkę poddaje się suszeniu przez 16 godzin w "
                                       " temperaturze <b>40+-2 °C</b> i wilgotności względnej nie większej niż <b>50%</b>,"
                                       " po czym następuje okres regeneracji trwający od 1 do 2 godzin w standardowych "
                                       "warunkach laboratoryjnych.</p></body></html>"));
    }
    resize(sizeHint());
    CenterWidgets(this, parent);
}

OknoPotwierdzenieNarazenia::~OknoPotwierdzenieNarazenia()
{
    delete ui;
}

void OknoPotwierdzenieNarazenia::changeComboBox(short /*nrCombo*/, int /*index*/)
{
    if (ui->cbAlarmYesNo->currentIndex() == 0 && ui->cbTlumnik->currentIndex() == 0
            && ui->cbUszkodzenie->currentIndex() == 0 && ui->cbCzujkaOk->currentIndex() == 0)
    {
        ui->wynik_narazenia->setText("POZYTYWNY");
    } else {
        ui->wynik_narazenia->setText("NEGATYWNY");
    }


}

bool OknoPotwierdzenieNarazenia::getWynik() const
{
    return ui->cbAlarmYesNo->currentIndex() == 0 && ui->cbTlumnik->currentIndex() == 0 &&
           ui->cbUszkodzenie->currentIndex() == 0 && ui->cbCzujkaOk->currentIndex() == 0;
}

QString OknoPotwierdzenieNarazenia::getKomenatarz() const
{
    return ui->komentarz->toPlainText();
}

bool OknoPotwierdzenieNarazenia::czujkaUszkodzona() const
{
    return ui->cbUszkodzenie->currentIndex() == 1;
}

bool OknoPotwierdzenieNarazenia::czujkaOk() const
{
    return ui->cbCzujkaOk->currentIndex() == 1;
}

void OknoPotwierdzenieNarazenia::closeEvent(QCloseEvent *event)
{
    auto btn = questionQuit(QString::fromUtf8("Czujki Liniowe"),
                            QString::fromUtf8("Czy chcesz wyjść z badania bez zapisywania danych?"),
                            this);

    if (btn == QMessageBox::Cancel) {
        event->ignore();
        return;
    }

    if (btn == QMessageBox::Close) {
        event->accept();
        return;
    }
}

static int question(QWidget * parent, const QString & title, const QString & pytanie) {
    QMessageBox messageBox(QMessageBox::Question, title, pytanie, 
                        QMessageBox::Yes | QMessageBox::No, parent);

    messageBox.setButtonText(QMessageBox::Yes, QString::fromUtf8("Tak"));
    messageBox.setButtonText(QMessageBox::No, QString::fromUtf8("Nie"));
    return messageBox.exec();
}


void OknoPotwierdzenieNarazenia::pbCancel_clicked()
{
    int ret = question(this, QString::fromUtf8("Badanie : %1").arg(ui->testName->text()),
                             QString::fromUtf8("Czy napewno chcesz przerwać badanie?"));
    if (ret == QMessageBox::Yes)
        done(QDialog::Rejected);
    qDebug() << size();
}

void OknoPotwierdzenieNarazenia::removeButtons(bool tlumnik, bool alarm, bool uszkodz, bool czujkaOk)
{
    if (tlumnik) {
        ui->cbTlumnik->hide();
        ui->etTlumnik->hide();
        ui->formLayout->removeWidget(ui->cbTlumnik);
        ui->formLayout->removeWidget(ui->etTlumnik);
    }

    if (alarm) {
        ui->cbAlarmYesNo->hide();
        ui->etAlarm->hide();
        ui->formLayout->removeWidget(ui->cbAlarmYesNo);
        ui->formLayout->removeWidget(ui->etAlarm);
    }

    if (uszkodz) {
        ui->cbUszkodzenie->hide();
        ui->etUszkodzenie->hide();
        ui->formLayout->removeWidget(ui->cbUszkodzenie);
        ui->formLayout->removeWidget(ui->etUszkodzenie);
    }

    if (czujkaOk) {
        ui->etCzujkaOk->hide();
        ui->cbCzujkaOk->hide();
        ui->formLayout->removeWidget(ui->etCzujkaOk);
        ui->formLayout->removeWidget(ui->cbCzujkaOk);
    }
}
