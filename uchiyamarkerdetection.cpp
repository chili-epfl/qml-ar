#include "uchiyamarkerdetection.h"
#include <stdio.h>
#include <iostream>
#include <uchiya/mylib/mytimer.h>
#include <QDebug>
#include <QElapsedTimer>

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

void UchiyaMarkerDetection::drawCG()
{
    visible *papers = m_llah.GetVisiblePaper();

    // for detected papers
    for(visible::iterator itpa=(*papers).begin(); itpa!=(*papers).end(); itpa++)
    {
        qDebug() << "Uchiya Paper ID" << (*itpa)->id;
        // position: (*itpa)->H
        // color: (*itpa)->r, (*itpa)->g, (*itpa)->b

    }
}

IplImage* UchiyaMarkerDetection::getSrcPtr() {
    return m_camimg;
}

IplImage *UchiyaMarkerDetection::getDstPtr()
{
    return m_img;
}

void UchiyaMarkerDetection::showimg() {
    // show image
    m_img.Resize(m_camimg);
    m_llah.DrawBinary(m_img);
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
