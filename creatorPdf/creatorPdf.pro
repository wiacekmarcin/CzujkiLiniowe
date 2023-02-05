TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG += qt

QT       += core core5compat

SOURCES += \
        main.cpp \
        pdfcreator.cpp \
        task.cpp


win32: LIBS += -L$$PWD/../libHaru-2.2.0-vc/lib/ -llibhpdf

INCLUDEPATH += $$PWD/../libHaru-2.2.0-vc/include
DEPENDPATH += $$PWD/../libHaru-2.2.0-vc/include

HEADERS += \
    pdfcreator.h \
    task.h
