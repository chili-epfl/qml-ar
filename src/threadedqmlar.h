#ifndef THREADEDQMLAR_H
#define THREADEDQMLAR_H

#include <QObject>
#include <QThread>
#include <QQuickImageProvider>
#include "qmlar.h"

/*
 * This class is a wrapper around QMLAR object
 * which is placed at a separate thread and controlled
 * by this object
 */

class ThreadedQMLAR : public QObject
{ Q_OBJECT
    Q_PROPERTY(int camera_id WRITE setCameraId READ getCameraId)
    Q_PROPERTY(QString image_filename WRITE setImageFilename READ getImageFilename)
    Q_PROPERTY(QObject* camera READ getCamera)
    Q_PROPERTY(QMatrix4x4 mvp_matrix READ getMVPMatrix NOTIFY newMVPMatrix)
    Q_PROPERTY(int image_width READ getImageWidth WRITE setImageWidth)
    Q_PROPERTY(QVariantList blobs READ getBlobs NOTIFY newBlobs)
    Q_PROPERTY(QVariantList markers READ getMarkers NOTIFY newMarkers)
    Q_PROPERTY(double FPSMean READ getFPSMean NOTIFY imageUpdated)
    Q_PROPERTY(double FPSStd READ getFPSStd NOTIFY imageUpdated)
    Q_PROPERTY(bool markers_visible READ markers_visible NOTIFY newMarkers)
private:
    // instance to QMLAR object
    QMLAR* instance;

    // thread to run in
    QThread* thread;
public:
    ThreadedQMLAR();

    // getters
    int getCameraId();
    QString getImageFilename();
    QMatrix4x4 getMVPMatrix();
    int getImageWidth();
    QQuickImageProvider *getImageProvider();
    QObject* getCamera();
    QVariantList getBlobs();
    QVariantList getMarkers();

    // FPS
    double getFPSMean();
    double getFPSStd();

    bool markers_visible();

public slots:
    // initialize from camera id (default value -1)
    void setCameraId(int camera_id = -1);

    // initialize from image
    void setImageFilename(QString filename);

    // set width of the image to process (on startup)
    void setImageWidth(int new_width);

    // start camera if required
    void startCamera();

signals:
    // notify QML part when new matrix is available
    void newMVPMatrix(QMatrix4x4);

    // notify about new image
    void imageUpdated();

    // on new blobs from detector
    void newBlobs(QVariantList);

    // on new markers
    void newMarkers(QVariantList);

    // initialize from camera id (default value -1)
    void setCameraIdSignal(int camera_id = -1);

    // initialize from image
    void setImageFilenameSignal(QString filename);

    // set width of the image to process (on startup)
    void setImageWidthSignal(int new_width);

    // start camera if required
    void startCameraSignal();
};

#endif // THREADEDQMLAR_H
