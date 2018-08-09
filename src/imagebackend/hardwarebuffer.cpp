#include "hardwarebuffer.h"
#include "timelogger.h"
#include <QCoreApplication>
#define EGL_EGLEXT_PROTOTYPES
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES/gl.h"
#define GL_GLEXT_PROTOTYPES
#include "GLES/glext.h"
#include "android/hardware_buffer.h"

HardwareBuffer::HardwareBuffer()
{
    AHardwareBuffer_Desc usage;
    AHardwareBuffer* graphicBuf = nullptr;
    EGLClientBuffer clientBuf = nullptr;

    usage.format = AHARDWAREBUFFER_FORMAT_R8G8B8X8_UNORM;
    usage.height = 512;
    usage.width = 512;
    usage.layers = 1;
    usage.rfu0 = 0;
    usage.rfu1 = 0;
    usage.stride = 10;
    usage.usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_RARELY | AHARDWAREBUFFER_USAGE_GPU_COLOR_OUTPUT;

    TimeLoggerLog("%s %d %d %d %d", "NV21 usage", usage.format, usage.height, usage.width, usage.usage);

    int status = AHardwareBuffer_allocate(&usage, &graphicBuf);

    TimeLoggerLog("%s %d %p", "NV21 allocate", status, graphicBuf);

    if(graphicBuf == nullptr) {
        TimeLoggerLog("%s", "NV21 Killing the app");
        QCoreApplication::exit(-1);
    }
}

void HardwareBuffer::load()
{

}
