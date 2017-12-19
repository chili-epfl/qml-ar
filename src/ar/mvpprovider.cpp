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
    mvp_matrix.fill(0);
    emit newMVPMatrix();
}
