QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += DEFVAL
SOURCES += \
    danetestu.cpp \
    listabadan.cpp \
    oczekiwanienaurzadzenia.cpp \
    parametrybadania.cpp \
    parametrybadania_gen.cpp \
    parametrybadaniaczujkidlg.cpp \
    parametrybadaniadlg.cpp \
    parametrybadaniaogolnedlg.cpp \
    proceduratestowa.cpp \
    serialdevice.cpp \
    test1parametrytestu.cpp \
    test2potwierdzenie.cpp \
    test3sprawdzenie.cpp \
    test4montaz.cpp \
    test6stabilizacjaczujki.cpp \
    test7badanie.cpp \
    teststanowiskadlg.cpp \
    wybortestu.cpp \
    zasilacz.cpp \
    crc8.cpp \
    debugdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    parametrykalibracyjnedlg.cpp \
    sterownik.cpp \
    serialmessage.cpp \
    sterownikustawieniawidget.cpp \
    testzasilaczadlg.cpp \
    teststerownikadlg.cpp \
    ustawienia.cpp \
    ustawienia_gen.cpp \
    ustawieniafiltrow.cpp \
    ustawienianapedow.cpp \
    test5zasilanieczujki.cpp

HEADERS += \
    danetestu.h \
    listabadan.h \
    oczekiwanienaurzadzenia.h \
    parametrybadania.h \
    parametrybadania_gen.h \
    parametrybadaniaczujkidlg.h \
    parametrybadaniadlg.h \
    parametrybadaniaogolnedlg.h \
    proceduratestowa.h \
    serialdevice.h \
    test1parametrytestu.h \
    test2potwierdzenie.h \
    test3sprawdzenie.h \
    test4montaz.h \
    test6stabilizacjaczujki.h \
    test7badanie.h \
    teststanowiskadlg.h \
    wybortestu.h \
    zasilacz.h \
    crc8.h \
    debugdialog.h \
    mainwindow.h \
    parametrykalibracyjnedlg.h \
    sterownik.h \
    serialmessage.h \
    sterownikustawieniawidget.h \
    testzasilaczadlg.h \
    teststerownikadlg.h \
    ustawienia.h \
    ustawienia_gen.h \
    ustawieniafiltrow.h \
    ustawienianapedow.h \
    test5zasilanieczujki.h

FORMS += \
    debugdialog.ui \
    listabadan.ui \
    mainwindow.ui \
    oczekiwanienaurzadzenia.ui \
    parametrybadaniaczujkidlg.ui \
    parametrybadaniadlg.ui \
    parametrybadaniaogolnedlg.ui \
    parametrykalibracyjnedlg.ui \
    sterownikustawieniawidget.ui \
    test1parametrytestu.ui \
    test2potwierdzenie.ui \
    test3sprawdzenie.ui \
    test4montaz.ui \
    test6stabilizacjaczujki.ui \
    test7badanie.ui \
    teststanowiskadlg.ui \
    testzasilaczadlg.ui \
    teststerownikadlg.ui \
    ustawieniafiltrow.ui \
    ustawienianapedow.ui \
    wybortestu.ui \
    test5zasilanieczujki.ui

TRANSLATIONS += \
    CzujkiLinioweApp_pl_PL.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc \
    ikony.qrc
