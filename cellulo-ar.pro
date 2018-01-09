##### QT
# Name of the application

# Qt configuration for Quick applications
QT += qml quick multimedia opengl multimediawidgets 3dinput 3drender
CONFIG += qt plugin c++11 nostrip

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
INCLUDEPATH += src/markers src/utils src/imagebackend src/ar src/camera src

SOURCES += src/imagebackend/opencvbackend.cpp \
    src/imagebackend/qtbackend.cpp \
    src/camera/cameraframegrabber.cpp \
    src/camera/voidviewfinder.cpp \
    src/utils/qvideoframehelpers.cpp \
    src/markers/marker.cpp \
    src/markers/markerstorage.cpp \
    src/camera/portablebackendfactory.cpp \
    src/markers/markerdetector.cpp \
    src/markers/uchiyamarkerdetector.cpp \
    src/imagebackend/markerbackend.cpp \
    src/imagebackend/imagebackend.cpp \
    src/camera/calibratedcamera.cpp \
    src/camera/calibratedcamerafilestorage.cpp \
    src/utils/configjson.cpp \
    src/ar/mvpprovider.cpp \
    src/ar/markermvpprovider.cpp \
    src/utils/mymatconverter.cpp \
    src/ar/perspectivecamera.cpp \
    src/ar/pose.cpp \
    src/ar/posecamerapnp.cpp \
    src/ar/worldimage.cpp \
    src/celluloar.cpp \
    src/celluloarqmlplugin.cpp \
    src/utils/timelogger.cpp

HEADERS += \
    src/imagebackend/opencvbackend.h \
    src/imagebackend/qtbackend.h \
    src/camera/cameraframegrabber.h \
    src/camera/voidviewfinder.h \
    src/utils/qvideoframehelpers.h \
    src/markers/marker.h \
    src/markers/markerstorage.h \
    src/camera/portablebackendfactory.h \
    src/markers/markerdetector.h \
    src/markers/uchiyamarkerdetector.h \
    src/imagebackend/markerbackend.h \
    src/imagebackend/imagebackend.h \
    src/camera/calibratedcamera.h \
    src/camera/calibratedcamerafilestorage.h \
    src/utils/configjson.h \
    src/ar/mvpprovider.h \
    src/ar/markermvpprovider.h \
    src/utils/mymatconverter.h \
    src/ar/perspectivecamera.h \
    src/ar/pose.h \
    src/ar/posecamerapnp.h \
    src/ar/worldimage.h \
    src/celluloar.h \
    src/celluloarqmlplugin.h \
    src/config.h \
    src/utils/timelogger.h

QML_SOURCES += \
    src/CelluloARComponent.qml \
    src/CelluloARInitTypes.qml

DISTFILES = $$QML_SOURCES

##### LIBRARIES
# QtOpenCV library
QT_OPENCV_PATH = $$PWD/QtOpenCV
INCLUDEPATH += $${QT_OPENCV_PATH}/
DEPENDPATH += $${QT_OPENCV_PATH}/
HEADERS += $${QT_OPENCV_PATH}/cvmatandqimage.h
SOURCES += $${QT_OPENCV_PATH}/cvmatandqimage.cpp

# OpenCV for Linux (not Android) library
linux:!android {
    # using pkg-config
    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

# OpenCV for Android library
android {
    # full path to OpenCV Android SDK
    OPENCV_PATH = $$PWD/opencv-android

    INCLUDEPATH += $${OPENCV_PATH}/sdk/native/jni/include/

    LIBS += -L$${OPENCV_PATH}/sdk/native/libs/armeabi-v7a \
            -Wl,--start-group\
            -lopencv_calib3d\
            -lopencv_core\
            -lopencv_dnn\
            -lopencv_features2d\
            -lopencv_flann\
            -lopencv_highgui\
            -lopencv_imgcodecs\
            -lopencv_imgproc\
            -lopencv_ml\
            -lopencv_objdetect\
            -lopencv_photo\
            -lopencv_shape\
            -lopencv_stitching\
            -lopencv_superres\
            -lopencv_video\
            -lopencv_videoio\
            -lopencv_videostab\
            -Wl,--end-group\


    LIBS += -L$${OPENCV_PATH}/sdk/native/3rdparty/libs/armeabi-v7a \
            -Wl,--start-group\
            -lcpufeatures\
            -lIlmImf\
            -llibjasper\
            -llibjpeg\
            -llibpng\
            -llibprotobuf\
            -llibtiff\
            -llibwebp\
            -ltbb\
            -ltegra_hal\
            -Wl,--end-group\

# neon instructions (for yuv2rgb)
#    QMAKE_CXXFLAGS += -mfloat-abi=softfp -mfpu=neon -flax-vector-conversions
}

# YUV2RGB library
SOURCES += yuv2rgb/yuv2rgb.cpp
HEADERS += yuv2rgb/yuv2rgb.h

# UchiyaMarkers library
SOURCES += uchiya/blob.cpp \
    uchiya/bloblist.cpp \
    uchiya/hashtable.cpp \
    uchiya/llah.cpp \
    uchiya/llahparam.cpp \
    uchiya/paper.cpp \
    uchiya/paperlist.cpp \
    uchiya/window.cpp \
    uchiya/mylib/combination.cpp \
    uchiya/mylib/mycam.cpp \
    uchiya/mylib/myimage.cpp \
    uchiya/mylib/mylabel.cpp \
    uchiya/mylib/mymat.cpp \
    uchiya/mylib/mytimer.cpp

HEADERS += uchiya/blob.h \
    uchiya/bloblist.h \
    uchiya/hashtable.h \
    uchiya/llah.h \
    uchiya/llahparam.h \
    uchiya/paper.h \
    uchiya/paperlist.h \
    uchiya/window.h \
    uchiya/mylib/combination.h \
    uchiya/mylib/mycam.h \
    uchiya/mylib/myimage.h \
    uchiya/mylib/mylabel.h \
    uchiya/mylib/mymat.h \
    uchiya/mylib/mytimer.h \
    uchiya/mylib/opencvpath.h

# Installation
TEMPLATE = lib
TARGET = celluloarplugin
CONFIG += plugin
CONFIG -= android_install

TARGET = $$qtLibraryTarget($$TARGET)
uri = CelluloAR

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
