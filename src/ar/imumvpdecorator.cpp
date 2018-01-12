#include "imumvpdecorator.h"

IMUMVPDecorator::IMUMVPDecorator(MVPProvider *mvp_provider, IMU *imu)
{
    Q_ASSERT(mvp_provider != NULL);
    Q_ASSERT(imu != NULL);
    this->provider = mvp_provider;
    this->imu = imu;
    last_imu_pose_available = false;
    connect(mvp_provider, SIGNAL(newMVPMatrix(), this, SLOT(updateLastMV()));
}

void IMUMVPDecorator::updateLastMV()
{
    last_mv = provider->getMVMatrix();
}

QMatrix4x4 IMUMVPDecorator::getCurrentPose()
{
    QMatrix4x4 res;
    res.setToIdentity();
    res.rotate(IMU.getRotAngle(), IMU.getRotAxis());
    return res;
}

void IMUMVPDecorator::updatePose()
{
    QMatrix4x4 mvp_new;
    QMatrix4x4 delta_mv;
    delta_mv.setToIdentity();

    QMatrix4x4 current_imu_pose = getCurrentPose();

    if(last_imu_pose_available)
    {
        delta_mv = current_imu_pose * last_imu_pose.inverted();
    }
    else
    mvp_new = provider->getPMatrix() * delta_mv * last_mv;

    last_imu_pose = current_imu_pose;
    last_imu_pose_available = true;

}
