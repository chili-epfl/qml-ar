##### QT
# Name of the application
TARGET=cellulo-ar-chest

# Qt configuration for Quick applications
QT += quick multimedia opengl multimediawidgets 3dinput 3drender
CONFIG += c++11

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

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

RESOURCES += qml.qrc

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

# android: installing UchiyaMarkers
# .txt files to device
# See https://stackoverflow.com/questions/20573838/qt-5-2-including-external-file-into-an-android-package
# https://forum.qt.io/topic/69946/qt-android-assets/4
# https://falsinsoft.blogspot.ch/2017/01/qt-creator-include-additional-files.html
android {
    # From: http://community.kde.org/Necessitas/Assets
    markerdata.path = /assets/data
    markerdata.files += $$PWD/../uchiya/data/*
    markerdata.depends += FORCE

    INSTALLS += markerdata
}


# adding assets/ folder
assets.path = /assets
assets.files += $$PWD/assets/*
assets.depends += FORCE
INSTALLS += assets
