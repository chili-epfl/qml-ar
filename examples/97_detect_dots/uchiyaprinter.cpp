/**
 * @file uchiyaprinter.cpp
 * @brief This class returns the list of detected dots
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-10-27
 */

#include "uchiyaprinter.h"

UchiyaPrinter::UchiyaPrinter()
{
    detector = new UchiyaMarkerDetector;
    connect(detector, &UchiyaMarkerDetector::dotsFound, this, &UchiyaPrinter::updateDots);
    detector->loadMarkerPositions("./markers.json");
}

QVector<QVector2D> UchiyaPrinter::getDots(QImage img)
{
    detector->process(img);
    return buffer;

}

void UchiyaPrinter::updateDots(PipelineContainer<QPair<QImage, QVector<QVector2D>>> data)
{
    buffer = data.o().second;
}
