#ifndef QTBACKEND_H
#define QTBACKEND_H

#include <QQuickImageProvider>
#include <QString>
#include <QtMultimedia/QCamera>
#include "cameraframegrabber.h"

class QtCameraBackEnd : public QObject, public QQuickImageProvider
{ Q_OBJECT
public:
    QtCameraBackEnd();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    virtual ~QtCameraBackEnd();

private:
    QImage buf;
    QCamera* camera;
    CameraFrameGrabber* frameGrabber;
public slots:
    void handleFrame(QImage img);
};

#endif // QTBACKEND_H
