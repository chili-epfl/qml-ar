#ifndef TRACKINGMVPDECORATOR_H
#define TRACKINGMVPDECORATOR_H

#include "posepredictor.h"
#include "imageproviderasync.h"
#include "markerstorage.h"

/*
 * This class decorates marker detector and marker mvp provider
 * Adding the ability to detect markers only on predicted area
 */

class TrackingDecorator : public ImageProviderAsync
{ Q_OBJECT
private:
    PosePredictor* predictor;

    // do use region of the image
    bool use_region;

    // MVP storage
    QMatrix4x4 MV, P;

    // storage for markers
    MarkerStorage storage;

    // blacken an image where there are no markers
    QImage blacken(QImage source);
public:
    TrackingDecorator(PosePredictor* p);

signals:
    // blackened image
    void imageAvailable(QImage);

public slots:
    // called on new MVP matrix from provider
    void onNewPMatrix(QMatrix4x4 p);
    void onNewMVMatrix(QMatrix4x4 mv);

    // called on new available markers
    void onNewMarkers(MarkerStorage storage);

    // process input
    void setInput(QImage img);
};

#endif // TRACKINGMVPDECORATOR_H
