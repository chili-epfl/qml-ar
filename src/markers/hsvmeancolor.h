#ifndef HSVMEANCOLOR_H
#define HSVMEANCOLOR_H

#include <QVector2D>
#include <QVector>
#include <QColor>


/*
 * This class calculates mean Hue
 * in HSV models
 */

class HSVMeanColor
{
private:
    // sum vector of all hues
    QVector2D h_vector;

    // all values of H in deg.
    QVector<double> h_values;

    // difference between angles in degrees
    static double angleDifference(double a1, double a2);
public:
    HSVMeanColor();

    // add color to take mean of
    void addColor(QColor c);

    // return mean hue (0 to 360)
    double meanHue();

    // standard deviation
    double stdHue();
};

#endif // HSVMEANCOLOR_H
