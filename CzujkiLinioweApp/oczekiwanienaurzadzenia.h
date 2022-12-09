#ifndef OCZEKIWANIENAURZADZENIA_H
#define OCZEKIWANIENAURZADZENIA_H

#include <QTimer>
#include <QDialog>

namespace Ui {
class OczekiwanieNaUrzadzenia;
}

class OczekiwanieNaUrzadzenia : public QDialog
{
    Q_OBJECT

public:
    explicit OczekiwanieNaUrzadzenia(QWidget *parent = nullptr);
    ~OczekiwanieNaUrzadzenia();
public slots:
    void zasilacz(bool success, int state);
private slots:
    void timeout();
private:
    Ui::OczekiwanieNaUrzadzenia *ui;
    QTimer timer;
    short cntTmt;
    bool zasilaczOk;
    bool sterownikOk;
#ifdef DEFVAL
    static constexpr short maxCzas = 10;
#else
    static constexpr short maxCzas = 60;
#endif
};

#endif // OCZEKIWANIENAURZADZENIA_H
