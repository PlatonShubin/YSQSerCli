QT += core \
    network
QT -= gui

CONFIG += c++11

TARGET = YourSimpleServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../YourSimpleClient/socketclientcore.cpp \
    ../YourSimpleClient/datablock.cpp \
    socketservercore.cpp

HEADERS += \
    ../YourSimpleClient/socketclientcore.h \
    ../YourSimpleClient/datablock.h \
    socketservercore.h
