#include "parametrybadaniadlg.h"
#include "ui_parametrybadaniadlg.h"

ParametryBadaniaDlg::ParametryBadaniaDlg(bool editData, const Ustawienia & u, ParametryBadania *d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametryBadaniaDlg),
    dane(d),
    checkValidTmr(this)
{
    ui->setupUi(this);
    //ui->error->setVisible(false);
    ui->tabBadanie->init(editData, u, d, ui->error);
    ui->tabCzujki->init(editData, u, d, ui->error);

    d->setOdtwarzalnoscCmaxCrep(u.getOdtwarzalnoscCmaxCrep());
    d->setOdtwarzalnoscCrepCmin(u.getOdtwarzalnoscCrepCmin());
    d->setOdtwarzalnoscCmin(u.getMinimalnaWartoscCzujkiCn());
    d->setPowtarzalnoscCmaxCmin(u.getPowtarzalnoscCmaxCmin());
    d->setDrogaoptycznaCmaxCmin(u.getDlugoscDrogiOptycznejCmaxCmin());
    d->setRozproszoneSwiatloCmaxCmin(u.getRozproszoneSwiatloCmaxCmin());
    d->setTolerancjaNapieciaZasilaniaCmaxCmin(u.getTolerancjaNapieciaZasilaniaCmaxCmin());
    d->setSucheCieploCmaxCmin(u.getOdpornoscSucheGoraceCmaxCmin());
    d->setZimnoCmaxCmin(u.getOdpornoscZimnoCmaxCmin());
    d->setWilgotneCieploOdpornoscCmaxCmin(u.getOdpornoscWilgotneGoraceCmaxCmin());
    d->setWilgotneCieploWytrzymaloscCmaxCmin(u.getWytrzymaloscWilgotneGoraceCmaxCmin());
    d->setWibracjeCmaxCmin(u.getWytrzymaloscWibracjeCmaxCmin());
    d->setUderzenieCmaxCmin(u.getOdpornoscUderzoniowaCmaxCmin());
    d->setKorozjaSO2CmaxCmin(u.getWytrzymaloscKorozyjnaSO2CmaxCmin());
    d->setZakloceniaEMCCmaxCmin(u.getOdpornoscElektroMagnetycznaCmaxCmin());
    
    d->setSzybkieZmianyWartoscTlumnikaA(u.getSzybkieZmianyWartoscTlumnikaA());
    d->setSzybkieZmianyWartoscTlumnikaB(u.getSzybkieZmianyWartoscTlumnikaB());
    d->setWartoscTlumienieDlaKataNieWspolosiowosci(u.getWartoscTlumienieDlaKataNieWspolosiowosci());
    d->setNiewspolosiowoscMinimalnyKatProducentMierzony(u.getNiewspolosiowoscMinimalnyKatProducentMierzony());
    d->setMinimalneNapieciaTolerancjaNapiecia(u.getMinimalneNapieciaTolerancjaNapiecia());
    d->setMaksymalneNapieciaTolerancjaNapiecia(u.getMaksymalneNapieciaTolerancjaNapiecia());
    d->setMinimalnyCzasOczekiwaniaPowtarzalnosc1Test(u.getMinimalnyCzasOczekiwaniaPowtarzalnosc1Test());
    d->setMaksymalnyCzasOczekiwaniaPowtarzalnosc1Test(u.getMaksymalnyCzasOczekiwaniaPowtarzalnosc1Test());


    connect(&checkValidTmr, &QTimer::timeout, this, &ParametryBadaniaDlg::checkValid);
    checkValidTmr.setInterval(1000);
    checkValidTmr.start();
    connect(ui->pbOK, &QPushButton::clicked, this, [this]() { this->pbOK_clicked(); });
    connect(ui->pbCancel, &QPushButton::clicked, this, [this]() { this->pbCancel_clicked(); });
    adjustSize();
}

ParametryBadaniaDlg::~ParametryBadaniaDlg()
{
    checkValidTmr.stop();
    delete ui;
}

void ParametryBadaniaDlg::checkValid()
{
    bool valid = false;
    if (ui->tabWidget->currentWidget() == ui->tabBadanie) {
        if (ui->tabBadanie->check())
            valid = ui->tabCzujki->check();
    } else if (ui->tabWidget->currentWidget() == ui->tabCzujki) {
        if (ui->tabCzujki->check())
            valid = ui->tabBadanie->check();
    }
    ui->pbOK->setEnabled(valid);

}

void ParametryBadaniaDlg::pbOK_clicked()
{
    ui->tabBadanie->save(dane);
    ui->tabCzujki->save(dane);
    done(QDialog::Accepted);
}


void ParametryBadaniaDlg::pbCancel_clicked()
{
    //qDebug() << "clicked";
    done(QDialog::Rejected);
}




