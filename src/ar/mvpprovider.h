#ifndef MVPPROVIDER_H
#define MVPPROVIDER_H

#include <QMatrix4x4>
#include <QObject>

/*
 * This class represents a QML interface
 * to provide ModelViewProjection matrix
 */

class MVPProvider : public QObject
{ Q_OBJECT Q_PROPERTY(QMatrix4x4 mvp_matrix READ getMVPMatrix NOTIFY newMVPMatrix)
protected:
    // last ModelViewProjection matrix
    QMatrix4x4 mvp_matrix;
public:
    // initialize with eye matrix
    MVPProvider();

    // return MVP matrix
    QMatrix4x4 getMVPMatrix();

signals:

    // emitted when new MVP matrix is available
    void newMVPMatrix();
};

#endif // MVPPROVIDER_H
