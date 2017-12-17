#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "portablebackendfactory.h"
#include "imagebackend.h"
#include "markerbackend.h"
#include "markerstorage.h"
#include "uchiyamarkerdetector.h"
#include "calibratedcamerafilestorage.h"

int main(int argc, char *argv[])
{
    // default Qt + QML init
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // creating a camera backend object
    QQuickImageProvider* provider = PortableCameraBackendFactory::getBackend();
    //QQuickImageProvider* provider = new ImageBackend("images/image.jpg");

    // creating Uchiya marker detector
    UchiyaMarkerDetector* detector = new UchiyaMarkerDetector;

    // creating Calibrated Camera
    CalibratedCameraFileStorage* camera_matrix = new CalibratedCameraFileStorage;

    // setting up assets path (os-dependent)
#ifdef Q_OS_ANDROID
    QString ASSETS_PATH = "assets:/";
#else
    QString ASSETS_PATH = ":/assets/";
#endif

    // loading marker positions
    detector->loadMarkerPositions(ASSETS_PATH + "markers.json");

    // loading camera matrix
    camera_matrix->populateFromFile(ASSETS_PATH + "camera_matrix.json");

    // setting camera matrix
    detector->setCameraProjectionMatrix(camera_matrix->getMatrix());

    // adding UchiyaBackEnd (decorating camera object)
    MarkerBackEnd* backend = new MarkerBackEnd(provider, detector);

    // adding marker detector as a backend
    engine.rootContext()->setContextProperty("detector", detector);
    engine.addImageProvider(QLatin1String("camera"), backend);

    // loading qml
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // starting the app
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
