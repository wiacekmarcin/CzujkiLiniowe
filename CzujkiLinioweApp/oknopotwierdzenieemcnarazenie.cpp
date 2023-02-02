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
        //QRect screenGeometry = host()screen()->availableSize();
        //int x = (screenGeometry.width() - widget->width()) / 2;
        //int y = (screenGeometry.height() - widget->height()) / 2;
        //widget->move(x, y);
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

    ui->eTransmitter->setText(daneTestu.getNazwaNumerTransmitter());
    ui->eReceiver->setText(daneTestu.getNazwaNumerReceiver());

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->done(QDialog::Accepted); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    connect(ui->cbAlarmYesNo, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int index) { this->changeComboBox(0,index); });
    connect(ui->cbTlumnik, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int index) { this->changeComboBox(1,index); });
    connect(ui->cbUszkodzenie, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int index) { this->changeComboBox(3,index); });

    ui->cbTlumnik->setVisible(false);
    ui->etTlumnik->setVisible(false);
    ui->cbAlarmYesNo->setVisible(false);
    ui->etAlarm->setVisible(false);
    ui->cbUszkodzenie->setVisible(false);
    ui->etUszkodzenie->setVisible(false);
    if (daneTestu.getId() == DRY_HEAT) {
        ui->narazenie->setText(QString::fromUtf8("<html><head/><body style=\"font-size:14pt;\"><p>"
                                       "Czujka powinna zostać podłączona a następnie ustawiona na maksymalną czułość i "
                                       "zostawiona na <b>16 godzin w temperaturze w 55 ± 2 °C</b> .</p>"
                                       "<p>Pod koniec czasu narażenia należy umieśić tłumnik 6 dB w torze optycznym.</p>"
                                       "<p>Czujka powinna zostać wyłączona i przeniesiona do pomieszczenia z warunkami "
                                       "pokojowymi na czas 1 godziny.</p></body></html>"));
        ui->cbTlumnik->setVisible(true);
        ui->etTlumnik->setVisible(true);
        ui->cbAlarmYesNo->setVisible(true);
        ui->etAlarm->setVisible(true);
    } else if (daneTestu.getId() == COLD) {
        ui->narazenie->setText(QString::fromUtf8("<html><head/><body style=\"font-size:14pt;\"><p>"
                                       "Czujka powinna zostać podłączona a następnie ustawiona na maksymalną czułość i "
                                       "zostawiona na <b>16 godzin w temperaturze w -10 ± 3 °C</b> .</p>"
                                       "<p>Należy pilnować aby na czujce nie pojawił się lód lub szron</p>"
                                       "<p>Pod koniec czasu narażenia należy umieśić tłumnik 6 dB w torze optycznym.</p>"
                                       "<p>Czujka powinna zostać wyłączona i przeniesiona do pomieszczenia z warunkami "
                                       "pokojowymi na czas 1 godziny.</p></body></html>"));
        ui->cbTlumnik->setVisible(true);
        ui->etTlumnik->setVisible(true);
        ui->cbAlarmYesNo->setVisible(true);
        ui->etAlarm->setVisible(true);
    } else if (daneTestu.getId() == DAMP_HEAT_STADY_STATE_OPERATIONAL) {
        ui->narazenie->setText(QString::fromUtf8("<html><head/><body style=\"font-size:14pt;\"><p>"
                                   "Czujka powinna zostać podłączona a następnie ustawiona na maksymalną czułość i "
                                   "zostawiona na <b>4 doby w temperaturze w 40 ± 2 °C i wilgotności względnej 93 ± 3 %</b> .</p>"
                                   "<p>Czujka powinna zostać wyłączona i przeniesiona do pomieszczenia z warunkami "
                                   "pokojowymi na czas 1 godziny.</p></body></html>"));
        ui->cbAlarmYesNo->setVisible(true);
        ui->etAlarm->setVisible(true);
    } else if (daneTestu.getId() == DAMP_HEAT_STADY_STATE_ENDURANCE) {
        ui->narazenie->setText(QString::fromUtf8("<html><head/><body style=\"font-size:14pt;\"><p>"
                               "Czujki nie powinno się podłączać , tylko powinna zostać umieszoczna w pomieszczeniu "
                               " <b>21 dni w temperaturze w 40 ± 2 °C i wilgotności względnej 93 ± 3 %</b> .</p>"
                               "<p>Czujka powinna zostać przeniesiona do pomieszczenia z warunkami "
                               "pokojowymi na czas 1 godziny w celu regeneracji.</p></body></html>"));
        ui->wynik_narazenia->setVisible(false);
        ui->etWynik->setVisible(false);
    } else if (daneTestu.getId() == VIBRATION) {
        ui->narazenie->setText(QString::fromUtf8("<html><head/><body style=\"font-size:14pt;\"><p>"
                               "Czujki nie powinno się podłączać , ale powinna zostać zamontowana zgodnie z normą do sztywnego uchwytu.</p>"
                               "<p>Drgania należy przykładać kolejno w każdej z trzech wzajemnie prostopadłych osi. Element należy "
                               "zamontować tak, aby jedna z trzech osi była prostopadła do jego normalnej osi montażowej.</p>"
                               "<p>Stosuje się następujące warunki : zakres częstotliwości <b>10 Hz do 150 Hz</b>, "
                               "amplituda przyspieszenia <b>9,81 ms<sup>-2</sup> (1,0 gn)</b>, liczba osi <b>3</b>,"
                               " szybkość przemiatania <b>1 oktawa/min</b>, liczba cykli przemiatania <b>20</b></p>"
                               "<p>Należy zadbać o to, aby wyrównanie urządzeń wibracyjnych nie uległo znaczącym zmianom "
                               "po zakończeniu testu</p><p>Po zakończeniu testu nie należy regulować ustawienia kątowego</p></body></html>"));
        ui->cbUszkodzenie->setVisible(true);
        ui->etUszkodzenie->setVisible(true);
    } else if (daneTestu.getId() == IMPACT) {
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
        ui->cbUszkodzenie->setVisible(false);
        ui->etUszkodzenie->setVisible(false);
        ui->cbAlarmYesNo->setVisible(true);
        ui->etAlarm->setVisible(true);
    } else if (daneTestu.getId() == SULPHUR_DIOXIDE_SO2_CORROSION) {
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
        ui->wynik_narazenia->setVisible(false);
        ui->etWynik->setVisible(false);
    }
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
        if (idTest == DRY_HEAT || idTest == COLD || idTest == DAMP_HEAT_STADY_STATE_OPERATIONAL || idTest == IMPACT) {
            if (index == 1) {
                text.append(QString::fromUtf8("[Czujka zgłosiła błąd poczas bezczynności]"));
            } else {
                text.remove(QString::fromUtf8("[Czujka zgłosiła błąd poczas bezczynności]"));
            }
        }
    } else if (nrCombo == 1) {
        if (idTest == DRY_HEAT || idTest == COLD) {
            if (index == 1) {
                text.append(QString::fromUtf8("[Czujka nie wykryła tłumnika 6dB w czasie 30s]"));
            } else {
                text.remove(QString::fromUtf8("[Czujka nie wykryła tłumnika 6dB w czasie 30s]"));
            }
        }
    } else if (nrCombo == 2 || idTest == IMPACT) {
        if (idTest == VIBRATION) {
            if (index == 1) {
                text.append(QString::fromUtf8("[Czujka została uszkodzona fizycznie]"));
            } else {
                text.remove(QString::fromUtf8("[Czujka została uszkodzona fizycznie]"));
            }
        }
    }


    ui->komentarz->setPlainText(text);
    if (idTest == DRY_HEAT || idTest == COLD) {
        if (ui->cbAlarmYesNo->currentIndex() == 0 && ui->cbTlumnik->currentIndex() == 0) {
            ui->wynik_narazenia->setText("POZYTYWNY");
        } else {
            ui->wynik_narazenia->setText("NEGATYWNY");
        }
    } else if (idTest == DAMP_HEAT_STADY_STATE_OPERATIONAL) {
        if (ui->cbAlarmYesNo->currentIndex() == 0) {
            ui->wynik_narazenia->setText("POZYTYWNY");
        } else {
            ui->wynik_narazenia->setText("NEGATYWNY");
        }
    } else if (idTest == VIBRATION) {
        if (ui->cbUszkodzenie->currentIndex() == 0) {
            ui->wynik_narazenia->setText("POZYTYWNY");
        } else {
            ui->wynik_narazenia->setText("NEGATYWNY");
        }
    } else if (idTest == IMPACT) {
        if (ui->cbAlarmYesNo->currentIndex() == 0 && ui->cbUszkodzenie->currentIndex() == 0) {
            ui->wynik_narazenia->setText("POZYTYWNY");
        } else {
            ui->wynik_narazenia->setText("NEGATYWNY");
        }
    }
}

bool OknoPotwierdzenieEMCNarazenie::getWynik() const
{
    if (idTest == DAMP_HEAT_STADY_STATE_ENDURANCE)
        return true;
    else if (idTest == DRY_HEAT || idTest == COLD)
        return ui->cbAlarmYesNo->currentIndex() == 0 && ui->cbTlumnik->currentIndex() == 0;
    else if (idTest == DAMP_HEAT_STADY_STATE_OPERATIONAL)
        return ui->cbAlarmYesNo->currentIndex() == 0;
    else if (idTest == VIBRATION)
        return ui->cbUszkodzenie->currentIndex() == 0;
    else if (idTest == IMPACT)
        return ui->cbAlarmYesNo->currentIndex() == 0 && ui->cbUszkodzenie->currentIndex() == 0;
    else
        return true;
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
