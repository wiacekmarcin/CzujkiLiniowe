QT       += core gui 
QT += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#DEFINES += TESTVAL
#DEFINES += DEFVAL
#DEFINES += NOSERIAL

SOURCES += \
    danetestu.cpp \
    listabadan.cpp \
    oczekiwanienaurzadzenia.cpp \
    oknobadaniakata.cpp \
    oknobadaniamaksymalnegokata.cpp \
    oknobadaniereakcji6db.cpp \
    oknoczekaniabadaniekatowe.cpp \
    oknopotwierdzenieemcnarazenie.cpp \
    oknopotwierdzenienarazenia.cpp \
    oknoresetuzasilaniaczujki.cpp \
    oknotestrozproszoneswiatlo.cpp \
    parametrybadania.cpp \
    parametrybadania_gen.cpp \
    parametrybadaniaczujkidlg.cpp \
    parametrybadaniadlg.cpp \
    parametrybadaniaogolnedlg.cpp \
    powtarzalnoscwyniki.cpp \
    proceduratestowa.cpp \
    sterownik_worker.cpp \
    oknozerowanieurzadzenia.cpp \
    oknoparametrytestu.cpp \
    oknosprawdzeniedanych.cpp \
    oknomontaz.cpp \
    oknostabilizacjaczujki.cpp \
    oknobadaniatlumienia.cpp \
    oknowynikbadaniatlumienia.cpp \
    oknopodsumowanietestu.cpp \
    symulator.cpp \
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
    oknozasilaniaczujki.cpp \
    zasilacz_worker.cpp

HEADERS += \
    danetestu.h \
    listabadan.h \
    oczekiwanienaurzadzenia.h \
    oknobadaniakata.h \
    oknobadaniamaksymalnegokata.h \
    oknobadaniereakcji6db.h \
    oknoczekaniabadaniekatowe.h \
    oknopotwierdzenieemcnarazenie.h \
    oknopotwierdzenienarazenia.h \
    oknoresetuzasilaniaczujki.h \
    oknotestrozproszoneswiatlo.h \
    parametrybadania.h \
    parametrybadania_gen.h \
    parametrybadaniaczujkidlg.h \
    parametrybadaniadlg.h \
    parametrybadaniaogolnedlg.h \
    powtarzalnoscwyniki.h \
    proceduratestowa.h \
    sterownik_worker.h \
    oknozerowanieurzadzenia.h \
    oknoparametrytestu.h \
    oknosprawdzeniedanych.h \
    oknomontaz.h \
    oknostabilizacjaczujki.h \
    oknobadaniatlumienia.h \
    oknowynikbadaniatlumienia.h \
    oknopodsumowanietestu.h \
    symulator.h \
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
    oknozasilaniaczujki.h \
    zasilacz_worker.h

FORMS += \
    debugdialog.ui \
    listabadan.ui \
    mainwindow.ui \
    oczekiwanienaurzadzenia.ui \
    oknobadaniakata.ui \
    oknobadaniamaksymalnegokata.ui \
    oknobadaniereakcji6db.ui \
    oknoczekaniabadaniekatowe.ui \
    oknopotwierdzenieemcnarazenie.ui \
    oknopotwierdzenienarazenia.ui \
    oknoresetuzasilaniaczujki.ui \
    oknotestrozproszoneswiatlo.ui \
    parametrybadaniaczujkidlg.ui \
    parametrybadaniadlg.ui \
    parametrybadaniaogolnedlg.ui \
    parametrykalibracyjnedlg.ui \
    powtarzalnoscwyniki.ui \
    sterownikustawieniawidget.ui \
    oknozerowanieurzadzenia.ui \
    oknoparametrytestu.ui \
    oknosprawdzeniedanych.ui \
    oknomontaz.ui \
    oknostabilizacjaczujki.ui \
    oknobadaniatlumienia.ui \
    oknowynikbadaniatlumienia.ui \
    oknopodsumowanietestu.ui \
    symulator.ui \
    teststanowiskadlg.ui \
    testzasilaczadlg.ui \
    teststerownikadlg.ui \
    ustawieniafiltrow.ui \
    ustawienianapedow.ui \
    wybortestu.ui \
    oknozasilaniaczujki.ui

#TRANSLATIONS += \
#    CzujkiLinioweApp_pl_PL.ts
#CONFIG += lrelease
#CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc \
    ikony.qrc
