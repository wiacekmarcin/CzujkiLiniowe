#include "oknozerowanieurzadzenia.h"
#include "ui_oknozerowanieurzadzenia.h"

#include "sterownik.h"

#include <QMessageBox>
#include <QRadioButton>

OknoZerowanieUrzadzenia::OknoZerowanieUrzadzenia(bool ramiona_, bool filtry_, bool wozek_, Sterownik *device_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoZerowanieUrzadzenia),
    timer(this),
    errorMsg(false),
    ramiona(ramiona_),
    filtry(filtry_),
    wozek(wozek_),
    device(device_)
{
    ui->setupUi(this);

    buttons[0] = nullptr;
    buttons[1] = ui->rb_pierwszy_poziom;
    buttons[2] = ui->rb_pierwwszy_pion;
    buttons[3] = ui->rb_a;
    buttons[4] = ui->rb_b;
    buttons[5] = ui->rb_c;
    buttons[6] = ui->wozek_poziom;
    buttons[7] = ui->wozek_pion;
    buttons[8] = ui->rb_drugi_poziom;
    buttons[9] = ui->rb_drugi_pion;

    init();
#ifdef DEFVAL
        ui->pbDalej->setVisible(true);
        connect(ui->pbDalej, &QPushButton::clicked, this, [this](){ accept(); });
#else
        ui->pbDalej->setVisible(false);
#endif
        connect(ui->pbBreak, &QPushButton::clicked, this, [this](){ this->pbCancel_clicked(); });
        connect(ui->pbAgain, &QPushButton::clicked, this, [this](){ init(); });
}

void OknoZerowanieUrzadzenia::init()
{
    ui->frameError->setVisible(false);
    for (short id = 0; id < 10; ++id) {
        silnikZero[id] = id == 0;
        if (!ramiona && (id == 1 || id == 2 || id == 8 || id == 9)) {
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
    ui->frame_nadajnik->setDisabled(!ramiona);
    ui->frame_odbiornik->setDisabled(!ramiona);
    ui->frame_wozek->setDisabled(!wozek);
    adjustSize();
    //QString debug = QString("<ul>Zerowanie urzadzenia");
    //if (filtry) debug+= QString("<li>filtry</li>");
    //if (ramiona) debug+= QString("<li>Nadajnik</li><li>Odbiornik</li>");
    //if (wozek) debug+= QString("<li>wozek</li>");
    //debug+= "</ul>";
    //emit debug(DEBUG_TEST, debug);
    device->setZerowanieUrzadzen(ramiona, filtry, wozek);
    ui->error->setVisible(false);
    timer.singleShot(wozek ? 20000 : 5000, this, &OknoZerowanieUrzadzenia::timeout);
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
    accept();
}

void OknoZerowanieUrzadzenia::timeout()
{
    ui->frameError->setVisible(true);
}

void OknoZerowanieUrzadzenia::pbCancel_clicked()
{
    int ret = QMessageBox::question(this, QString("Oczekiwanie zerowanie stanowiska"),
                                    "Czy napewno chcesz przerwać badanie");
    if (ret == QMessageBox::Yes)
        reject();
}
