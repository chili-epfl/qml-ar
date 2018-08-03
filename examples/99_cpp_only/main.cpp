/**
 * @file main.cpp
 * @brief This project shows how to use QML-AR w/o QML
 * (using only Qt GUI)
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

#include <QGuiApplication>

// need to include the library
#include "threadedqmlar.cpp"

int main(int argc, char *argv[])
{
    // default Qt GUI init
    QGuiApplication app(argc, argv);

    // create an instance
    ThreadedQMLAR ar;

    // do the same calls as ARComponent.qml does
    ar.getImageProvider();
    ar.setImageWidth(600);
    ar.setCameraId(1);

    // run your app
    return app.exec();
}
