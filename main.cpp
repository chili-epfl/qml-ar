#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "portablebackendfactory.h"
#include "imagebackend.h"
#include "markerbackend.h"
#include "uchiyamarkerdetector.h"
#include "calibratedcamerafilestorage.h"
#include "perspectivecamera.h"
#include "markermvpprovider.h"

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

    // setting up assets path (os-dependent)
#ifdef Q_OS_ANDROID
    QString ASSETS_PATH = "assets:/";
#else
    QString ASSETS_PATH = ":/assets/";
#endif

    // loading marker positions
    detector->loadMarkerPositions(ASSETS_PATH + "markers.json");

    // loading camera matrix
    PerspectiveCamera* camera_matrix = new PerspectiveCamera(new CalibratedCameraFileStorage(ASSETS_PATH + "camera_matrix.json"));

    // adding UchiyaBackEnd (decorating camera object)
    MarkerBackEnd* backend = new MarkerBackEnd(provider, detector);

    // creating a ModelView provider
    MVPProvider* mvp_matrix = new MarkerMVPProvider(detector, camera_matrix);

    // adding marker detector as a backend
    engine.rootContext()->setContextProperty("mvp", mvp_matrix);
    engine.addImageProvider(QLatin1String("camera"), backend);

    // loading qml
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // starting the app
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
