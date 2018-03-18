QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        MainWindow.cpp \
        LoginForm.cpp \
        Utilities.cpp \
    Protocol.cpp

HEADERS += \
        MainWindow.h \
        LoginForm.h \
        Utilities.h \
    Types.h \
    Protocol.h

FORMS += \
        MainWindow.ui \
        LoginForm.ui
