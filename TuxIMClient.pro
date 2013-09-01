#-------------------------------------------------
#
# Project created by QtCreator 2013-08-30T16:32:34
#
#-------------------------------------------------

QT       += core gui network

TARGET = TuxIMClient
TEMPLATE = app


SOURCES += main.cpp\
    netsocket.cpp \
    config.cpp \
    imapi.cpp \
    userinformation.cpp \
    chatwindow.cpp \
    tux.cpp

HEADERS  += \
    netsocket.h \
    config.h \
    imapi.h \
    datatype.h \
    userinformation.h \
    chatwindow.h \
    tux.h

FORMS    += \
    userinformation.ui \
    chatwindow.ui \
    tux.ui

RESOURCES += \
    images/images.qrc
