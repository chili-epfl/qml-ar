#ifndef CAMERAFRAMEGRABBER_H
#define CAMERAFRAMEGRABBER_H

// Qt includes
#include <QAbstractVideoSurface>
#include <QList>

/*
 * This class implements a Viewfinder intrface for the QCamera
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

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const;

    bool present(const QVideoFrame &frame);

    static QImage VideoFrameToImage(QVideoFrame &frame);

signals:
    void frameAvailable(QImage frame);

public slots:

};

#endif // CAMERAFRAMEGRABBER_H
