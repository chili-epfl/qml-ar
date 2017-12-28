#include "mvpprovider.h"

void MVPProvider::setMVPMatrix(QMatrix4x4 mat)
{
    if(mat != mvp_matrix)
    {
        if(!is_valid)
            qDebug() << "Matrix available";

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

void MVPProvider::reset()
{
    if(is_valid)
    {
        qDebug() << "Hiding objects";
        mvp_matrix.fill(0);
        is_valid = 0;
        emit newMVPMatrix();
    }
}
