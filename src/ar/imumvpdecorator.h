#ifndef IMUMVPDECORATOR_H
#define IMUMVPDECORATOR_H

#include "qml-imu/src/IMU.h"
#include "mvpprovider.h"

class IMUMVPDecorator : public MVPProvider
{ Q_OBJECT
private:
    MVPProvider* provider;
    IMU* imu;
    QMatrix4x4 last_mv;
    QMatrix4x4 last_imu_pose;
    bool last_imu_pose_available;
    QMatrix4x4 getCurrentPose();
public:
    IMUMVPDecorator(MVPProvider* mvp_provider, IMU* imu);

public slots:
    void updateLastMV();
    void updatePose();
};

#endif // IMUMVPDECORATOR_H
