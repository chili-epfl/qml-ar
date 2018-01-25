#include "randomimagebackend.h"

RandomImageBackend::RandomImageBackend(int w, int h, int dt_ms) : ImageProviderAsync()
{
    this->w = w;
    this->h = h;
    data = (uchar*) malloc(w * h * 3 * max_images);
    connect(&t, &QTimer::timeout, this, &RandomImageBackend::updateImage);
    t.start(dt_ms);
    current_image = 0;
}

void RandomImageBackend::updateImage()
{
    QImage image = QImage(data + current_image * w * h * 3, w, h, QImage::Format_RGB888);
    current_image++;
    current_image %= max_images;
    emit imageAvailable(image.copy());
}
