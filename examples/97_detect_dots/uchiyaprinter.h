#ifndef UCHIYAPRINTER_H
#define UCHIYAPRINTER_H

#include <QObject>
#include "uchiyamarkerdetector.h"

/// @class Print dots on marker
class UchiyaPrinter : public QObject
{ Q_OBJECT
private:
    QVector<QVector2D> buffer;
    UchiyaMarkerDetector* detector;
public:
    UchiyaPrinter();
    QVector<QVector2D> getDots(QImage img);
public slots:
    void updateDots(PipelineContainer<QPair<QImage, QVector<QVector2D> > >);
};

#endif // UCHIYAPRINTER_H
