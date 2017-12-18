#ifndef MARKER_H
#define MARKER_H

/*
 * This class holds the position in the sheet
 * of a single marker
 */

#include <QVector2D>
#include <QString>
#include <QMatrix4x4>
#include "worldpointimages.h"

class Marker
{
private:
    // number of the marker
    int id;

    // name of the marker
    QString name;

    // postition of the marker in the activity sheet
    // top-left corner, in millimetres
    QVector2D position_mm;

    // size of the single side in mm
    double size_mm;

    // correspondences 2D -> 3D
    WorldPointImages correspondences;

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

    void addCorrespondence(QVector3D world_point, QVector2D image_point);
    WorldPointImages& getCorrespondences();
    void undetect();

    QString toString();
};

#endif // MARKER_H
