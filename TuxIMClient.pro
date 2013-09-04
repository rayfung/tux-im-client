#-------------------------------------------------
#
# Project created by QtCreator 2013-08-30T16:32:34
#
#-------------------------------------------------

QT       += core gui network

ubuntu {
    LIBS        += -lQtMultimediaKit
    INCLUDEPATH += /usr/include/QtMobility
    INCLUDEPATH += /usr/include/QtMultimediaKit
} else {
    QT          += multimedia
}

TARGET = TuxIMClient
TEMPLATE = app


SOURCES += main.cpp\
    netsocket.cpp \
    config.cpp \
    imapi.cpp \
    userinformation.cpp \
    chatwindow.cpp \
    tux.cpp \
    network/server.cpp \
    network/datapool.cpp \
    login.cpp \
    registration.cpp \
    utils.cpp \
    personalcenter.cpp \
    inputbox.cpp \
    devicemanager.cpp

HEADERS  += \
    netsocket.h \
    config.h \
    imapi.h \
    datatype.h \
    userinformation.h \
    chatwindow.h \
    tux.h \
    network/server.h \
    network/datapool.h \
    login.h \
    registration.h \
    utils.h \
    personalcenter.h \
    inputbox.h \
    devicemanager.h

FORMS    += \
    userinformation.ui \
    chatwindow.ui \
    tux.ui \
    login.ui \
    registration.ui \
    personalcenter.ui

RESOURCES += \
    images/images.qrc
