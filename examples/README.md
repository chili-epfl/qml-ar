## Examples for QML-AR

1. `00_chest`: This project renders a Chest and a cuboid using qml-ar.
The cuboid will be at (0, 0) and the chest at (210, 297).
The default camera on Desktop is "1" (second), and front on Android.
Note that you need to print the `ar_demo_sheet.pdf` on A4

2. `01_simplest`: This project renders a cube at (0, 0, 0) using qml-ar.
3. `02_image`: This project renders a cube at (0, 0, 0) using qml-ar. Image input is used instead of a camera.
4. `03_avr`: This project renders a cube at (0, 0, 0) using qml-ar. The Augmented Virtual Reality (or Mixed Reality) is enabled.
5. `04_clicked`: This project renders a cube at (0, 0, 0) using qml-ar. Moreover, it allows to estimate the clicked point x, y in the world.
6. `05_markers`: This project renders a cube at (0, 0, 0) using qml-ar. Moreover, it show how to react on markers dissapearance with a message to the user.
7. `06_mvp`: This project renders a cube at (0, 0, 0) using qml-ar. Moreover, it show how to obtain the raw MVP matrix from AR lib.
8. `07_property_setting`: This project shows how to manage properties of your the 3D scene by dynamically changing the cube position.
9. `98_test_image`: This project is similar to `02_image`, but it exits right after it can detect markers. Useful for testing the library after installation.
10. `99_cpp_only`: This project shows how to use QML-AR w/o QML (using only Qt GUI).