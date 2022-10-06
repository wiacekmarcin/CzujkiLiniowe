#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <QDialog>

namespace Ui {
class DebugDialog;
}

class DebugDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DebugDialog(QWidget *parent = nullptr);
    ~DebugDialog();
    void add(const QString & text);
private:
    Ui::DebugDialog *ui;
};

#endif // DEBUGDIALOG_H
