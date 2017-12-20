#include "mvpprovider.h"

MVPProvider::MVPProvider()
{
    reset();
}

QMatrix4x4 MVPProvider::getMVPMatrix()
{
    return mvp_matrix;
}

void MVPProvider::reset()
{
    qDebug() << "Hiding objects";
    mvp_matrix.fill(0);
    emit newMVPMatrix();
}
