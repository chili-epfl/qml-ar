## AR module for QML
This module adds seamless Augmented Reality to your Qt/QML application.

The framework uses <a href="http://limu.ait.kyushu-u.ac.jp/~uchiyama/me/code/UCHIYAMARKERS/index.html">Random dot markers</a> by Hideaki Uchiyama, which are chosen over other possibilities due to their small negative visual impact on the scene they are put on.

The image processing is done using OpenCV.

## Gallery:
Android:
<p>
<img src="/examples/00_chest/screenshots/android1.jpg?raw=true" height="200" alt="Sample application on Android" />
<img src="/examples/00_chest/screenshots/android2.jpg?raw=true" height="200" alt="Sample application on Android" />
<img src="/examples/00_chest/screenshots/android3.jpg?raw=true" height="200" alt="Sample application on Android" />
<img src="/examples/00_chest/screenshots/android4.jpg?raw=true" height="200" alt="Sample application on Android" />
<img src="/examples/00_chest/screenshots/android_mixed.jpg?raw=true" height="200" alt="Mixed reality output" />
</p>

Linux:
<p>
<img src="/examples/00_chest/screenshots/linux1.jpg?raw=true" height="200" alt="Sample application on Linux" />
<img src="/examples/00_chest/screenshots/linux2.jpg?raw=true" height="200" alt="Sample application on Linux" />
<img src="/examples/00_chest/screenshots/linux3.jpg?raw=true" height="200" alt="Sample application on Linux" />
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
2. Download and print a pdf file with markers:<br /><a href="/examples/00_chest/demo_sheet/ar_demo_sheet.pdf"><img src="/examples/00_chest/demo_sheet/ar_demo_sheet.png" width="200" /></a>
3. Attach camera to your PC (device 1 is used by default). Point camera towards the printed sheet
4. A chest from Qt examples and a cuboid should appear, like on the screenshots above.

## The simplest demo
The project `01_simplest` is specifically designed to show how easy it is to obtain minimal functionality:

```
// main.qml
import QtQuick.Window 2.2; import AR 1.0
Window { visible: true; height: 500; width: 500
  ARComponent { arSceneComponent: Qt.createComponent("Activity.qml"); width: 500 }
}

// Activity.qml
import Qt3D.Core 2.0; import Qt3D.Extras 2.0
Entity { components: [
        PhongMaterial { ambient: Qt.rgba(0.5, 0.5, 0.5, 1); },
        CuboidMesh { xExtent: 50; yExtent: 50; zExtent: 50 } ] }
```

<img src="/examples/01_simplest/screenshot_linux.jpg?raw=true" height="100" alt="The simplest example" />

## More demos
See README in the `examples/` folder.

## Dependencies
*All of these are downloaded automatically using `git clone --recursive`:*
  - [yuv2rgb](https://github.com/andrechen/yuv2rgb.git): Library for converting NV21 data from Android camera to RGB data.
  - [uchiya](https://github.com/chili-epfl/ARUchiya.git): Random dot markers by Hideaki Uchiyama, adapted for Qt build.
  - [QtOpenCV](https://github.com/dbzhang800/QtOpenCV.git): Abstraction over OpenCV calls done in Qt-style.
  - [qml-imu](https://github.com/chili-epfl/qml-imu.git): Library for obtaining phone orientation on Android using Qt Sensors and a Kalman filter.

## Application Structure
<img src="/doc/components_v2.png" />

## Build documentation
```
$ sudo apt-get -y install doxygen doxyqml
$ doxygen
```

The docs will appear inside the `doc/` folder.

## Adding markers to your scene
You can add random dot markers to your `svg` file by using our tool: <a href="https://github.com/chili-epfl/qml-ar-inkscape">qml-ar-inkscape</a>

## Mixed reality
<img src="/examples/00_chest/screenshots/android_mixed.jpg?raw=true" height="300" alt="Mixed reality output" />
Mixed reality is supported both on Android and Linux.
See comments in the files of the example application on how to enable this mode.
Currently, camera image rendering is done on CPU in this mode, which also results in low resolution and low FPS.
Moreover, the image looks far away (whereas objects look close) since the image is not shifted.
Framework tested on Xiaomi Redmi 4A, and both with Google Cardboard and Gear VR.

## Battery usage
SM-T810 discharges from 100% to 10% in 2 hours 20 minutes. Temperature (half of the back surface lying on a table) rises up to 50 °C.<br />
<img src="/notebooks/battery-SMT810/BatteryLog06Mar18.csv_level.png" width="250px" />
<img src="/notebooks/battery-SMT810/BatteryLog06Mar18.csv_temperature.png" width="250px" />
<img src="/notebooks/battery-SMT810/BatteryLog06Mar18.csv_voltage.png" width="250px" />

## Clicked point estimation
The library also is capable of estimating the position in the world for a point clicked on the screen. See the signal `clickedOnActivity` of the `ARComponent`. The program assumes that the world point is at the plane with markers (z = 0).<br />
<img src="/examples/00_chest/screenshots/linux_3dpoint.jpg" width="250px" />

## Contact
<a href="http://sergeivolodin.github.io">Sergei Volodin</a>, <a href="mailto:wafa.johal@gmail.com">Wafa Johal</a>, EPFL, <a href="http://chili.epfl.ch">CHILI lab</a>
