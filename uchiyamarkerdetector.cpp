#include "uchiyamarkerdetector.h"
#include <stdio.h>
#include <iostream>
#include <uchiya/mylib/mytimer.h>
#include <QDebug>
#include <QElapsedTimer>
#include <QMatrix4x4>
#include "QtOpenCV/cvmatandqimage.h"

// see main.cpp and main.h from
// UchiyaMarkers project

UchiyaMarkerDetector::UchiyaMarkerDetector(): MarkerDetector()
{
    is_initialized = false;
}

void UchiyaMarkerDetector::initialize(int h, int w)
{
    Q_ASSERT(is_initialized == false);
    qDebug() << "init" << h << w;
    m_camimg.Init(w, h);		// allocate image
    m_img.Init(w, h);			// allocate image
    initMarkers();		// tracking initialization
    is_initialized = true;
}

void UchiyaMarkerDetector::initMarkers()
{
    m_llah.Init(m_img.w, m_img.h);		// set image size

    // load markers
    char name[256];
    int nummarker = 10;

    for(int i=0;i<nummarker;i++){
#ifdef Q_OS_ANDROID
        const char* fn_template = "assets:/data/%d.txt";
#else
        const char* fn_template = "./data/%d.txt";
#endif
        snprintf(name,sizeof(name),fn_template,i);
        m_llah.AddPaper(name);
        qDebug() << "Uchiya" << name << "loaded";
    }

}

void UchiyaMarkerDetector::extractMarkers()
{
    // list of detected markers
    visible *papers = m_llah.GetVisiblePaper();

    // resetting projection matrices for markers
    markers.resetH();

    // for detected papers
    for(visible::iterator itpa=(*papers).begin(); itpa!=(*papers).end(); itpa++)
    {
        // id of the marker in Uchiya library
        int id = (*itpa)->id - 1;

        // marker in the storage
        Marker* m = markers.getPointer(id);

        if(m == NULL) continue;

        // scaling by marker size in MM
        QMatrix4x4 scaler_to_pixels = QMatrix4x4();
        scaler_to_pixels.scale(marker_size_pixels / m->getSizeMM(),
                               marker_size_pixels / m->getSizeMM(), 0);

        // translating by the position of the marker
        QMatrix4x4 translate_to_marker = QMatrix4x4();
        translate_to_marker.translate(-m->getPositionMM().x(), m->getSizeMM() + m->getPositionMM().y(), 0);

        // resulting projection matrix
        projection_matrix = getProjectionMatrixFromMarkerUchiya((*itpa)->H)
                * scaler_to_pixels * translate_to_marker;

        // setting it to the marker object
        m->setH(projection_matrix);
    }
}

void UchiyaMarkerDetector::drawPreview() {
    // show image
    m_img.Resize(m_camimg);
    //m_llah.DrawBinary(m_img);
    m_llah.DrawPts(m_img);

    extractMarkers();
}

void UchiyaMarkerDetector::preparePreview()
{
    IplImage dst = *(IplImage*) m_img;
    cv::Mat dst2mat = cv::cvarrToMat(&dst, false); // second parameter disables data copying
    output_buffer = QtOcv::mat2Image(dst2mat);
}

QMatrix4x4 UchiyaMarkerDetector::getProjectionMatrixFromMarkerUchiya(MyMat src)
{
    QMatrix4x4 dst;
    dst.data()[0] = src(0,0);
    dst.data()[1] = src(1,0);
    dst.data()[2] = 0.0f;
    dst.data()[3] = src(2,0);

    dst.data()[4] = src(0,1);
    dst.data()[5] = src(1,1);
    dst.data()[6] = 0.0f;
    dst.data()[7] = src(2,1);

    dst.data()[8] = 0.0f;
    dst.data()[9] = 0.0f;

    // z axis now also has values -600...600
    dst.data()[10] = 1.0f / marker_size_pixels;
    dst.data()[11] = 0.0f;

    dst.data()[12] = src(0,2);
    dst.data()[13] = src(1,2);
    dst.data()[14] = 0.0f;
    dst.data()[15] = src(2,2);

    return(dst);
}

void UchiyaMarkerDetector::prepareInput()
{
    cv::Mat src2mat = QtOcv::image2Mat(input_buffer, CV_8UC3);
    IplImage src2mat2ipl = src2mat;
    cvCopy(&src2mat2ipl, (IplImage*) m_camimg);
}

void UchiyaMarkerDetector::process()
{
    // if the image is invalid, no need for marker detection
    // this happens at the start of the application
    if(input_buffer.height() * input_buffer.width() <= 0)
    {
        qDebug() << "Empty image";
        return;
    }

    // initialize marker detection pipeline
    // when we know the camera image size
    if(!is_initialized)
    {
        qDebug() << "Initializing UchiyaMarkerDetector";
        initialize(input_buffer.height(), input_buffer.width());
    }

    Q_ASSERT(is_initialized);

    // putting camera src image to Uchiya pipeline
    prepareInput();

    // measuring processing time
    QElapsedTimer timer;
    timer.start();

    // extracting dots
    m_llah.Extract(m_camimg);

    m_llah.SetPts();
    m_llah.CoordinateTransform(static_cast<double>(m_camimg.h));

    m_llah.RetrievebyTracking();
    m_llah.FindPaper(4);
    m_llah.RetrievebyMatching();
    m_llah.FindPaper(8);

    // creating preview image
    drawPreview();

    // obtaining Uchiya image dst and returning it
    preparePreview();

    // tell the parent to recompute projection matrix
    recomputeProjector();

    qDebug() << "Uchiya dt" << timer.elapsed() << "ms";
}
