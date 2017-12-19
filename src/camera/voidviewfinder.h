#ifndef VOIDVIEWFINDER_H
#define VOIDVIEWFINDER_H

// Qt includes
#include <QAbstractVideoSurface>
#include <QList>

// A ViewFinder for QCamera which does nothing
// when frames do arrive

class VoidViewFinder : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit VoidViewFinder(QObject *parent = 0) {Q_UNUSED(parent)}

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const;

    bool present(const QVideoFrame &frame) {Q_UNUSED(frame); return true;}
};

#endif // VOIDVIEWFINDER_H
