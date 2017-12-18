#include "mymatconverter.h"
#include <QVector3D>

QMatrix3x3 MyMatConverter::convert3x3(MyMat m)
{
    QMatrix3x3 result;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            result(i, j) = m(i, j);
    return result;
}

QVector3D MyMatConverter::convert3x1(MyMat m)
{
    QVector3D result;
    for(int i = 0; i < 3; i++)
        result[i] = m(i, 0);
    return result;
}
