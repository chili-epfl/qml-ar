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

void UchiyaPrinter::updateDots(PipelineContainer<QVector<QVector2D> > data)
{
    QVector<QVector2D> points = data.o();
    qDebug() << points;
}
