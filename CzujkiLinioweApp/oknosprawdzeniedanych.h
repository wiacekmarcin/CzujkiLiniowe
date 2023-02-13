#ifndef OKNOSPRAWDZENIEDANYCH_H
#define OKNOSPRAWDZENIEDANYCH_H

#include <QDialog>
#include <QCloseEvent>
#include "danetestu.h"

namespace Ui {
    class OknoSprawdzenieDanych;
}

class OknoSprawdzenieDanych : public QDialog
{
    Q_OBJECT

public:
    explicit OknoSprawdzenieDanych(const DaneTestu & test,
                                QWidget *parent = nullptr);
    ~OknoSprawdzenieDanych();
    void closeEvent(QCloseEvent *event);
protected:
    void pbCancel_clicked();
private:
    Ui::OknoSprawdzenieDanych *ui;
};

#endif // OKNOSPRAWDZENIEDANYCH_H
