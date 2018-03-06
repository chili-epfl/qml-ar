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

    // reset after 3 seconds
    reset_ms = 3000;

    // update resulting MVP on new pose from IMU
    connect(imu, SIGNAL(stateChanged()), this, SLOT(updatePose()));

    timer.setParent(this);

    // checking if waited for too long
    connect(&timer, SIGNAL(timeout()), this, SLOT(checkIfTooLong()));
    timer.start(500);
}

void IMUMVPDecorator::setP(PipelineContainer<QMatrix4x4> p)
{
    last_p = p;
}

void IMUMVPDecorator::setMV(PipelineContainer<QMatrix4x4> mv)
{
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
    last_mv = mv.o();

    // id from provider
    object_in_process = mv.info();

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

    return res;
}

void IMUMVPDecorator::updatePose()
{
    // no pose => no MV from provider => nothing
    if(!last_imu_pose_available) return;

    // difference in pose since last MV from provider
    QMatrix4x4 delta_mv = getCurrentPose() * last_imu_pose.inverted();

    // new MVP
    mv_matrix = delta_mv * last_mv;
    p_matrix = last_p;

    // telling others about update
    setMVPMatrix(p_matrix * mv_matrix);
}
