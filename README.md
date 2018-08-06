## AR module for QML
This module adds seamless Augmented Reality to your Qt/QML application.

The framework uses <a href="http://limu.ait.kyushu-u.ac.jp/~uchiyama/me/code/UCHIYAMARKERS/index.html">Random dot markers</a> by Hideaki Uchiyama, which are chosen over other possibilities due to their small negative visual impact on the scene they are put on.

The image processing is done using OpenCV.

## Gallery:
Android:
<p>
<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/examples/00_chest/screenshots/android1.jpg" height="200" alt="Sample application on Android" />
<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/examples/00_chest/screenshots/android2.jpg" height="200" alt="Sample application on Android" />
<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/examples/00_chest/screenshots/android3.jpg" height="200" alt="Sample application on Android" />
<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/examples/00_chest/screenshots/android4.jpg" height="200" alt="Sample application on Android" />
<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/examples/00_chest/screenshots/android_mixed.jpg" height="200" alt="Mixed reality output" />
</p>

Linux:
<p>
<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/examples/00_chest/screenshots/linux1.jpg" height="200" alt="Sample application on Linux" />
<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/examples/00_chest/screenshots/linux2.jpg" height="200" alt="Sample application on Linux" />
<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/examples/00_chest/screenshots/linux3.jpg" height="200" alt="Sample application on Linux" />
</p>

## Prerequisites (Common)
1. <a href="https://www.qt.io/download">Qt 5</a> (tested with 5.10.0) and Qt Creator (tested with 4.5.0), tested on Desktop and Android kits
2. git

Example for Ubuntu Trusty:
```
$ sudo apt-get update
$ sudo apt-get install -qy git software-properties-common; QT=510;
$ sudo add-apt-repository -y ppa:beineri/opt-qt-5.10.1-trusty; sudo apt-get update
$ sudo apt-get -qy install qt${QT}base qt${QT}xmlpatterns libpulse-dev libglib2.0-dev build-essential libgtk2.0-dev pkg-config qt5-qmake qtbase5-dev libqt5xmlpatterns5-dev qt${QT}quickcontrols qt${QT}multimedia qt${QT}3d qt${QT}sensors qt${QT}quickcontrols2
$ export QMAKESPEC=linux-g++
$ . /opt/qt$QT/bin/qt$QT-env.sh
```

## Prerequisites (Linux)
1. <a href="https://github.com/opencv/opencv">OpenCV for Desktop</a> (tested with bf418ba34).
2. Tested on 4xAMD A8-7410 APU (GPU not used)

Example for Ubuntu Trusty:
```
$ sudo add-apt-repository -y ppa:lkoppel/opencv; sudo apt-get update
$ sudo apt-get -qy install libopencv-dev
```

## Prerequisites (Android)
1. <a href="https://sourceforge.net/projects/opencvlibrary/files/opencv-android/">OpenCV for Android</a> (tested with 3.3.1). Put the symlink pointing to the OpenCV for Android as `qml-ar/opencv-android` (inside the repo).
2. Android SDK (tested with r22.3) and NDK (tested with r15c), need to be set up with Qt. Run `export ANDROID_NDK_ROOT=/path/to/ndk` before compilation.
3. Android device (tested with Samsung Galaxy SM-T810 running on cyanogenmod). *Up to 5 cores required, GPU is used for image rendering*

Example for Ubuntu Trusty: *need to download things by hand in GUI, including Qt for Android.*

