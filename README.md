## AR module for QML
This module allows to add an AR component to your QML application simply by importing our library. The framework uses <a href="http://limu.ait.kyushu-u.ac.jp/~uchiyama/me/code/UCHIYAMARKERS/index.html">Random dot markers</a> by Hideaki Uchiyama, which are chosen over other possibilities due to their small negative visual impact on the scene they are put on.

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

## Prerequisites (Linux)
1. <a href="https://github.com/opencv/opencv">OpenCV for Desktop</a> (tested with bf418ba34)
2. Tested on 4xAMD A8-7410 APU (GPU not used)

## Prerequisites (Android)
1. <a href="https://sourceforge.net/projects/opencvlibrary/files/opencv-android/">OpenCV for Android</a> (tested with 3.3.1)
2. Android SDK (tested with r22.3) and NDK (tested with r15c), need to be set up with Qt
3. Android device (tested with Samsung Galaxy SM-T810 running on cyanogenmod) *Up to 5 cores required, GPU is used for image rendering*

## Installation
[![Build Status](https://travis-ci.org/chili-epfl/qml-ar.svg?branch=master)](https://travis-ci.org/chili-epfl/qml-ar)

1. `git clone git@github.com:chili-epfl/qml-ar.git` (master branch is usually working)
2. `cd qml-ar; git submodule init && git submodule update`
3. Create a symlink called `opencv-android` pointing to the OpenCV for Android
4. **Linux:** `mkdir build_linux; cd build_linux; qmake ..; make -j10 install`
5. **Android:** `export ANDROID_NDK_ROOT=/path/to/ndk ; mkdir build_android; cd build_android; qmake ..; make -j10 install` (same as for Linux but with ANDROID_NDK_ROOT variable)
6. Open Qt Creator, open `examples/00_chest/ar-chest.pro`, compile and run on Desktop/Android

## Running
1. Download pdf and print the example file with markers:<br /><a href="/examples/00_chest/demo_sheet/ar_demo_sheet.pdf"><img src="/examples/00_chest/demo_sheet/ar_demo_sheet.png" width="200" /></a>
2. Attach camera to your PC (device 1 is used by default). Point camera towards the printed sheet
3. A chest from Qt examples and a cuboid should appear

## Application Structure
<img src="/doc/components_v2.png" />

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
<a href="mailto:sergei.volodin@epfl.ch">Sergei Volodin</a>, <a href="mailto:wafa.johal@gmail.com">Wafa Johal</a>, EPFL, <a href="http://chili.epfl.ch">CHILI lab</a>
