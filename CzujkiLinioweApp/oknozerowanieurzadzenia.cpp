#include "oknozerowanieurzadzenia.h"
#include "ui_oknozerowanieurzadzenia.h"

#include "sterownik.h"

#include <QMessageBox>
#include <QRadioButton>

OknoZerowanieUrzadzenia::OknoZerowanieUrzadzenia(bool nadajnik_, bool odbiornik_, bool filtry_, bool wozek_,
                                                 const QString & trans, const QString & receiv,
                                                 Sterownik *device_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoZerowanieUrzadzenia),
    timer(this),
    errorMsg(false),
    nadajnik(nadajnik_),
    odbiornik(odbiornik_),
    filtry(filtry_),
    wozek(wozek_),
    device(device_)
{
    ui->setupUi(this);
    ui->transmitter->setText(trans);
    ui->reciver->setText(receiv);

    buttons[0] = nullptr;
    buttons[1] = ui->rb_transmitter_poziom;
    buttons[2] = ui->rb_transmitter_pion;
    buttons[3] = ui->rb_a;
    buttons[4] = ui->rb_b;
    buttons[5] = ui->rb_c;
    buttons[6] = ui->wozek_poziom;
    buttons[7] = ui->wozek_pion;
    buttons[8] = ui->rb_receiver_poziom;
    buttons[9] = ui->rb_receiver_pion;

    init();
#ifdef TESTVAL
        ui->pbDalej->setVisible(true);
        connect(ui->pbDalej, &QPushButton::clicked, this, [this](){ done(QDialog::Accepted); });
#else
        ui->pbDalej->setVisible(false);
#endif
        connect(ui->pbBreak, &QPushButton::clicked, this, [this](){ this->pbCancel_clicked(); });
        connect(ui->pbAgain, &QPushButton::clicked, this, [this](){ init(); });
        adjustSize();
}

void OknoZerowanieUrzadzenia::init()
{
    ui->frameError->setVisible(false);
    for (short id = 0; id < 10; ++id) {
        silnikZero[id] = id == 0;
        if (!nadajnik && (id == 1 || id == 2)) {
            silnikZero[id] = true;
            buttons[id]->setDisabled(true);
        } else if (!odbiornik && (id == 8 || id == 9)) {
            silnikZero[id] = true;
            buttons[id]->setDisabled(true);
        } else if (!filtry && (id == 3 || id == 4 || id == 5)) {
            silnikZero[id] = true;
            buttons[id]->setDisabled(true);
        } else if (!wozek && (id == 6 || id == 7)) {
            silnikZero[id] = true;
            buttons[id]->setDisabled(true);
        }
        else
            silnikZero[id] = false;
    }

    //for (short i = 1; i <= 9; ++i) {
    //    buttons[i]->setStyleSheet("color:black");
    //}

    ui->frame_filtry->setDisabled(!filtry);
    ui->frame_transmitter->setDisabled(!nadajnik);
    ui->frame_receiver->setDisabled(!odbiornik);
    ui->frame_wozek->setDisabled(!wozek);
    adjustSize();
    //QString debug = QString::fromUtf8("<ul>Zerowanie urzadzenia");
    //if (filtry) debug+= QString::fromUtf8("<li>filtry</li>");
    //if (ramiona) debug+= QString::fromUtf8("<li>Nadajnik</li><li>Odbiornik</li>");
    //if (wozek) debug+= QString::fromUtf8("<li>wozek</li>");
    //debug+= "</ul>";
    //emit debug(DEBUG_TEST, debug);
    device->setZerowanieUrzadzen(nadajnik, odbiornik, filtry, wozek);
    ui->error->setVisible(false);
    unsigned int timCzas = 1000;
    if (filtry)
        timCzas += 5000;
    if (nadajnik || odbiornik)
        timCzas += 11000;
    if (wozek)
        timCzas += 60000;
    timer.singleShot(timCzas, this, &OknoZerowanieUrzadzenia::timeout);
}

OknoZerowanieUrzadzenia::~OknoZerowanieUrzadzenia()
{
    timer.stop();
    delete ui;
}

void OknoZerowanieUrzadzenia::ster_setPositionDone(short silnik, RuchSilnikaType ruch)
{
    if (!ruch.home || ruch.move)
        return;

    if (ruch.err) {
        ui->error->setVisible(true);
        buttons[silnik]->setStyleSheet("color:red");
        ui->frameError->setVisible(true);
        return;
    }

    if (ruch.inter) {
        ui->error->setVisible(true);
        buttons[silnik]->setStyleSheet("color:blue");
        ui->frameError->setVisible(true);
        return;
    }

    silnikZero[silnik] = true;
    if (buttons[silnik])
        buttons[silnik]->setChecked(true);

    for (short id = 1; id < 10; ++id) {
        if (!silnikZero[id])
            return;
    }
    timer.stop();
    done(QDialog::Accepted);
}

void OknoZerowanieUrzadzenia::timeout()
{
    ui->frameError->setVisible(true);
}

static int question(QWidget * parent, const QString & title, const QString & pytanie) {
    QMessageBox messageBox(QMessageBox::Question, title, pytanie, 
                        QMessageBox::Yes | QMessageBox::No, parent);

    messageBox.setButtonText(QMessageBox::Yes, QString::fromUtf8("Tak"));
    messageBox.setButtonText(QMessageBox::No, QString::fromUtf8("Nie"));
    return messageBox.exec();
}

void OknoZerowanieUrzadzenia::pbCancel_clicked()
{
    int ret = question(this, QString::fromUtf8("Oczekiwanie zerowanie stanowiska"),
                                    QString::fromUtf8("Czy napewno chcesz przerwać badanie"));
    if (ret == QMessageBox::Yes)
        done(QDialog::Rejected);
}
