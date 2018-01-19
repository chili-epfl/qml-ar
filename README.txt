## AR module for QML

1. git submodule init && git submodule update
2. Create a symlink called opencv-android pointing to the OpenCV Android SDK
3. qmake .; make -j10 install
4. cd example; qmake .; make -j10; ./qml-ar-chest
