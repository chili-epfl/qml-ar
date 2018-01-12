#include "imumvpdecorator.h"

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
    // MV from provider
    last_mv = provider->getMVMatrix();

    // obtaining current pose
    last_imu_pose = getCurrentPose();
    last_imu_pose_available = true;

    // calculating resulting MVP
    updatePose();
}

QMatrix4x4 IMUMVPDecorator::getCurrentPose()
{
    // obtaining rotation from IMU
    QMatrix4x4 res;
    res.setToIdentity();
    res.rotate(imu->getRotAngle(), imu->getRotAxis());
    return res;
}

void IMUMVPDecorator::updatePose()
{
    // no pose => no MV from provider => nothing
    if(!last_imu_pose_available) return;

    // difference in pose since last MV from provider
    QMatrix4x4 delta_mv = getCurrentPose() * last_imu_pose.inverted();;

    // new MVP
    QMatrix4x4 mvp_new = provider->getPMatrix() * delta_mv * last_mv;

    // telling others about update
    setMVPMatrix(mvp_new);
}
