#include "wybortestu.h"
#include "qcheckbox.h"
#include "ui_wybortestu.h"
#include <QRadioButton>
#include <QCheckBox>
#include <QButtonGroup>

WyborTestu::WyborTestu(const ListaTestow & testy, bool powtarzalnosc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WyborTestu)
    ,wyborTestu(-1)
{
    ui->setupUi(this);

    for (int s = 0; s < testy.nazwyTestow.size(); s++) {
        QRadioButton* radioButton = new QRadioButton(ui->groupBox);
        radioButton->setObjectName(QString("radioButton%1").arg(s));
        radioButton->setText(QString(testy.nazwyTestow[s]));
        radioButton->setAutoExclusive(true);
        radio.append(qMakePair(s, radioButton));
        //radioButton->setEnabled(s == 0 ? !powtarzalnosc : powtarzalnosc);
        ui->gridLayout->addWidget(radioButton, s, 0, 1, 1);
        //if (s == 0 && !powtarzalnosc) {
        //    radioButton->setChecked(true);
        //}

        QCheckBox * checkbox = new QCheckBox(ui->groupBox);
        checkbox->setObjectName(QString("checkbox%1").arg(s));
        checkbox->setText("Wykonany");
        checkbox->setCheckable(false);
        checkbox->setEnabled(s == 0 ? !powtarzalnosc : powtarzalnosc);
        if (testy.wykonane.contains(s)) {
            checkbox->setChecked(true);
            radioButton->setCheckable(false);
        }
        ui->gridLayout->addWidget(checkbox, s, 1, 1, 1);
    }


    connect(ui->pbCancel, &QPushButton::clicked, this, [this]() { this->reject(); });
    connect(ui->pbNext, &QPushButton::clicked, this, [this]() { this->accept(); });
    connect(ui->pbCancel, &QPushButton::pressed, this, [this]() { this->reject(); });
    connect(ui->pbNext, &QPushButton::pressed, this, [this]() { this->accept(); });
}

WyborTestu::~WyborTestu()
{
    delete ui;
}

short WyborTestu::getWyborTestu() const
{
    for(auto r :radio) {
        if (r.second->isChecked())
            return r.first;
    }
    return -1;
}

