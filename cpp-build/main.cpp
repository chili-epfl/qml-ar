#include <QGuiApplication>
#include "threadedqmlar.cpp"

int main(int argc, char *argv[])
{
    // default Qt + QML init
    QGuiApplication app(argc, argv);

    ThreadedQMLAR ar;
    ar.getImageProvider();
    ar.setImageWidth(600);
    ar.setCameraId(1);

    return app.exec();
}
