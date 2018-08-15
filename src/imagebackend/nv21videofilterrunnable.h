/**
 * @file nv21videofilterrunnable.h
 * @brief This class converts NV21 input QVideoFrame(texture) to RGB888, also doing downscaling and HSV thresholding
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-26
 */

#ifndef NV21VIDEOFILTERRUNNABLE_H
#define NV21VIDEOFILTERRUNNABLE_H

#include <QVideoFilterRunnable>
#include <QtOpenGL>
#include <QtConcurrent>
#include "pipelinecontainer.h"
#include "android/hardware_buffer.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"

/**
 * @brief Maximal EGLImage size in bytes
 */
#define MAX_INPUT_SIZE 600000

/**
 * @brief If defined, the class will print GL/EGL/HardwareBuffer return codes
 */
#define DEBUG_NV21_FILTER

class NV21VideoFilter;

/**
 * @brief The NV21VideoFilterRunnable class converts NV21 format to RGB888
 *
 * @see https://github.com/aseba-community/thymio-ar/blob/eb942a5e96761303512a07dc6e5057749ff8939e/vision-video-filter.h
 * @see http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
 * @see https://gist.github.com/rexguo/6696123
 * @see https://www.khronos.org/files/egl-1-4-quick-reference-card.pdf
 * @see https://www.khronos.org/registry/EGL/extensions/KHR/EGL_KHR_image_base.txt
 * @see https://software.intel.com/en-us/articles/using-opengl-es-to-accelerate-apps-with-legacy-2d-guis
 *
 */

class NV21VideoFilterRunnable : public QObject, public QVideoFilterRunnable
{ Q_OBJECT
public:
    /**
     * @brief VisionVideoFilterRunnable constructor
     * @param filter parent manager object
     */
    explicit NV21VideoFilterRunnable(NV21VideoFilter* filter);

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

private:
    /**
     * @brief Output image buffer
     */
    QImage image;

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
     * @brief The current number of the image
     */
    int image_id;

    /**
     * @brief The container to wrap output image in
     */
    PipelineContainerInfo image_info;

    /**
     * @brief Output framebuffer
     */
    QOpenGLFramebufferObject* out_fbo;

    /**
     * @brief Output texture
     */
    GLuint outTex;

    /**
     * @brief Input usage for the hardware buffer
     */
    AHardwareBuffer_Desc usage;

    /**
     * @brief Output usage for the hardware buffer
     */
    AHardwareBuffer_Desc usage1;

    /**
     * @brief Android 8 HardwareBuffer
     */
    AHardwareBuffer* graphicBuf;

    /**
     * @brief Android Native buffer
     */
    EGLClientBuffer clientBuf;

    /**
     * @brief EGL display
     */
    EGLDisplay disp;

    /**
     * @brief EGLImage to use for data transfer
     */
    EGLImageKHR imageEGL;

    /**
     * @brief Buffer for reading from EGLImage to CPU
     */
    unsigned char readBuffer[MAX_INPUT_SIZE];

signals:
    /**
     * @brief Output scaled down 4x, binarized using HSV threshold, image
     * @param img Grayscale binarized image
     */
    void imageConverted(PipelineContainer<QImage> img);
};
#endif // NV21VIDEOFILTERRUNNABLE_H
