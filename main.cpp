#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "opencvbackend.h"
#include "qtbackend.h"
#include "uchiyabackend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // creating provider connected to the actual camera
    // using OpenCV for Linux
    // And Qt for Android
    QQuickImageProvider* provider = NULL;
#ifdef Q_OS_ANDROID
    provider = new QtCameraBackEnd();
#elif defined Q_OS_LINUX
    // using 1 as camera id
    provider = new OpenCVCameraBackEnd(1);
#else
    #error "OS other than Android and Linux are not supported"
#endif

    // adding UchiyaBackEnd (decorating camera object)

    UchiyaBackEnd* backend = new UchiyaBackEnd(provider);

    engine.rootContext()->setContextProperty("md", backend);
    engine.addImageProvider(QLatin1String("camera"), backend);

    // loading qml
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
