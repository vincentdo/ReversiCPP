#-------------------------------------------------
#
# Project created by QtCreator 2013-12-01T20:47:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReversiCPP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    board.cpp \
    instructions.cpp

HEADERS  += mainwindow.h \
    board.h \
    instructions.h

FORMS    += mainwindow.ui \
    instructions.ui
