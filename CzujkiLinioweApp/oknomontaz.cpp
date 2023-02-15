#include "oknomontaz.h"
#include "ui_oknomontaz.h"

#include <QMessageBox>


static int questionQuit(const QString & title, const QString & pytanie, QWidget * parent) {
    QMessageBox messageBox(QMessageBox::Question, title, pytanie,
                        QMessageBox::Close | QMessageBox::Cancel, parent);


    messageBox.setButtonText(QMessageBox::Close, QString::fromUtf8("Zamknij"));
    messageBox.setButtonText(QMessageBox::Cancel, QString::fromUtf8("Anuluj"));

    return messageBox.exec();
}

/*
 * opticalLen = 0 brak
 * 1 = minimalne
 * 2 = maksymalna
 */
OknoMontaz::OknoMontaz(bool usuniecieZabezp, bool minRozstawienie, bool maxRozstawienienie, bool rozproszone,
                       bool systemNadajnikObiornik, const DaneTestu &daneTestu, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoMontaz)
{
    ui->setupUi(this);
    (void)systemNadajnikObiornik;

    ui->numerNadajnika->setText(daneTestu.getNumerTransmitter());
    ui->numerReceiver->setText(daneTestu.getNumerReceiver());
    ui->testName->setText(daneTestu.getName());

    QString nad = daneTestu.getNazwaTransmitter_a();
    QString odb = daneTestu.getNazwaReceiver_a();

    ui->eTransmitter->setText(QString::fromUtf8("%1:").arg(nad));
    ui->eReceiver->setText(QString::fromUtf8("%1:").arg(odb));

    if (minRozstawienie || maxRozstawienienie) {
        ui->frame_dlugoscDrogiOptycznej->setVisible(true);
        ui->eminimalnerozstawienie->setVisible(minRozstawienie);
        ui->emaksymalnerozstawienie->setVisible(maxRozstawienienie);
        ui->minimalnerozstawienie->setVisible(minRozstawienie);
        ui->maksymalnerozstawienie->setVisible(maxRozstawienienie);
        ui->minimalnerozstawienie->setText(QString("%1 m").arg(daneTestu.getMinimalneRozstawienie()));
        ui->maksymalnerozstawienie->setText(QString("%1 m").arg(daneTestu.getMaksymalneRozstawienie()));
    } else
        ui->frame_dlugoscDrogiOptycznej->setVisible(false);

    ui->frameBlokada->setVisible(usuniecieZabezp);

    //if (systemNadajnikObiornik) {
        ui->infoBlokada->setText(QString::fromUtf8("Pamiętaj o zdjęciu blokad z ramion %1 i %2.").arg(nad, odb));
    //} else
    //    ui->infoBlokada->setText(QString::fromUtf8("Pamiętaj o zdjęciu blokad z ramion %1.").arg(nad));

    ui->swiatloRozproszone->setVisible(rozproszone);

    connect(ui->pbDalej, &QPushButton::clicked, this, [this]() { this->done(QDialog::Accepted); });
    connect(ui->pbPrzerwij, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    adjustSize();
}

OknoMontaz::~OknoMontaz()
{
    delete ui;
}

static int question(QWidget * parent, const QString & title, const QString & pytanie) {
    QMessageBox messageBox(QMessageBox::Question, title, pytanie, 
                        QMessageBox::Yes | QMessageBox::No, parent);

    messageBox.setButtonText(QMessageBox::Yes, QString::fromUtf8("Tak"));
    messageBox.setButtonText(QMessageBox::No, QString::fromUtf8("Nie"));
    return messageBox.exec();
}

void OknoMontaz::pbCancel_clicked()
{
    int ret = question(this, QString::fromUtf8("Badanie : %1").arg(ui->testName->text()),
                             QString::fromUtf8("Czy napewno chcesz przerwać badanie"));
    if (ret == QMessageBox::Yes)
        done(QDialog::Rejected);
}

void OknoMontaz::closeEvent(QCloseEvent *event)
{
    auto btn = questionQuit(QString::fromUtf8("CzujkiLiniowe"),
                            QString::fromUtf8("Czy chcesz wyjść z badania bez zapisywania danych"),
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
