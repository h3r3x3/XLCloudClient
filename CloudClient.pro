#-------------------------------------------------
#
# Project created by QtCreator 2013-01-27T19:53:03
#
#-------------------------------------------------

QT       += core gui webkit sql network phonon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CloudClient
TEMPLATE = app

LIBS += -lqjson

SOURCES += main.cpp\
        mainwindow.cpp \
    thundercore.cpp \
    util.cpp \
    thunderpanel.cpp \
    videopanel.cpp \
    qmpwidget.cpp \
    mplayer.cpp \
    logview.cpp \
    preferencesdlg.cpp \
    browser.cpp \
    addcloudtask.cpp \
    transf0r.cpp \
    downloader.cpp \
    downloaderchildwidget.cpp \
    saycapcha.cpp \
    fileselectorline.cpp \
    mediaplayer.cpp \
    osd.cpp

HEADERS  += mainwindow.h \
    thundercore.h \
    CloudObject.h \
    util.h \
    thunderpanel.h \
    videopanel.h \
    qmpwidget.h \
    mplayer.h \
    logview.h \
    preferencesdlg.h \
    browser.h \
    addcloudtask.h \
    transf0r.h \
    downloaderchildwidget.h \
    downloader.h \
    saycapcha.h \
    fileselectorline.h \
    mediaplayer.h \
    osd.h

FORMS    += mainwindow.ui \
    thunderpanel.ui \
    videopanel.ui \
    logview.ui \
    preferencesdlg.ui \
    browser.ui \
    addcloudtask.ui \
    transf0r.ui \
    downloaderchildwidget.ui \
    saycapcha.ui \
    fileselectorline.ui

RESOURCES += \
    resources.qrc
