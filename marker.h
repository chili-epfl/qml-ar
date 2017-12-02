#ifndef MARKER_H
#define MARKER_H

/*
 * This class holds the position in the sheet
 * of a single marker
 */

#include <QVector2D>
#include <QString>
#include <QMatrix4x4>

class Marker
{
private:
    // number of the marker in Uchiya library
    int id;

    // name of the marker
    QString name;

    // postition of the marker in the activity sheet
    // in millimetres
    QVector2D position_mm;

    // size in mm
    double size_mm;

    // homography
    // res * [X Y 0 1]' -> [x y 0 1]
    QMatrix4x4 H;

    void setPositionMM(QVector2D position_mm_);
public:
    Marker();
    Marker(double x_mm, double y_mm);
    Marker(QVector2D position_mm_);

    void setName(QString name_);
    void setId(int id_);
    void setSizeMM(double size_);

    const QVector2D getPositionMM();
    double getSizeMM();

    void setH(QMatrix4x4 H_);
    QMatrix4x4 getH();
    void resetH();

    QString toString();
};

#endif // MARKER_H
