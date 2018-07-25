/**
 * @file uchiyamarkerdetector.cpp
 * @brief 
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "timelogger.h"
#include "uchiyamarkerdetector.h"
#include <stdio.h>
#include <iostream>
#include <uchiya/mylib/mytimer.h>
#include <QDebug>
#include <QElapsedTimer>
#include <QMatrix4x4>
#include "QtOpenCV/cvmatandqimage.h"
#include "opencv2/core.hpp"
#include <vector>
#include <opencv2/calib3d.hpp>
#include "opencv2/imgproc.hpp"
#include "mymatconverter.h"
#include <QGenericMatrix>
#include <QJsonArray>

using std::vector;

    /**
    * @brief See main.cpp and main.h from
    * UchiyaMarkers project
    */

UchiyaMarkerDetector::UchiyaMarkerDetector(): MarkerDetector()
{
    is_initialized = false;
}

void UchiyaMarkerDetector::initialize(int h, int w)
{
    Q_ASSERT(is_initialized == false);
    this->h = h;
    this->w = w;
    m_camimg.Init(w, h);		// allocate image
    initMarkers();		// tracking initialization
    is_initialized = true;
}

void UchiyaMarkerDetector::initMarkers()
{
    m_llah.Init(m_camimg.w, m_camimg.h);		// set image size

    /**
    * @brief Going through all markers
    */
    QMap<int, Marker>::iterator marker_in_map;
    for(marker_in_map = markers.begin(); marker_in_map != markers.end(); marker_in_map++)
    {
    /**
    * @brief Will write marker config here
    */
        QString s;
        QTextStream ss(&s);

    /**
    * @brief Dots as json array
    */
        QJsonArray dots = marker_in_map.value().getConfig().value("dots_uchiya").toArray();

    /**
    * @brief Going through dots and adding them to ss
    */
        QJsonArray::iterator dot;
        ss << dots.size() << "\n";
        for(dot = dots.begin(); dot != dots.end(); dot++)
        {
    /**
    * @brief Adding dot coordinates
    */
            ss << (*dot).toObject().value("x").toDouble() << " "
               << (*dot).toObject().value("y").toDouble() << "\n";
        }

    /**
    * @brief Rewinding the stream
    */
        ss.seek(0);

    /**
    * @brief Adding paper with stream as input
    */
        m_llah.AddPaper(ss);

    /**
    * @brief Marker added
    */
        TimeLoggerLog("Marker %d loaded", marker_in_map.key());
    }
}

void UchiyaMarkerDetector::extractMarkers()
{
    /**
    * @brief List of detected markers
    */
    visible *papers = m_llah.GetVisiblePaper();

    /**
    * @brief Forget all detected markers
    * This removes point correspondences
    */
    markers.undetect();

    /**
    * @brief Largest markers (if any)
    */
    Marker* biggest_marker = NULL;
    double biggest_determinant = 0;

    /**
    * @brief For detected papers
    */
    for(visible::iterator itpa=(*papers).begin(); itpa!=(*papers).end(); itpa++)
    {
    /**
    * @brief Id of the marker in Uchiya library
    */
        int id = (*itpa)->id - 1;

    /**
    * @brief Marker in the storage
    */
        Marker* m = markers.getPointer(id);

    /**
    * @brief No marker in the database
    * This happens if camera saw a marker
    * Not from this activity
    */
        if(m == NULL) continue;

    /**
    * @brief Homography matrix from Uchiya library
    * Top-left 3x3 submatrix is used
    * Warning: matrix uses mirrored y axis
    */
        QMatrix4x4 homography = MyMatConverter::convert3x3((*itpa)->H);

    /**
    * @brief Using only 1 biggest marker to speed up
    * (parts of image which are not biggest marker
    * Will be blackened on next iteration)
    * Biggest = by marker area ~ H determinant
    * TODO: use visible area instead of total area
    */
        QMatrix4x4 homography_1 = homography;
        homography_1(3, 3) = 1;
        double det = fabs(homography_1.determinant());
        if(det > biggest_determinant)
        {
    /**
    * @brief Forgetting old marker
    */
            if(biggest_marker)
                biggest_marker->undetect();

    /**
    * @brief Saving new marker
    */
            biggest_determinant = det;
            biggest_marker = m;
        }
        else break;

    /**
    * @brief Basic idea:
    * 1. Need: correspondences world <-> image
    * 2. Homography * Uchiya Coordinates (0-600 x 0-600) = Image coordinates of corners
    * 3. Obtaining world coordinates of marker corners from MarkerStorage
    * 4. Adding pairs (world corner, image corner) to Marker as a correspondence
    */

    /**
    * @brief Get marker size in mm
    */
        double marker_size = m->getSizeMM();

    /**
    * @brief Fill in the marker corners in the actual sheet (mm)
    * Corners (in order): top-left, bottom-left, top-right, bottom-right
    * Positioning (mm)    (0, 0)    (0, s)       (s, 0)     (s, s)
    */
        QVector3D marker_tl = m->getPositionMM();
        QVector3D marker_bl = marker_tl;
        QVector3D marker_br = marker_tl;
        QVector3D marker_tr = marker_tl;

        marker_bl += QVector3D(0          , marker_size, 0);
        marker_br += QVector3D(marker_size, marker_size, 0);
        marker_tr += QVector3D(marker_size, 0          , 0);

    /**
    * @brief Marker coordinate system (marker has size 600)
    */
        QVector3D marker_uchiya_tl_affine = QVector3D(0  , 600, 0);
        QVector3D marker_uchiya_bl_affine = QVector3D(0  , 0  , 0);
        QVector3D marker_uchiya_br_affine = QVector3D(600, 0  , 0);
        QVector3D marker_uchiya_tr_affine = QVector3D(600, 600, 0);

    /**
    * @brief Add all of the points to a vector
    */
        QVector<QVector3D> marker_corners;
        marker_corners.append(marker_tl);
        marker_corners.append(marker_bl);
        marker_corners.append(marker_br);
        marker_corners.append(marker_tr);

    /**
    * @brief All of the marker corners corresponding to real world corners
    */
        QVector<QVector3D> marker_uchiya_affine_corners;
        marker_uchiya_affine_corners.push_back(marker_uchiya_tl_affine);
        marker_uchiya_affine_corners.push_back(marker_uchiya_bl_affine);
        marker_uchiya_affine_corners.push_back(marker_uchiya_br_affine);
        marker_uchiya_affine_corners.push_back(marker_uchiya_tr_affine);

    /**
    * @brief Length should be equal since number of corners is the same
    */
        Q_ASSERT(marker_uchiya_affine_corners.size() == marker_corners.size());
        Q_ASSERT(marker_uchiya_affine_corners.size() == 4);

    /**
    * @brief Go through the vector, compute image point and
    * Add a correspondence
    */
        for(int i = 0; i < marker_corners.size(); i++)
        {
    /**
    * @brief Obtain current marker point in the world coordinates
    */
            QVector3D world_marker_point = marker_corners[i];

    /**
    * @brief Get uchiya marker coordinates point to 2D form (ignore z)
    */
            QVector4D uchiya_marker_point_affine = marker_uchiya_affine_corners[i];
            uchiya_marker_point_affine.setZ(1);

    /**
    * @brief Obtain marker image point using homography matrix
    */
            QVector4D image_marker_point_affine = homography.map(uchiya_marker_point_affine);

    /**
    * @brief Check if the z dimension is nonzero
    * Otherwise, perspective division is impossible
    * (point is certainly invisible)
    */
            Q_ASSERT(image_marker_point_affine.z() != 0);

    /**
    * @brief Convert image point from affine to 2D form
    */
            QVector2D image_marker_point = QVector2D(image_marker_point_affine.x() / image_marker_point_affine.z(),
                                                     image_marker_point_affine.y() / image_marker_point_affine.z());

    /**
    * @brief Uchiya library mirrors y axis
    */
            image_marker_point.setY(h - image_marker_point.y());

    /**
    * @brief Add the calculated correspondence
    */
            m->addCorrespondence(world_marker_point, image_marker_point);
        }
    }
}

