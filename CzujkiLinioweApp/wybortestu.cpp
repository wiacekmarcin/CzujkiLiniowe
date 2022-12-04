#include "wybortestu.h"
#include "qcheckbox.h"
#include "ui_wybortestu.h"
#include <QRadioButton>
#include <QCheckBox>

WyborTestu::WyborTestu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WyborTestu)
{
    ui->setupUi(this);
    nazwyTestow << "Odtwarzalność" << "Powtarzalność" << "Zależność kierunkowa" << "Zmiana parametrów zasilania"
                << "Szybkie zmiany tłumienia" << "Powolne zmiany tłumienia" << "Zależność od długości drogi optycznej"
                << "Czułość na pożar" << "Światło rozproszone" << "Odporność na suche gorąco"
                << "Odporność na zimno" << "Odporność na wilgotne gorąco, stan ustalony"
                << "Wytrzymałość na wilgotne gorąco, stan ustalony" << "Wytrzymałość na wibracje"
                << "Odporność na wyładowania elektrostatyczne"
                << "Odporność na wypromieniowanie pola elektromagnetycznego"
                << "Odporność na przewodzone zakłocenia indukowane przez pola elektromagnetyczne"
                << "Odporność na szybkie impulsy przejściowe"
                << "Odporność na powolne udary napęciowe wysokiej energii"
                << "Wytrzymałość na korozję powodowaną przez dwutlenek siarki"
                << "Odporność na uderzenie"
                ;
    for (int s = 0; s < nazwyTestow.size(); s++) {
        QRadioButton* radioButton = new QRadioButton(ui->groupBox);
        radioButton->setObjectName(QString("radioButton%1").arg(s));
        radioButton->setText(QString(nazwyTestow[s]));
        radioButton->setAutoExclusive(true);

        ui->gridLayout->addWidget(radioButton, s/2, 0+2*(s%2), 1, 1);

        QCheckBox * checkbox = new QCheckBox(ui->groupBox);
        checkbox->setObjectName(QString("checkbox%1").arg(s));
        checkbox->setText("Wykonany");
        checkbox->setCheckable(false);
        ui->gridLayout->addWidget(checkbox, s/2, 1+2*(s%2), 1, 1);

    }
}

WyborTestu::~WyborTestu()
{
    delete ui;
}

