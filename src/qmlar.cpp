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
    return mvp_buffer;
}

int QMLAR::getImageWidth()
{
    return image_width;
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

void QMLAR::setMVP(QMatrix4x4 mvp)
{
    mvp_buffer = mvp;
    emit newMVPMatrix();
}

void QMLAR::connectAll()
{
    // camera -> scaler
    connect(raw_provider, SIGNAL(imageAvailable(QImage)), scaler, SLOT(setInput(QImage)));

    // camera -> QML
    connect(raw_provider, SIGNAL(imageAvailable(QImage)), &marker_backend, SLOT(setCamera(QImage)));

    // scaler -> tracking
    connect(scaler, SIGNAL(imageAvailable(QImage)), tracking, SLOT(setInput(QImage)));

    // scaler -> resolution
    connect(scaler, SIGNAL(imageAvailable(QImage)), perspective_camera, SLOT(setResolution(QImage&)));

    // tracking -> blobs
    connect(tracking, SIGNAL(imageAvailable(QImage)), blob_detector, SLOT(setInput(QImage)));

    // blobs -> markers
    connect(blob_detector, SIGNAL(imageAvailable(QImage)), detector, SLOT(setInput(QImage)));

    // markers -> QML
    connect(detector, SIGNAL(previewUpdated(QImage)), &marker_backend, SLOT(setPreview(QImage)));

    // markers -> MVP
    connect(detector, SIGNAL(markersUpdated(MarkerStorage)), mvp_provider, SLOT(recompute(MarkerStorage)));

    // markers -> tracking
    connect(detector, SIGNAL(markersUpdated(MarkerStorage)), tracking, SLOT(onNewMarkers(MarkerStorage)));

    // mvp -> tracking
    connect(mvp_provider, SIGNAL(newMVMatrix(QMatrix4x4)), tracking, SLOT(onNewMVMatrix(QMatrix4x4)));
    connect(mvp_provider, SIGNAL(newPMatrix(QMatrix4x4)), tracking, SLOT(onNewPMatrix(QMatrix4x4)));

    // mvp -> imu
    connect(mvp_provider, SIGNAL(newMVMatrix(QMatrix4x4)), mvp_imu_decorated, SLOT(updateLastMV(QMatrix4x4)));

    // output MVP matrix from IMU decorator
    connect(mvp_imu_decorated, SIGNAL(newMVPMatrix(QMatrix4x4)), this, SLOT(setMVP(QMatrix4x4)));
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
    mvp_provider = new MarkerMVPProvider(perspective_camera);

    // creating linear pose predictor
    predictor = new LinearPosePredictor();

    // adding tracking to marker detector
    tracking = new TrackingDecorator(detector, predictor, mvp_provider);

    // decorating MVP with IMU
    mvp_imu_decorated = new IMUMVPDecorator(imu);

    // creating image scaler
    scaler = new ImageScaler(image_width);

    connectAll();

    // now the object is initialized
    is_initialized = true;
}
