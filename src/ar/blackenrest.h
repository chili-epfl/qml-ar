#ifndef TRACKINGMVPDECORATOR_H
#define TRACKINGMVPDECORATOR_H

#include "posepredictor.h"
#include "imageproviderasync.h"
#include "markerstorage.h"

/**
 * @file blackenrest.h
 * @brief Paints image parts not containing markers in black
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

/**
 * @brief Paints image parts not containing markers in black
 *
 * This class decorates marker detector and marker mvp provider
 * Adding the ability to detect markers only on predicted area
 * by blackening everything else on the image, except for the
 * area where marker is expected to be
 *
 * WorldImageCorrespondences assumed to contain marker corners in counter-
 * clockwise order
 */

class BlackenRest : public ImageProviderAsync
{ Q_OBJECT
private:

    /**
     * @brief Info of the object in process
     */
    PipelineContainerInfo object_in_process;

    /**
     * @brief predictor of the pose (1 frame interval)
     */

    PosePredictor* predictor;

    /**
     * @brief do use region of the image
     */

    bool use_region;

    /**
     * @brief MVP storage
     */

    QMatrix4x4 MV, P;

    /**
     * @brief storage for markers
     */

    MarkerStorage storage;

    /**
     * @brief blacken an image where there are no markers
     */

    QImage blacken(QImage source);
public:

    /**
     * @brief Initialize using a PosePredictor for prediction of pose
     * in short intervals of time between MV updates
     * @param p PosePredictor object pointer
     */

    BlackenRest(PosePredictor* p);

signals:

    /**
     * @brief Blackened image (output)
     * @param I resulting image
     */

    void imageAvailable(PipelineContainer<QImage>);

public slots:

    /**
     * @brief To be called to set/update P (projection) matrix
     * @param p P matrix
     */

    void onNewPMatrix(PipelineContainer<QMatrix4x4> p);

    /**
     * @brief To be called to set/update MV (ModelView) matrix
     * @param mv MV matrix
     */

    void onNewMVMatrix(PipelineContainer<QMatrix4x4> mv);

    /**
     * @brief To be called on new available detected markers
     */

    void onNewMarkers(PipelineContainer<MarkerStorage> storage);

    /**
     * @brief Set input and process it. Calls imageAvailable at the end.
     */

    void setInput(PipelineContainer<QImage> img);
};

#endif // TRACKINGMVPDECORATOR_H
