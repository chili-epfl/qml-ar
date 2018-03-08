#ifndef IMUMVPDECORATOR_H
#define IMUMVPDECORATOR_H

#include "mvpprovider.h"
#include "qml-imu/src/IMU.h"
#include <QElapsedTimer>
#include <QLinkedList>
#include <QTimer>

/*
 * This class decorates an MVP provider
 * by adding IMU delta movement
 * If underlying MVP provider got matrix MV1 at t1
 * Then before next matrix MV2 at t2
 * this class will output a matrix based on MV1
 * with added rotation from IMU
 */

// pose with a timestamp
class TimeStampedIMUPose {
public:
    QMatrix4x4 pose;
    qint64 timestamp;
    TimeStampedIMUPose(QMatrix4x4 pose);
};

class IMUMVPDecorator : public MVPProvider
{ Q_OBJECT
private:
    // reset if no markers detected within this interval
    int reset_ms;

    // underlying IMU object
    IMU* imu;

    // last MV from provider
    QMatrix4x4 last_mv;

    // last P
    QMatrix4x4 last_p;

    // last pose from IMU at the
    // moment last_mv was updated
    QMatrix4x4 last_imu_pose;

    // true if last_imu_pose is valid
    bool last_imu_pose_available;

    // returns current IMU pose
    QMatrix4x4 getCurrentIMUPose();

    // timer for resetting pose after a certain amount of seconds
    QElapsedTimer since_update;

    // timer for checking if waited for too long
    QTimer timer;

    // latency of mvp matrix
    double mvp_latency;

    // Poses from mvp
    QLinkedList<TimeStampedIMUPose> imu_poses;

    // max freq * 2 seconds
    static const int MAX_IMU_POSES = 2000;

    // get pose delay mseconds ago
    QMatrix4x4 getDelayedIMUPose(int delay);

    // IMU pose corrected by image latency
    QMatrix4x4 getLatencyCorrectedIMUPose();

    // current delay mode
    int delay_mode;
public:
    // decorate MVP provider and an IMU
    IMUMVPDecorator(IMU* imu, int delay_mode = DELAY_NONE);

    // delay mode
    // None: use current IMU readings
    // All: use old (-latency in time) IMU readings
    // Correct: transform resulting MVP by M_now - M_picture_taken
    enum DelayMode {DELAY_NONE, DELAY_ALL, DELAY_CORRECT};

public slots:
    // update resulting MVP matrix
    void updatePose();

    // set matrices which to decorate
    // with high-frequency IMU updates
    void setP(PipelineContainer<QMatrix4x4> p);
    void setMV(PipelineContainer<QMatrix4x4> mv);

    // check if no MVP matrix obtained
    // for too long
    void checkIfTooLong();

    // latency of MVP matrix
    void setMVPLatency(double latency);
};

#endif // IMUMVPDECORATOR_H
