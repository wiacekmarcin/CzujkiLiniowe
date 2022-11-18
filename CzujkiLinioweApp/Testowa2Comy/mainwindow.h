#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QList>
#include <QString>
#include <QStringList>
#include <QTimer>
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

    typedef enum _serialZasWork
    {
        SEND_NONE = 0,
        SEND_IDENT = 1,
        SEND_VOLT,
        SEND_CURR
    } SerialZasWork;

private slots:
    void openSerialPortZasilacz();
    void closeSerialPortZasilacz();
    void writeDataZasilacz(const QByteArray &data);
    void readDataZasilacz();
    void handleErrorZasilacz(QSerialPort::SerialPortError error);
    void findZasilacz();
    void timeoutZasilacz();
protected:

    bool findSterownik();
    QList<QStringList> getComPorts();

    void showStatusMessage(const QString &message);
private:
    Ui::MainWindow *ui = nullptr;
    QLabel *m_status = nullptr;
    QSerialPort *m_serialZas = nullptr;
    QSerialPort *m_serialSter = nullptr;
    QString m_portNameZas = "N/A";
    SerialZasWork zasWork;
    QTimer readMeas;
};
#endif // MAINWINDOW_H
