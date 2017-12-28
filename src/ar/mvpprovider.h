#ifndef MVPPROVIDER_H
#define MVPPROVIDER_H

#include <QMatrix4x4>
#include <QObject>

/*
 * This class represents a QML interface
 * to provide ModelViewProjection matrix
 *
 * INPUT/OUTPUT COORDINATE SYSTEM (OpenCV/Cellulo):
 * x right
 * y down
 * z from reader
 */

class MVPProvider : public QObject
{ Q_OBJECT Q_PROPERTY(QMatrix4x4 mvp_matrix READ getMVPMatrix NOTIFY newMVPMatrix)
protected:
    // last ModelViewProjection matrix
    QMatrix4x4 mvp_matrix;

    // valid = corresponds to a real mvp matrix
    bool is_valid;

    // set MVP matrix internally
    void setMVPMatrix(QMatrix4x4 mat);
public:
    // initialize with eye matrix
    MVPProvider();

    // return MVP matrix
    QMatrix4x4 getMVPMatrix();

    // invalidate matrix
    // makes all objects invisible
    void reset();
signals:

    // emitted when new MVP matrix is available
    void newMVPMatrix();
};

#endif // MVPPROVIDER_H
