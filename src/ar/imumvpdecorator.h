#ifndef IMUMVPDECORATOR_H
#define IMUMVPDECORATOR_H

#include "qml-imu/src/IMU.h"
#include "mvpprovider.h"

/*
 * This class decorates an MVP provider
 * by adding IMU delta movement
 * If underlying MVP provider got matrix MV1 at t1
 * Then before next matrix MV2 at t2
 * this class will output a matrix based on MV1
 * with added rotation from IMU
 */

class IMUMVPDecorator : public MVPProvider
{ Q_OBJECT
private:
    // underlying MVP provider
    MVPProvider* provider;

    // underlying IMU object
    IMU* imu;

    // last MV from provider
    QMatrix4x4 last_mv;

    // last pose from IMU at the
    // moment last_mv was updated
    QMatrix4x4 last_imu_pose;

    // true if last_imu_pose is valid
    bool last_imu_pose_available;

    // returns current IMU pose
    QMatrix4x4 getCurrentPose();
public:
    // decorate MVP provider and an IMU
    IMUMVPDecorator(MVPProvider* mvp_provider, IMU* imu);

public slots:
    // set MV matrix from provider
    void updateLastMV();

    // update resulting MVP matrix
    void updatePose();
};

#endif // IMUMVPDECORATOR_H
