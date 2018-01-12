#include "mvpprovider.h"
#include "timelogger.h"

void MVPProvider::setMVPMatrix(QMatrix4x4 mat)
{
    if(mat != mvp_matrix)
    {
        if(!is_valid)
            TimeLoggerLog("%s", "Matrix available");

        mvp_matrix = mat;
        is_valid = 1;
        emit newMVPMatrix();
    }
}

MVPProvider::MVPProvider()
{
    is_valid = 1;
    reset();
}

QMatrix4x4 MVPProvider::getMVPMatrix()
{
    return mvp_matrix;
}

QMatrix4x4 MVPProvider::getMVMatrix()
{
    return mv_matrix;
}

QMatrix4x4 MVPProvider::getPMatrix()
{
    return p_matrix;
}

bool MVPProvider::isValid()
{
    return is_valid;
}

void MVPProvider::reset()
{
    if(is_valid)
    {
        TimeLoggerLog("%s", "Hiding objects");
        mvp_matrix.fill(0);
        is_valid = 0;
        emit newMVPMatrix();
    }
}
