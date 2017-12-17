#ifndef MVPPROVIDER_H
#define MVPPROVIDER_H

#include <QMatrix4x4>
#include <QObject>

/*
 *
 * This class represents a QML interface
 * to provide ModelViewProjection matrix
 *
 */

class MVPProvider : public QObject
{ Q_OBJECT Q_PROPERTY(QMatrix4x4 mvp_matrix READ getMVPMatrix NOTIFY newMVPMatrix)
private:
    QMatrix4x4 mvp_matrix;
public:
    MVPProvider();
    QMatrix4x4 getMVPMatrix();

signals:
    void newMVPMatrix();
};

#endif // MVPPROVIDER_H
