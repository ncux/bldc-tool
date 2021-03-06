
DEFINES += QML # for test

QT += core gui bluetooth serialport network

android | ios : DEFINES += QML
ios{
    QT -= serialport
    DEFINES += NO_SERIAL_PORT
}
android{
    QT -= serialport
    DEFINES += NO_SERIAL_PORT
}

CONFIG += c++11
contains(DEFINES,QML){
    include(QML.pri)
} else {
    include(Widgets.pri)
}

HEADERS += \
    datatypes.h \
    mcvalues.h \
    bleinterface.h \
    bldcinterface.h \
    lib-qt-qml-tricks/src/qqmlhelpers.h \
    digitalfiltering.h \
    packetinterface.h \
    utility.h \
    serialization.h \
    mcconfiguration.h \
    appconfiguration.h

SOURCES += \
    main.cpp \
    packetinterface.cpp \
    bldcinterface.cpp \
    mcvalues.cpp \
    bleinterface.cpp \
    lib-qt-qml-tricks/src/qqmlhelpers.cpp \
    digitalfiltering.cpp \
    utility.cpp \
    serialization.cpp \
    mcconfiguration.cpp \
    appconfiguration.cpp
