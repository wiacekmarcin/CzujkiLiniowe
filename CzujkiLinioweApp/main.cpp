#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QObject>
#include <QSerialPort>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "CzujkiLinioweApp_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Ustawienia u;
    u.save();
    a.setStyleSheet("QLineEdit[readOnly=\"true\"] {background-color: #F0F0F0;}");
    MainWindow w;
    w.setStyleSheet("QLineEdit[readOnly=\"true\"] {background-color: #F0F0F0;}");
    w.show();
    return a.exec();
}

