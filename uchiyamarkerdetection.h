#ifndef UCHIYAMARKERDETECTION_H
#define UCHIYAMARKERDETECTION_H

#include <uchiya/mylib/myimage.h>
#include <uchiya/llah.h>

class UchiyaMarkerDetection
{
private:
    MyImage m_camimg;
    MyImage m_img;
    LLAH m_llah;

    bool m_binarymode = false;
    bool m_stopmode = false;
    bool m_viewmode = false;
    bool m_ptmode = false;
    void showimg();
    void camInit(int h, int w);
    void trackingInit();
    void drawCG();
public:
    UchiyaMarkerDetection(int h, int w);
    void process();
    MyImage getimg();
};

#endif // UCHIYAMARKERDETECTION_H
