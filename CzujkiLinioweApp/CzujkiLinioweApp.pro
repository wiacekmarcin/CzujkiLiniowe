QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Testowa2Comy/sterownik.cpp \
    Testowa2Comy/zasilacz.cpp \
    Testowa2Comy/zasilaczwidget.cpp \
    badaniedane.cpp \
    crc8.cpp \
    debugdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    parametrybadania.cpp \
    parametrybadania1.cpp \
    parametrybadania2.cpp \
    parametrykalibracyjnedlg.cpp \
    serialdevice.cpp \
    serialmessage.cpp \
    sterownikustawieniawidget.cpp \
    testmiernikadlg.cpp \
    teststerownikadlg.cpp \
    ustawienia.cpp \
    ustawienia_gen.cpp \
    ustawieniafiltrow.cpp \
    ustawienianapedow.cpp

HEADERS += \
    Testowa2Comy/sterownik.h \
    Testowa2Comy/zasilacz.h \
    Testowa2Comy/zasilaczwidget.h \
    badaniedane.h \
    crc8.h \
    debugdialog.h \
    mainwindow.h \
    parametrybadania.h \
    parametrybadania1.h \
    parametrybadania2.h \
    parametrykalibracyjnedlg.h \
    serialdevice.h \
    serialmessage.h \
    sterownikustawieniawidget.h \
    testmiernikadlg.h \
    teststerownikadlg.h \
    ustawienia.h \
    ustawienia_gen.h \
    ustawieniafiltrow.h \
    ustawienianapedow.h

FORMS += \
    Testowa2Comy/zasilaczwidget.ui \
    debugdialog.ui \
    mainwindow.ui \
    parametrybadania.ui \
    parametrybadania1.ui \
    parametrybadania2.ui \
    parametrykalibracyjnedlg.ui \
    sterownikustawieniawidget.ui \
    testmiernikadlg.ui \
    teststerownikadlg.ui \
    ustawieniafiltrow.ui \
    ustawienianapedow.ui

TRANSLATIONS += \
    CzujkiLinioweApp_pl_PL.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
