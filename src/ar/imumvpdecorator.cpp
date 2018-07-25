/**
 * @file imumvpdecorator.cpp
 * @brief This class decorates an MVP provider by adding IMU delta movement
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "timelogger.h"
#include "imumvpdecorator.h"
#include <QtMath>

IMUMVPDecorator::IMUMVPDecorator(IMU *imu, int delay_mode)
{
    // saving provider and imu
    Q_ASSERT(imu != NULL);
    this->imu = imu;

    // no pose available for now
    // will be set on first MVP from provider
    last_imu_pose_available = false;

    // reset after 3 seconds
    reset_ms = 3000;

    // mvp matrix latency
    mvp_latency = 0;

    // saving delay mode
    this->delay_mode = delay_mode;

    // update resulting MVP on new pose from IMU
    connect(imu, SIGNAL(rotationChanged(QQuaternion)), this, SLOT(IMUUpdated(QQuaternion)));

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

    // setting latency to object latency
    setMVPLatency(QDateTime::currentMSecsSinceEpoch() - mv.info().start());

    // id from provider
    object_in_process = mv.info();

    // obtaining current pose
    if(delay_mode == DELAY_NONE)
    {
        // approach 0: use current pose
        last_imu_pose = getCurrentIMUPose();
    }
    else if(delay_mode == DELAY_ALL || delay_mode == DELAY_CORRECT)
    {
        // approach 1, 2: delay IMU on MVP save
        last_imu_pose = getLatencyCorrectedIMUPose();
    }

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

void IMUMVPDecorator::setMVPLatency(double latency)
{
    mvp_latency = latency;
}

void IMUMVPDecorator::IMUUpdated(QQuaternion imu_rotation)
{
    // resulting 4x4 rotation matrix
    QMatrix4x4 res;

    // initializing with I
    res.setToIdentity();

    // obtaining rotation axis
    float x, y, z, theta;
    imu_rotation.getAxisAndAngle(&x, &y, &z, &theta);

    // new axis in OpenCV coordinate system
    // swapping x and y
    QVector3D new_axis(y, x, z);

    // rotating matrix by (axis, angle)
    res.rotate(qRadiansToDegrees(theta), new_axis);

    // saving
    current_imu_pose = res;

    // updating output
    updatePose();
}

QMatrix4x4 IMUMVPDecorator::getLatencyCorrectedIMUPose()
{
    return getDelayedIMUPose(qRound(mvp_latency));
}

QMatrix4x4 IMUMVPDecorator::getDelayedIMUPose(int delay)
{
    Q_ASSERT(!imu_poses.isEmpty());

    QLinkedList<TimeStampedIMUPose>::iterator current = imu_poses.begin();
    QLinkedList<TimeStampedIMUPose>::iterator it;

    // searching for closest IMU pose
    for(it = current; it != imu_poses.end(); it++)
    {
        if((*current).timestamp - (*it).timestamp > delay)
        {
            // choosing min over current and prev
            int delta1 = (*current).timestamp - (*it).timestamp;
            int delta2 = (*current).timestamp - (*(it - 1)).timestamp;

            // will do it-- later
            if(delta1 < delta2) it++;
            break;
        }
    }

    // one back
    it--;
    Q_ASSERT(it != imu_poses.end());

    // returning past pose
    return (*it).pose;
}

QMatrix4x4 IMUMVPDecorator::getCurrentIMUPose()
{
    // identity if no pose available
    if(!imu->isStartupComplete())
        return QMatrix4x4();

    return current_imu_pose;
}

void IMUMVPDecorator::updatePose()
{
    QMatrix4x4 current_imu = getCurrentIMUPose();

    // updating imu_poses buffer
    imu_poses.push_front(current_imu);
    if(imu_poses.size() > MAX_IMU_POSES)
        imu_poses.pop_back();

    // no pose => no MV from provider => nothing
    if(!last_imu_pose_available) return;

    // obtaining curre tpose
    QMatrix4x4 use_imu_pose;
    if(delay_mode == DELAY_NONE || delay_mode == DELAY_CORRECT)
    {
        // approach 0, 2: current pose
        use_imu_pose = getCurrentIMUPose();
    }
    else if(delay_mode == DELAY_ALL)
    {
        // approach 1: old pose
        use_imu_pose = getLatencyCorrectedIMUPose();
    }

    // difference in pose since last MV from provider
    QMatrix4x4 delta_mv = use_imu_pose * last_imu_pose.inverted();

    // new MVP
    mv_matrix = delta_mv * last_mv;
    p_matrix = last_p;

    // telling others about update
    setMVPMatrix(p_matrix * mv_matrix);
}

TimeStampedIMUPose::TimeStampedIMUPose(QMatrix4x4 pose)
{
    this->pose = pose;
    this->timestamp = QDateTime::currentMSecsSinceEpoch();
}
