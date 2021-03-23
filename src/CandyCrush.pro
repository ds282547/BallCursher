#-------------------------------------------------
#
# Project created by QtCreator 2015-06-27T19:52:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CandyCrush
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    game.cpp \
    stone.cpp \
    pixmaps.cpp \
    stone_base.cpp \
    bar.cpp \
    dropaniitem.cpp \
    stone_v.cpp \
    stone_h.cpp \
    stone_boom.cpp \
    stone_star.cpp

HEADERS  += mainwindow.h \
    game.h \
    stone.h \
    pixmaps.h \
    gamedata.h \
    stone_base.h \
    bar.h \
    dropaniitem.h \
    stone_v.h \
    stone_h.h \
    stone_boom.h \
    stone_star.h

FORMS    += mainwindow.ui \
    game.ui

RESOURCES += \
    pic.qrc
