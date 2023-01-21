#include "wybortestu.h"
#include "qcheckbox.h"
#include "ui_wybortestu.h"
#include <QRadioButton>
#include <QCheckBox>
#include <QButtonGroup>

WyborTestu::WyborTestu(const QMap<int, DaneTestu> & testy, bool powtarzalnosc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WyborTestu)
    ,wyborTestu(-1)
{
    ui->setupUi(this);
    int num = 0;
    for( const auto & t : testy.keys()) {
        const DaneTestu & test = testy[t];
        QRadioButton* radioButton = new QRadioButton(ui->groupBox);
        radioButton->setObjectName(QString("radioButton%1").arg(t));
        radioButton->setText(QString(test.getName()));
        radioButton->setAutoExclusive(true);
        radio.append(qMakePair(t, radioButton));
        radioButton->setEnabled(t == REPRODUCIBILITY ? !powtarzalnosc : powtarzalnosc);
        ui->gridLayout->addWidget(radioButton, num, 0, 1, 1);
        if (t == REPRODUCIBILITY && !powtarzalnosc) {
            radioButton->setChecked(true);
        }

        QCheckBox * checkbox = new QCheckBox(ui->groupBox);
        checkbox->setObjectName(QString("checkbox%1").arg(t));
        checkbox->setText("Wykonany");
        checkbox->setEnabled(t == REPRODUCIBILITY ? !powtarzalnosc : powtarzalnosc);
        if (test.getWykonany()) {
            checkbox->setChecked(true);
            radioButton->setCheckable(false);
        }
        checkbox->setCheckable(false);
        ui->gridLayout->addWidget(checkbox, num, 1, 1, 1);
        num += 1;
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

