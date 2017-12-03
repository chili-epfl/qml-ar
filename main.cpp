#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "portablebackendfactory.h"
#include "imagebackend.h"
#include "markerbackend.h"
#include "markerstorage.h"
#include "uchiyamarkerdetector.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // creating a camera backend object
    QQuickImageProvider* provider = PortableCameraBackendFactory::getBackend();
    //QQuickImageProvider* provider = new ImageBackend("image.jpg");

    // creating Uchiya marker detector
    UchiyaMarkerDetector* detector = new UchiyaMarkerDetector;

#ifdef Q_OS_ANDROID
        detector->loadMarkerPositions("assets:/markers.json");
#else
        detector->loadMarkerPositions(":/assets/markers.json");
#endif

    // adding UchiyaBackEnd (decorating camera object)
    MarkerBackEnd* backend = new MarkerBackEnd(provider, detector);

    // adding marker detector as a backend
    engine.rootContext()->setContextProperty("detector", detector);
    engine.addImageProvider(QLatin1String("camera"), backend);

    // loading qml
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
