#include "qmlar.h"
#include "timelogger.h"
#include "qml-imu/src/IMU.h"
#include "qvector3d.h"

QMLAR::QMLAR()
{
    // initially, object is not initialized
    is_initialized = false;
    camera_id = -2;
    image_filename = "";
    raw_provider = NULL;
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
    initialize();
}

void QMLAR::setImageFilename(QString filename)
{
    Q_ASSERT(!is_initialized);
    TimeLoggerLog("Opening image %s", filename.toStdString().c_str());
    image_filename = filename;
    raw_provider = new ImageBackend(filename);
    initialize();
}

void QMLAR::setQMLCamera(QObject *camera)
{
    Q_ASSERT(camera != NULL);
    TimeLoggerLog("%s", "Setting camera from QML camera");
    QCamera* camera_ = qvariant_cast<QCamera*>(camera->property("mediaObject"));
    raw_provider = new QtCameraBackend(camera_);
    this->qml_camera = camera;
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

void QMLAR::update()
{
    // doing nothing if not initialized
    if(!is_initialized) return;

    // requesting image from provider as pixmap
    QPixmap input = raw_provider->requestPixmap("raw", NULL, QSize());

    // obtaining source image
    QImage source = input.toImage();

    // if no image is available, do nothing
    if(source.width() * source.height() <= 0)
        return;

    // scaling it if necessary
    if(image_width != 0)
        source = source.scaledToWidth(image_width);

    // send input to marker detector
    detector->setInput(source);

    // detect markers
    detector->process();
}

double QMLAR::getUpdateMS()
{
    return timer.interval();
}

void QMLAR::setUpdateMS(double value)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(value);
}

QObject *QMLAR::getQMLCamera()
{
    return qml_camera;
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

    // adding UchiyaBackEnd (decorating camera object)
    marker_backend.initialize(detector);

    // creating a ModelView provider
    mvp_provider = new MarkerMVPProvider(detector, perspective_camera);

    // decorating MVP with IMU
    mvp_imu_decorated = new IMUMVPDecorator(mvp_provider, imu);

    // notify QML on each update of MVP matrix from IMU
    connect(mvp_imu_decorated, SIGNAL(newMVPMatrix()), this, SLOT(newMVPMatrixSlot()));

    // now the object is initialized
    is_initialized = true;
}
