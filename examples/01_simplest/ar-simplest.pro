##### QT
# Name of the application
TEMPLATE = app

TARGET = ar_simplest

# Qt configuration for Quick applications
QT += quick multimedia opengl multimediawidgets 3dinput 3drender
CONFIG += c++11

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

##### SOURCES/HEADERS (this project)
SOURCES += main.cpp
RESOURCES += qml.qrc

# adding assets/ folder
# See https://stackoverflow.com/questions/20573838/qt-5-2-including-external-file-into-an-android-package
# https://forum.qt.io/topic/69946/qt-android-assets/4
# https://falsinsoft.blogspot.ch/2017/01/qt-creator-include-additional-files.html
assets.path = /assets
assets.files += $$PWD/assets/*
assets.depends += FORCE
INSTALLS += assets
