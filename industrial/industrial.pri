QT       +=  sql

CONFIG += c++11
QMAKE_CFLAGS += -std=c99
LIBS += -lpthread libwsock32 libws2_32

HEADERS += \
    $$PWD/opcuaclientmanager.h \
    $$PWD/open62541.h \
    $$PWD/realtimevariablemanager.h \
    $$PWD/variableconfigtool.h \
    $$PWD/variabledatabase.h \
    $$PWD/variablesystem.h

SOURCES += \
    $$PWD/opcuaclientmanager.cpp \
    $$PWD/open62541.c \
    $$PWD/realtimevariablemanager.cpp \
    $$PWD/variableconfigtool.cpp \
    $$PWD/variabledatabase.cpp \
    $$PWD/variablesystem.cpp
