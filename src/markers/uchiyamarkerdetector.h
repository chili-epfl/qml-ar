/**
 * @file uchiyamarkerdetector.h
 * @brief Wrapper around C-style UchiyaMarkers project
 * obtained from main.cpp file from UchiyaMarkers project
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef UCHIYAMARKERDETECTOR_H
#define UCHIYAMARKERDETECTOR_H

#include "uchiya/mylib/myimage.h"
#include "uchiya/llah.h"
#include "uchiya/mylib/mymat.h"
#include "markerdetector.h"
#include <QMatrix4x4>
#include <QVector>
#include <QVector2D>

/**
 * @brief Wrapper around C-style UchiyaMarkers project
 * obtained from main.cpp file from UchiyaMarkers project
 *
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
{ Q_OBJECT
private:
    /**
    * @brief See main.h from UchiyaMarkers project
    */
    MyImage m_camimg;
    LLAH m_llah;

    /**
    * @brief Height and width
    */
    int h, w;

    /**
    * @brief One side of a square marker in pixels
    */
    const double marker_size_pixels = 600.0f;

    /**
    * @brief Are the images allocated?
    */
    bool is_initialized;

    void camInit(int h, int w);
    void initMarkers();
    void extractMarkers();

    /**
    * @brief Allocate space for images in memory
    */
    void initialize(int h, int w);

    /**
    * @brief Convert input to IplImage for UchiyaMarkers
    */
    void prepareInput(QImage source);

public:
    /**
    * @brief Initialize given height and width of the image
    */
    UchiyaMarkerDetector();

    virtual ~UchiyaMarkerDetector() {}

    /**
    * @brief Do marker processing
    */
    virtual QPair<MarkerStorage, QImage> process(QImage source);

signals:
    /**
    * @brief Emits image coords. of blobs which belong
    * To a marker
    */
    void dotsFound(PipelineContainer<QPair<QImage, QVector<QVector2D>>>);
    void dotsAll(PipelineContainer<QVector<QVector2D>>);
};

#endif // UCHIYAMARKERDETECTOR_H
