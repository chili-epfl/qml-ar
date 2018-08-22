##### QT
# Name of the application

# Qt configuration for Quick applications
QT += qml quick multimedia opengl multimediawidgets 3dinput 3drender sensors concurrent
CONFIG += qt c++11 nostrip

# turning on optimization
# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -Os

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O3

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEPENDSPATH += $$PWD

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

##### SOURCES/HEADERS (this project)
INCLUDEPATH += $$PWD/src/markers $$PWD/src/utils $$PWD/src/imagebackend $$PWD/src/ar $$PWD/src/camera $$PWD/src $$PWD

SOURCES += $$PWD/src/imagebackend/opencvbackend.cpp \
    $$PWD/src/imagebackend/qtbackend.cpp \
    $$PWD/src/camera/cameraframegrabber.cpp \
    $$PWD/src/camera/voidviewfinder.cpp \
    $$PWD/src/utils/qvideoframehelpers.cpp \
    $$PWD/src/markers/marker.cpp \
    $$PWD/src/markers/markerstorage.cpp \
    $$PWD/src/camera/portablebackendfactory.cpp \
    $$PWD/src/markers/markerdetector.cpp \
    $$PWD/src/markers/uchiyamarkerdetector.cpp \
    $$PWD/src/imagebackend/markerbackend.cpp \
    $$PWD/src/imagebackend/imagebackend.cpp \
    $$PWD/src/camera/calibratedcamera.cpp \
    $$PWD/src/camera/calibratedcamerafilestorage.cpp \
    $$PWD/src/utils/configjson.cpp \
    $$PWD/src/ar/mvpprovider.cpp \
    $$PWD/src/ar/markermvpprovider.cpp \
    $$PWD/src/utils/mymatconverter.cpp \
    $$PWD/src/ar/perspectivecamera.cpp \
    $$PWD/src/ar/pose.cpp \
    $$PWD/src/ar/posecamerapnp.cpp \
    $$PWD/src/ar/worldimage.cpp \
    $$PWD/src/qmlar.cpp \
    $$PWD/src/qmlarplugin.cpp \
    $$PWD/src/utils/timelogger.cpp \
    $$PWD/src/ar/imumvpdecorator.cpp \
    $$PWD/src/ar/posepredictor.cpp \
    $$PWD/src/ar/linearposepredictor.cpp \
    $$PWD/src/markers/blobdetector.cpp \
    $$PWD/src/imagebackend/qtcamera2qml.cpp \
    $$PWD/src/imagebackend/androidfilter.cpp \
    $$PWD/src/imagebackend/imageproviderasync.cpp \
    $$PWD/src/imagebackend/imagescaler.cpp \
    $$PWD/src/markers/hsvintervaldetector.cpp \
    $$PWD/src/markers/hsvmeancolor.cpp \
    $$PWD/src/markers/huethreshold.cpp \
    $$PWD/src/markers/meanstdcalculator.cpp \
    $$PWD/src/imagebackend/randomimagebackend.cpp \
    $$PWD/src/threadedqmlar.cpp \
    $$PWD/src/utils/fpscalculator.cpp \
    $$PWD/src/ar/posefilter.cpp \
    $$PWD/src/utils/pipelinecontainer.cpp \
    $$PWD/src/ar/blackenrest.cpp \
    $$PWD/src/utils/pipelinecontainerinfo.cpp \
    $$PWD/src/utils/latencycalculator.cpp \
    $$PWD/src/imagebackend/delayfilter.cpp \
    $$PWD/src/imagebackend/framesdelaycalculator.cpp \
    $$PWD/src/utils/qlinkedlistat.cpp

HEADERS += \
    $$PWD/src/imagebackend/opencvbackend.h \
    $$PWD/src/imagebackend/qtbackend.h \
    $$PWD/src/camera/cameraframegrabber.h \
    $$PWD/src/camera/voidviewfinder.h \
    $$PWD/src/utils/qvideoframehelpers.h \
    $$PWD/src/markers/marker.h \
    $$PWD/src/markers/markerstorage.h \
    $$PWD/src/camera/portablebackendfactory.h \
    $$PWD/src/markers/markerdetector.h \
    $$PWD/src/markers/uchiyamarkerdetector.h \
    $$PWD/src/imagebackend/markerbackend.h \
    $$PWD/src/imagebackend/imagebackend.h \
    $$PWD/src/camera/calibratedcamera.h \
    $$PWD/src/camera/calibratedcamerafilestorage.h \
    $$PWD/src/utils/configjson.h \
    $$PWD/src/ar/mvpprovider.h \
    $$PWD/src/ar/markermvpprovider.h \
    $$PWD/src/utils/mymatconverter.h \
    $$PWD/src/ar/perspectivecamera.h \
    $$PWD/src/ar/pose.h \
    $$PWD/src/ar/posecamerapnp.h \
    $$PWD/src/ar/worldimage.h \
    $$PWD/src/qmlar.h \
    $$PWD/src/qmlarplugin.h \
    $$PWD/src/config.h \
    $$PWD/src/utils/timelogger.h \
    $$PWD/src/ar/imumvpdecorator.h \
    $$PWD/src/ar/posepredictor.h \
    $$PWD/src/ar/linearposepredictor.h \
    $$PWD/src/markers/blobdetector.h \
    $$PWD/src/imagebackend/qtcamera2qml.h \
    $$PWD/src/imagebackend/androidfilter.h \
    $$PWD/src/imagebackend/imageproviderasync.h \
    $$PWD/src/imagebackend/imagescaler.h \
    $$PWD/src/markers/hsvintervaldetector.h \
    $$PWD/src/markers/hsvmeancolor.h \
    $$PWD/src/markers/huethreshold.h \
    $$PWD/src/markers/meanstdcalculator.h \
    $$PWD/src/imagebackend/randomimagebackend.h \
    $$PWD/src/threadedqmlar.h \
    $$PWD/src/utils/fpscalculator.h \
    $$PWD/src/ar/posefilter.h \
    $$PWD/src/utils/pipelinecontainer.h \
    $$PWD/src/ar/blackenrest.h \
    $$PWD/src/utils/pipelinecontainerinfo.h \
    $$PWD/src/utils/latencycalculator.h \
    $$PWD/src/imagebackend/delayfilter.h \
    $$PWD/src/imagebackend/framesdelaycalculator.h \
    $$PWD/src/utils/qlinkedlistat.h

