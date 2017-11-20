#include "uchiyamarkerdetection.h"
#include <stdio.h>
#include <iostream>
#include <uchiya/mylib/mytimer.h>

UchiyaMarkerDetection::UchiyaMarkerDetection(int h, int w)
{
    camInit(h, w);			// camera initialization
    trackingInit();		// tracking initialization
}

void UchiyaMarkerDetection::camInit(int h, int w)
{
    m_camimg.Init(w, h);		// allocate image
    m_img.Init(w, h);			// allocate image
}

void UchiyaMarkerDetection::trackingInit()
{
    m_llah.Init(m_img.w, m_img.h);		// set image size

    // load markers
    char name[256];
    int nummarker = 10;

    for(int i=0;i<nummarker;i++){
        snprintf(name,sizeof(name),"./data/%d.txt",i);
        m_llah.AddPaper(name);
        std::cout << name << " loaded" << std::endl;
    }

}

void UchiyaMarkerDetection::drawCG()
{
    visible *papers = m_llah.GetVisiblePaper();

    // for detected papers
    for(visible::iterator itpa=(*papers).begin(); itpa!=(*papers).end(); itpa++)
    {
        printf("Paper ID: %d\n",(*itpa)->id);
        // position: (*itpa)->H
        // color: (*itpa)->r, (*itpa)->g, (*itpa)->b

    }
}

MyImage UchiyaMarkerDetection::getimg() {
    // get image
    // push the image to m_camimg
    return m_camimg;
}

void UchiyaMarkerDetection::showimg() {
    // show image
    if(m_binarymode){
        m_llah.DrawBinary(m_camimg);

        if(m_ptmode){
            m_llah.DrawPts(m_camimg);
        }
    }

    // result in m_camimg
    //drawimg();

    if(m_viewmode && !m_binarymode){
        drawCG();
    }
}

void UchiyaMarkerDetection::process()
{
    m_img.Resize(m_camimg);

    MyTimer::Push("Time");

    m_llah.SetPts();
    m_llah.CoordinateTransform(static_cast<double>(m_img.h));

    m_llah.RetrievebyTracking();
    m_viewmode = m_llah.FindPaper(4);
    m_llah.RetrievebyMatching();
    m_viewmode |= m_llah.FindPaper(8);

    m_llah.Extract(m_img);

    showimg();

    MyTimer::Pop();
}
