/**
 * @file nv21videofilterrunnable.h
 * @brief This class converts NV21 format to RGB888
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-26
 */

#define EGL_EGLEXT_PROTOTYPES
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES/gl.h"
#define GL_GLEXT_PROTOTYPES
#include "GLES/glext.h"

#include "android/hardware_buffer.h"
#include "nv21videofilterrunnable.h"
#include "nv21videofilter.h"
#include "qvideoframehelpers.h"
#include "timelogger.h"
#include <QImage>
#include <QThreadPool>
#include <QOpenGLContext>
#include <qvideoframe.h>
#include <cstdio>
#include <QTextStream>
#include <strings.h>

NV21VideoFilterRunnable::NV21VideoFilterRunnable(const NV21VideoFilterRunnable& backend) : QObject(nullptr)
{
    this->parent = (NV21VideoFilterRunnable*) &backend;
    gl = nullptr;
}

NV21VideoFilterRunnable::NV21VideoFilterRunnable(NV21VideoFilter *f) : filter(f), gl(nullptr), image_id(0)
{
}

NV21VideoFilterRunnable::~NV21VideoFilterRunnable()
{
    if (gl != nullptr) {
        //gl->glDeleteFramebuffers(1, &framebuffer);
        gl->glDeleteRenderbuffers(1, &renderbuffer);
    }
}

