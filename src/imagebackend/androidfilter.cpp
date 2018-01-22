#include "androidfilter.h"

#include <QOpenGLContext>

#define DEBUG_FPS

#ifdef DEBUG_FPS
#include <QElapsedTimer>
const int FPS_PRINT_PERIOD = 500;       ///< Period of printing the FPS estimate, in milliseconds
#endif

class TextureBuffer : public QAbstractVideoBuffer
{
public:
    TextureBuffer(uint id) : QAbstractVideoBuffer(GLTextureHandle), m_id(id) { }
    MapMode mapMode() const { return NotMapped; }
    uchar *map(MapMode, int *, int *) { return 0; }
    void unmap() { }
    QVariant handle() const { return QVariant::fromValue<GLuint>(m_id); }

private:
    GLuint m_id;
};

class AndroidFilterRunnable : public QVideoFilterRunnable
{
public:
    AndroidFilterRunnable()
    {
        timer.start();
    }

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) override;

#ifdef DEBUG_FPS
    QElapsedTimer timer;
    qint64 millisElapsed = 0;
    qreal frames = 0;
#endif
};

QVideoFrame AndroidFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{ Q_UNUSED(surfaceFormat) Q_UNUSED(flags)
#ifdef DEBUG_FPS
    qint64 millis = timer.restart();
    millisElapsed += millis;
    if(millisElapsed >= FPS_PRINT_PERIOD){
        qDebug("running at %lf FPS",frames * (1000.0f/FPS_PRINT_PERIOD));
        millisElapsed = 0;
        frames = 0;
    }

    ++frames;
#endif

#ifdef Q_OS_ANDROID
    // Stays in GPU Memory --> FAST
    GLuint textureId = input->handle().toUInt();

    return QVideoFrame(new TextureBuffer(textureId), input->size(), input->pixelFormat());

    /*
    // Maps to CPU Memory --> SLOW
    if (input->map(QAbstractVideoBuffer::ReadOnly)) {
        input->unmap();
    }
    */

    return *input;
#else
    return *input;
#endif
}

AndroidFilter::AndroidFilter(QObject *parent)
: QAbstractVideoFilter(parent)
{
}

QVideoFilterRunnable *AndroidFilter::createFilterRunnable()
{
    return new AndroidFilterRunnable;
}
