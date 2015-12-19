#-------------------------------------------------
#
# Project created by QtCreator 2015-04-03T17:42:57
#
#-------------------------------------------------

TARGET = plantumlqeditor
TEMPLATE = app

win32:RC_ICONS += resources/icon.ico

#-------------------------------------------------

# In QtCreator, switch "Shadow build" off!
DESTDIR     = build
OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR     = $$DESTDIR/moc
RCC_DIR     = $$DESTDIR/rcc
UI_DIR      = $$DESTDIR/ui

#-------------------------------------------------

QT += core gui svg widgets webkitwidgets

CONFIG += static

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic

debug {
    CONFIG += debug

    QMAKE_CXXFLAGS_DEBUG   += -Og
}
release {
    CONFIG  += warn_off release

    DEFINES += QT_NO_DEBUG_OUTPUT
    DEFINES += QT_NO_DEBUG

    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE += -Ofast
}

unix:QMAKE_DISTCLEAN  += -rf $$DESTDIR
win32:QMAKE_DISTCLEAN += /s /f /q $$DESTDIR && rd /s /q $$DESTDIR

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
    README.md \
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
