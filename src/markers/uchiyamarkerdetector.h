#ifndef UCHIYAMARKERDETECTOR_H
#define UCHIYAMARKERDETECTOR_H

#include <uchiya/mylib/myimage.h>
#include <uchiya/llah.h>
#include "uchiya/mylib/mymat.h"
#include "markerdetector.h"
#include "blobdetector.h"
#include <QMatrix4x4>

/*
 * Wrapper around C-style UchiyaMarkers project
 * obtained from main.cpp file from UchiyaMarkers project
 * Notice: some functionality was removed
 *
 * INPUT/OUTPUT COORDINATE SYSTEM (OpenCV/QML):
 * x right
 * y down
 * z from reader
 *
 * Config must have additional property 'dots_uchiya'
 * and marker ids must be consecutive numbers from 0 to n - 1
 *
 * example:
 * {
 *  "markers": [
 *    {
      "name": "big",
 *      "id": 0,
 *      "x_mm": 9.5,
 *      "y_mm": 7.9,
 *      "size_mm": 150,
 *      "dots_uchiya": [
 *        {
 *          "x": 81,
 *          "y": 111
 *        },
 *        {
 *          "x": 569,
 *          "y": 460
 *        }
 *      ]
 *    }
 *  ]
 * }
 */

class UchiyaMarkerDetector: public MarkerDetector
{
private:
    // see main.h from UchiyaMarkers project
    MyImage m_camimg;
    MyImage m_img;
    LLAH m_llah;

    // one side of a square marker in pixels
    const double marker_size_pixels = 600.0f;

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

    // blob detector
    BlobDetector blob_detector;

    // maximal number of dots to detect
    const int max_dots = 50;
public:
    // initialize given height and width of the image
    UchiyaMarkerDetector();

    // do marker processing
    void process();
};

#endif // UCHIYAMARKERDETECTOR_H
