#include "timelogger.h"
#include "imumvpdecorator.h"

IMUMVPDecorator::IMUMVPDecorator(IMU *imu)
{
    // saving provider and imu
    Q_ASSERT(imu != NULL);
    this->imu = imu;

    // no pose available for now
    // will be set on first MVP from provider
    last_imu_pose_available = false;

    // update resulting MVP on new pose from IMU
    connect(imu, SIGNAL(stateChanged()), this, SLOT(updatePose()));

    // reset after 3 seconds
    reset_ms = 3000;

    // checking if waited for too long
    connect(&timer, SIGNAL(timeout()), this, SLOT(checkIfTooLong()));
    timer.start(500);
}

void IMUMVPDecorator::setP(QMatrix4x4 p)
{
    last_p = p;
}

void IMUMVPDecorator::setMV(QMatrix4x4 mv)
{
    TimeLoggerLog("Updating MVP from Provider valid = %d", isValid(mv))

    if(!isValid(mv))
    {
        // reset our matrix if no IMU available
        if(!imu->isStartupComplete()) reset();

        // do nothing if pose invalid and on Android
        return;
    }

    // remembering current time
    since_update.start();

    // MV from provider
    last_mv = mv;

    // forgetting previous displacement vector
    imu->resetDisplacement();

    // obtaining current pose
    last_imu_pose = getCurrentPose();

    // now the pose is valid
    last_imu_pose_available = true;

    // calculating resulting MVP
    updatePose();
}

void IMUMVPDecorator::checkIfTooLong()
{
    // resetting MVP matrix (waited too long)
    if(since_update.elapsed() > reset_ms)
    {
        TimeLoggerLog("%s", "Waited for too long w/o markers");
        reset();
        last_imu_pose_available = false;
        return;
    }
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
    QMatrix4x4 mvp_new = last_p * delta_mv * last_mv;

    // telling others about update
    setMVPMatrix(mvp_new);
}
