#ifndef UCHIYABACKEND_H
#define UCHIYABACKEND_H

#include "qtbackend.h"
#include "uchiyamarkerdetection.h"

class UchiyaBackEnd : public QtCameraBackEnd
{
public:
    UchiyaMarkerDetection* md;
    UchiyaBackEnd();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
    ~UchiyaBackEnd();
private:
    QImage processUchiya(QImage image);
};

#endif // UCHIYABACKEND_H
