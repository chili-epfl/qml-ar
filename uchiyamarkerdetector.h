#ifndef UCHIYAMARKERDETECTOR_H
#define UCHIYAMARKERDETECTOR_H

#include <uchiya/mylib/myimage.h>
#include <uchiya/llah.h>
#include "uchiya/mylib/mymat.h"
#include "markerdetector.h"

#include <QMatrix4x4>

// Wrapper around C-style UchiyaMarkers project
// obtained from main.cpp file from UchiyaMarkers project
// some functionality was removed

class UchiyaMarkerDetector: public MarkerDetector
{
private:
    // see main.h from UchiyaMarkers project
    MyImage m_camimg;
    MyImage m_img;
    LLAH m_llah;

    // are the images allocated?
    bool is_initialized;

    // see main.cpp from UchiyaMarkers project
    void drawPreview();

    void camInit(int h, int w);
    void initMarkers();
    void extractMarkers();

    // allocate space for images in memory
    void initialize(int h, int w);

    // convert input to IplImage for UchiyaMarkers
    void prepareInput();

    // convert preview image to QImage
    void preparePreview();

    // a hack which uses UchiyaMarkers projection matrix
    void recomputeProjectorUchiya();
    QMatrix4x4 getProjectionMatrixFromMarkerUchiya(MyMat src);
public:
    // initialize given height and width of the image
    UchiyaMarkerDetector();

    // do marker processing
    void process();
};

#endif // UCHIYAMARKERDETECTOR_H
