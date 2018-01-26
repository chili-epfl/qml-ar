#include <QVariant>
#include <QtQml>
#include <QCameraInfo>
#include "qmlar.h"
#include "imageproviderasync.h"
#include "uchiyamarkerdetector.h"
#include "imagebackend.h"
#include "perspectivecamera.h"
#include "portablebackendfactory.h"
#include "markerbackend.h"
#include "markermvpprovider.h"
#include "calibratedcamerafilestorage.h"
#include "imumvpdecorator.h"
#include "posepredictor.h"
#include "trackingdecorator.h"
#include "blobdetector.h"
#include "qtcamera2qml.h"
#include "imagescaler.h"
#include "markerstorage.h"
#include "hsvintervaldetector.h"
#include "huethreshold.h"
#include "qtbackend.h"
#include "timelogger.h"
#include "qml-imu/src/IMU.h"
#include "qvector3d.h"
#include "linearposepredictor.h"
#include "randomimagebackend.h"
#include "qtcamera2qml.h"
#include "worldimage.h"

QMLAR::QMLAR()
{
    // initially, object is not initialized
    is_initialized = false;
    camera_id = -2;
    image_filename = "";
    raw_provider = NULL;
    perspective_camera = NULL;
    camera_wrapper = NULL;
    marker_backend = new MarkerBackEnd();
    marker_storage = new MarkerStorage();
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

    // Random provider (debug)
    //init_type = INIT_IMAGE;
    //raw_provider = new RandomImageBackend();

    // camera
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
    if(perspective_camera == NULL)
        return image_width;
    else
        return perspective_camera->width();
}

void QMLAR::setImageWidth(int new_width)
{
    image_width = new_width;
}

