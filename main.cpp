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
    //QQuickImageProvider* provider = new ImageBackend("images/image.jpg");

    // creating Uchiya marker detector
    UchiyaMarkerDetector* detector = new UchiyaMarkerDetector;

#ifdef Q_OS_ANDROID
    detector->loadMarkerPositions("assets:/markers.json");
#else
    detector->loadMarkerPositions(":/assets/markers.json");
#endif

    float c_p_m_[] = {5.9740803084926324e+02, 0., 3.2367345813470314e+02,
                      0., 5.9740803084926324e+02, 2.5857594808156688e+02,
                      0., 0., 1. };
    QMatrix3x3 c_p_m(c_p_m_);
    c_p_m *= 1e-10;
    //c_p_m(2, 2) = 1;
    detector->setCameraProjectionMatrix(c_p_m);

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
