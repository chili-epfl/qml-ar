#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "portablebackendfactory.h"
#include "uchiyabackend.h"
#include "markerstorage.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // loading marker positions
    MarkerStorage ms;
    ms.populateFromFile("markers.json");

    // creating a camera backend object
    QQuickImageProvider* provider = PortableCameraBackendFactory::getBackend(1);

    // adding UchiyaBackEnd (decorating camera object)
    UchiyaBackEnd* backend = new UchiyaBackEnd(provider);

    // adding marker detection as a backend
    engine.rootContext()->setContextProperty("md", backend);
    engine.addImageProvider(QLatin1String("camera"), backend);

    // loading qml
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
