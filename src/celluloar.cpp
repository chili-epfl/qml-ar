#include "celluloar.h"
#include "timelogger.h"

CelluloAR::CelluloAR()
{
    // initially, object is not initialized
    is_initialized = false;
    camera_id = -2;
    image_filename = "";
    raw_provider = NULL;
}

int CelluloAR::getCameraId()
{
    Q_ASSERT(is_initialized);
    return camera_id;
}

void CelluloAR::setCameraId(int camera_id)
{
    Q_ASSERT(!is_initialized);
    TimeLoggerLog("Using camera %d", camera_id);
    this->camera_id = camera_id;
    raw_provider = PortableCameraBackendFactory::getBackend(camera_id);
    initialize();
}

void CelluloAR::setImageFilename(QString filename)
{
    Q_ASSERT(!is_initialized);
    TimeLoggerLog("Opening image %s", filename.toStdString().c_str());
    image_filename = filename;
    raw_provider = new ImageBackend(filename);
    initialize();
}

void CelluloAR::setQMLCamera(QObject *camera)
{
    Q_ASSERT(camera != NULL);
    TimeLoggerLog("%s", "Setting camera from QML camera");
    QCamera* camera_ = qvariant_cast<QCamera*>(camera->property("mediaObject"));
    raw_provider = new QtCameraBackend(camera_);
    this->qml_camera = camera;
    initialize();
}

QMatrix4x4 CelluloAR::getMVPMatrix()
{
    if(!is_initialized) return QMatrix4x4();
    return mvp_provider->getMVPMatrix();
}

void CelluloAR::newMVPMatrixSlot()
{
    Q_ASSERT(is_initialized);
    emit newMVPMatrix();
}

QQuickImageProvider *CelluloAR::getImageProvider()
{
    return &marker_backend;
}

void CelluloAR::update()
{
    if(raw_provider == NULL) return;
    // obtain input image from camera
    QSize sz;
    sz.setWidth(600);
    QPixmap input = raw_provider->requestPixmap("raw", &sz, sz);
    if(input.isNull()) return;

    // send input to marker detector
    detector->setInput(input.toImage().scaledToWidth(600));

    // detect markers
    detector->process();
}

double CelluloAR::getUpdateMS()
{
    return timer.interval();
}

void CelluloAR::setUpdateMS(double value)
{
    timer.start(value);
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

QObject *CelluloAR::getQMLCamera()
{
    return qml_camera;
}

QString CelluloAR::getImageFilename()
{
    Q_ASSERT(is_initialized);
    return image_filename;
}

void CelluloAR::initialize()
{
    Q_ASSERT(raw_provider != NULL);
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
    marker_backend.initialize(detector);

    // creating a ModelView provider
    mvp_provider = new MarkerMVPProvider(detector, perspective_camera);

    // notify QML on each update of MVP matrix
    connect(mvp_provider, SIGNAL(newMVPMatrix()), this, SLOT(newMVPMatrixSlot()));

    // now the object is initialized
    is_initialized = true;
}
