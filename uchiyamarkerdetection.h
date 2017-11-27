#ifndef UCHIYAMARKERDETECTION_H
#define UCHIYAMARKERDETECTION_H

#include <uchiya/mylib/myimage.h>
#include <uchiya/llah.h>
#include "uchiya/mylib/mymat.h"

#include <QMatrix4x4>

// Wrapper around C-style UchiyaMarkers project
// obtained from main.cpp file from UchiyaMarkers project
// some functionality was removed

class UchiyaMarkerDetection
{
private:
    // see main.h from UchiyaMarkers project
    MyImage m_camimg;
    MyImage m_img;
    LLAH m_llah;

    // see main.cpp from UchiyaMarkers project
    void showimg();
    void camInit(int h, int w);
    void trackingInit();
    void drawCG();
public:
    // initialize given height and width of the image
    UchiyaMarkerDetection(int h, int w);

    // do internal processing
    // call after setting *getimg() = <your source image>
    void process();

    // return pointer to source image
    IplImage *getSrcPtr();

    // return pointer to resulting image
    // (dots/markers)
    IplImage *getDstPtr();

    // last blob matrix
    MyMat H;
    bool isHValid = false;

    QMatrix4x4 getOrthoMatrix();
};

#endif // UCHIYAMARKERDETECTION_H
