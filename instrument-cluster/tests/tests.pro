include(gtest_dependency.pri)

QT += core gui widgets

TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt
CONFIG += moc


SOURCES += \
        main.cpp \
        tst_basicdisplay.cpp \
        tst_rpm.cpp \
        tst_speed.cpp \
        tst_zmqsubscriber.cpp

INCLUDEPATH += $$PWD/..

SOURCES += ../speed.cpp \
           ../rpm.cpp \
           ../zmqsubscriber.cpp
HEADERS += ../speed.h \
           ../rpm.h \
           ./zmqsubscriber.h

# Google Test
INCLUDEPATH += /usr/include/gtest
LIBS += -L/usr/lib -lQt6Widgets -lQt6Gui -lQt6Core -lgtest -lgtest_main -lgmock -lgmock_main -lpthread
