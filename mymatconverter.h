#ifndef MYMATCONVERTER_H
#define MYMATCONVERTER_H

#include <QVector3D>
#include <QMatrix4x4>
#include "uchiya/mylib/mymat.h"

/*
 *
 * This class converts Uchiya MyMat
 * to QVector/QMatrix
 *
 */

class MyMatConverter
{
public:
    // QMatrix3x3 is not used since it does not
    // support multiplication with a vector

    // convert 3x3 matrix to QMatrix4x4
    // 3x3 top-left submatrix is used
    static QMatrix4x4 convert3x3(MyMat m);

    // convert column with 3 rows to a QVector3D
    static QVector3D convert3x1(MyMat m);
};

#endif // MYMATCONVERTER_H
