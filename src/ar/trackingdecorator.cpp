#include "config.h"
#include "trackingdecorator.h"

TrackingDecorator::TrackingDecorator(MarkerDetector* detector, PosePredictor *predictor, MVPProvider *provider) :
    MarkerDetector()
{
    Q_ASSERT(predictor != NULL);
    Q_ASSERT(provider != NULL);
    Q_ASSERT(detector != NULL);
    this->predictor = predictor;
    this->provider = provider;
    this->detector = detector;
    connect(provider, SIGNAL(newMVPMatrix()), this, SLOT(onNewMVPMatrix()));
}

void TrackingDecorator::process()
{
    Pose predictedPose = predictor->predictPose();
    QMatrix4x4 MVP = provider->getPMatrix() * predictedPose.get4Matrix();
    WorldImageCorrespondences correspondences = detector->getCorrespondences();
    for(int i = 0; i < correspondences.size(); i++)
    {
        QVector3D world_point = correspondences.getWorldPoint(i);
        QVector3D predicted_image_point = MVP.map(correspondences.getImagePoint(i));
        predicted_image_point.setX(predicted_image_point.x() / predicted_image_point.z());
        predicted_image_point.setY(predicted_image_point.y() / predicted_image_point.z());
        TimeLoggerLog("Expecting point (%.2f %.2f %.2f) at (%.2f %.2f)", world_point.x(),
                      world_point.y(), world_point.z(), predicted_image_point.x(), predicted_image_point.y())
    }
    detector->process();
}

QImage TrackingDecorator::getPreview()
{
    return detector->getPreview();
}

QImage TrackingDecorator::getLastInput()
{
    return detector->getLastInput();
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
    Pose current_pose = Pose(provider->getMVMatrix());
    predictor->setCurrentPose(current_pose);
}