QVideoFrame NV21VideoFilterRunnable::run(QVideoFrame *inputFrame, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{
    Q_UNUSED(surfaceFormat); Q_UNUSED(flags);
    return run(inputFrame);
}

AHardwareBuffer_Desc usage, usage1;
AHardwareBuffer* graphicBuf = nullptr;
EGLAPI EGLClientBuffer clientBuf = nullptr;

EGLDisplay disp = nullptr;
EGLImageKHR imageEGL = nullptr;

QVideoFrame NV21VideoFilterRunnable::run(QVideoFrame *inputFrame)
{
    image_info = PipelineContainerInfo(image_id).checkpointed("Grabbed");
    int status = -111;
    int eglerror = -555;
    TimeLoggerLog("%s", "NV21 Start");

    auto size(inputFrame->size());
    auto height(size.height());
    auto width(size.width());

    auto outputHeight = height / 4;
    auto outputWidth(outputHeight * width / height);

    Q_ASSERT(inputFrame->handleType() == QAbstractVideoBuffer::HandleType::GLTextureHandle);

    if (gl == nullptr) {
        auto context(QOpenGLContext::currentContext());

        gl = context->extraFunctions();

        this->currentContext = QOpenGLContext::currentContext();

        auto version(context->isOpenGLES() ? "#version 300 es\n" : "#version 130\n");

        auto sampleByPixelF(float(height) / float(outputHeight));
        unsigned int sampleByPixelI(std::ceil(sampleByPixelF));
        auto uvDelta(QVector2D(1, 1) / QVector2D(width, height));

        QString vertex(version);
        vertex += R"(
            out vec2 uvBase;
            void main(void) {
                int id = gl_VertexID;
                uvBase = vec2((id << 1) & 2, id & 2);
                gl_Position = vec4(uvBase * 2.0 - 1.0, 0.0, 1.0);
            }
        )";

        QString fragment(version);
        fragment += R"(
                    #extension GL_OES_EGL_image_external_essl3 : require
                    //#extension EGL_KHR_image_base : require -> error
                    //#extension EGL_KHR_image : require -> error

                    in lowp vec2 uvBase;
                    uniform sampler2D image;
                    const uint sampleByPixel = %1u;
                    const lowp vec2 uvDelta = vec2(%2, %3);
                    //out lowp float fragment;
                out lowp vec4 fragment;

                    lowp vec3 rgb2hsv(lowp vec3 c)
                    {
                        lowp vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
                        lowp vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
                        lowp vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

                        lowp float d = q.x - min(q.w, q.y);
                        lowp float e = 1.0e-10;
                        return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
                    }

                    void main(void) {
                        lowp vec3 sum = vec3(0.0, 0.0, 0.0);
                        for (uint x = 0u; x < sampleByPixel; ++x) {
                            for (uint y = 0u; y < sampleByPixel; ++y) {
                                lowp vec2 uv = uvBase + vec2(x, y) * uvDelta;
                                sum += texture(image, uv).bgr;
                            }
                        }
                        lowp float divisor = float(sampleByPixel * sampleByPixel);
                        lowp vec3 rgb = vec3(
                                    sum.b / divisor,
                                    sum.g / divisor,
                                    sum.r / divisor);

                        lowp vec3 hsv = rgb2hsv(rgb);

                        lowp float h = hsv.r;
                        lowp float s = hsv.g;
                        lowp float v = hsv.b;

                        lowp float diff;
                        lowp float mean_h_ = 0.0;
                        lowp float delta_h_ = 20.0 / 360.0;
                        lowp float min_s_ = 50. / 255.0;
                        lowp float max_s_ = 1.0;
                        lowp float min_v_ = min_s_;
                        lowp float max_v_ = 1.0;

                        if(h > 0.0) { diff = h - mean_h_; }
                        else { diff = mean_h_ - h; }

                        if(0.5 - diff < diff) { diff = 0.5 - diff; }

                        lowp float value;

                        if(s >= min_s_ && s <= max_s_ &&
                                v >= min_v_ && v <= max_v_ &&
                                diff < delta_h_) { value = 1.0; }
                        else value = 0.0;

                fragment = vec4(1.0, value, value, 1.0);
                    }
        )";

        program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertex);
        program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragment.
                                        arg(sampleByPixelI).
                                        arg(uvDelta.x()).
                                        arg(uvDelta.y()));
        program.link();
        imageLocation = program.uniformLocation("image");

        gl->glGenRenderbuffers(1, &renderbuffer);
        gl->glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        gl->glRenderbufferStorage(GL_RENDERBUFFER, QOpenGLTexture::RGBA8_UNorm, outputWidth, outputHeight);
        gl->glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // setting third argument to QOpenGLTexture::R8_UNorm results in 0x500 error.
        out_fbo = new QOpenGLFramebufferObject(outputWidth, outputHeight);

        Q_ASSERT(out_fbo->bind());
        gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
        gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);

        TimeLoggerLog("%s", "NV21 context OK");


        TimeLoggerLog("%s", "NV21 001");

        usage.format = AHARDWAREBUFFER_FORMAT_R8G8B8X8_UNORM;
        usage.height = outputHeight;
        usage.width = outputWidth;
        usage.layers = 1;
        usage.rfu0 = 0;
        usage.rfu1 = 0;
        usage.stride = 10;
        usage.usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_NEVER | AHARDWAREBUFFER_USAGE_GPU_COLOR_OUTPUT;

        TimeLoggerLog("%s %d %d %d %d", "NV21 usage", usage.format, usage.height, usage.width, usage.usage);

        // returns -22 if first is NULL
        status = AHardwareBuffer_allocate(&usage, &graphicBuf);

        TimeLoggerLog("%s %d %p", "NV21 allocate", status, graphicBuf);

        AHardwareBuffer_describe(graphicBuf, &usage1);
        TimeLoggerLog("NV21 buffer format %d %d", usage.format, usage1.format);
        TimeLoggerLog("NV21 buffer height %d %d", usage.height, usage1.height);
        TimeLoggerLog("NV21 buffer width %d %d", usage.width, usage1.width);
        TimeLoggerLog("NV21 buffer layers %d %d", usage.layers, usage1.layers);
        TimeLoggerLog("NV21 buffer rfu0 %d %d", usage.rfu0, usage1.rfu0);
        TimeLoggerLog("NV21 buffer rfu1 %d %d", usage.rfu1, usage1.rfu1);
        TimeLoggerLog("NV21 buffer stride %d %d", usage.stride, usage1.stride);
        TimeLoggerLog("NV21 buffer usage %d %d", usage.usage, usage1.usage);

        clientBuf = eglGetNativeClientBufferANDROID(graphicBuf);
        eglerror = eglGetError(); TimeLoggerLog("NV21 %s ret %d", "getNativeClient", eglerror);

        TimeLoggerLog("%s %p ", "NV21 client", clientBuf);

        disp = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        eglerror = eglGetError(); TimeLoggerLog("NV21 %s ret %d", "eglGetDisplay", eglerror);

    //    EGLint eglImageAttributes[] = {EGL_WIDTH, outputWidth, EGL_HEIGHT, outputHeight,
    //                                   EGL_MATCH_FORMAT_KHR,  EGL_FORMAT_RGBA_8888_KHR, EGL_IMAGE_PRESERVED_KHR,
    //                                   EGL_TRUE, EGL_NONE};
        // specifying width -> getting an error (invalid argument) from createImageKHR
        // same for format.
        EGLint eglImageAttributes[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE};

        imageEGL = eglCreateImageKHR(disp, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID, clientBuf, eglImageAttributes);
        eglerror = eglGetError(); TimeLoggerLog("NV21 %s ret %d", "eglCreateImageKHR", eglerror);
        TimeLoggerLog("%s disp %p image %p", "NV21 createImage", disp, imageEGL);
    }

    gl->glActiveTexture(GL_TEXTURE0);
    gl->glBindTexture(QOpenGLTexture::Target2D, inputFrame->handle().toUInt());

    // WORKS to obtain the source image
