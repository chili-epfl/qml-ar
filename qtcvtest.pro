TARGET=qtcvtest

QT += quick
QT += multimedia
CONFIG += c++11

linux:!android {
    # using pkg-config
    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

android {
    # full path to OpenCV Android SDK
    OPENCV_PATH = /home/opt/opencv-android

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
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    opencvbackend.cpp \
    qtbackend.cpp \
    cameraframegrabber.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    opencvbackend.h \
    qtbackend.h \
    cameraframegrabber.h
