/**
 * @file hsvmeancolor.h
 * @brief This class calculates mean Hue
 * in HSV models
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef HSVMEANCOLOR_H
#define HSVMEANCOLOR_H

#include <QVector2D>
#include <QVector>
#include <QColor>


/**
 * @brief This class calculates mean Hue
 * in HSV models
 */

class HSVMeanColor
{
private:
    /**
    * @brief Sum vector of all hues
    */
    QVector2D h_vector;

    /**
    * @brief All values of H in deg.
    */
    QVector<double> h_values;

    /**
    * @brief Difference between angles in degrees
    */
    static double angleDifference(double a1, double a2);
public:
    HSVMeanColor();

    /**
    * @brief Add color to take mean of
    */
    void addColor(QColor c);
    void addColor(int h);

    /**
    * @brief Return mean hue (0 to 360)
    */
    double meanHue();

    /**
    * @brief Standard deviation
    */
    double stdHue();
};

#endif // HSVMEANCOLOR_H
