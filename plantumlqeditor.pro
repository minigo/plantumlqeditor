#-------------------------------------------------
#
# Project created by QtCreator 2015-04-03T17:42:57
#
#-------------------------------------------------

TARGET = plantumlqeditor
TEMPLATE = app

QT += core gui svg widgets

lessThan(QT_VERSION, 5.6) {
    QT += webkitwidgets
    DEFINES += USE_WEBKIT
} else {
    QT += webenginewidgets
}

win32:RC_ICONS += resources/icon.ico
macx:ICON = resources/icon.icns

#-------------------------------------------------

# In QtCreator, switch "Shadow build" off!
CONFIG += debug_and_release

CONFIG += static

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic

CONFIG(debug, debug|release){
    DESTDIR     = ./debug
    OBJECTS_DIR = debug/obj
    MOC_DIR     = debug/moc
    RCC_DIR     = debug/rcc
    UI_DIR      = debug/ui

    CONFIG += debug

    QMAKE_CXXFLAGS_DEBUG  += -Og

    unix:QMAKE_DISTCLEAN  += -rf $$DESTDIR
    win32:QMAKE_DISTCLEAN += /s /f /q $$DESTDIR && rd /s /q $$DESTDIR
}

CONFIG(release, debug|release){
    DESTDIR     = ./release
    OBJECTS_DIR = release/obj
    MOC_DIR     = release/moc
    RCC_DIR     = release/rcc
    UI_DIR      = release/ui

    CONFIG  += warn_off release

    DEFINES += QT_NO_DEBUG_OUTPUT
    DEFINES += QT_NO_DEBUG

    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE += -O3

    unix:QMAKE_DISTCLEAN  += -rf $$DESTDIR
    win32:QMAKE_DISTCLEAN += /s /f /q $$DESTDIR && rd /s /q $$DESTDIR
}

#-------------------------------------------------

SOURCES += \
    src/main.cpp\
    src/mainwindow.cpp \
    src/recentdocuments.cpp \
    src/preferencesdialog.cpp \
    src/assistantxmlreader.cpp \
    src/previewwidget.cpp \
    src/previewwidgetsvg.cpp \
    src/previewwidgetweb.cpp \
    src/textedit.cpp \
    src/highlighter.cpp \
    src/filecache.cpp \
    src/utils.cpp \
    src/webview.cpp

HEADERS += \
    src/mainwindow.h \
    src/recentdocuments.h \
    src/settingsconstants.h \
    src/preferencesdialog.h \
    src/assistantxmlreader.h \
    src/previewwidget.h \
    src/previewwidgetsvg.h \
    src/previewwidgetweb.h \
    src/textedit.h \
    src/filecache.h \
    src/utils.h \
    src/webview.h \
    src/language.h

FORMS += \
    src/preferencesdialog.ui

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
