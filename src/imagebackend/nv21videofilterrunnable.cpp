/**
 * @file nv21videofilterrunnable.h
 * @brief This class converts NV21 format to RGB888
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-26
 */

#include "nv21videofilterrunnable.h"
#include "nv21videofilter.h"
#include "qvideoframehelpers.h"
#include "timelogger.h"
#include <QImage>
#include <QOpenGLContext>
#include <qvideoframe.h>
#include <cstdio>
#include <QTextStream>

NV21VideoFilterRunnable::NV21VideoFilterRunnable(const NV21VideoFilterRunnable& backend) : QObject(nullptr)
{
    this->parent = (NV21VideoFilterRunnable*) &backend;
    gl = nullptr;
}

NV21VideoFilterRunnable::NV21VideoFilterRunnable(NV21VideoFilter *f) : filter(f), gl(nullptr), image_id(0)
{
    watcher.setParent(this);
    this->currentContext = QOpenGLContext::currentContext();

    // waiting for async output
    connect(&watcher, SIGNAL(finished()), this, SLOT(handleFinished()));
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

void NV21VideoFilterRunnable::convert() {
    QWindow* surf = new QWindow;
    surf->setSurfaceType(QWindow::OpenGLSurface);
    surf->setFormat(parent->currentContext->format());
    surf->setScreen(parent->currentContext->screen());
    surf->create();

    QOpenGLContext* context = new QOpenGLContext;
    context->setFormat(parent->currentContext->format());
    context->setScreen(parent->currentContext->screen());
    context->setShareContext(parent->currentContext);
    context->create();
    context->makeCurrent(surf);

    parent->image = parent->obj->toImage();//.convertToFormat(QImage::Format_Grayscale8);
    //parent->image.save(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation).append("/converted.png"));
}

void NV21VideoFilterRunnable::handleFinished()
{
    // sending image
    emit imageConverted(PipelineContainer<QImage>
                        (image, image_info.checkpointed("NV21")));

    TimeLoggerThroughput("%s", "[ANALYZE] Begin NV21VideoFilter");
}

struct QVF {
  typedef QVideoFramePrivate* QVideoFrame::*type;
  friend type get(QVF);
};


QVideoFrame NV21VideoFilterRunnable::run(QVideoFrame *inputFrame)
{
    TimeLoggerLog("%s", "NV21 start");

    auto size(inputFrame->size());
    auto height(size.height());
    auto width(size.width());

    auto outputHeight = height / 4;
    auto outputWidth(outputHeight * width / height);

    //TimeLoggerLog("%s", "NV21 auto OK");

    Q_ASSERT(inputFrame->handleType() == QAbstractVideoBuffer::HandleType::GLTextureHandle);

    //TimeLoggerLog("%s", "NV21 assert OK");

    if (gl == nullptr) {
        //TimeLoggerLog("%s", "NV21 need GL...");
        auto context(QOpenGLContext::currentContext());

        //TimeLoggerLog("%s %d", "NV21 Context OK", context);

        gl = context->extraFunctions();

        //TimeLoggerLog("%s", "NV21 extra OK");

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
            in lowp vec2 uvBase;
            uniform sampler2D image;
            const uint sampleByPixel = %1u;
            const lowp vec2 uvDelta = vec2(%2, %3);
            out lowp float fragment;
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

                    if(h > 0.0) {
                        diff = h - mean_h_;
                    }
                    else {
                        diff = mean_h_ - h;
                    }

                    if(0.5 - diff < diff) {
                        diff = 0.5 - diff;
                    }

                    if(s >= min_s_ && s <= max_s_ &&
                            v >= min_v_ && v <= max_v_ &&
                            diff < delta_h_)
                    {
                        fragment = 1.0;
                    }
                    else fragment = 0.0;
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
        gl->glRenderbufferStorage(GL_RENDERBUFFER, QOpenGLTexture::R8_UNorm, outputWidth, outputHeight);
        gl->glBindRenderbuffer(GL_RENDERBUFFER, 0);

        //gl->glGenFramebuffers(1, &framebuffer);
        obj = new QOpenGLFramebufferObject(outputWidth, outputHeight);
        Q_ASSERT(obj->bind());
        //gl->glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
        gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    TimeLoggerLog("%s", "NV21 context OK");

    gl->glActiveTexture(GL_TEXTURE0);
    gl->glBindTexture(QOpenGLTexture::Target2D, inputFrame->handle().toUInt());
    gl->glTexParameteri(QOpenGLTexture::Target2D, QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
    gl->glTexParameteri(QOpenGLTexture::Target2D, QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);
    gl->glTexParameteri(QOpenGLTexture::Target2D, GL_TEXTURE_MIN_FILTER, QOpenGLTexture::Nearest);

    program.bind();
    program.setUniformValue(imageLocation, 0);
    program.enableAttributeArray(0);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, obj->handle());
    gl->glViewport(0, 0, outputWidth, outputHeight);
    gl->glDisable(GL_BLEND);
    gl->glDrawArrays(GL_TRIANGLES, 0, 3);
    gl->glPixelStorei(GL_PACK_ALIGNMENT, 1);

    if(!watcher.isRunning()) {
        image_info = PipelineContainerInfo(image_id);
        image_id++;
        QFuture<void> future = QtConcurrent::run(*this, &NV21VideoFilterRunnable::convert);
        watcher.setFuture(future);
    }

    TimeLoggerLog("%s", "NV21 sent");

    return *inputFrame;
}
