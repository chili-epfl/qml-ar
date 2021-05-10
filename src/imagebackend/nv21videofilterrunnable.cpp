/**
 * @file nv21videofilterrunnable.h
 * @brief This class converts NV21 format to RGB888
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-26
 */

// GLES/GL includes
#define EGL_EGLEXT_PROTOTYPES
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES/gl.h"
#define GL_GLEXT_PROTOTYPES
#include "GLES/glext.h"

// Hardware buffer
#include "android/hardware_buffer.h"
#ifdef USEGRAPHICBUFFER
#include "GraphicBuffer/GraphicBuffer.h"
#endif

// QMLAR includes
#include "nv21videofilterrunnable.h"
#include "nv21videofilter.h"
#include "timelogger.h"

// Qt includes
#include <QDebug>
#include <QImage>
#include <QString>
#include <QOpenGLContext>

// C functions
#include <cstdio>

// print or not print return codes
#ifdef DEBUG_NV21_FILTER
#define GL_CHECK_ERROR() {error = glGetError(); TimeLoggerLog("GL call: %d", error);}
#define EGL_CHECK_ERROR() {error = eglGetError(); TimeLoggerLog("EGL call: %d", error);}
#define HWB_CHECK_ERROR() {TimeLoggerLog("HardwareBuffer call: %d", error);}
#else
#define GL_CHECK_ERROR()
#define EGL_CHECK_ERROR()
#define HWB_CHECK_ERROR()
#endif

NV21VideoFilterRunnable::NV21VideoFilterRunnable(NV21VideoFilter *f) : filter(f), gl(nullptr), image_id(0),
    graphicBuf(nullptr), clientBuf(nullptr), disp(nullptr), imageEGL(nullptr)
{
    show_processed = false;
    active = true;
}

NV21VideoFilterRunnable::~NV21VideoFilterRunnable()
{
    if (gl != nullptr) {
        // removing the created texture
        gl->glDeleteTextures(1, &outTex);
    }
}

QVideoFrame NV21VideoFilterRunnable::run(QVideoFrame *inputFrame, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{
    Q_UNUSED(surfaceFormat); Q_UNUSED(flags);

    // implementation of active
    return active ? run(inputFrame) : *inputFrame;
}

QVideoFrame NV21VideoFilterRunnable::run(QVideoFrame *inputFrame)
{
    // setting current time to the container to measure latency
    image_info = PipelineContainerInfo(image_id++).checkpointed("Grabbed");

    // variable to check for errors in functions
    int error __attribute__((unused)) = 0;

    // size of the input frame
    auto size(inputFrame->size());

    // height of the input frame
    auto height(size.height());

    // width of the input frame
    auto width(size.width());

    // output height: scaling down 4x
    auto outputHeight = height / 4;

    // output width: scaling down 4x
    auto outputWidth(outputHeight * width / height);

    // checking that frame is a texture (on Android it's true)
    Q_ASSERT(inputFrame->handleType() == QAbstractVideoBuffer::HandleType::GLTextureHandle);

    // on the first run, set up everything
    if (gl == nullptr) {
        // GL context variable
        auto context(QOpenGLContext::currentContext());

        // functions to use
        gl = context->extraFunctions();

        // creating output texture
        gl->glGenTextures(1, &outTex); GL_CHECK_ERROR();

        // binding output texture to TEXTURE_2D
        gl->glBindTexture(GL_TEXTURE_2D, outTex); GL_CHECK_ERROR();

        // setting output texture parameters
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); GL_CHECK_ERROR();
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); GL_CHECK_ERROR();
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); GL_CHECK_ERROR();
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); GL_CHECK_ERROR();

        // obtaining current GL family
        auto version(context->isOpenGLES() ? "#version 300 es\n" : "#version 130\n");
        qDebug() << "gl version " << version;



        // see https://github.com/aseba-community/thymio-ar/blob/eb942a5e96761303512a07dc6e5057749ff8939e/vision-video-filter.h
        // this is for averaging the image
        auto sampleByPixelF(float(height) / float(outputHeight));
        unsigned int sampleByPixelI(uint(std::ceil(sampleByPixelF)));
        auto uvDelta(QVector2D(1, 1) / QVector2D(width, height));

        // vertex shader for NV21->RGB conversion
        QString vertex(version);
        vertex += R"(
            out vec2 uvBase;
            void main(void) {
                int id = gl_VertexID;
                uvBase = vec2((id << 1) & 2, id & 2);
                gl_Position = vec4(uvBase * 2.0 - 1.0, 0.0, 1.0);
            }
        )";

        // fragment shader
        // 1. Averages data over pixels (assuming that input texture is RGBA) for downscaling
        //    Despite the fact that it's NV21 if mapped, it seems that a shader still receives RGBA data
        //    Meaning that the conversion is done by GPU at some point automagically
        // 2. Converts RGB->HSV
        // 3. Thresholds HSV based on parameters
        QString fragment(version);
        fragment +=
#ifndef USEGRAPHICBUFFER
                R"(
                    #extension GL_OES_EGL_image_external_essl3 : require
                )"
