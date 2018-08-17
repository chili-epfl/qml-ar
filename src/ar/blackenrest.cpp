/**
 * @file blackenrest.cpp
 * @brief Paints image parts not containing markers in black
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "timelogger.h"
#include "blackenrest.h"
#include <QPainter>
#include <QtCore>
#include <QPen>
#include <QColor>
#include <QRect>
#include <QPointF>

BlackenRest::BlackenRest(PosePredictor *predictor) : ImageProviderAsync()
{
    Q_ASSERT(predictor != NULL);

    // saving arguments
    this->predictor = predictor;

    // disable newPolygon() by default
    this->usePolygon = false;

    // initially, not using the region
    use_region = false;
}

void BlackenRest::setInput(PipelineContainer<QImage> img)
{
    object_in_process = img.info();
    TimeLoggerThroughput("%s", "[ANALYZE] Begin Blacken");
    QImage blackened = blacken(img.o());
    TimeLoggerThroughput("%s", "[ANALYZE] End Blacken");

    emit imageAvailable(PipelineContainer<QImage>
                        (blackened,object_in_process.checkpointed("BlackenRest")));
}

void BlackenRest::setUsePolygon(bool value)
{
    usePolygon = value;
}

void BlackenRest::onNewPMatrix(PipelineContainer<QMatrix4x4> p)
{
    P = p.o();
}

void BlackenRest::onNewMVMatrix(PipelineContainer<QMatrix4x4> mv)
{
    predictor->setCurrentPose(mv.o());
    MV = mv.o();
}

void BlackenRest::calculatePolygon() {

    // obtaining current MVP
    QMatrix4x4 MVP = P * MV;

    // obtaining image correspondences
    WorldImageCorrespondences correspondences = storage.getCorrespondences();

    // resulting polygon containing marker
    QPolygonF marker;

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

        // OpenGL NDC -> 0-1 image coordinates
        predicted_image_point.setX((predicted_image_point.x() + 1) / 2.);
        predicted_image_point.setY((1 - predicted_image_point.y()) / 2.);

        // adding point as marker corner
        marker.append(QPointF(predicted_image_point.x(), predicted_image_point.y()));
    }

    // telling listeners about new marker
    emit newPolygon(marker);
}

void BlackenRest::onNewMarkers(PipelineContainer<MarkerStorage> storage)
{
    use_region = storage.o().markersDetected();
    this->storage = storage.o();
    if(use_region && usePolygon)
        calculatePolygon();
}

QImage BlackenRest::blacken(QImage source)
{
    if(!use_region)
        return source;

    // obtaining predicted pose
    Pose predictedPose = predictor->predictPose();

    // obtaining new MVP
    QMatrix4x4 MVP = P * predictedPose.get4Matrix();

    // obtaining image correspondences
    WorldImageCorrespondences correspondences = storage.getCorrespondences();

    // resulting polygon containing marker
    QPolygon marker;

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

        // adding point as marker corner
        marker.append(QPoint(predicted_image_point.x(), predicted_image_point.y()));
    }

    // blackened output
    QImage augmented_input = source;

    // removing all but the selected rectangle
    if(marker.size() > 0)
    {
        // copying the image
        QPainter p(&augmented_input);

        // all image selection
        QPainterPath pp_all;
        pp_all.addRect(0, 0, source.width(), source.height());

        // marker selection
        QPainterPath pp;
        pp.addPolygon(marker);

        // want to blacken
        p.setBrush(QBrush(Qt::black));

        // blackening all but marker
        p.drawPath(pp_all - pp);

        // returning resulting image
        return augmented_input;
    }

    return source;
}
