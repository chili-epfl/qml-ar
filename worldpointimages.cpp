#include "worldpointimages.h"
#include "calibratedcamera.h"
#include <QMatrix4x4>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <QtOpenCV/cvmatandqimage.h>

WorldPointImages::WorldPointImages()
{

}

void WorldPointImages::assertValid()
{
    Q_ASSERT(world_points.size() == image_points.size());
}

void WorldPointImages::add(QVector3D world_point, QVector2D image_point)
{
    world_points.append(world_point);
    image_points.append(image_point);
    assertValid();
}

void WorldPointImages::join(WorldPointImages &that)
{
    int length = that.world_points.size();
    for(int i = 0; i < length; i++)
    {
        world_points.append(that.world_points.at(i));
        image_points.append(that.image_points.at(i));
    }
    assertValid();
}

void WorldPointImages::clear()
{
    world_points.clear();
    image_points.clear();
    assertValid();
}

int WorldPointImages::size()
{
    return world_points.size();
}

QMatrix4x4 WorldPointImages::computePnP(CalibratedCamera* camera)
{
    Q_ASSERT(camera != NULL);
    Q_ASSERT(size() > 0);

    int points_n = size();

    qDebug() << "Size: " << size();

    cv::Mat cv_world_points(points_n, 3, CV_64F);
    cv::Mat cv_image_points(points_n, 2, CV_64F);

    for(int i = 0; i < points_n; i++)
    {
        cv_world_points.at<double>(i, 0) = world_points.at(i).x();
        cv_world_points.at<double>(i, 1) = world_points.at(i).y();
        cv_world_points.at<double>(i, 2) = world_points.at(i).z();

        cv_image_points.at<double>(i, 0) = image_points.at(i).x();
        cv_image_points.at<double>(i, 1) = image_points.at(i).y();
    }

    cv::Mat cv_camera_matrix(3, 3, CV_64F);

    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            cv_camera_matrix.at<double>(i, j) = camera->getMatrix()(i, j);

    cv::Mat rvec;
    cv::Mat tvec;

    bool res = cv::solvePnP(cv_world_points, cv_image_points, cv_camera_matrix, cv::Mat(), rvec, tvec, false, CV_ITERATIVE);

    qDebug() << res << tvec.size().height << tvec.size().width;

    QVector4D rvec_qt;
    for(int i = 0; i < 4; i++)
        rvec_qt[i] = rvec.at<double>(i, 0);

    QVector3D tvec_qt;
    for(int i = 0; i < 3; i++)
        tvec_qt[i] = tvec.at<double>(i, 0);

    qDebug() << rvec_qt << tvec_qt;

    QMatrix4x4 result;
    result.rotate(QQuaternion(rvec_qt));
    result.translate(tvec_qt);

    return result;
}
