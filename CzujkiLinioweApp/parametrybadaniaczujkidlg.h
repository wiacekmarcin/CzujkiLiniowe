#ifndef PARAMETRYBADANIACZUJKIDLG_H
#define PARAMETRYBADANIACZUJKIDLG_H

#include "parametrybadania.h"
#include "ustawienia.h"

#include <QWidget>
#include <QVector>
#include <QPair>
#include <QPalette>

class QLineEdit;
class QLabel;
class QFrame;
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
    void createCzujkaTable(ParametryBadania *badanie);
    void createCzujkaTableReadOlny(ParametryBadania *badanie);
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

    static constexpr char etNadajnik[] = { "Nadajnik" };
    static constexpr char etNadajnikOdbiornik[] = { "Nadajnik-odbiornik" };
    static constexpr char etOdbiornik[] = { "Odbiornik" };
    static constexpr char etReflektor[] = { "Reflektor" };

    static const char* etTypTransmitter[2];
    static const char* etTransmitter_a[2];
    static const char* etNumerTransmitter[2];
    static const char* etTransmitter[2];

    static const char* etTypReceiver[2];
    static const char* etReceiver_a[2];
    static const char* etNumerReceiver[2];
    static const char* etReceiver[2];

    static const short maxNumCzujek = 7;

    QVector<QPair<QLineEdit*, QLineEdit*>> m_numbers;
    ParametryBadania * badanie;
    QLabel * errorLabel;
    bool testOdtwarzalnosci;

    void configCellLE(QLineEdit *l, const QString &objectName);
    void configCellLAB(QLabel *l, const QString &objectName, bool s = false, const QSize &s1 = QSize(), const QSize &s2 = QSize());
};

#endif // PARAMETRYBADANIACZUJKIDLG_H
