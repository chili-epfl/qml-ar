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
}

void TrackingDecorator::process()
{
    // obtaining predicted pose
    Pose predictedPose = predictor->predictPose();

    // obtaining new MVP
    QMatrix4x4 MVP = provider->getPMatrix() * predictedPose.get4Matrix();

    // obtaining image correspondences
    WorldImageCorrespondences correspondences = detector->getCorrespondences();

    //output_buffer = input_buffer;
    //QPainter p(&output_buffer);

    //p.setBrush(QBrush(Qt::black));

//    double left = input_buffer.width();
//    double right = 0;
//    double top = 0;
//    double bottom = input_buffer.height();

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

//        if(predicted_image_point.x() > top)
//            top = predicted_image_point.x();
//        if(predicted_image_point.x() < bottom)
//            bottom = predicted_image_point.x();
//        if(predicted_image_point.y() < left)
//            left = predicted_image_point.y();
//        if(predicted_image_point.y() > right)
//            right = predicted_image_point.y();

        //p.drawEllipse(predicted_image_point.x(), predicted_image_point.y(), 10, 10);

        TimeLoggerLog("Expecting point (%.2f %.2f %.2f) at (%.2f %.2f), was at (%.2f, %.2f)", world_point.x(),
                      world_point.y(), world_point.z(), predicted_image_point.x(), predicted_image_point.y(),
                      image_point.x(), image_point.y())
    }

    //p.drawRect(left, top, right - left, bottom - top);

    // setting input to underlying detector
    detector->setInput(input_buffer);

    // TODO: blacken parts of the images w/o possible markers

    // doing marker detection
    // MVP will be updated if markers
    // were detected
    detector->process();

    // obtaining preview
    output_buffer = detector->getPreview();
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
