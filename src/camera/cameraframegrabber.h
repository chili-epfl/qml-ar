/**
 * @file cameraframegrabber.h
 * @brief This class implements a Viewfinder interface for the QCamera which stores images
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef CAMERAFRAMEGRABBER_H
#define CAMERAFRAMEGRABBER_H

#include <QList>
#include <QVideoFrame>
#include <QAbstractVideoSurface>

/**
 * @brief This class implements a Viewfinder interface for the QCamera which stores images
 *
 * The camera calls the present() method and this class emits frameAvailable signal
 * The signal contains a QImage with the frame
 *
 * Source: http://omg-it.works/how-to-grab-video-frames-directly-from-qcamera/
 *
 * Adaptation: Sergei Volodin, CHILI, EPFL, 2018
 *
 */

class CameraFrameGrabber : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit CameraFrameGrabber(QObject *parent = 0);

    /**
    * @brief Dummy function returning a list
    */
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const;

    /**
    * @brief Callback for the parent object
    * Should be called on each new frame (e.g. by QCamera)
    * @param frame New frame to process
    */
    bool present(const QVideoFrame &frame);

signals:
    /**
    * @brief This will be emited after present() was called
    * And the image was successfully obtained
    * @param frame Resulting frame
    */
    void frameAvailable(QImage frame);

public slots:
};

#endif // CAMERAFRAMEGRABBER_H
