#ifndef MARKER_H
#define MARKER_H

/*
 * This class holds the position in the sheet
 * of a single marker
 */

#include <QVector2D>
#include <QString>

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

    void setPositionMM(QVector2D position_mm_);
public:
    Marker();
    Marker(double x_mm, double y_mm);
    Marker(QVector2D position_mm_);

    void setName(QString name_);
    void setId(int id_);

    const QVector2D getPositionMM();

    QString toString();
};

#endif // MARKER_H