QQuickImageProvider *QMLAR::getImageProvider()
{
    return marker_backend;
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

QVariantList QMLAR::getBlobs()
{
    QVariantList result;
    QVector<QVector2D>::iterator it;
    for(it = last_blobs.begin(); it != last_blobs.end(); it++)
    {
        result << *it;
    }
    return result;
}

QVariantList QMLAR::getMarkers()
{
    QVariantList result{};

    QMap<int, Marker>::iterator it;
    for(it = marker_storage->begin(); it != marker_storage->end(); it++)
    {
        WorldImageCorrespondences c = it.value().getCorrespondences();
        if(c.size() != 4) continue;
        // positioning (mm)    (0, 0)    (0, s)       (s, 0)     (s, s)
        result << c.getImagePoint(0).toVector2D();
        result << c.getImagePoint(1).toVector2D();
        result << c.getImagePoint(3).toVector2D();
        result << c.getImagePoint(2).toVector2D();
    }

    return result;
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

void QMLAR::setBlobs(QVector<QVector2D> blobs)
{
    last_blobs = blobs;
    emit newBlobs();
}

void QMLAR::setMarkers(MarkerStorage &storage)
{
    *marker_storage = storage;
    emit newMarkers();
}

void QMLAR::hueAvailable(double mean, double std)
{ Q_UNUSED(mean) Q_UNUSED(std)
    // severing input to hsv_interval after first available result

    // X HSV -> thresholding
    disconnect(dynamic_cast<UchiyaMarkerDetector*>(detector), SIGNAL(dotsFound(QPair<QImage, QVector<QVector2D>>)),
            hsv_interval, SLOT(newPoints(QPair<QImage, QVector<QVector2D>>)));

    // X tracking -> blobs
    disconnect(tracking, SIGNAL(imageAvailable(QImage)), blob_detector, SLOT(setInput(QImage)));

    // X blobs -> markers
    disconnect(blob_detector, SIGNAL(imageAvailable(QImage)), detector, SLOT(setInput(QImage)));

    // tracking -> hsv
    connect(tracking, SIGNAL(imageAvailable(QImage)), hue_threshold, SLOT(setInput(QImage)));

    // hsv -> detector
    //connect(hue_threshold, SIGNAL(imageAvailable(QImage)), detector, SLOT(setInput(QImage)));
}

void QMLAR::connectAll()
{
    hue_threshold->setColor(0, 20);
//    hue_threshold->setS(50,100);
//    hue_threshold->setV(100,100);
    connect(scaler, &ImageProviderAsync::imageAvailable, hue_threshold, &HueThreshold::setInput);
    connect(hue_threshold, &HueThreshold::imageAvailable, marker_backend, &MarkerBackEnd::setPreview);
    connect(hue_threshold, &HueThreshold::imageAvailable, this, &QMLAR::imageUpdated);
    //connect(hue_threshold, &HueThreshold::imageAvailable, detector, &UchiyaMarkerDetector::setInput);

    //connect(detector, &UchiyaMarkerDetector::markersUpdated, mvp_provider, &MarkerMVPProvider::recompute);

    //connect(mvp_provider, &MarkerMVPProvider::newMVPMatrix, this, &QMLAR::setMVP);

//    // camera -> scaler
      connect(raw_provider, SIGNAL(imageAvailable(QImage)), scaler, SLOT(setInput(QImage)));

//    // camera -> QML
//    connect(raw_provider, SIGNAL(imageAvailable(QImage)), marker_backend, SLOT(setCamera(QImage)));

//    // scaler -> tracking
//    connect(scaler, SIGNAL(imageAvailable(QImage)), tracking, SLOT(setInput(QImage)));

//    // scaler -> resolution
      connect(scaler, SIGNAL(imageAvailable(QImage)), perspective_camera, SLOT(setResolution(QImage)));

//    // tracking -> blobs
//    connect(tracking, SIGNAL(imageAvailable(QImage)), blob_detector, SLOT(setInput(QImage)));

//    // tracking -> threshold
//    connect(tracking, SIGNAL(imageAvailable(QImage)), hue_threshold, SLOT(setInput(QImage)));

//    // blobs -> markers
//    connect(blob_detector, SIGNAL(imageAvailable(QImage)), detector, SLOT(setInput(QImage)));

//    // blobs -> QML
//    //connect(blob_detector, SIGNAL(blobsUpdated(QVector<QVector2D>)), this, SLOT(setBlobs(QVector<QVector2D>)));

//    // markers -> QML
//    //connect(detector, SIGNAL(previewUpdated(QImage)), marker_backend, SLOT(setPreview(QImage)));
//    //connect(detector, SIGNAL(markersUpdated(MarkerStorage&)), this, SLOT(setMarkers(MarkerStorage&)));

//    // threshold -> QML
//    connect(hue_threshold, SIGNAL(imageAvailable(QImage)), marker_backend, SLOT(setPreview(QImage)));

//    // markers -> MVP
//    connect(detector, SIGNAL(markersUpdated(MarkerStorage)), mvp_provider, SLOT(recompute(MarkerStorage)));

//    // markers -> tracking
//    connect(detector, SIGNAL(markersUpdated(MarkerStorage)), tracking, SLOT(onNewMarkers(MarkerStorage)));

//    // markers -> HSV detector
//    qRegisterMetaType<QPair<QImage, QVector<QVector2D>>>("QPair<QImage, QVector<QVector2D>>");
//    connect(dynamic_cast<UchiyaMarkerDetector*>(detector), SIGNAL(dotsFound(QPair<QImage, QVector<QVector2D>>)),
//            hsv_interval, SLOT(newPoints(QPair<QImage, QVector<QVector2D>>)), Qt::QueuedConnection);

//    // HSV -> this
//    connect(hsv_interval, SIGNAL(hAvailable(double,double)), this, SLOT(hueAvailable(double, double)));

//    // HSV -> thresholding
//    connect(hsv_interval, SIGNAL(hAvailable(double,double)), hue_threshold, SLOT(setColor(double,double)));
//    connect(hsv_interval, SIGNAL(sAvailable(double,double)), hue_threshold, SLOT(setS(double,double)));
//    connect(hsv_interval, SIGNAL(vAvailable(double,double)), hue_threshold, SLOT(setV(double,double)));

//    // mvp -> tracking
//    connect(mvp_provider, SIGNAL(newMVMatrix(QMatrix4x4)), tracking, SLOT(onNewMVMatrix(QMatrix4x4)));
//    connect(mvp_provider, SIGNAL(newPMatrix(QMatrix4x4)), tracking, SLOT(onNewPMatrix(QMatrix4x4)));

//    // mvp -> FPS
//    connect(detector, SIGNAL(markersUpdated(MarkerStorage)), this, SIGNAL(imageUpdated()));

//    // mvp -> imu
//    connect(mvp_provider, SIGNAL(newMVMatrix(QMatrix4x4)), mvp_imu_decorated, SLOT(setMV(QMatrix4x4)));
//    connect(mvp_provider, SIGNAL(newPMatrix(QMatrix4x4)), mvp_imu_decorated, SLOT(setP(QMatrix4x4)));

//    // output MVP matrix from IMU decorator
//    connect(mvp_imu_decorated, SIGNAL(newMVPMatrix(QMatrix4x4)), this, SLOT(setMVP(QMatrix4x4)));
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

    // allowing up to 10 parallel tasks
    QThreadPool::globalInstance()->setMaxThreadCount(10);

    // creating blob detector
    blob_detector = new BlobDetector(max_dots);

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
    tracking = new TrackingDecorator(predictor);

    // decorating MVP with IMU
    mvp_imu_decorated = new IMUMVPDecorator(imu);

    // creating image scaler
    scaler = new ImageScaler(image_width);

    // creating HSV interval detector
    hsv_interval = new HSVIntervalDetector(1000);

    // creating hsv thresholder
    hue_threshold = new HueThreshold();

    // connecting everything
    connectAll();

    // now the object is initialized
    is_initialized = true;
}
