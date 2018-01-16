#ifndef TRACKINGMVPDECORATOR_H
#define TRACKINGMVPDECORATOR_H

#include "posepredictor.h"
#include "markerdetector.h"
#include "markermvpprovider.h"

/*
 * This class decorates marker detector and marker mvp provider
 * Adding the ability to detect markers only on predicted area
 */

class TrackingDecorator : public MarkerDetector
{ Q_OBJECT
private:
    PosePredictor* predictor;
    MVPProvider* provider;
    MarkerDetector* detector;
public:
    TrackingDecorator(MarkerDetector *detector, PosePredictor* predictor, MVPProvider *provider);

    // do marker detection
    void process();

    // obtain last input buffer
    QImage getLastInput();

    // iterators for going through the map
    QMap<int, Marker>::iterator begin();
    QMap<int, Marker>::iterator end();

    // return all of the 3D-2D correspondences
    WorldImageCorrespondences getCorrespondences();

public slots:
    // called on new MVP matrix from provider
    void onNewMVPMatrix();
};

#endif // TRACKINGMVPDECORATOR_H
