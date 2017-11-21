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

    void showimg();
    void camInit(int h, int w);
    void trackingInit();
    void drawCG();
public:
    UchiyaMarkerDetection(int h, int w);
    void process();
    IplImage *getimg();
    IplImage *getres();
};

#endif // UCHIYAMARKERDETECTION_H
