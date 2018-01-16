#include "config.h"
#include "trackingdecorator.h"
#include <QPainter>
#include <QtCore>
#include <QPen>
#include <QColor>
#include <QRect>

TrackingDecorator::TrackingDecorator(MarkerDetector* detector, PosePredictor *predictor, MVPProvider *provider) :
    MarkerDetector()
{
    Q_ASSERT(predictor != NULL);
    Q_ASSERT(provider != NULL);
    Q_ASSERT(detector != NULL);

    // saving arguments
    this->predictor = predictor;
    this->provider = provider;
    this->detector = detector;

    // updating current pose on new MVP
    connect(provider, SIGNAL(newMVPMatrix()), this, SLOT(onNewMVPMatrix()));

    // patch the connection from detector to provider via this object
    disconnect(detector, SIGNAL(markersUpdated()), (MarkerMVPProvider*) provider, SLOT(recompute()));
    connect(detector, SIGNAL(markersUpdated()), this, SLOT(detectorMarkersUpdated()));
    connect(this, SIGNAL(markersUpdated()), (MarkerMVPProvider*) provider, SLOT(recompute()));
}

void TrackingDecorator::process()
{
    // obtaining predicted pose
    Pose predictedPose = predictor->predictPose();

    // obtaining new MVP
    QMatrix4x4 MVP = provider->getPMatrix() * predictedPose.get4Matrix();

    // obtaining image correspondences
    WorldImageCorrespondences correspondences = detector->getCorrespondences();

    // corners of the bounding box of markers
    double x_min = input_buffer.width();
    double x_max = 0;
    double y_min = input_buffer.height();
    double y_max = 0;

    // mapping correspondences to image coordinate system
    for(int i = 0; i < correspondences.size(); i++)
    {
        // original 3D world point
        QVector3D world_point = correspondences.getWorldPoint(i);
        QVector3D image_point = correspondences.getImagePoint(i);
        QVector4D world_point_4d = QVector4D(world_point);
        world_point_4d.setW(1);

        // predicted by predictor image point
        QVector4D predicted_image_point_4d = MVP.map(world_point_4d);
        QVector3D predicted_image_point = predicted_image_point_4d.toVector3D();

        // affine transform
        predicted_image_point.setX(predicted_image_point.x() / predicted_image_point.z());
        predicted_image_point.setY(predicted_image_point.y() / predicted_image_point.z());

        // OpenGL NDC -> image coordinates
        predicted_image_point.setX((predicted_image_point.x() + 1) / 2. * input_buffer.width());
        predicted_image_point.setY((1 - predicted_image_point.y()) / 2. * input_buffer.height());

        if(predicted_image_point.x() > x_max)
            x_max = predicted_image_point.x();
        if(predicted_image_point.x() < x_min)
            x_min = predicted_image_point.x();
        if(predicted_image_point.y() > y_max)
            y_max = predicted_image_point.y();
        if(predicted_image_point.y() < y_min)
            y_min = predicted_image_point.y();

        TimeLoggerLog("Expecting point (%.2f %.2f %.2f) at (%.2f %.2f), was at (%.2f, %.2f)", world_point.x(),
                      world_point.y(), world_point.z(), predicted_image_point.x(), predicted_image_point.y(),
                      image_point.x(), image_point.y())
    }

    // removing all but the selected rectangle

    // setting input to underlying detector
    detector->setInput(input_buffer);

    // TODO: blacken parts of the images w/o possible markers

    // doing marker detection
    // MVP will be updated if markers
    // were detected
    detector->process();

    // obtaining preview
    output_buffer = detector->getPreview();


    if(y_min < y_max && x_min < x_max)
    {
        QImage augmented_input = input_buffer;
        QPainter p(&augmented_input);
        p.setBrush(QBrush(Qt::black));
        p.drawRect(0, 0, input_buffer.width(), y_min);
        p.drawRect(0, y_max, input_buffer.width(), input_buffer.height() - y_max);
        p.drawRect(0, y_min, x_min, y_max - y_min);
        p.drawRect(x_max, y_min, input_buffer.width() - x_max, y_max - y_min);
        output_buffer = augmented_input;
    }

    // painting markers bounding box
//    QPainter p(&output_buffer);
//    p.setPen(QPen(Qt::black));
//    if(y_min < y_max && x_min < x_max) p.drawRect(x_min, y_min, x_max - x_min, y_max - y_min);
}

QImage TrackingDecorator::getLastInput()
{
    return input_buffer;
}

QMap<int, Marker>::iterator TrackingDecorator::begin()
{
    return detector->begin();
}

QMap<int, Marker>::iterator TrackingDecorator::end()
{
    return detector->end();
}

WorldImageCorrespondences TrackingDecorator::getCorrespondences()
{
    return detector->getCorrespondences();
}

void TrackingDecorator::onNewMVPMatrix()
{
    // adding this pose to the predictor
    Pose current_pose = Pose(provider->getMVMatrix());
    predictor->setCurrentPose(current_pose);
}

void TrackingDecorator::detectorMarkersUpdated()
{

}
