#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QList>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QMutex>

#include "zasilacz.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    

    void showStatusMessage(const QString &message);
private:
    Ui::MainWindow *ui = nullptr;
    QLabel *m_status = nullptr;

};
#endif // MAINWINDOW_H
