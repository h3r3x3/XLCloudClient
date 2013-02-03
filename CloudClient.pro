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

de.files = CloudClient.desktop
de.path = /usr/share/applications

bi.files = CloudClient
bi.path = /usr/local/bin/

icon.files = resources/images/thunder-256.png
icon.path = /usr/share/icons/hicolor/256x256/apps/

INSTALLS += de bi icon

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
    osd.cpp \
    urllineedit.cpp \
    searchlineedit.cpp

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
    osd.h \
    urllineedit.h \
    searchlineedit.h

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
