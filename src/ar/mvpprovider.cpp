#include "mvpprovider.h"
#include "timelogger.h"

void MVPProvider::setMVPMatrix(QMatrix4x4 mat)
{
    if(mat != mvp_matrix)
    {
        if(!isValid(mvp_matrix))
            TimeLoggerLog("%s", "Matrix available");

        mvp_matrix = mat;
        emit newPMatrix(p_matrix);
        emit newMVMatrix(mv_matrix);
        emit newMVPMatrix(mvp_matrix);
    }
}

MVPProvider::MVPProvider()
{
    reset();
}

void MVPProvider::reset()
{
    if(isValid(mvp_matrix))
    {
        TimeLoggerLog("%s", "Hiding objects");
        mvp_matrix.setToIdentity();
        mv_matrix.setToIdentity();
        emit newMVPMatrix(mvp_matrix);
    }
}

bool MVPProvider::isValid(QMatrix4x4 mat)
{
    return !mat.isIdentity();
}
