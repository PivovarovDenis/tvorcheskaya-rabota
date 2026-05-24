QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = IDEF0Generator
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    claudeapiclient.cpp \
    idef0parser.cpp \
    drawioexporter.cpp

HEADERS += \
    mainwindow.h \
    claudeapiclient.h \
    idef0parser.h \
    drawioexporter.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
