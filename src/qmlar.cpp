#include "qmlar.h"
#include "qtbackend.h"
#include "timelogger.h"
#include "qml-imu/src/IMU.h"
#include "qvector3d.h"
#include "linearposepredictor.h"
#include <typeinfo>
#include <QVariant>
#include <QtQml>
#include <QCameraInfo>
#include "qtcamera2qml.h"

QMLAR::QMLAR()
{
    // initially, object is not initialized
    is_initialized = false;
    camera_id = -2;
    image_filename = "";
    raw_provider = NULL;
    camera_wrapper = NULL;
}

int QMLAR::getCameraId()
{
    Q_ASSERT(is_initialized);
    return camera_id;
}

void QMLAR::setCameraId(int camera_id)
{
    Q_ASSERT(!is_initialized);
    TimeLoggerLog("Using camera %d", camera_id);
    this->camera_id = camera_id;
    raw_provider = PortableCameraBackendFactory::getBackend(camera_id);
    init_type = INIT_CAMERA;
    initialize();
}

void QMLAR::setImageFilename(QString filename)
{
    Q_ASSERT(!is_initialized);
    TimeLoggerLog("Opening image %s", filename.toStdString().c_str());
    image_filename = filename;
    raw_provider = new ImageBackend(filename);
    init_type = INIT_IMAGE;
    initialize();
}

QMatrix4x4 QMLAR::getMVPMatrix()
{
    if(!is_initialized) return QMatrix4x4();
    return mvp_imu_decorated->getMVPMatrix();
}

int QMLAR::getImageWidth()
{
    return image_width;
}

void QMLAR::newMVPMatrixSlot()
{
    Q_ASSERT(is_initialized);
    emit newMVPMatrix();
}

void QMLAR::setImageWidth(int new_width)
{
    image_width = new_width;
}

QQuickImageProvider *QMLAR::getImageProvider()
{
    return &marker_backend;
}

QObject* QMLAR::getCamera()
{
    if(init_type == INIT_CAMERA && PortableCameraBackendFactory::cameraViewfinderAvailable())
    {
        QCamera* camera = dynamic_cast<QtCameraBackend*>(raw_provider)->getCamera();
        if(camera_wrapper == NULL)
            camera_wrapper = new QtCamera2QML(camera);
        TimeLoggerLog("%s", "Returning camera object");
        return camera_wrapper;
    }
    else
    {
        TimeLoggerLog("%s", "No camera in this mode");
        return NULL;
    }
}

void QMLAR::startCamera()
{
    if(init_type == INIT_CAMERA && PortableCameraBackendFactory::cameraViewfinderAvailable())
    {
        TimeLoggerLog("%s", "Starting camera");
        dynamic_cast<QtCameraBackend*>(raw_provider)->start();
    }
}

void QMLAR::connectAll()
{
    connect(raw_provider, SIGNAL(imageAvailable(QImage)), scaler, SLOT(setInput(QImage)));
    connect(scaler, SIGNAL(imageAvailable(QImage)), blob_detector, SLOT(setInput(QImage)));
    connect(blob_detector, SIGNAL(imageAvailable(QImage)), tracking, SLOT(setInput(QImage)));
    connect(tracking, SIGNAL(markersUpdated(MarkerStorage), this, SIGNAL(imageUpdated()));
    // notify QML on each update of MVP matrix from IMU
    connect(mvp_imu_decorated, SIGNAL(newMVPMatrix()), this, SLOT(newMVPMatrixSlot()));
}

QString QMLAR::getImageFilename()
{
    Q_ASSERT(is_initialized);
    return image_filename;
}

void QMLAR::initialize()
{
    Q_ASSERT(raw_provider != NULL);
    // creating Uchiya marker detector
    detector = new UchiyaMarkerDetector;

    // setting up assets path (os-dependent)
#ifdef Q_OS_ANDROID
    QString ASSETS_PATH = "assets:/";
#else
    QString ASSETS_PATH = ":/assets/";
#endif

    // creating blob detector
    blob_detector = new BlobDetector();

    // loading marker positions
    detector->loadMarkerPositions(ASSETS_PATH + "markers.json");

    // connecting to IMU
    imu = new IMU();

    // setting Accelerometer bias (TODO: fix hardcode)
    imu->setProperty("accBias", QVector3D(0.397, -0.008, -0.005));

    // loading camera matrix
    camera_matrix = new CalibratedCameraFileStorage(ASSETS_PATH + "camera_matrix.json");

    // decorating camera matrix object
    // allowing to obtain perspective matrix
    perspective_camera = new PerspectiveCamera(camera_matrix);

    // creating a ModelView provider
    mvp_provider = new MarkerMVPProvider(detector, perspective_camera);

    // creating linear pose predictor
    predictor = new LinearPosePredictor();

    // adding tracking to marker detector
    tracking = new TrackingDecorator(detector, predictor, mvp_provider);

    // decorating tracking object to obtain images
    marker_backend.initialize(tracking);
    marker_backend.setCameraBackend(raw_provider);

    // decorating MVP with IMU
    mvp_imu_decorated = new IMUMVPDecorator(mvp_provider, imu);

    // creating image scaler
    scaler = new ImageScaler(image_width);

    connectAll();

    // now the object is initialized
    is_initialized = true;
}
