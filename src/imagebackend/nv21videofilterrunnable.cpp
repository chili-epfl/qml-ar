/**
 * @file nv21videofilterrunnable.h
 * @brief This class converts NV21 format to RGB888
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-26
 */

#include "nv21videofilterrunnable.h"
#include "nv21videofilter.h"
#include <QImage>

NV21VideoFilterRunnable::NV21VideoFilterRunnable(NV21VideoFilter *f) : filter(f), gl(nullptr)
{

}

NV21VideoFilterRunnable::~NV21VideoFilterRunnable()
{
    if (gl != nullptr) {
        gl->glDeleteFramebuffers(1, &framebuffer);
        gl->glDeleteRenderbuffers(1, &renderbuffer);
    }
}

QVideoFrame NV21VideoFilterRunnable::run(QVideoFrame *inputFrame, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{
    Q_UNUSED(surfaceFormat); Q_UNUSED(flags);
    return run(inputFrame);
}

QVideoFrame NV21VideoFilterRunnable::run(QVideoFrame *inputFrame)
{
    auto size(inputFrame->size());
    auto height(size.height());
    auto width(size.width());

    auto outputHeight = 20;//height;
    auto outputWidth(outputHeight * width / height);

    Q_ASSERT(inputFrame->handleType() == QAbstractVideoBuffer::HandleType::GLTextureHandle);


    if (gl == nullptr) {
        auto context(QOpenGLContext::currentContext());
        gl = context->extraFunctions();

        auto version(context->isOpenGLES() ? "#version 300 es\n" : "#version 130\n");

        auto sampleByPixelF(float(height) / float(outputHeight));
        unsigned int sampleByPixelI(std::ceil(sampleByPixelF));
        auto uvDelta(QVector2D(1, 1) / QVector2D(width, height));
        auto lumaScaled(QVector3D(0.299, 0.587, 0.114) / (sampleByPixelI * sampleByPixelI));

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
            const lowp vec3 lumaScaled = vec3(%4, %5, %6);
            out lowp float fragment;
            void main(void) {
                lowp vec3 sum = vec3(0, 0, 0);
                for (uint x = 0u; x < sampleByPixel; ++x) {
                    for (uint y = 0u; y < sampleByPixel; ++y) {
                        lowp vec2 uv = uvBase + vec2(x, y) * uvDelta;
                        sum += texture(image, uv).bgr;
                    }
                }
                fragment = dot(sum, lumaScaled);
            }
        )";

        program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertex);
        program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragment.
                                        arg(sampleByPixelI).
                                        arg(uvDelta.x()).
                                        arg(uvDelta.y()).
                                        arg(lumaScaled.x()).
                                        arg(lumaScaled.y()).
                                        arg(lumaScaled.z()));
        program.link();
        imageLocation = program.uniformLocation("image");

        gl->glGenRenderbuffers(1, &renderbuffer);
        gl->glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        gl->glRenderbufferStorage(GL_RENDERBUFFER, QOpenGLTexture::R8_UNorm, outputWidth, outputHeight);
        gl->glBindRenderbuffer(GL_RENDERBUFFER, 0);

        gl->glGenFramebuffers(1, &framebuffer);
        gl->glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
        gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    gl->glActiveTexture(GL_TEXTURE0);
    gl->glBindTexture(QOpenGLTexture::Target2D, inputFrame->handle().toUInt());
    gl->glTexParameteri(QOpenGLTexture::Target2D, QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
    gl->glTexParameteri(QOpenGLTexture::Target2D, QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);
    gl->glTexParameteri(QOpenGLTexture::Target2D, GL_TEXTURE_MIN_FILTER, QOpenGLTexture::Nearest);

    program.bind();
    program.setUniformValue(imageLocation, 0);
    program.enableAttributeArray(0);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    gl->glViewport(0, 0, outputWidth, outputHeight);
    gl->glDisable(GL_BLEND);
    gl->glDrawArrays(GL_TRIANGLES, 0, 3);

    QImage result(outputWidth, outputHeight, QImage::Format_Grayscale8);

    gl->glPixelStorei(GL_PACK_ALIGNMENT, 1);
    gl->glReadPixels(0, 0, outputWidth, outputHeight, QOpenGLTexture::Red, QOpenGLTexture::UInt8,
                     result.bits());

    qDebug() << result;

    return *inputFrame;
}
