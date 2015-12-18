#-------------------------------------------------
#
# Project created by QtCreator 2015-04-03T17:42:57
#
#-------------------------------------------------

TARGET = plantumlqeditor
TEMPLATE = app

win32:RC_ICONS += resources/icon.ico

#-------------------------------------------------

#unix:QMAKE_CC = gcc-4.8
#unix:QMAKE_CXX = g++-4.8

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic

#-------------------------------------------------

#win32 {
#    release: DESTDIR = build/release/win
#    debug:   DESTDIR = build/debug/win
#}
#unix {
#    release: DESTDIR = build/release/unix
#    debug:   DESTDIR = build/debug/unix
#}

QT += core gui svg widgets webkitwidgets

CONFIG += static

#OBJECTS_DIR = $$DESTDIR/.obj
#MOC_DIR     = $$DESTDIR/.moc
#RCC_DIR     = $$DESTDIR/.qrc
#UI_DIR      = $$DESTDIR/.ui

#CONFIG += debug_and_release
#CONFIG += debug
#CONFIG += release

QMAKE_CXXFLAGS_DEBUG   += -Og

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -Ofast

unix:QMAKE_DISTCLEAN  += -rf build release debug
win32:QMAKE_DISTCLEAN += /s /f /q build && rd /s /q build
win32:QMAKE_DISTCLEAN += /s /f /q release && rd /s /q release
win32:QMAKE_DISTCLEAN += /s /f /q debug && rd /s /q debug

#-------------------------------------------------

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    recentdocuments.cpp \
    preferencesdialog.cpp \
    assistantxmlreader.cpp \
    previewwidget.cpp \
    previewwidgetsvg.cpp \
    previewwidgetweb.cpp \
    textedit.cpp \
    highlighter.cpp \
    filecache.cpp \
    utils.cpp \
    webview.cpp

HEADERS += \
    mainwindow.h \
    recentdocuments.h \
    settingsconstants.h \
    preferencesdialog.h \
    assistantxmlreader.h \
    previewwidget.h \
    previewwidgetsvg.h \
    previewwidgetweb.h \
    textedit.h \
    filecache.h \
    utils.h \
    webview.h

FORMS += \
    preferencesdialog.ui

OTHER_FILES += \
    assistant.xml \
    icons/* \
    resources/*.svg \
    resources/*.png \
    CHANGELOG \
    AUTHORS \
    COPYING \
    README \
    INSTALL \
    TODO \
    plantumlqeditor-mime.xml \
    plantumlqeditor.desktop \
    README-mime \
    docs/*

RESOURCES += \
    plantumlqeditor.qrc

#resources.files = resources/*
#resources.path  = $$[QT_INSTALL_DATA]/resources
#INSTALLS += resources

DISTFILES += \
    assistant.xml \
    assistant-icons/* \
    resources/*.svg \
    resources/*.png \
    CHANGELOG \
    AUTHORS \
    COPYING \
    README.md \
    INSTALL \
    TODO

unix:DISTFILES += \
    plantumlqeditor-mime.xml \
    plantumlqeditor.desktop \
    README-mime
