/**
 * @file uchiyaprinter.h
 * @brief This class returns the list of detected dots
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-10-27
 */

#ifndef UCHIYAPRINTER_H
#define UCHIYAPRINTER_H

#include <QObject>
#include "uchiyamarkerdetector.h"

/// @class This class returns the list of detected dots
class UchiyaPrinter : public QObject
{ Q_OBJECT
private:
    /// @brief Buffer with dots
    QVector<QVector2D> buffer;

    /// @brief Marker Detector
    UchiyaMarkerDetector* detector;
public:
    UchiyaPrinter();
    /**
     * @brief getDots Get list of detected dots
     * @param img Source image
     * @return Vector with dots
     */
    QVector<QVector2D> getDots(QImage img);
public slots:
    /**
     * @brief updateDots Called by marker detector on new dots
     */
    void updateDots(PipelineContainer<QPair<QImage, QVector<QVector2D> > >);
};

#endif // UCHIYAPRINTER_H
