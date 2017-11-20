#ifndef VOIDVIEWFINDER_H
#define VOIDVIEWFINDER_H

// Qt includes
#include <QAbstractVideoSurface>
#include <QList>

class VoidViewFinder : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit VoidViewFinder(QObject *parent = 0) {}

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const;

    bool present(const QVideoFrame &frame) {}
};

#endif // VOIDVIEWFINDER_H
