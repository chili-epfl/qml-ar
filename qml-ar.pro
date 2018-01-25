##### QT

# including base file
include(qml-ar.pri)

# creating a plugin
CONFIG += plugin

# Installation
TEMPLATE = lib
TARGET = qmlarplugin
CONFIG += plugin
CONFIG -= android_install

TARGET = $$qtLibraryTarget($$TARGET)
uri = AR

#File installation
qmldir.files = qmldir
qml.files = $$QML_SOURCES
OTHER_FILES += qmldir.files qml.files

unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    qml.path = $$installPath
    target.path = $$installPath
    INSTALLS += qmldir target qml
}
