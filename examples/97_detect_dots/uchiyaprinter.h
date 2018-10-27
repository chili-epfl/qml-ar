#ifndef UCHIYAPRINTER_H
#define UCHIYAPRINTER_H

#include <QObject>
#include "uchiyamarkerdetector.h"

/// @class Print dots on marker
class UchiyaPrinter : public QObject
{ Q_OBJECT
private:
    UchiyaMarkerDetector* detector;
public:
    UchiyaPrinter();
    void getDots(QImage img);
public slots:
    void updateDots(PipelineContainer<QVector<QVector2D> >);
};

#endif // UCHIYAPRINTER_H
