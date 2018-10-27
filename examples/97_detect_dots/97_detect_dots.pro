# Qt configuration for Quick applications
QT += qml quick multimedia opengl multimediawidgets 3dinput 3drender sensors concurrent
CONFIG += qt c++11 nostrip console


CONFIG += no_moc_predefs
CONFIG += c++11 console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# OpenCV for Linux (not Android) library
linux:!android {
    # using pkg-config
    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

SOURCES += \
        main.cpp \
    $$PWD/../../src/markers/uchiyamarkerdetector.cpp \
    $$PWD/../../src/utils/configjson.cpp \
    $$PWD/../../src/utils/pipelinecontainer.cpp \
    $$PWD/../../src/utils/pipelinecontainerinfo.cpp \
    $$PWD/../../src/utils/timelogger.cpp \
    $$PWD/../../src/ar/markermvpprovider.cpp \
    $$PWD/../../src/imagebackend/opencvbackend.cpp \
    $$PWD/../../src/markers/marker.cpp \
    $$PWD/../../src/markers/markerdetector.cpp \
    $$PWD/../../src/markers/markerstorage.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += ../../src/utils ../../src ../../src/camera ../../src/markers ../../src/imagebackend ../../src/ar

HEADERS += \
    $$PWD/../../src/markers/uchiyamarkerdetector.h \
    $$PWD/../../src/utils/configjson.h \
    $$PWD/../../src/utils/pipelinecontainer.h \
    $$PWD/../../src/utils/pipelinecontainerinfo.h \
    $$PWD/../../src/utils/timelogger.h \
    $$PWD/../../src/config.h \
    $$PWD/../../src/ar/markermvpprovider.h \
    $$PWD/../../src/imagebackend/opencvbackend.h \
    $$PWD/../../src/markers/marker.h \
    $$PWD/../../src/markers/markerdetector.h \
    $$PWD/../../src/markers/markerstorage.h

# UchiyaMarkers library
SOURCES += $$PWD/../../uchiya/blob.cpp \
    $$PWD/../../uchiya/bloblist.cpp \
    $$PWD/../../uchiya/hashtable.cpp \
    $$PWD/../../uchiya/llah.cpp \
    $$PWD/../../uchiya/llahparam.cpp \
    $$PWD/../../uchiya/paper.cpp \
    $$PWD/../../uchiya/paperlist.cpp \
    $$PWD/../../uchiya/window.cpp \
    $$PWD/../../uchiya/mylib/combination.cpp \
    $$PWD/../../uchiya/mylib/mycam.cpp \
    $$PWD/../../uchiya/mylib/myimage.cpp \
    $$PWD/../../uchiya/mylib/mylabel.cpp \
    $$PWD/../../uchiya/mylib/mymat.cpp \
    $$PWD/../../uchiya/mylib/mytimer.cpp

HEADERS += $$PWD/../../uchiya/blob.h \
    $$PWD/../../uchiya/bloblist.h \
    $$PWD/../../uchiya/hashtable.h \
    $$PWD/../../uchiya/llah.h \
    $$PWD/../../uchiya/llahparam.h \
    $$PWD/../../uchiya/paper.h \
    $$PWD/../../uchiya/paperlist.h \
    $$PWD/../../uchiya/window.h \
    $$PWD/../../uchiya/mylib/combination.h \
    $$PWD/../../uchiya/mylib/mycam.h \
    $$PWD/../../uchiya/mylib/myimage.h \
    $$PWD/../../uchiya/mylib/mylabel.h \
    $$PWD/../../uchiya/mylib/mymat.h \
    $$PWD/../../uchiya/mylib/mytimer.h \
    $$PWD/../../uchiya/mylib/opencvpath.h

INCLUDEPATH += $$PWD/../../uchiya $$PWD/../..

DEPENDSPATH += $$PWD/../../

##### LIBRARIES
# QtOpenCV library
QT_OPENCV_PATH = $$PWD/../../QtOpenCV
INCLUDEPATH += $${QT_OPENCV_PATH}/
DEPENDPATH += $${QT_OPENCV_PATH}/
HEADERS += $${QT_OPENCV_PATH}/cvmatandqimage.h
SOURCES += $${QT_OPENCV_PATH}/cvmatandqimage.cpp
