##### QT
# Name of the application
TEMPLATE = app

# including base file
include(../../qml-ar.pri)

TARGET = qml-ar-cpp

# Qt configuration for Quick applications
QT += multimedia opengl multimediawidgets 3dinput 3drender
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

##### SOURCES/HEADERS (this project)
SOURCES += main.cpp
RESOURCES += assets.qrc

# adding assets/ folder
# See https://stackoverflow.com/questions/20573838/qt-5-2-including-external-file-into-an-android-package
# https://forum.qt.io/topic/69946/qt-android-assets/4
# https://falsinsoft.blogspot.ch/2017/01/qt-creator-include-additional-files.html
assets.path = /assets
assets.files += $$PWD/../00_chest/assets/*
assets.depends += FORCE
INSTALLS += assets
