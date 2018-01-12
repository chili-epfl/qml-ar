#ifndef MVPPROVIDER_H
#define MVPPROVIDER_H

#include <QMatrix4x4>
#include <QObject>

/*
 * This class represents a QML interface
 * to provide ModelViewProjection matrix
 *
 * INPUT/OUTPUT COORDINATE SYSTEM (OpenCV/QML):
 * x right
 * y down
 * z from reader
 */

class MVPProvider : public QObject
{ Q_OBJECT
protected:
    // last ModelViewProjection matrix
    QMatrix4x4 mvp_matrix;

    // last ModelView matrix
    QMatrix4x4 mv_matrix;

    // last ModelViewProjection matrix
    QMatrix4x4 p_matrix;

    // valid = corresponds to a real mvp matrix
    bool is_valid;

    // set MVP matrix internally
    void setMVPMatrix(QMatrix4x4 mat);
public:
    // initialize with eye matrix
    MVPProvider();

    // return MVP matrix
    QMatrix4x4 getMVPMatrix();

    // return MVP matrix
    QMatrix4x4 getMVMatrix();

    // return MVP matrix
    QMatrix4x4 getPMatrix();

    // returns true if result is actual matrix
    bool isValid();

    // invalidate matrix
    // makes all objects invisible
    void reset();
signals:

    // emitted when new MVP matrix is available
    void newMVPMatrix();
};

#endif // MVPPROVIDER_H
