/**
 * @file nv21videofilterrunnable.h
 * @brief This class converts NV21 format to RGB888
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-26
 */

#ifndef NV21VIDEOFILTERRUNNABLE_H
#define NV21VIDEOFILTERRUNNABLE_H

#include <QVideoFilterRunnable>
#include <QtOpenGL>

class NV21VideoFilter;

/**
 * @brief The NV21VideoFilterRunnable class converts NV21 format to RGB888
 */

class NV21VideoFilterRunnable : public QVideoFilterRunnable
{
public:
    /**
     * @brief VisionVideoFilterRunnable constructor
     * @param filter parent manager object
     */
    explicit VisionVideoFilterRunnable(NV21VideoFilter* filter);

    ~VisionVideoFilterRunnable();

    /**
     * @brief Process single frame
     * @param input Input frame from camera
     * @param surfaceFormat (unused)
     * @param flags (unused)
     * @return Frame is returned unchanged
     */
    QVideoFrame run(QVideoFrame* input, const QVideoSurfaceFormat& surfaceFormat, RunFlags flags);
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
};
#endif // NV21VIDEOFILTERRUNNABLE_H
