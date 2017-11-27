#include "uchiyamarkerdetection.h"
#include <stdio.h>
#include <iostream>
#include <uchiya/mylib/mytimer.h>
#include <QDebug>
#include <QElapsedTimer>
#include <QMatrix4x4>

// see main.cpp and main.h from
// UchiyaMarkers project

UchiyaMarkerDetection::UchiyaMarkerDetection(int h, int w)
{
    qDebug() << "init" << h << w;
    m_camimg.Init(w, h);		// allocate image
    m_img.Init(w, h);			// allocate image
    trackingInit();		// tracking initialization
}

void UchiyaMarkerDetection::trackingInit()
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

QMatrix4x4 UchiyaMarkerDetection::getProjectionMatrix(MyMat src)
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
    dst.data()[10] = 1.0f;
    dst.data()[11] = 0.0f;

    dst.data()[12] = src(0,2);
    dst.data()[13] = src(1,2);
    dst.data()[14] = 0.0f;
    dst.data()[15] = src(2,2);

    return(dst);
}


void UchiyaMarkerDetection::drawCG()
{
    visible *papers = m_llah.GetVisiblePaper();

    float zeros[]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //                0  1  2  3  4  5  6  7  8  9

    float scales[] = {0, 1000, 0, 0, 0, 0, 600, 1500, 0, 0};
    //                0  1     2  3  4  5  6    7     8  9

    float tr_xs[]  = {0, 634, 0, 0, 0, 0, 1770, 95, 0, 0};
    //                0  1    2  3  4  5  6     7    8  9

    float tr_ys[]  = {0, 2119, 0, 0, 0, 0, 2590, 79, 0, 0};
    //                0  1     2  3  4  5  6     7    8  9

    QMatrix4x4 res(zeros);

    int i = 0;

    // for detected papers
    for(visible::iterator itpa=(*papers).begin(); itpa!=(*papers).end(); itpa++)
    {
        int id = (*itpa)->id;
        float scale = 600. / scales[id - 1] * 600;

        const float scale_arr[] = {scale, 0, 0, 0,
                                   0, scale, 0, 0,
                                   0, 0, 1, 0,
                                   0, 0, 0, 1};

        const float translate_arr[] = {1, 0, 0, -tr_xs[id - 1] / scale,
                                       0, 1, 0, tr_ys[id - 1] / scale,
                                       0, 0, 1, 0,
                                       0, 0, 0, 1};

        /*QMatrix4x4 m = getOrthoMatrix() * getProjectionMatrix((*itpa)->H) *
                  QMatrix4x4(translate_arr) * QMatrix4x4(scale_arr);*/

        QMatrix4x4 m = getOrthoMatrix() * getProjectionMatrix((*itpa)->H) *
        QMatrix4x4({300, 0, 0, 0,
                    0, 300, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1});


        if(id == 8) H = m;

        qDebug() << "Uchiya Paper ID" << id << m;

        res += m;
        i += 1;
    }

    res /= i;
    //H = res;
}

IplImage* UchiyaMarkerDetection::getSrcPtr() {
    return m_camimg;
}

IplImage *UchiyaMarkerDetection::getDstPtr()
{
    return m_img;
}

QMatrix4x4 UchiyaMarkerDetection::getOrthoMatrix()
{
    // https://www.ibm.com/support/knowledgecenter/ssw_aix_72/com.ibm.aix.opengl/doc/openglrf/glOrtho.htm
    // https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glOrtho.xml
    //                glOrtho(0, m_cam.w, 0, m_cam.h, -1, 1);
    /*void glOrtho(GLdouble  Left,
         GLdouble  Right,
         GLdouble  Bottom,
         GLdouble  Top,
         GLdouble  Near,
         GLdouble  Far)    */

    float l = 0, r = m_camimg.w, bot = 0, top = m_camimg.h, near = -1, far = 1;

    float tx = -(r + l)/(r - l);
    float ty = -(top + bot) / (top - bot);
    float tz = -(far + near) / (far - near);

    const float a[] = {2 / (r - l), 0.0f, 0.0f, tx, 0.0f, 2 / (top - bot), 0.0f, ty, 0.0f, 0.0f, -2/(far-near), tz, 0.0f, 0.0f, 1.0f};

    return QMatrix4x4(a);
}

void UchiyaMarkerDetection::showimg() {
    // show image
    m_img.Resize(m_camimg);
    //m_llah.DrawBinary(m_img);
    m_llah.DrawPts(m_img);

    drawCG();
}

void UchiyaMarkerDetection::process()
{
    QElapsedTimer timer;
    timer.start();

    m_llah.Extract(m_camimg);

    m_llah.SetPts();
    m_llah.CoordinateTransform(static_cast<double>(m_camimg.h));

    m_llah.RetrievebyTracking();
    m_llah.FindPaper(4);
    m_llah.RetrievebyMatching();
    m_llah.FindPaper(8);

    showimg();

    qDebug() << "Uchiya dt" << timer.elapsed() << "ms";
}
