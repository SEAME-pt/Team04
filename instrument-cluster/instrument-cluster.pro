QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# Zero MQ
INCLUDEPATH += /usr/include
LIBS += -L/usr/lib/x86_64-linux-gnu -lzmq

SOURCES += \
    main.cpp \
    basicdisplay.cpp \
    rpm.cpp \
    speed.cpp \
    zmqpublisher.cpp \
    zmqsubscriber.cpp

HEADERS += \
    basicdisplay.h \
    config.h \
    rpm.h \
    speed.h \
    zmqpublisher.h \
    zmqsubscriber.h

FORMS += \
    basicdisplay.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