QML_SOURCES += \
    $$PWD/src/ARComponent.qml \
    $$PWD/src/AR3DScene.qml \
    $$PWD/src/ARViewfinder.qml \
    $$PWD/src/ARBlobs.qml \
    $$PWD/src/ARMarkers.qml \
    $$PWD/src/ARMenu.qml \

DISTFILES = $$QML_SOURCES

RESOURCES += \
    $$PWD/ch_epfl_chili_qmlar.qrc

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

    # enabling HardwareBuffer on Android API 26 and higher
    ANDROID_API=$$(ANDROID_NDK_PLATFORM)

    # adding libraries for HardwareBuffer
    greaterThan(ANDROID_API, "android-25") {
        LIBS += -lEGL -lnativewindow -lGLESv3
        HEADERS += $$PWD/src/imagebackend/nv21videofilter.h \
                   $$PWD/src/imagebackend/nv21videofilterrunnable.h
        SOURCES += $$PWD/src/imagebackend/nv21videofilter.cpp \
                   $$PWD/src/imagebackend/nv21videofilterrunnable.cpp
        message("Using HardwareBuffer because Android API is >= 26");
        message("Run $ export ANDROID_NDK_PLATFORM=android-25 (or less) before qmake to disable");
    }

    # not adding libraries and telling how to enable this just in case
    else {
        LIBS += -lc -lEGL
        HEADERS += $$PWD/src/imagebackend/nv21videofilter.h \
                   $$PWD/src/imagebackend/nv21videofilterrunnable.h
        SOURCES += $$PWD/src/imagebackend/nv21videofilter.cpp \
                   $$PWD/src/imagebackend/nv21videofilterrunnable.cpp
        message("Not using HardwareBuffer because Android API is < 26");
        message("Run $ export ANDROID_NDK_PLATFORM=android-26 (or more) before qmake to enable");
        HEADERS += $$PWD/GraphicBuffer/GraphicBuffer.h $$PWD/GraphicBuffer/DynamicLibrary.h
        SOURCES += $$PWD/GraphicBuffer/GraphicBuffer.cpp $$PWD/GraphicBuffer/DynamicLibrary.cpp
    }

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
    QMAKE_CXXFLAGS += -mfloat-abi=softfp -mfpu=neon -flax-vector-conversions -ftree-vectorize -ffast-math -DANDROID_NDK
    DEFINES += ARM_NEON_ENABLE
}

# YUV2RGB library
SOURCES += $$PWD/yuv2rgb/yuv2rgb.cpp
HEADERS += $$PWD/yuv2rgb/yuv2rgb.h
INCLUDEPATH += $$PWD/yuv2rgb

# UchiyaMarkers library
SOURCES += $$PWD/uchiya/blob.cpp \
    $$PWD/uchiya/bloblist.cpp \
    $$PWD/uchiya/hashtable.cpp \
    $$PWD/uchiya/llah.cpp \
    $$PWD/uchiya/llahparam.cpp \
    $$PWD/uchiya/paper.cpp \
    $$PWD/uchiya/paperlist.cpp \
    $$PWD/uchiya/window.cpp \
    $$PWD/uchiya/mylib/combination.cpp \
    $$PWD/uchiya/mylib/mycam.cpp \
    $$PWD/uchiya/mylib/myimage.cpp \
    $$PWD/uchiya/mylib/mylabel.cpp \
    $$PWD/uchiya/mylib/mymat.cpp \
    $$PWD/uchiya/mylib/mytimer.cpp

HEADERS += $$PWD/uchiya/blob.h \
    $$PWD/uchiya/bloblist.h \
    $$PWD/uchiya/hashtable.h \
    $$PWD/uchiya/llah.h \
    $$PWD/uchiya/llahparam.h \
    $$PWD/uchiya/paper.h \
    $$PWD/uchiya/paperlist.h \
    $$PWD/uchiya/window.h \
    $$PWD/uchiya/mylib/combination.h \
    $$PWD/uchiya/mylib/mycam.h \
    $$PWD/uchiya/mylib/myimage.h \
    $$PWD/uchiya/mylib/mylabel.h \
    $$PWD/uchiya/mylib/mymat.h \
    $$PWD/uchiya/mylib/mytimer.h \
    $$PWD/uchiya/mylib/opencvpath.h

INCLUDEPATH += $$PWD/uchiya

# QML-IMU library
SOURCES += \
    $$PWD/qml-imu/src/ExtendedKalmanFilter.cpp \
    $$PWD/qml-imu/src/IMU.cpp \
    $$PWD/qml-imu/src/AccelerometerBiasEstimator.cpp

HEADERS += \
    $$PWD/qml-imu/src/ExtendedKalmanFilter.h \
    $$PWD/qml-imu/src/IMU.h \
    $$PWD/qml-imu/src/AccelerometerBiasEstimator.h

INCLUDEPATH += $$PWD/qml-imu/src/