## Installation
[![Build Status for Linux](https://travis-ci.org/chili-epfl/qml-ar.svg?branch=master)](https://travis-ci.org/chili-epfl/qml-ar)

Build and install the library:
```
$ git clone --recursive https://github.com/chili-epfl/qml-ar.git; cd qml-ar
$ mkdir build_linux; cd build_linux; /path/to/qt5_desktop/bin/qmake ..; make -j10 install; cd ..
$ mkdir build_android; cd build_android; /path/to/qt5_android/bin/qmake ..; make -j10 install; cd ..
```
(`make install` command might require `sudo`)

Example for Ubuntu Trusty for step 2: `mkdir build_linux; cd build_linux; qmake ..; make -j10; sudo make install; cd ..`

## A simple demo
1. Open Qt Creator, open `qml-ar/examples/00_chest/ar-chest.pro`, compile and run on Desktop/Android
2. Download and print a pdf file with markers:<br /><a href="/examples/00_chest/demo_sheet/ar_demo_sheet.pdf"><img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/examples/00_chest/demo_sheet/ar_demo_sheet.png" width="200" /></a>
3. Attach camera to your PC (device 1 is used by default). Point camera towards the printed sheet
4. A chest from Qt examples and a cuboid should appear, like on the screenshots above.

## The simplest demo
The project `01_simplest` is specifically designed to show how easy it is to obtain minimal functionality:

```
// main.qml
import QtQuick.Window 2.2; import ch.epfl.chili.qmlar 1.0
Window { visible: true; height: 500; width: 500
  ARComponent { arSceneComponent: Qt.createComponent("Activity.qml"); width: 500 }
}

// Activity.qml
import Qt3D.Core 2.0; import Qt3D.Extras 2.0
Entity { components: [
        PhongMaterial { ambient: Qt.rgba(0.5, 0.5, 0.5, 1); },
        CuboidMesh { xExtent: 50; yExtent: 50; zExtent: 50 } ] }
```

<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/examples/01_simplest/screenshot_linux.jpg" height="100" alt="The simplest example" />

## More demos
See README in the `examples/` folder.

## Dependencies
*All of these are downloaded automatically using `git clone --recursive`:*
  - [yuv2rgb](https://github.com/andrechen/yuv2rgb.git): Library for converting NV21 data from Android camera to RGB data.
  - [uchiya](https://github.com/chili-epfl/ARUchiya.git): Random dot markers by Hideaki Uchiyama, adapted for Qt build.
  - [QtOpenCV](https://github.com/dbzhang800/QtOpenCV.git): Abstraction over OpenCV calls done in Qt-style.
  - [qml-imu](https://github.com/chili-epfl/qml-imu.git): Library for obtaining phone orientation on Android using Qt Sensors and a Kalman filter.

## Application Structure

4. Performance. Number of threads. FPS, Latency. Bottlenecks. Images for Desktop/Android. Tracking. IMU methods. Different classes.
5. Inkscape extension demo (from Slides)
6. Possible extensions (random color, hide dots, robustness, FPS: GPU)...

The markers were chosen due to their relative small visual impact on the scene. The traditional black-and-white AR markers such as <a href="https://github.com/chili-epfl/chilitags">Chilitags</a>, when put on the scene, distract the eye because of their high contrast (this ease of finding them is the reason they are chosen as an object for detection). The aim of this project was to find a less visually noticeable alternative to traditional markers, or *seamless* markers. Of all of the types of seamless markers the <a href="http://limu.ait.kyushu-u.ac.jp/~uchiyama/me/code/UCHIYAMARKERS/index.html">Random Dots</a> project done by Hideaki Uchiyama was chosen because of the simplicity of the concept over the other markers (such as complex shapes, circles of dots, lines etc) and because of the availability of the source code for the project.

The original Uchiya library was written for Windows/GLUT and it a proof of concept. The goal of this project was to make it fully-configurable and able to run on Android mobile devices and Linux platforms. This was achieved using shaders and threads to speed up the code. Moreover, one of the features is support of Qt/QML, which is the target framework for this library. This project uses OpenCV to process images and does not use any AR-specific graphical processing library.

The application relies heavily on the Qt-specific tools, such as signals and slots, and the QtConcurrent function calls to parallelize the image processing. The whole library consists of many C++ classes which are then connected together to form a processing pipeline in the `QMLAR` class. This class is then wrapped inside the `ThreadedQMLAR` class and exported to QML. The library creates about a dozen threads.

The diagram below shows the path each image takes before the pose can be inferred from that image. First, the image is grabbed from the camera using an Image Provider, which is a platform-specific class object. An `OpenCVCameraBackend` is used on Linux and a `QtCameraBackend` is used on Android. The latter needs to take the image from the GPU and is expected to be rewritten using shaders, incuding the next few steps in the pipeline. This is the last step in the pipeline which uses the GPU (and the Linux version doesn't use it at all)

<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/doc/components_v2.png" />

After obtaining the image in the main memory, the part of it which does not contain markers is painted black using `BlackenRest`. The detected marker position from the previous iteration is used. At the first iteration or at the iteration right after the markers were lost, this component has no effect. This is a small heuristic which allows to speed up the process of tracking, since blobs now can be detected only on non-blackened area containing markers on the previous frame. This technique therefore requires a decent framerate.

After blackening, the image is binarized using a `HueThreshold` instance. The app supports dynamic calibration of the HSV values, however, at this point it is hardcoded to detect only <font color="red">red</font> dots. This can be changed inside the `qmlar.cpp` file. Then the image is fed inside the Uchiya library which outputs the homography mapping an undistorted marker image with dots to the actual camera shot. These are used to calculate the four marker corners for each detected marker. This data is then fed inside the `MarkerMVPProvider` which obtains the extrinsic camera parameters (or the ModelView matrix) using a `cv::solvePnP` call.

The marker parameters (coordinates of dots and the location of the marker) are contained inside the `/assets/markers.json` file of the *application*, see the <a href="https://github.com/chili-epfl/qml-ar-inkscape">repository of the tool which creates markers</a> for format description. The camera parameters are inside the `/assets/camera_matrix.json` with `camera_matrix` containing the flattened matrix components along with `width` and `height` of the image at calibration time. See `/examples/00_chest/assets/camera_matrix.json` in this repository for an example. Camera is calibrated using standard OpenCV tools.

On Android, there is also an additional step which makes the use of the IMU pose to correct the MVP in between camera frames to increase the frequency of updates and decrease the latency, thus making the AR view more smooth. Since the onboard phone's inertial sensors don't have enough precision for the position to be estimate (it drifts significantly even during the small time between camera frames), only the angle is corrected using the `IMUMVPDecorator` class. This class simply modifies the MVP by rotating it additionally by the &Delta;MVP estimated from the IMU, thus, increasing the perceived FPS of pose updates.

The class also tries to correct for the latency in the pipeline using the following method: time of the shot (when the photo was grabbed from the device) is remembered for each element in the pipeline. Moreover, the IMU measurements are kept for the last few second. Then, when it's time to send a new pose to QML, the class additionally rotates the pose by the difference between current IMU reading and the reading when the shot was taken. Since on Android a texture output is used for the camera image, which doesn't add any delay, along with this latency correction method it allows to decrease the perceived latency quite significantly since most of the movements the user makes are the camera rotations. Note that this method doesn't compensate neither for the translational latency nor for the latency when *the object* is rotated, not the camera (since no IMU readings are available in that case).

On Linux, no IMU is assumed to exist, and also the image is rendered using updates for the `Image` QML component, which in turn requests a QImage obtained using the OpenCV backend. This method seems quite fast on Linux despite it's inefficiency and doesn't lead to a decrease in FPS.

At the end, the application exports the MVP matrix to a QML component `ARComponent`, which uses it as the `Camera` parameter in Qt3D. This component is also capable of loading the user-defined scene.

## Build documentation
```
$ sudo apt-get -y install doxygen doxyqml
$ doxygen
```

The docs will appear inside the `doc/` folder.

## Adding markers to your scene
You can add random dot markers to your `svg` file by using our tool: <a href="https://github.com/chili-epfl/qml-ar-inkscape">qml-ar-inkscape</a>

## Mixed reality
<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/examples/00_chest/screenshots/android_mixed.jpg" height="300" alt="Mixed reality output" />
Mixed reality is supported both on Android and Linux.
See comments in the files of the example application on how to enable this mode.
Currently, camera image rendering is done on CPU in this mode, which also results in low resolution and low FPS.
Moreover, the image looks far away (whereas objects look close) since the image is not shifted.
Framework tested on Xiaomi Redmi 4A, and both with Google Cardboard and Gear VR.

## Battery usage
SM-T810 discharges from 100% to 10% in 2 hours 20 minutes. Temperature (half of the back surface lying on a table) rises up to 50 °C.<br />
<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/notebooks/battery-SMT810/BatteryLog06Mar18.csv_level.png" width="250px" />
<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/notebooks/battery-SMT810/BatteryLog06Mar18.csv_temperature.png" width="250px" />
<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/notebooks/battery-SMT810/BatteryLog06Mar18.csv_voltage.png" width="250px" />

## Clicked point estimation
The library also is capable of estimating the position in the world for a point clicked on the screen. See the signal `clickedOnActivity` of the `ARComponent`. The program assumes that the world point is at the plane with markers (z = 0).<br />
<img src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/examples/00_chest/screenshots/linux_3dpoint.jpg" width="250px" />

## Contact
<a href="http://sergeivolodin.github.io">Sergei Volodin</a>, <a href="mailto:wafa.johal@gmail.com">Wafa Johal</a>, EPFL, <a href="http://chili.epfl.ch">CHILI lab</a>
