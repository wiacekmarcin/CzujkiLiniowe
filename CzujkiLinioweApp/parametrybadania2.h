#ifndef PARAMETRYBADANIA2_H
#define PARAMETRYBADANIA2_H

#include <QWidget>

namespace Ui {
class ParametryBadania2;
}

class ParametryBadania2 : public QWidget
{
    Q_OBJECT

public:
    explicit ParametryBadania2(QWidget *parent = nullptr);
    ~ParametryBadania2();

protected:
    void switchOdbiornikReflektor(bool odbiornik);
private slots:
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::ParametryBadania2 *ui;
};

#endif // PARAMETRYBADANIA2_H
