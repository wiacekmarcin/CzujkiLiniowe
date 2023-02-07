#ifndef HASLODIALOG_H
#define HASLODIALOG_H

#include <QDialog>

namespace Ui {
class HasloDialog;
}

class HasloDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HasloDialog(QWidget *parent = nullptr);
    ~HasloDialog();
    QString getHaslo() const;

private:
    Ui::HasloDialog *ui;

};

#endif // HASLODIALOG_H