//    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, imageEGL);
//    eglerror = eglGetError(); TimeLoggerLog("NV21 %s ret %d", "eglImageTargetTexture", eglerror);

    gl->glTexParameteri(QOpenGLTexture::Target2D, QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
    gl->glTexParameteri(QOpenGLTexture::Target2D, QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);
    gl->glTexParameteri(QOpenGLTexture::Target2D, GL_TEXTURE_MIN_FILTER, QOpenGLTexture::Nearest);

    TimeLoggerLog("%s", "NV21 002");

    program.bind();
    program.setUniformValue(imageLocation, 0);
    program.enableAttributeArray(0);

    gl->glBindFramebuffer(GL_FRAMEBUFFER, out_fbo->handle());
    gl->glViewport(0, 0, outputWidth, outputHeight);
    gl->glDisable(GL_BLEND);
    gl->glDrawArrays(GL_TRIANGLES, 0, 3);
    gl->glPixelStorei(GL_PACK_ALIGNMENT, 1);
    gl->glFinish();

    //EGLint TextureHandle;
    //gl->glGenTextures(1, (unsigned int*) &TextureHandle);

    /*
     *
     * D libar-chest.so: (null):0 ((null)): 1533831728971	+1 ms	0x8367f970	[QVideoFrame NV21VideoFilterRunnable::run(QVideoFrame*)@../src/imagebackend/nv21videofilterrunnable.cpp:243]	NV21 GL_EXT_debug_marker GL_ARM_rgba8 GL_ARM_mali_shader_binary GL_OES_depth24 GL_OES_depth_texture GL_OES_depth_texture_cube_map GL_OES_packed_depth_stencil GL_OES_rgb8_rgba8 GL_EXT_read_format_bgra GL_OES_compressed_paletted_texture GL_OES_compressed_ETC1_RGB8_texture GL_OES_standard_derivatives GL_OES_EGL_image GL_OES_EGL_image_external GL_OES_EGL_image_external_essl3 GL_OES_EGL_sync GL_OES_texture_npot GL_OES_vertex_half_float GL_OES_required_internalformat GL_OES_vertex_array_object GL_OES_mapbuffer GL_EXT_texture_format_BGRA8888 GL_EXT_texture_rg GL_EXT_texture_type_2_10_10_10_REV GL_OES_fbo_render_mipmap GL_OES_element_index_uint GL_EXT_shadow_samplers GL_OES_texture_compression_astc GL_KHR_texture_compression_astc_ldr GL_KHR_texture_compression_astc_hdr GL_KHR_texture_compression_astc_sliced_3d GL_KHR_debug GL_EXT_occlusion_query_boolean GL_EXT
*/
//    const char* r1 = (const char*) glGetString(GL_EXTENSIONS);
//    const char* r2 = eglQueryString(eglGetCurrentDisplay(), EGL_EXTENSIONS);
//    TimeLoggerLog("NV21 %s /// %s", r1, r2);

//    EGLint bitmapaddr, pitch, origin;
//    eglQuerySurface(disp, imageEGL, EGL_BITMAP_POINTER_KHR, &bitmapaddr);
//    eglerror = eglGetError(); TimeLoggerLog("NV21 %s ret %d", "eglQuery1", eglerror);

//    eglQuerySurface(disp, imageEGL, EGL_BITMAP_PITCH_KHR, &pitch);
//    eglerror = eglGetError(); TimeLoggerLog("NV21 %s ret %d", "eglQuery2", eglerror);

//    eglQuerySurface(disp, imageEGL, EGL_BITMAP_ORIGIN_KHR, &origin);
//    eglerror = eglGetError(); TimeLoggerLog("NV21 %s ret %d", "eglQuery3", eglerror);

//    TimeLoggerLog("NV21 addr %d pitch %d origin %d", bitmapaddr, pitch, origin);

    // or +    gl->glBindTexture(GL_TEXTURE_2D, textureID);

//    gl->glActiveTexture(GL_TEXTURE0);

    GLuint outTex;
    gl->glGenTextures(1, &outTex);

    gl->glBindTexture(GL_TEXTURE_2D, outTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // THIS LINE MAKES THE SOURCE FRAME APPEAR
    gl->glBindTexture(GL_TEXTURE_2D, inputFrame->handle().toUInt());
//    gl->glBindFramebuffer(GL_FRAMEBUFFER, out_fbo->handle());
    // WORKS with gl->glBindTexture(GL_TEXTURE_2D, inputFrame->handle().toUInt());
    gl->glCopyTexImage2D(GL_TEXTURE_2D, 0, QOpenGLTexture::RGBA8_UNorm, 0, 0, outputWidth, outputHeight, 0);

    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, imageEGL); // this call makes the image original even if I draw on original texture!
    eglerror = eglGetError(); TimeLoggerLog("NV21 %s ret %d", "eglImageTargetTexture", eglerror);

    TimeLoggerLog("%s", "NV21 006'");    

    uchar data[600000];
    bzero(data, 600000);
    unsigned char *readPtr, *writePtr;
    writePtr = data;    

    status = AHardwareBuffer_lock(graphicBuf, AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN, -1, NULL, (void**) &readPtr);
    int stride = usage1.stride;
    TimeLoggerLog("%s %d %p stride %d", "NV21 007 LOCK", status, readPtr, stride);

    //gl->glReadPixels(0, 0, outputWidth, outputHeight, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, data);

    bool allZeros = true;
    for (int row = 0; row < outputHeight; row++) {
        memcpy(writePtr, readPtr, outputWidth * 4);
        for(int col = 0; col < outputWidth * 4; col++) {
            if(readPtr[col] != 0) allZeros = false;
        }
        readPtr = (unsigned char *)(int(readPtr) + stride * 4);
        writePtr = (unsigned char *)(int(writePtr) + outputWidth * 4);

    }

    TimeLoggerLog("%s %d", "NV21 011 allzeros", allZeros);

    status = AHardwareBuffer_unlock(graphicBuf, NULL);

    TimeLoggerLog("%s %d", "NV21 UNLOCK", status);

    image = QImage(data, outputWidth, outputHeight, QImage::Format_RGBA8888);

    TimeLoggerLog("%s", "NV21 012");

    QString s;
    QTextStream ss(&s);
    ss << "/shader" << image_id++ << ".png";
    image.save(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation).append(s));

    emit imageConverted(PipelineContainer<QImage>(image.convertToFormat(QImage::Format_Grayscale8), image_info.checkpointed("Sent")));

    TimeLoggerLog("%s", "NV21 IMsave OK");

    return *inputFrame;
}
