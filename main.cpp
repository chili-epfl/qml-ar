#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "opencvbackend.h"
#include "qtbackend.h"
#include "uchiyabackend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // creating provider connected to the actual camera
    QQuickImageProvider* provider = NULL;
#ifdef Q_OS_ANDROID
    provider = new QtCameraBackEnd();
#elif defined Q_OS_LINUX
    provider = new OpenCVCameraBackEnd();
#else
    #error "OS other than Android and Linux are not supported"
#endif

    // adding UchiyaBackEnd
    engine.addImageProvider(QLatin1String("camera"), new UchiyaBackEnd(provider));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
