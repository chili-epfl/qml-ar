#include "randomimagebackend.h"
#include "timelogger.h"

RandomImageBackend::RandomImageBackend(int w, int h, int dt_ms) : ImageProviderAsync()
{
    this->w = w;
    this->h = h;
    int S = w * h * 3 * max_images;
    data = (uchar*) malloc(S);

    for(int i = 0; i < S; i++)
        data[i] = (i * i + 11123 + i) % 255;

    connect(&t, &QTimer::timeout, this, &RandomImageBackend::updateImage);
    t.start(dt_ms);
    current_image = 0;
}

void RandomImageBackend::updateImage()
{
    QImage image = QImage(data + current_image * w * h * 3, w, h, QImage::Format_RGB888);
    current_image++;
    current_image %= max_images;
    TimeLoggerLog("%s", "Generated image");
    emit imageAvailable(image.copy());
}
