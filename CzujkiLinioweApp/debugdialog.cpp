#include "debugdialog.h"
#include "ui_debugdialog.h"
#include <QDate>
#include <QTime>
DebugDialog::DebugDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugDialog)
{
    ui->setupUi(this);

    connect(ui->pb1, &QPushButton::clicked, this, [this]() { this->ui->pt_comm->clear(); });
    connect(ui->pb2, &QPushButton::clicked, this, [this]() { this->ui->pt_user->clear(); });
    connect(ui->pb3, &QPushButton::clicked, this, [this]() { this->ui->pt_gui->clear(); });
    connect(ui->pb4, &QPushButton::clicked, this, [this]() { this->ui->pt_test->clear(); });
}

DebugDialog::~DebugDialog()
{
    delete ui;
}

void DebugDialog::add(DebugType t, const QString &text)
{
    if (t == DEBUG_COM) {
        ui->pt_comm->appendHtml(QString("<div><div>%1</div><div>%2</div></div>").arg(QTime::currentTime().toString("HH:mm:ss.zzz"), text));
    } else if (t == DEBUG_USER) {
        ui->pt_user->appendHtml(QString("<div><div>%1</div><div>%2</div></div>").arg(QTime::currentTime().toString("HH:mm:ss.zzz"), text));
    } else if (t == DEBUG_GUI) {
        ui->pt_gui->appendHtml(QString("<div><div>%1</div><div>%2</div></div>").arg(QTime::currentTime().toString("HH:mm:ss.zzz"), text));
    } else if (t == DEBUG_TEST) {
        ui->pt_test->appendHtml(QString("<div><div>%1</div><div>%2</div></div>").arg(QTime::currentTime().toString("HH:mm:ss.zzz"), text));
    }
}
