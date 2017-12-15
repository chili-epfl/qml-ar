#ifndef MYMATCONVERTER_H
#define MYMATCONVERTER_H

#include <QMatrix3x3>
#include "mymatconverter.h"
#include "uchiya/mylib/mymat.h"

class MyMatConverter
{
public:
    static QMatrix3x3 convert3x3(MyMat m);
    static QVector3D convert3x1(MyMat m);
};

#endif // MYMATCONVERTER_H
