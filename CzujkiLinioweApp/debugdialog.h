#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <QDialog>

namespace Ui {
class DebugDialog;
}

typedef enum eDebugType {
    DEBUG_COM,
    DEBUG_USER,
    DEBUG_GUI,
    DEBUG_TEST,
} DebugType;

class DebugDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DebugDialog(QWidget *parent = nullptr);
    ~DebugDialog();
    void add(DebugType t, const QString & text);
private:
    Ui::DebugDialog *ui;
};

#endif // DEBUGDIALOG_H
