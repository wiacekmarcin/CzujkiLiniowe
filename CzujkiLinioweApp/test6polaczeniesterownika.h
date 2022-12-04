#ifndef TEST6POLACZENIESTEROWNIKA_H
#define TEST6POLACZENIESTEROWNIKA_H

#include <QDialog>

namespace Ui {
class Test6PolaczenieSterownika;
}

class Test6PolaczenieSterownika : public QDialog
{
    Q_OBJECT

public:
    explicit Test6PolaczenieSterownika(QWidget *parent = 0);
    ~Test6PolaczenieSterownika();

private:
    Ui::Test6PolaczenieSterownika *ui;
};

#endif // TEST6POLACZENIESTEROWNIKA_H