void UchiyaMarkerDetector::prepareInput(QImage source)
{
    static cv::Mat src2mat;
    src2mat = QtOcv::image2Mat_shared(source);
    static IplImage src2mat2ipl;
    src2mat2ipl = (IplImage) src2mat;
    m_camimg.m_img = &src2mat2ipl;
}

QPair<MarkerStorage, QImage> UchiyaMarkerDetector::process(QImage source)
{
    /**
    * @brief If the image is invalid, no need for marker detection
    * This happens at the start of the application
    */
    if(source.height() * source.width() <= 0)
    {
        TimeLoggerLog("%s", "Empty image");
        return qMakePair(markers, source);
    }

    /**
    * @brief Initialize marker detection pipeline
    * When we know the camera image size
    */
    if(!is_initialized)
    {
        TimeLoggerLog("%s", "Initializing UchiyaMarkerDetector");
        initialize(source.height(), source.width());
    }

    TimeLoggerThroughput("%s", "[ANALYZE] Begin Uchiya");

    /**
    * @brief Copying input
    */
    if(source.isGrayscale())
    {
    /**
    * @brief Copying grayscale bits
    */
        m_llah.Extract(source);
    }
    else
    {
    /**
    * @brief Copying input to OpenCV matrices
    */
        prepareInput(source);
    /**
    * @brief Passing detected blobs to the library
    */
        m_llah.Extract(m_camimg);
    }

    TimeLoggerThroughput("%s", "[ANALYZE] Begin UchiyaSetPts");
    m_llah.SetPts();
    /**
    * @brief M_llah.SetPts(blob_detector.getBlobs());
    */
    m_llah.CoordinateTransform(source.height());
    TimeLoggerThroughput("%s", "[ANALYZE] End UchiyaSetPts");

    TimeLoggerThroughput("%s", "[ANALYZE] Begin UchiyaTracking");
    m_llah.RetrievebyTracking();
    m_llah.FindPaper(6);
    TimeLoggerThroughput("%s", "[ANALYZE] End UchiyaTracking");

    if(m_llah.GetVisiblePaper()->size() == 0)
    {
        TimeLoggerThroughput("%s", "[ANALYZE] Begin UchiyaMatching");
        m_llah.RetrievebyMatching();
        m_llah.FindPaper(10);
        TimeLoggerThroughput("%s", "[ANALYZE] End UchiyaMatching");
    }

    /**
    * @brief Extracting WorldImage correspondences
    */
    extractMarkers();

    TimeLoggerThroughput("%s", "End marker detection");

    PipelineContainerInfo info = object_in_process.checkpointed("UchiyaMarkerDetector");

    /**
    * @brief Sending obtained dots
    */
    emit dotsFound(PipelineContainer<QPair<QImage, QVector<QVector2D> > >
                   (qMakePair(source, m_llah.foundDots()), info));
    emit dotsAll(PipelineContainer<QVector<QVector2D> >
                 (m_llah.foundDots(true), info));

    /**
    * @brief Returning markers
    */
    QPair<MarkerStorage, QImage> result = qMakePair(markers, QImage());

    TimeLoggerThroughput("%s", "[ANALYZE] End Uchiya");

    /**
    * @brief Returning markers
    */
    return result;
}
