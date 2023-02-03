#include "wybortestu.h"
#include "ui_wybortestu.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QStyle>
#include <QScreen>
#include <QLabel>

WyborTestu::WyborTestu(const QMap<int, DaneTestu> & testy, bool powtarzalnosc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WyborTestu)
    ,wyborTestu(-1)
{
    ui->setupUi(this);
    int num = 0;

    for( const auto & t : testy.keys()) {
        const DaneTestu & test = testy[t];
        if (t == REPRODUCIBILITY) {
            if (!powtarzalnosc) {
                QRadioButton* radioButton = new QRadioButton(ui->groupBox);
                radioButton->setObjectName(QString("radioButton%1").arg(t));
                radioButton->setText(QString(test.getName()));
                radioButton->setAutoExclusive(true);
                radio.append(qMakePair(t, radioButton));
                ui->gridLayout->addWidget(radioButton, num, 0, 1, 1);
                radioButton->setChecked(true);
            } else {
                QLabel * label = new QLabel(ui->groupBox);
                label->setObjectName(QString("label%1").arg(t));
                label->setIndent(20);
                label->setText(QString(test.getName()));
                ui->gridLayout->addWidget(label, num, 0, 1, 1);
            }
        } else {
            if (!powtarzalnosc || test.getWykonany()) {
                QLabel * label = new QLabel(ui->groupBox);
                label->setObjectName(QString("label%1").arg(t));
                label->setIndent(20);
                label->setText(QString(test.getName()));
                ui->gridLayout->addWidget(label, num, 0, 1, 1);
            } else {
                QRadioButton* radioButton = new QRadioButton(ui->groupBox);
                radioButton->setObjectName(QString("radioButton%1").arg(t));
                radioButton->setText(QString(test.getName()));
                radioButton->setAutoExclusive(true);
                radio.append(qMakePair(t, radioButton));
                ui->gridLayout->addWidget(radioButton, num, 0, 1, 1);
                radioButton->setChecked(true);
            }
        }

        QLabel * label = new QLabel(ui->groupBox);
        label->setObjectName(QString("label_wykonany%1").arg(t));
        label->setText(test.getWykonany() ? QString::fromUtf8("WYKONANY") : QString::fromUtf8("NIE WYKONANY"));
        ui->gridLayout->addWidget(label, num, 1, 1, 1);
        num += 1;
    }

    connect(ui->pbCancel, &QPushButton::clicked, this, [this]() { this->done(QDialog::Rejected); });
    connect(ui->pbNext, &QPushButton::clicked, this, [this]() { this->done(QDialog::Accepted); });
    connect(ui->pbCancel, &QPushButton::pressed, this, [this]() { this->done(QDialog::Rejected); });
    connect(ui->pbNext, &QPushButton::pressed, this, [this]() { this->done(QDialog::Accepted); });
    adjustSize();
#if QT_VERSION >= 0x060000
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            screen()->availableGeometry()
        )
    );
#endif    
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

