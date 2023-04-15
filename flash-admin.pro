#-------------------------------------------------
#
# Project created by QtCreator 2017-02-09T13:21:12
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flash-admin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogperson.cpp \
    db.cpp \
    logindialog.cpp \
    dialogdivision.cpp \
    dialogsvt.cpp \
    dialogmni.cpp \
    deployer.cpp \
    colortablemodel.cpp

HEADERS  += mainwindow.h \
    dialogperson.h \
    db.h \
    logindialog.h \
    dialogdivision.h \
    dialogsvt.h \
    dialogmni.h \
    deployer.h \
    colortablemodel.h

FORMS    += mainwindow.ui \
    dialogperson.ui \
    logindialog.ui \
    dialogdivision.ui \
    dialogsvt.ui \
    dialogmni.ui

RESOURCES += \
    flash-admin.qrc
