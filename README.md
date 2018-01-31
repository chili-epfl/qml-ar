## AR module for QML

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
1. `git submodule init && git submodule update`
2. Create a symlink called `opencv-android` pointing to the OpenCV for Android
3. `mkdir build_linux; cd build_linux; qmake ..; make -j10 install`
4. Same for Android, also `export ANDROID_NDK_ROOT=/path/to/ndk` required
5. Open Qt Creator, open `example/ar-chest.pro`, compile and run on Desktop/Android

## Running
1. Download pdf and print the example file with markers:<br /><a href="/example/demo_sheet/ar_demo_sheet.pdf"><img src="/example/demo_sheet/ar_demo_sheet.png" width="200" /></a>
2. Attach camera to your PC (device 1 is used by default). Point camera towards the printed sheet
3. A chest from Qt examples and a cuboid should appear

## Gallery:
Android:
<p>
<img src="/example/screenshots/android1.jpg?raw=true" height="200" alt="Sample application on Android" />
<img src="/example/screenshots/android2.jpg?raw=true" height="200" alt="Sample application on Android" />
<img src="/example/screenshots/android3.jpg?raw=true" height="200" alt="Sample application on Android" />
<img src="/example/screenshots/android4.jpg?raw=true" height="200" alt="Sample application on Android" />
</p>

Linux:
<p>
<img src="/example/screenshots/linux1.jpg?raw=true" height="200" alt="Sample application on Linux" />
<img src="/example/screenshots/linux2.jpg?raw=true" height="200" alt="Sample application on Linux" />
<img src="/example/screenshots/linux3.jpg?raw=true" height="200" alt="Sample application on Linux" />
</p>
