#ifndef PARAMETRYBADANIACZUJKIDLG_H
#define PARAMETRYBADANIACZUJKIDLG_H

#include "parametrybadania.h"
#include "ustawienia.h"

#include <QWidget>
#include <QVector>
#include <QPair>
class QLineEdit;
class QLabel;
namespace Ui {
class ParametryBadaniaCzujkiDlg;
}

class ParametryBadaniaCzujkiDlg : public QWidget
{
    Q_OBJECT

public:
    explicit ParametryBadaniaCzujkiDlg(QWidget *parent = nullptr);
    ~ParametryBadaniaCzujkiDlg();
    void init(bool edit, const Ustawienia & u, ParametryBadania * badanie, QLabel *err);
    bool check();
    void save(ParametryBadania * badanie);
protected:
    void switchOdbiornikReflektor(bool odbiornik);
    void czujkaNrEdited(short id);
    void showInfo7Number(bool show);
    void showInfoSorted(bool sorted);
    void showError(const QString & err);
private slots:
    void changeTypUkladu(int index);

private:
    Ui::ParametryBadaniaCzujkiDlg *ui;
    static constexpr char etTypNadajnika[] = { "Typ nadajnika" };
    static constexpr char etTypNadajnikaOdbiornika[] = { "Typ nadajnika-odbiornika" };
    static constexpr char etTypOdbiornika[] = { "Typ odbiornika" };
    static constexpr char etTypReflektora[] = { "Typ reflektora" };
    static constexpr char etNadajnika[] = { "Nadajnika" };
    static constexpr char etNadajnikaOdbiornika[] = { "Nadajnika-odbiornika" };
    static constexpr char etOdbiornika[] = { "Odbiornika" };
    static constexpr char etReflektora[] = { "Reflektora" };
    static constexpr char etNumerNadajnika[] = { "Numer nadajnika" };
    static constexpr char etNumerNadajnikaOdiornika[] = { "Numer nadajnika-odbiornika" };
    static constexpr char etNumerOdbiornika[] = { "Numer odbiornika" };
    static constexpr char etNumerReflektora[] = { "Numer reflektora" };

    static constexpr char etNazwaNadajnika[] = { "Nadajnik" };
    static constexpr char etNazwaNadajnikaOdbiornika[] = { "Nadajnik-odbiornik" };
    static constexpr char etNazwaOdbiornika[] = { "Odbiornik" };
    static constexpr char etNazwaReflektora[] = { "Reflektor" };

    static const char* etTypPierwszy[2];
    static const char* etTypDrugi[2];
    static const char* etNumerPierwszy[2];
    static const char* etNumerDrugi[2];
    static const char* etPierwszy[2];
    static const char* etDrugi[2];
    static const char* etNazwa[4];

    static const short maxNumCzujek = 7;

    QVector<QPair<QLineEdit*, QLineEdit*>> m_numbers;
    ParametryBadania * badanie;
    QLabel * errorLabel;
    bool testOdtwarzalnosci;
};

#endif // PARAMETRYBADANIACZUJKIDLG_H
