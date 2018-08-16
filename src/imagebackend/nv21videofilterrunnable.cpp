/**
 * @file nv21videofilterrunnable.h
 * @brief This class converts NV21 format to RGB888
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-26
 */

#if __ANDROID_API__ >= 26

// GLES/GL includes
#define EGL_EGLEXT_PROTOTYPES
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES/gl.h"
#define GL_GLEXT_PROTOTYPES
#include "GLES/glext.h"

// Hardware buffer
#include "android/hardware_buffer.h"

// QMLAR includes
#include "nv21videofilterrunnable.h"
#include "nv21videofilter.h"
#include "timelogger.h"

// Qt includes
#include <QImage>
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
    graphicBuf(nullptr), clientBuf(nullptr), disp(nullptr), imageEGL(0)
{
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
    return run(inputFrame);
}

QVideoFrame NV21VideoFilterRunnable::run(QVideoFrame *inputFrame)
{
    // setting current time to the container to measure latency
    image_info = PipelineContainerInfo(image_id++).checkpointed("Grabbed");

    // variable to check for errors in functions
    int error = 0;

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

        // see https://github.com/aseba-community/thymio-ar/blob/eb942a5e96761303512a07dc6e5057749ff8939e/vision-video-filter.h
        // this is for averaging the image
        auto sampleByPixelF(float(height) / float(outputHeight));
        unsigned int sampleByPixelI(std::ceil(sampleByPixelF));
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
        fragment += R"(
                    #extension GL_OES_EGL_image_external_essl3 : require
                    #extension GL_OES_EGL_image_external : require

                    in lowp vec2 uvBase;
                    uniform sampler2D image;
                    const uint sampleByPixel = %1u;
                    const lowp vec2 uvDelta = vec2(%2, %3);
                    //out lowp float fragment;

                    // output RGBA
                    out lowp vec4 fragment;

                    // convert RGB to HSV, see https://gist.github.com/msbarry/cd98f928542f5152111a
                    lowp vec3 rgb2hsv(lowp vec3 c)
                    {
                        lowp vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
                        lowp vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
                        lowp vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

                        lowp float d = q.x - min(q.w, q.y);
                        lowp float e = 1.0e-10;
                        return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
                    }

                    // main function
                    void main(void) {
                        // sum of input pixels for this output pixels
                        lowp vec3 sum = vec3(0.0, 0.0, 0.0);

                        // summing up the input pixels
                        for (uint x = 0u; x < sampleByPixel; ++x) {
                            for (uint y = 0u; y < sampleByPixel; ++y) {
                                lowp vec2 uv = uvBase + vec2(x, y) * uvDelta;
                                sum += texture(image, uv).bgr;
                            }
                        }

                        // dividing by the number of summands
                        lowp float divisor = float(sampleByPixel * sampleByPixel);
                        lowp vec3 rgb = vec3(
                                    sum.b / divisor,
                                    sum.g / divisor,
                                    sum.r / divisor);

                        // obtaining HSV data
                        lowp vec3 hsv = rgb2hsv(rgb);

                        lowp float h = hsv.r;
                        lowp float s = hsv.g;
                        lowp float v = hsv.b;

                        // difference in HUE (0-1)
                        lowp float diff;

                        // want this mean hue (0-1)
                        lowp float mean_h_ = 0.0;

                        // maximal deviation in HUE (0-1)
                        lowp float delta_h_ = 20.0 / 360.0;

                        // min saturation (0-1)
                        lowp float min_s_ = 50. / 255.0;

                        // max saturation (0-1)
                        lowp float max_s_ = 1.0;

                        // min value (0-1)
                        lowp float min_v_ = min_s_;

                        // max value (0-1)
                        lowp float max_v_ = 1.0;

                        // calculating shortest distance between angles
                        if(h > 0.0) { diff = h - mean_h_; }
                        else { diff = mean_h_ - h; }
                        if(0.5 - diff < diff) { diff = 0.5 - diff; }

                        // output: 1.0 or 0.0
                        lowp float value;

                        // doing the thresholding
                        if(s >= min_s_ && s <= max_s_ &&
                                v >= min_v_ && v <= max_v_ &&
                                diff < delta_h_) { value = 1.0; }
                        else value = 0.0;

                        // output: white or black
                        fragment = vec4(value, value, value, 1.0);
                    }
        )";

        // creating vertex shader
        program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertex);

        // creating fragment shader
        program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragment.
                                        arg(sampleByPixelI).
                                        arg(uvDelta.x()).
                                        arg(uvDelta.y()));
        program.link();
        imageLocation = program.uniformLocation("image");

        // creating output framebuffer
        out_fbo = new QOpenGLFramebufferObject(outputWidth, outputHeight);

        // and binding it
        gl->glBindFramebuffer(GL_FRAMEBUFFER, out_fbo->handle()); GL_CHECK_ERROR();

        // attaching the created texture as to the framebuffer (texture will contain output RGBA data)
        gl->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, outTex, 0); GL_CHECK_ERROR();

        // filling in the usage for HardwareBuffer
        usage.format = AHARDWAREBUFFER_FORMAT_R8G8B8X8_UNORM;
        usage.height = outputHeight;
        usage.width = outputWidth;
        usage.layers = 1;
        usage.rfu0 = 0;
        usage.rfu1 = 0;
        usage.stride = 10;
        usage.usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_NEVER
                | AHARDWAREBUFFER_USAGE_GPU_COLOR_OUTPUT;

        // creating an Android 8 HardwareBuffer
        error = AHardwareBuffer_allocate(&usage, &graphicBuf); HWB_CHECK_ERROR();

        // obtaining buffer description -> can get real stride in the code which follows
        AHardwareBuffer_describe(graphicBuf, &usage1);

        // obtaining native buffer
        clientBuf = eglGetNativeClientBufferANDROID(graphicBuf); EGL_CHECK_ERROR();

        // obtaining the EGL display
        disp = eglGetDisplay(EGL_DEFAULT_DISPLAY); EGL_CHECK_ERROR();

        // specifying the image attributes
        EGLint eglImageAttributes[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE};

        // creating an EGL image
        imageEGL = eglCreateImageKHR(disp, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID, clientBuf, eglImageAttributes);
        EGL_CHECK_ERROR();
    }

    //setting current texture
    gl->glActiveTexture(GL_TEXTURE0); GL_CHECK_ERROR();

    // binding INPUT texture
    gl->glBindTexture(QOpenGLTexture::Target2D, inputFrame->handle().toUInt());

    // specifying INPUT texture parameters
    gl->glTexParameteri(QOpenGLTexture::Target2D, QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge); GL_CHECK_ERROR();
    gl->glTexParameteri(QOpenGLTexture::Target2D, QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge); GL_CHECK_ERROR();
    gl->glTexParameteri(QOpenGLTexture::Target2D, GL_TEXTURE_MIN_FILTER, QOpenGLTexture::Nearest); GL_CHECK_ERROR();

    // binding our shader program
    program.bind();
    program.setUniformValue(imageLocation, 0);
    program.enableAttributeArray(0);

    // binding OUTPUT framebuffer
    gl->glBindFramebuffer(GL_FRAMEBUFFER, out_fbo->handle()); GL_CHECK_ERROR();

    // DRAWING the fragments using shader and input frame to out_fbo
    gl->glViewport(0, 0, outputWidth, outputHeight); GL_CHECK_ERROR();
    gl->glDisable(GL_BLEND); GL_CHECK_ERROR();
    gl->glDrawArrays(GL_TRIANGLES, 0, 3); GL_CHECK_ERROR();
    gl->glPixelStorei(GL_PACK_ALIGNMENT, 1); GL_CHECK_ERROR();

    // binding the OUTPUT texture
    gl->glBindTexture(GL_TEXTURE_2D, outTex); GL_CHECK_ERROR();

    // attaching an EGLImage to OUTPUT texture
    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, imageEGL); EGL_CHECK_ERROR();

    // pointer to READ from EGLImage
    unsigned char *readPtr;

    // pointer to WRITE the image (in memory)
    unsigned char *writePtr = readBuffer;

    // locking the buffer
    error = AHardwareBuffer_lock(graphicBuf, AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN, -1, NULL, (void**) &readPtr);
    HWB_CHECK_ERROR();

    // obtaining real stride
    int stride = usage1.stride;

    // copying all rows from EGLImage to readBuffer
    for (int row = 0; row < outputHeight; row++) {
        memcpy(writePtr, readPtr, outputWidth * 4);
        readPtr = (unsigned char *)(int(readPtr) + stride * 4);
        writePtr = (unsigned char *)(int(writePtr) + outputWidth * 4);

    }

    // unlocking the buffer
    error = AHardwareBuffer_unlock(graphicBuf, NULL); HWB_CHECK_ERROR();

    // wrapping the buffer inside a QImage
    image = QImage(readBuffer, outputWidth, outputHeight, QImage::Format_RGBA8888);

    // converting the image to Grayscale
    image = image.convertToFormat(QImage::Format_Grayscale8);

    // sending the black/white image to listeners
    emit imageConverted(PipelineContainer<QImage>(image, image_info.checkpointed("Sent")));

    // returning the input frame unchanged for the preview
    return *inputFrame;
}

#endif
