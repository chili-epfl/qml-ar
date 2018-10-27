#include "uchiyaprinter.h"

UchiyaPrinter::UchiyaPrinter()
{
    detector = new UchiyaMarkerDetector;
    connect(detector, &UchiyaMarkerDetector::dotsFound, this, &UchiyaPrinter::updateDots);
}

void UchiyaPrinter::getDots(QImage img)
{
    detector->process(img);

}

void UchiyaPrinter::updateDots(PipelineContainer<QPair<QImage, QVector<QVector2D>>> data)
{
    auto points = data.o();
    qDebug() << points;
}
