#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QObject>
#include <QFont>
#include <QDebug>

//#include "pdfcreator.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
/*
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "CzujkiLinioweApp_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
*/
    //if ()

    Ustawienia u;
    //u.save();
#if QT_VERSION >= 0x060000
    QFont font(QStringList() << "Arial" << "Times New Roman");
    a.setFont(font);
#endif    
    a.setStyleSheet("QLineEdit[readOnly=\"true\"] {background-color: #F0F0F0;}");
    MainWindow w(nullptr);
    w.setStyleSheet("QLineEdit[readOnly=\"true\"] {background-color: #F0F0F0;}");
    w.activateWindow();
    w.show();
    w.setWindowModality(Qt::WindowModal);

    return a.exec();
}

