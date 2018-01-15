#include "imumvpdecorator.h"
#include "config.h"

IMUMVPDecorator::IMUMVPDecorator(MVPProvider *mvp_provider, IMU *imu)
{
    // saving provider and imu
    Q_ASSERT(mvp_provider != NULL);
    Q_ASSERT(imu != NULL);
    this->provider = mvp_provider;
    this->imu = imu;

    // no pose available for now
    // will be set on first MVP from provider
    last_imu_pose_available = false;

    // update our MVP on matrix from provider
    connect(mvp_provider, SIGNAL(newMVPMatrix()), this, SLOT(updateLastMV()));

    // update resulting MVP on new pose from IMU
    connect(imu, SIGNAL(stateChanged()), this, SLOT(updatePose()));
}

void IMUMVPDecorator::updateLastMV()
{
    TimeLoggerLog("Updating MVP from Provider valid = %d", provider->isValid());

    if(!provider->isValid())
    {
        // reset our matrix if no IMU available
//        if(imu->getRotAxis().length() < 1e-5) reset();
        if(!imu->isStartupComplete()) reset();

        // do nothing if pose invalid and on Android
        return;
    }

    // MV from provider
    last_mv = provider->getMVMatrix();

    // forgetting previous displacement vector
    imu->resetDisplacement();

    // obtaining current pose
    last_imu_pose = getCurrentPose();

    // now the pose is valid
    last_imu_pose_available = true;

    // calculating resulting MVP
    updatePose();
}

QMatrix4x4 IMUMVPDecorator::getCurrentPose()
{
    // resulting 4x4 rotation matrix
    QMatrix4x4 res;

    // initializing with I
    res.setToIdentity();

    // identity if no pose available
    if(!imu->isStartupComplete())
        return res;

    // obtaining rotation axis
    QVector3D axis = imu->getRotAxis();

    // new axis in OpenCV coordinate system
    QVector3D new_axis;

    // swapping x and y
    new_axis.setX(axis.y());
    new_axis.setY(axis.x());

    // keeping z
    new_axis.setZ(axis.z());

    // rotating matrix by (axis, angle)
    res.rotate(imu->getRotAngle(), new_axis);

    QVector3D displacement = imu->getLinearDisplacement();
    TimeLoggerLog("Displacement dx %.2f dy %.2f dz %.2f", displacement.x(), displacement.y(), displacement.z());

    return res;
}

void IMUMVPDecorator::updatePose()
{
    // no pose => no MV from provider => nothing
    if(!last_imu_pose_available) return;

    TimeLoggerLog("%s", "Updating MVP from IMU");

    // difference in pose since last MV from provider
    QMatrix4x4 delta_mv = getCurrentPose() * last_imu_pose.inverted();;

    // new MVP
    QMatrix4x4 mvp_new = provider->getPMatrix() * delta_mv * last_mv;

    // telling others about update
    setMVPMatrix(mvp_new);
}
