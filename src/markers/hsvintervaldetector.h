#ifndef HSVINTERVALDETECTOR_H
#define HSVINTERVALDETECTOR_H

#include <QObject>
#include <QImage>
#include <QVector>
#include <QVector2D>
#include <QColor>

/*
 * This class receives matched points
 * and deduces the color interval they are in
 */

class HSVIntervalDetector : public QObject
{ Q_OBJECT
private:
    // found colors
    QVector<QColor> colors;

public:
    HSVIntervalDetector();
    virtual ~HSVIntervalDetector() {}

public slots:
    // on new set of points
    void newPoints(QPair<QImage, QVector<QVector2D>> points);

    // print all found HSV points
    void printAll();
};

#endif // HSVINTERVALDETECTOR_H
