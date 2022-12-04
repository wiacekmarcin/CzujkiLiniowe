#ifndef PARAMETRYBADANIA2_H
#define PARAMETRYBADANIA2_H

#include "danebadania.h"
#include "ustawienia.h"

#include <QWidget>
#include <QVector>
#include <QPair>
class QLineEdit;
class QLabel;
namespace Ui {
class ParametryBadania2;
}

class ParametryBadania2 : public QWidget
{
    Q_OBJECT

public:
    explicit ParametryBadania2(QWidget *parent = nullptr);
    ~ParametryBadania2();
    void init(const Ustawienia & u, DaneBadania * badanie, QLabel *err);
    bool check();
    void save(DaneBadania * badanie);
protected:
    void switchOdbiornikReflektor(bool odbiornik);
private slots:
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::ParametryBadania2 *ui;
    static constexpr char etTypNadajnika[] = { "Typ nadajnika" };
    static constexpr char etTypNadajnikaOdbiornika[] = { "Typ nadajnika-odbiornika" };
    static constexpr char etTypOdbiornika[] = { "Typ odbiornika" };
    static constexpr char etTypReflektora[] = { "Typ reflektora" };
    static constexpr char etNadajnika[] = { "Nadajnika" };
    static constexpr char etNadajnikaOdbiornika[] = { "Nadajnika-odbiornika" };
    static constexpr char etOdbiornika[] = { "Odbiornika" };
    static constexpr char etReflektora[] = { "Reflektora" };
    static constexpr char etNumerNadajnika[] = { "Numer\nnadajnika" };
    static constexpr char etNumerNadajnikaOdiornika[] = { "Numer\nnadajnika-odbiornika" };
    static constexpr char etNumerOdbiornika[] = { "Numer\nodbiornika" };
    static constexpr char etNumerReflektora[] = { "Numer\nreflektora" };

    static const char* etTypPierwszy[2];
    static const char* etTypDrugi[2];
    static const char* etNumerPierwszy[2];
    static const char* etNumerDrugi[2];
    static const char* etPierwszy[2];
    static const char* etDrugi[2];

    QVector<QPair<QLineEdit*, QLineEdit*>> m_numbers;
    DaneBadania * badanie;
    QLabel * errorLabel;
};

#endif // PARAMETRYBADANIA2_H
