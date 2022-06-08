QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

INCLUDEPATH += $$PWD/../../thirdparty/poppler/include
LIBS += -L$$PWD/../../thirdparty/poppler/lib -lpoppler-qt5
