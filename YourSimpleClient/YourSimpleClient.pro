#-------------------------------------------------
#
# Project created by QtCreator 2016-07-19T07:46:22
#
#-------------------------------------------------

QT += core \
    network
QT -= gui

CONFIG += c++11

TARGET = YourSimpleClient
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
SOURCES += main.cpp \
    socketclientchat.cpp \
    datablock.cpp \
    socketclientcore.cpp

HEADERS += \
    socketclientchat.h \
    datablock.h \
    socketclientcore.h
