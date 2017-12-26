#include "celluloar.h"

CelluloAR::CelluloAR()
{
    // initially, object is not initialized
    is_initialized = false;
    camera_id = -2;
    image_filename = "";
}

int CelluloAR::getCameraId()
{
    Q_ASSERT(is_initialized);
    return camera_id;
}

void CelluloAR::setCameraId(int camera_id)
{
    Q_ASSERT(!is_initialized);
    this->camera_id = camera_id;
    raw_provider = PortableCameraBackendFactory::getBackend(camera_id);
    initialize();
}

void CelluloAR::setImageFilename(QString filename)
{
    Q_ASSERT(!is_initialized);
    image_filename = filename;
    raw_provider = new ImageBackend(filename);
    initialize();
}

QMatrix4x4 CelluloAR::getMVPMatrix()
{
    Q_ASSERT(is_initialized);
    return mvp_provider->getMVPMatrix();
}

void CelluloAR::newMVPMatrixSlot()
{
    Q_ASSERT(is_initialized);
    emit newMVPMatrix();
}

QQuickImageProvider *CelluloAR::getImageProvider()
{
    Q_ASSERT(is_initialized);
    return marker_backend;
}

QString CelluloAR::getImageFilename()
{
    Q_ASSERT(is_initialized);
    return image_filename;
}

void CelluloAR::initialize()
{
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
    camera_matrix = new CalibratedCameraFileStorage(ASSETS_PATH + "camera_matrix.json");

    // decorating camera matrix object
    // allowing to obtain perspective matrix
    perspective_camera = new PerspectiveCamera(camera_matrix);

    // adding UchiyaBackEnd (decorating camera object)
    marker_backend = new MarkerBackEnd(raw_provider, detector);

    // creating a ModelView provider
    mvp_provider = new MarkerMVPProvider(detector, perspective_camera);

    // notify QML on each update of MVP matrix
    connect(mvp_provider, SIGNAL(newMVPMatrix()), this, SLOT(newMVPMatrixSlot()));

    // now the object is initialized
    is_initialized = true;
}
