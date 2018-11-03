/**
 * @file imumvpdecorator.h
 * @brief This class decorates an MVP provider by adding IMU delta movement
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef IMUMVPDECORATOR_H
#define IMUMVPDECORATOR_H

#include "mvpprovider.h"
#include "qml-imu/src/IMU.h"
#include <QElapsedTimer>
#include <QLinkedList>
#include <QTimer>

/**
 * @brief Pose with a timestamp
 */

class TimeStampedIMUPose {
public:

    /**
     * @brief The pose
     */
    QMatrix4x4 pose;

    /**
     * @brief Timestamp of the pose
     */
    qint64 timestamp;

    /**
     * @brief Construct from pose, remembers current time
     */
    TimeStampedIMUPose(QMatrix4x4 pose);
};

/**
 * @brief This class decorates an MVP providerby adding IMU delta movement
 *
 * If underlying MVP provider got matrix MV1 at t1
 * Then before next matrix MV2 at t2
 * this class will output a matrix based on MV1
 * with added rotation from IMU
 */

class IMUMVPDecorator : public MVPProvider
{ Q_OBJECT
private:
    /**
    * @brief Reset if no markers detected within this interval
    */
    int reset_ms;

    /**
    * @brief Underlying IMU object
    */
    IMU* imu;

    /**
    * @brief Last MV from provider
    */
    QMatrix4x4 last_mv;

    /**
    * @brief Last P
    */
    QMatrix4x4 last_p;

    /**
    * @brief Current IMU rotation
    */
    QMatrix4x4 current_imu_pose;

    /**
    * @brief Last pose from IMU at the
    * Moment last_mv was updated
    */
    QMatrix4x4 last_imu_pose;

    /**
    * @brief True if last_imu_pose is valid
    */
    bool last_imu_pose_available;

    /**
    * @brief Returns current IMU pose
    */
    QMatrix4x4 getCurrentIMUPose();

    /**
    * @brief Timer for resetting pose after a certain amount of seconds
    */
    QElapsedTimer since_update;

    /**
    * @brief Timer for checking if waited for too long
    */
    QTimer timer;

    /**
    * @brief Latency of mvp matrix
    */
    double mvp_latency;

    /**
    * @brief Poses from mvp
    */
    QLinkedList<TimeStampedIMUPose> imu_poses;

    /**
    * @brief Max freq * 2 seconds
    */
    static const int MAX_IMU_POSES = 2000;

    /**
    * @brief Get pose delay mseconds ago
    */
    QMatrix4x4 getDelayedIMUPose(int delay);

    /**
    * @brief IMU pose corrected by image latency
    */
    QMatrix4x4 getLatencyCorrectedIMUPose();

    /**
    * @brief Current delay mode
    */
    int delay_mode;
public:
    /**
    * @brief Decorate MVP provider and an IMU
    * @param imu IMU object pointer
    * @param delay_mode How to process data?
    * @see DelayMode
    */
    IMUMVPDecorator(IMU* imu, int delay_mode = DELAY_NONE);

    /**
    * @brief Delay mode enum
    */
    enum DelayMode {
        /// @brief Use current IMU readings
        DELAY_NONE,

        /// @brief Use old (-latency in time) IMU readings
        DELAY_ALL,

        /// @brief Transform resulting MVP by M_now - M_picture_taken
        DELAY_CORRECT

    };

    /** @brief Time w/o markers after which pose is discarded
     * And the scene is hidden */
    unsigned getResetMs();

public slots:
    /**
    * @brief Update resulting MVP matrix
    */
    void updatePose();

    /**
    * @brief Set matrices which to decorate with high-frequency IMU updates
    * @param p P matrix
    */
    void setP(PipelineContainer<QMatrix4x4> p);

    /**
    * @brief Set matrices which to decorate with high-frequency IMU updates
    * @param mv MV matrix
    */

    void setMV(PipelineContainer<QMatrix4x4> mv);

    /**
    * @brief Check if no MVP matrix obtained for too long
    */
    void checkIfTooLong();

    /**
    * @brief Latency of MVP matrix
    * @param latency latency in ms
    */
    void setMVPLatency(double latency);

    /**
    * @brief Set IMU rotation
    * @param imu_rotation rotation by IMU now
    */
    void IMUUpdated(QQuaternion imu_rotation);

    /**
     * @brief setResetMs Set the time in ms after which
     * the pose is considered invalid if no MVP updates
     * from markers are given
     * @param reset_ms The delay in ms
     */
    void setResetMs(unsigned reset_ms);
};

#endif // IMUMVPDECORATOR_H