#endif
                    R"(
                    #extension GL_OES_EGL_image_external : require

                    in lowp vec2 uvBase;
                    uniform sampler2D image;

                    // output RGBA
                    out lowp vec4 fragment;

                    // main function
                    void main(void) {
                        // output: white or black
                        fragment = vec4(100., 0.2, 0.3, 1.0);
                    }
        )";

        // creating vertex shader
        program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertex);

        // creating fragment shader
        program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragment);
//                                        arg(sampleByPixelI).
//                                        arg(uvDelta.x()).
//                                        arg(uvDelta.y()));

        // linking the shader
        program.link();

        // the input image location for shader
        imageLocation = program.uniformLocation("image");

        // obtaining shader uniform locations
//        mean_h_ = program.uniformLocation("mean_h_");
//        delta_h_ = program.uniformLocation("delta_h_");
//        min_s_ = program.uniformLocation("min_s_");
//        max_s_ = program.uniformLocation("max_s_");
//        min_v_ = program.uniformLocation("min_v_");
//        max_v_ = program.uniformLocation("max_v_");
//        marker_x_ = program.uniformLocation("marker_x");
//        marker_y_ = program.uniformLocation("marker_y");

        // creating output framebuffer
        out_fbo = new QOpenGLFramebufferObject(outputWidth, outputHeight);

        // and binding it
        gl->glBindFramebuffer(GL_FRAMEBUFFER, out_fbo->handle()); GL_CHECK_ERROR();

        // attaching the created texture as to the framebuffer (texture will contain output RGBA data)
        gl->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, outTex, 0); GL_CHECK_ERROR();
    }

    //setting current texture
    gl->glActiveTexture(GL_TEXTURE0); GL_CHECK_ERROR();

    // binding INPUT texture
    gl->glBindTexture(QOpenGLTexture::Target2D, inputFrame->handle().toUInt()); GL_CHECK_ERROR();

    // specifying INPUT texture parameters
    gl->glTexParameteri(QOpenGLTexture::Target2D, QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge); GL_CHECK_ERROR();
    gl->glTexParameteri(QOpenGLTexture::Target2D, QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge); GL_CHECK_ERROR();
    gl->glTexParameteri(QOpenGLTexture::Target2D, GL_TEXTURE_MIN_FILTER, QOpenGLTexture::Nearest); GL_CHECK_ERROR();

    // binding our shader program
    program.bind();
    program.setUniformValue(imageLocation, 0);
    program.enableAttributeArray(0);

    // configuring the shader: setting hue threshold parameters
//    program.setUniformValue(mean_h_, GLfloat(mean_h));
//    program.setUniformValue(delta_h_, GLfloat(delta_h));
//    program.setUniformValue(min_s_, GLfloat(min_s));
//    program.setUniformValue(max_s_, GLfloat(max_s));
//    program.setUniformValue(min_v_, GLfloat(min_v));
//    program.setUniformValue(max_v_, GLfloat(max_v));

//    // setting marker location (blackening everything else)
//    if(marker.size() == 4)
//    {
//        program.setUniformValue(marker_x_, marker.at(0).x(), marker.at(1).x(), marker.at(2).x(), marker.at(3).x());
//        program.setUniformValue(marker_y_, marker.at(0).y(), marker.at(1).y(), marker.at(2).y(), marker.at(3).y());
//    }

//    // otherwise, showing all
//    else {
//        program.setUniformValue(marker_x_, 0, 1, 1, 0);
//        program.setUniformValue(marker_y_, 0, 0, 1, 1);
//    }

    // binding OUTPUT framebuffer
    gl->glBindFramebuffer(GL_FRAMEBUFFER, out_fbo->handle()); GL_CHECK_ERROR();

    // DRAWING the fragments using shader and input frame to out_fbo
    gl->glViewport(0, 0, outputWidth, outputHeight); GL_CHECK_ERROR();
    gl->glDisable(GL_BLEND); GL_CHECK_ERROR();
    gl->glDrawArrays(GL_TRIANGLES, 0, 3); GL_CHECK_ERROR();
    gl->glPixelStorei(GL_PACK_ALIGNMENT, 1); GL_CHECK_ERROR();

    // binding the OUTPUT texture
    gl->glBindTexture(GL_TEXTURE_2D, outTex); GL_CHECK_ERROR();


    // finish all draw calls
    gl->glFinish(); GL_CHECK_ERROR();


    // drawing output image on input frame, if requested
//    if(show_processed)
        gl->glCopyImageSubData(outTex, GL_TEXTURE_2D, 0, 0, 0, 0,
                                              inputFrame->handle().toUInt(), GL_TEXTURE_2D,
                                              0, 0, 0, 0, outputWidth, outputHeight, 1);

    // wrapping the buffer inside a QImage
    image = QImage(readBuffer, outputWidth, outputHeight, QImage::Format_RGBA8888);

#ifdef DEBUG_SHADER
    qDebug() << "SHADER " << "outputWidth=" << outputWidth << " outputHeight=" << outputHeight;
#endif

    // converting the image to Grayscale
    image = image.convertToFormat(QImage::Format_Grayscale8);

    static int i = 0;
    image.save(QString("/storage/emulated/0/shader%1.png").arg(++i));

    // sending the black/white image to listeners
    emit imageConverted(PipelineContainer<QImage>(image, image_info.checkpointed("Sent")));

    // returning the input frame unchanged for the preview
    return *inputFrame;
}
