/**
 * @file nv21videofilterrunnable.h
 * @brief This class converts NV21 format to RGB888
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-26
 */

#ifndef NV21VIDEOFILTERRUNNABLE_H
#define NV21VIDEOFILTERRUNNABLE_H

#define private public
#define protected public
#include <QVideoFilterRunnable>
#include <QtOpenGL>
#include <QtConcurrent>
#include "pipelinecontainer.h"

class NV21VideoFilter;

/**
 * @brief The NV21VideoFilterRunnable class converts NV21 format to RGB888
 *
 * Based on https://github.com/aseba-community/thymio-ar/blob/eb942a5e96761303512a07dc6e5057749ff8939e/vision-video-filter.h
 */

class NV21VideoFilterRunnable : public QObject, public QVideoFilterRunnable
{ Q_OBJECT
public:
    /**
     * @brief VisionVideoFilterRunnable constructor
     * @param filter parent manager object
     */
    explicit NV21VideoFilterRunnable(NV21VideoFilter* filter);

    NV21VideoFilterRunnable* parent;

    ~NV21VideoFilterRunnable();

    /**
     * @brief Process single frame
     * @param input Input frame from camera
     * @param surfaceFormat (unused)
     * @param flags (unused)
     * @return Frame is returned unchanged
     */
    QVideoFrame run(QVideoFrame* input, const QVideoSurfaceFormat& surfaceFormat, RunFlags flags);

    /**
     * @brief Process single frame
     * @param input Input frame from camera
     * @return Frame is returned unchanged
     */
    QVideoFrame run(QVideoFrame* input);

    QImage image;
    NV21VideoFilterRunnable(const NV21VideoFilterRunnable &backend);
private:

    /**
     * @brief parent object
     */
    NV21VideoFilter* filter;

    /**
     * @brief gl context
     */
    QOpenGLExtraFunctions* gl;

    /**
     * @brief shader program to run
     */
    QOpenGLShaderProgram program;

    /**
     * @brief source image
     */
    GLint imageLocation;

    /**
     * @brief source buffer
     */
    GLuint framebuffer;

    /**
     * @brief result buffer
     */
    GLuint renderbuffer;

    // source image id
    int image_id;

    PipelineContainerInfo image_info;

    /**
    * @brief Thread result
    */
    QFutureWatcher<void> watcher;

    QOpenGLContext *currentContext;

    QOpenGLFramebufferObject* out_fbo;

    GLuint outTex;

signals:
    void imageConverted(PipelineContainer<QImage>);
};
#endif // NV21VIDEOFILTERRUNNABLE_H
