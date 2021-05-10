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
    src/qmlar.cpp \
    src/qmlarplugin.cpp \
    src/utils/timelogger.cpp \
    src/ar/imumvpdecorator.cpp \
    src/ar/posepredictor.cpp \
    src/ar/linearposepredictor.cpp \
    src/markers/blobdetector.cpp \
    src/imagebackend/qtcamera2qml.cpp \
    src/imagebackend/androidfilter.cpp \
    src/imagebackend/imageproviderasync.cpp \
    src/imagebackend/imagescaler.cpp \
    src/markers/hsvintervaldetector.cpp \
    src/markers/hsvmeancolor.cpp \
    src/markers/huethreshold.cpp \
    src/markers/meanstdcalculator.cpp \
    src/imagebackend/randomimagebackend.cpp \
    src/threadedqmlar.cpp \
    src/utils/fpscalculator.cpp \
    src/ar/posefilter.cpp \
    src/utils/pipelinecontainer.cpp \
    src/ar/blackenrest.cpp \
    src/utils/pipelinecontainerinfo.cpp \
    src/utils/latencycalculator.cpp \
    src/imagebackend/delayfilter.cpp \
    src/imagebackend/framesdelaycalculator.cpp \
    src/utils/qlinkedlistat.cpp

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
    src/qmlar.h \
    src/qmlarplugin.h \
    src/config.h \
    src/utils/timelogger.h \
    src/ar/imumvpdecorator.h \
    src/ar/posepredictor.h \
    src/ar/linearposepredictor.h \
    src/markers/blobdetector.h \
    src/imagebackend/qtcamera2qml.h \
    src/imagebackend/androidfilter.h \
    src/imagebackend/imageproviderasync.h \
    src/imagebackend/imagescaler.h \
    src/markers/hsvintervaldetector.h \
    src/markers/hsvmeancolor.h \
    src/markers/huethreshold.h \
    src/markers/meanstdcalculator.h \
    src/imagebackend/randomimagebackend.h \
    src/threadedqmlar.h \
    src/utils/fpscalculator.h \
    src/ar/posefilter.h \
    src/utils/pipelinecontainer.h \
    src/ar/blackenrest.h \
    src/utils/pipelinecontainerinfo.h \
    src/utils/latencycalculator.h \
    src/imagebackend/delayfilter.h \
    src/imagebackend/framesdelaycalculator.h \
    src/utils/qlinkedlistat.h

QML_SOURCES += \
    src/ARComponent.qml \
    src/AR3DScene.qml \
    src/ARViewfinder.qml \
    src/ARBlobs.qml \
    src/ARMarkers.qml \
    src/ARMenu.qml \

DISTFILES = $$QML_SOURCES

RESOURCES += \
    ch_epfl_chili_qmlar.qrc

##### LIBRARIES
# QtOpenCV library
QT_OPENCV_PATH = QtOpenCV
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

USE_GRAPHICBUFFER=$$(USE_GRAPHICBUFFER)

# OpenCV for Android library
android {
    # full path to OpenCV Android SDK
    OPENCV_PATH = opencv-android
    OPENCV_PATH_ABSOLUTE = $$PWD/opencv-android

    INCLUDEPATH += $${OPENCV_PATH}/sdk/native/jni/include/

    LIBS += -L$${OPENCV_PATH_ABSOLUTE}/sdk/native/libs/armeabi-v7a \
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
        DEFINES += "USENV21FILTER=1"
        LIBS += -lEGL -lnativewindow -lGLESv3
        HEADERS += src/imagebackend/nv21videofilter.h \
                   src/imagebackend/nv21videofilterrunnable.h
        SOURCES += src/imagebackend/nv21videofilter.cpp \
                   src/imagebackend/nv21videofilterrunnable.cpp
        message("Using HardwareBuffer because Android API is >= 26");
        message("Run $ export ANDROID_NDK_PLATFORM=android-25 (or less) before qmake to disable");
    }

    # not adding libraries and telling how to enable this just in case
    else {
        message("Not using HardwareBuffer because Android API is < 26");
        message("Run $ export ANDROID_NDK_PLATFORM=android-26 (or more) before qmake to enable");

        equals(USE_GRAPHICBUFFER, "1") {
            DEFINES += "USEGRAPHICBUFFER=1"
            DEFINES += "USENV21FILTER=1"
            message("Using GraphicBuffer because USE_GRAPHICBUFFER = 1 and Android API < 26");
            message("Run $ export USE_GRAPHICBUFFER=0 to disable");

            LIBS += -lc -lEGL
            HEADERS += src/imagebackend/nv21videofilter.h \
                       src/imagebackend/nv21videofilterrunnable.h
            SOURCES += src/imagebackend/nv21videofilter.cpp \
                       src/imagebackend/nv21videofilterrunnable.cpp

            HEADERS += GraphicBuffer/GraphicBuffer.h GraphicBuffer/DynamicLibrary.h
            SOURCES += GraphicBuffer/GraphicBuffer.cpp GraphicBuffer/DynamicLibrary.cpp
        }
        else {
            LIBS += -lc
            message("Not using GraphicBuffer because USE_GRAPHICBUFFER = 0 and Android API < 26");
            message("Run $ export USE_GRAPHICBUFFER=1 to enable");
        }
    }

    LIBS += -L$${OPENCV_PATH_ABSOLUTE}/sdk/native/3rdparty/libs/armeabi-v7a \
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

# shader info
equals(PRINT_SHADER_INFO, "1") {
    message("Printing shader information because PRINT_SHADER_INFO=1");
    DEFINES += "DEBUG_SHADER=1";
}
else {
    message("Not printing shader information because PRINT_SHADER_INFO!=1");
}

# YUV2RGB library
SOURCES += yuv2rgb/yuv2rgb.cpp
HEADERS += yuv2rgb/yuv2rgb.h
INCLUDEPATH += yuv2rgb

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

INCLUDEPATH += uchiya

# QML-IMU library
SOURCES += \
    qml-imu/src/ExtendedKalmanFilter.cpp \
    qml-imu/src/IMU.cpp \
    qml-imu/src/AccelerometerBiasEstimator.cpp

HEADERS += \
    qml-imu/src/ExtendedKalmanFilter.h \
    qml-imu/src/IMU.h \
    qml-imu/src/AccelerometerBiasEstimator.h

INCLUDEPATH += qml-imu/src/
