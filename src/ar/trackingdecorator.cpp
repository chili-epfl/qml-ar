#include "timelogger.h"
#include "trackingdecorator.h"
#include <QPainter>
#include <QtCore>
#include <QPen>
#include <QColor>
#include <QRect>

TrackingDecorator::TrackingDecorator(PosePredictor *predictor) : ImageProviderAsync()
{
    Q_ASSERT(predictor != NULL);

    // saving arguments
    this->predictor = predictor;

    // initially, not using the region
    use_region = false;
}

void TrackingDecorator::setInput(QImage img)
{
    TimeLoggerLog("%s", "[ANALYZE] Begin Blacken");
    QImage blackened = blacken(img);
    TimeLoggerLog("%s", "[ANALYZE] End Blacken");
    emit imageAvailable(blackened);
}

void TrackingDecorator::onNewPMatrix(QMatrix4x4 p)
{
    P = p;
}

void TrackingDecorator::onNewMVMatrix(QMatrix4x4 mv)
{
    predictor->setCurrentPose(mv);
}

void TrackingDecorator::onNewMarkers(MarkerStorage storage)
{
    use_region = storage.markersDetected();
    this->storage = storage;
}

QImage TrackingDecorator::blacken(QImage source)
{
    if(!use_region)
        return source;

    TimeLoggerLog("%s", "Obtaining marker location");
    // obtaining predicted pose
    Pose predictedPose = predictor->predictPose();

    // obtaining new MVP
    QMatrix4x4 MVP = P * predictedPose.get4Matrix();

    // obtaining image correspondences
    WorldImageCorrespondences correspondences = storage.getCorrespondences();

    // corners of the bounding box of markers
    double x_min = source.width();
    double x_max = 0;
    double y_min = source.height();
    double y_max = 0;

    // mapping correspondences to image coordinate system
    for(int i = 0; i < correspondences.size(); i++)
    {
        // original 3D world point
        QVector3D world_point = correspondences.getWorldPoint(i);
        QVector4D world_point_4d = QVector4D(world_point);
        world_point_4d.setW(1);

        // predicted by predictor image point
        QVector4D predicted_image_point_4d = MVP.map(world_point_4d);
        QVector3D predicted_image_point = predicted_image_point_4d.toVector3D();

        // affine transform
        predicted_image_point.setX(predicted_image_point.x() / predicted_image_point.z());
        predicted_image_point.setY(predicted_image_point.y() / predicted_image_point.z());

        // OpenGL NDC -> image coordinates
        predicted_image_point.setX((predicted_image_point.x() + 1) / 2. * source.width());
        predicted_image_point.setY((1 - predicted_image_point.y()) / 2. * source.height());

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

    QImage augmented_input = source;

    TimeLoggerLog("%s", "Blackening input");
    // removing all but the selected rectangle
    if(y_min < y_max && x_min < x_max)
    {
        // copying the image
        QPainter p(&augmented_input);

        // painting all but marker region with black
        p.setBrush(QBrush(Qt::black));
        p.drawRect(0, 0, source.width(), y_min);
        p.drawRect(0, y_max, source.width(), source.height() - y_max);
        p.drawRect(0, y_min, x_min, y_max - y_min);
        p.drawRect(x_max, y_min, source.width() - x_max, y_max - y_min);

        return augmented_input;
    }

    return source;
}
