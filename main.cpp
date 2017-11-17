#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "opencvbackend.h"
#include "qtbackend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    //engine.addImageProvider(QLatin1String("cvimg"), new OpenCVCameraBackEnd);
    engine.addImageProvider(QLatin1String("cvimg"), new QtCameraBackEnd);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
